#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H
#include "CelestialObject.h"
#include "StarSystem.h"
#include "Nebula.h"
#include <QtWidgets/QWidget>
#include <vector>
#include <QTimer>
#include <unordered_set>

/**
 * @file GraphWidget.h
 * @brief Defines a custom Qt widget for rendering a galaxy graph with vertices and edges.
 */
/**
 * @struct W_Edge
 * @brief A lightweight struct for passing edge data to the GraphWidget.
 */
struct W_Edge {
    int from; ///< The index of the 'from' vertex.
    int to; ///< The index of the 'to' vertex.
    int weight; ///< The weight of the edge.
};

/**
 * @struct W_Vertex
 * @brief A lightweight struct for passing vertex data to the GraphWidget.
 */
struct W_Vertex {
    int id;
    double x, y;
    QString name;
};

/**
 * @file GraphWidget.h
 * @brief Defines the GraphWidget class, a Qt widget for custom 2D rendering.
 */

struct BackgroundStar {
    QPointF pos;
    int alpha;
    double size;
};

/**
 * @class GraphWidget
 * @brief A custom QWidget that handles all painting (QPainter) for the
 * galaxy graph, star systems, and nebulae, including zoom and animation.
 */
class GraphWidget : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor for GraphWidget.
     * @param parent The parent Qt widget.
     */
    GraphWidget(QWidget *parent = nullptr);

    /**
             * @brief Sets the graph data (vertices and edges) to be rendered.
             * @param v The vector of vertices.
             * @param e The vector of edges.
             * @param objects A non-owning pointer to the main celestial objects vector.
             */
    void setGraph(const std::vector<W_Vertex> &v, const std::vector<W_Edge> &e,
                  const std::vector<CelestialObject *> *objects);

    /**
         * @brief Enters "detail mode" (zoom) for a specific vertex.
         * @param vertexId The index of the vertex to focus on.
         */
    void zoomToVertex(int vertexId);

    /**
     * @brief Exits "detail mode" and resets the zoom.
     */
    void resetZoom();

    void setHighlightedNodes(const std::vector<int> &ids);

    /**
             * @brief Gets the ID of the vertex currently in focus (detail mode).
             * @return The ID of the detailed vertex, or -1 if not in detail mode.
             */
    int getDetailedVertexId() const;

    void zoomToPlanet(int planetIndex);

    void resetPlanetZoom();

    bool isInPlanetMode() const;

    void setShowAxialTilt(bool show);

    int getDetailedPlanetIndex() const;
signals:
    /**
     * @brief Qt Signal: Emitted when a user double-clicks on a vertex.
     * @param vertexId The ID of the vertex that was clicked.
     */
    void vertexDoubleClicked(int vertexId);

    void vertexClicked(int id);

    void backgroundClicked();

    void planetDoubleClicked(int planetIndex);

protected:
    /**
     * @brief Overridden Qt event handler for all custom painting.
     * @param event The paint event.
     */
    void paintEvent(QPaintEvent *event) override;

    /**
        * @brief Overridden Qt event handler for detecting clicks on vertices.
        * @param event The mouse event.
        */
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

private:
    ///< Local copy of the vertices to be drawn.
    std::vector<W_Vertex> vertices;
    ///< Local copy of the edges to be drawn.
    std::vector<W_Edge> edges;
    ///< Non-owning pointer to the original CelestialObject vector (for dynamic_cast).
    const std::vector<CelestialObject *> *celestialObjectsPtr = nullptr;
    ///< Flag to toggle between galaxy view and detail (zoom) view.
    bool isDetailMode = false;
    ///< The ID of the vertex currently in focus (-1 if none).
    int detailedVertexId = -1;
    ///< Qt Timer used to drive animations in detail mode (e.g., planet orbits).
    QTimer *animationTimer = nullptr;
    std::unordered_set<int> highlightedIds;
    std::vector<BackgroundStar> stars;

    void generateBackgroundStars();

    bool isPlanetMode = false;
    int detailedPlanetIndex = -1;
    double calculateOrbitStart(StarSystem* system, double starRadius);

    QPointF calculatePlanetOffset(const Planet* planet, double orbitStart, double scaleFactor) const;

    bool showAxialTilt = false;
};

#endif //GRAPHWIDGET_H
