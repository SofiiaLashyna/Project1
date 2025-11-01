#ifndef FRUCHTERMANREINGOLDLAYOUT_H
#define FRUCHTERMANREINGOLDLAYOUT_H

#include <vector>
#include <cmath>
#include <algorithm>
#include <QDebug>

struct Displacement {
    double dx = 0.0;
    double dy = 0.0;
};

namespace ForceConstants {
    const double K = 10.0;
    const double K_SQUARED = K * K;

    const int MAX_ITERATIONS = 250;

    const double COOLING_FACTOR = 0.95;

    const double INITIAL_TEMPERATURE = 50.0;
}

inline double fa(double distance, double weight) {
    return (distance / ForceConstants::K) * (1.0 / weight);
}


inline double fr(double distance) {
    if (distance < 1.0) distance = 1.0;
    return -ForceConstants::K_SQUARED / distance;
}


template<typename VertexType, typename EdgeType>
void calculateForceDirectedLayout(
    std::vector<VertexType>& vertices,
    const std::vector<EdgeType>& edges,
    int width,
    int height,
    RandomGenerator& rng)
{
    int n = vertices.size();
    if (n == 0) return;

    int pad = 50;

    const double pad_d = (double)pad;
    const double max_x = (double)width - pad_d;
    const double max_y = (double)height - pad_d;

    for (int i = 0; i < n; ++i) {
        vertices[i].x = rng.getDouble(pad_d, max_x);
        vertices[i].y = rng.getDouble(pad_d, max_y);
    }

    double maxWeight = 1.0;
    for (const auto& edge : edges) {
        if (edge.weight > maxWeight) {
            maxWeight = edge.weight;
        }
    }

    double temperature = ForceConstants::INITIAL_TEMPERATURE;

    std::vector<Displacement> displacements(n);

    for (int iter = 0; iter < ForceConstants::MAX_ITERATIONS; ++iter) {

        for (int i = 0; i < n; ++i) {
            displacements[i] = {0.0, 0.0};
        }

        for (int i = 0; i < n; ++i) {
            for (int k = i + 1; k < n; ++k) {
                double deltaX = vertices[k].x - vertices[i].x;
                double deltaY = vertices[k].y - vertices[i].y;

                double distance = std::sqrt(deltaX * deltaX + deltaY * deltaY);
                double force = fr(distance);

                double factor = force / distance;

                displacements[i].dx -= deltaX * factor;
                displacements[i].dy -= deltaY * factor;

                displacements[k].dx += deltaX * factor;
                displacements[k].dy += deltaY * factor;
            }
        }


        for (const auto& edge : edges) {
            int i = edge.from;
            int k = edge.to;

            if (i >= n || k >= n) continue;

            double deltaX = vertices[k].x - vertices[i].x;
            double deltaY = vertices[k].y - vertices[i].y;

            double distance = std::sqrt(deltaX * deltaX + deltaY * deltaY);

            double effectiveWeight = (edge.weight > 0) ? edge.weight : 1.0;
            double weightRatio = maxWeight / effectiveWeight;

            double force = fa(distance, effectiveWeight);

            double factor = force / distance;

            displacements[i].dx += deltaX * factor;
            displacements[i].dy += deltaY * factor;

            displacements[k].dx -= deltaX * factor;
            displacements[k].dy -= deltaY * factor;
        }


        for (int i = 0; i < n; ++i) {

            double dispLength = std::sqrt(displacements[i].dx * displacements[i].dx + displacements[i].dy * displacements[i].dy);
            if (dispLength > temperature) {
                double factor = temperature / dispLength;
                displacements[i].dx *= factor;
                displacements[i].dy *= factor;
            }

            vertices[i].x += displacements[i].dx;
            vertices[i].y += displacements[i].dy;

            vertices[i].x = std::min(max_x, std::max(pad_d, (double)vertices[i].x));
            vertices[i].y = std::min(max_y, std::max(pad_d, (double)vertices[i].y));
        }

        temperature *= ForceConstants::COOLING_FACTOR;

        if (temperature < 0.5) break;
    }
}

#endif //FRUCHTERMANREINGOLDLAYOUT_H
