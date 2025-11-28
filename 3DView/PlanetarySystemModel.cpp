#include "PlanetarySystemModel.h"
#include <QDebug>
#include <cmath>
#include <limits>
#include <algorithm>


PlanetarySystemModel::PlanetarySystemModel(QObject *parent)
    : QAbstractListModel(parent) {}

int PlanetarySystemModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return static_cast<int>(m_planets.size());
}

QVariant PlanetarySystemModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_planets.size())
        return QVariant();

    const auto &planet = m_planets[index.row()];

    switch (role) {
        case OrbitRadiusRole: return planet.orbitRadius;
        case PlanetSizeRole:  return planet.size;
        case PlanetColorRole: return planet.color;
        case RotationSpeedRole: return planet.speed;
        default: return QVariant();
    }
}

QHash<int, QByteArray> PlanetarySystemModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[OrbitRadiusRole] = "orbitRadius";
    roles[PlanetSizeRole] = "planetSize";
    roles[PlanetColorRole] = "planetColor";
    roles[RotationSpeedRole] = "rotationSpeed";
    return roles;
}

void PlanetarySystemModel::updateSystem(StarSystem* system) {
    beginResetModel();
    m_planets.clear();

    if (system) {
        const auto& planets = system->getPlanets();

        double minMass = std::numeric_limits<double>::max();
        double maxMass = std::numeric_limits<double>::lowest();

        for (const auto &p: planets) {
            minMass = std::min(minMass, p.getMass());
            maxMass = std::max(maxMass, p.getMass());
        }

        if (std::abs(maxMass - minMass) < 0.0001) maxMass = minMass + 1.0;

        double starRadius = 25.0;
        double baseOrbitStart = starRadius * 2.5;
        double orbitSpacing = 40.0;

        for (size_t i = 0; i < planets.size(); ++i) {
            const auto& p = planets[i];

            PlanetData data;

            double massNorm = (p.getMass() - minMass) / (maxMass - minMass);
            data.size = 5.0 + (10.0 * massNorm);

            data.orbitRadius = baseOrbitStart + (i * orbitSpacing);

            QColor c = p.getColor();
            if (!c.isValid()) c = QColor::fromHsv((i * 60) % 255, 180, 240);
            data.color = c;

            data.speed = 10000.0 * std::sqrt(data.orbitRadius * 0.1);
            
            m_planets.push_back(data);
        }
    }
    endResetModel();
}

void PlanetarySystemModel::clear() {
    beginResetModel();
    m_planets.clear();
    endResetModel();
}