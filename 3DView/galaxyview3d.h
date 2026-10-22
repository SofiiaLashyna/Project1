#ifndef GALAXYVIEW3D_H
#define GALAXYVIEW3D_H

#include <QWidget>
#include <QTimer>
#include <QVector3D>
#include <QVBoxLayout>
#include <QQuickWidget>
#include <QPushButton>
#include <QTextEdit>
#include "DijkstraPathList.h"
#include <QLabel>
#include "RandomUtilities.h"
#include "CelestialObject.h"
#include "Galaxy.h"
#include "GraphList.h"
#include "nlohmann/json.hpp"
#include "CelestialObject3DModel.h"
#include "PhysicsEngine.h"
#include "BlackHoleGravityField.h"
#include "CelestialBodyToRigidWrapper.h"
#include "GalaxyPhysicsController.h"
#include "GalaxyEditDialog.h"
#include "EditStarSystemDialog.h"
#include "EditNebulaDialog.h"
#include "PlanetarySystemModel.h"

class StarSystem;
class Nebula;

/**
 * @file GalaxyView3D.h
 * @brief Header for the 3D visualization view of the galaxy.
 */

namespace Ui {
    class GalaxyView3D;
}

/**
 * @class GalaxyView3D
 * @brief Main 3D interface for galaxy exploration.
 * * This class combines Qt Quick (QML) for high-performance 3D rendering with
 * a Bullet-based physics simulation. It manages 3D vertex positions,
 * orbital mechanics in 3D space, and provides interactive tools for
 * pathfinding and object editing.
 */
class GalaxyView3D : public QWidget {
    Q_OBJECT

protected:
    /** @brief Handles window resizing and updates the 3D viewport layout. */
    void resizeEvent(QResizeEvent *event) override;

public:
    /** @brief Initializes the 3D view, QML bridge, and physics controller. */
    explicit GalaxyView3D(QWidget *parent = nullptr);

    /** @brief Destructor: Ensures proper cleanup of 3D models and physics resources. */
    ~GalaxyView3D() override;

    /** @brief Container for the 3D coordinates of all celestial nodes in the galaxy. */
    std::vector<QVector3D> vertexPositions3D;

    /**
     * @brief Generates a new galaxy and prepares the 3D scene.
     * @param data JSON configuration for procedural generation.
     * @param rng Random generator for consistent 3D placement.
     */
    void generateAndDisplayGalaxy(const nlohmann::json &data, RandomGenerator &rng);

private slots:
    /** @brief Toggles the visibility of the object data panel. */
    void on_paramsButton_clicked();

    /** @brief Opens the main galaxy settings editor. */
    void on_editButton_clicked();

    /** @brief Focuses the 3D camera and opens parameters for a specific object. */
    void on_vertexDoubleClicked(int vertexId);

    /** @brief Resets the 3D camera to its default position. */
    void on_zoomOutButton_clicked();

    /** @brief Opens the specialized editor for the currently selected StarSystem or Nebula. */
    void on_editObjectButton_clicked();

    /** @brief Core simulation tick: updates physics and notifies the QML layer. */
    void onPhysicsTimerTick();

    /** @brief Handles single-click selection for pathfinding (start/end nodes). */
    void onVertexClicked(int vertexId);

    /** @brief Resets selections when clicking the void of space. */
    void onBackgroundClicked();

private:
    Ui::GalaxyView3D *ui;
    Galaxy<GraphList<CelestialObject *>> *galaxy = nullptr;

    // UI Elements
    int detailedVertexId = -1;
    QPushButton *paramsButton = nullptr;
    QWidget *paramsWindow = nullptr;
    QTextEdit *infoText = nullptr;
    QPushButton *editButton = nullptr;
    QPushButton *zoomOutButton = nullptr;

    // Data Pointers
    RandomGenerator *rngPtr = nullptr;
    nlohmann::json *dataPtr = nullptr;

    // 3D & QML Integration
    /** @brief The widget that hosts the QML-based 3D scene. */
    QQuickWidget *quickWidget = nullptr;
    /** @brief Data provider for the QML 3D scene. */
    CelestialObject3DModel *celestialModelPtr = nullptr;
    /** @brief Model for representing detailed planetary orbits in 3D. */
    PlanetarySystemModel *planetModelPtr = nullptr;

    // Physics System
    PhysicsEngine *physicsEngine = nullptr;
    GalaxyPhysicsController *physicsController = nullptr;
    BlackHoleGravityField *blackHoleField = nullptr;
    QTimer *simulationTimer = nullptr;
    /** @brief Maps 3D-specific physics wrappers to galaxy entities. */
    std::vector<CelestialBodyToRigidWrapper *> wrappersMap3D;
    double viewScale = 1.0;

    // Pathfinding (Dijkstra)
    int startNodeId = -1;
    int endNodeId = -1;
    QWidget *pathInfoWidget = nullptr;
    QLabel *pathStatusLabel = nullptr;
    QLabel *pathDetailsLabel = nullptr;
    QLabel *pathDistanceLabel = nullptr;
    bool isPathActive = false;

    /** @brief Initializes the floating UI for pathfinding results. */
    void setupPathInfoWidget();

    /** @brief Clears Dijkstra selection and removes visual path highlights. */
    void resetPathSelection();

    /** @brief Calculates the shortest path and triggers 3D path visualization. */
    void calculateShortestPath();

    /** * @brief Sends pathfinding data to the QML layer for rendering as 3D lines.
     * @param pathIndices Vector of vertex IDs forming the path.
     */
    void sendPathToQML(const std::vector<int> &pathIndices);

    /** @brief Retrieves the 3D position of a specific vertex by index. */
    QVector3D getObjectPosition(int index);

    /** @brief Applies a space-themed stylesheet to UI components. */
    void applySpaceStyle();

    /** @brief Configures the 3D physics environment. */
    void initPhysicsSimulation();

    /** @brief Creates a physical rigid body for a 3D celestial object. */
    void createPhysicsBody(CelestialObject *obj);

    /** @brief Updates the inspector panel with the latest object data. */
    void updateParametersWindow();

    /** @brief Shows detailed parameters for a selected CelestialObject. */
    void showObjectParameters(CelestialObject *obj);

    void editStarSystem(StarSystem *system);
    void editNebula(Nebula *nebula);

    /** @brief Checks if any new objects were added to sync them with the 3D scene. */
    void checkForNewObjects();

    /** * @brief Internal helper to set initial 3D physics state for a body.
     * @param wrapper The physics wrapper.
     * @param x, y, z Initial coordinates.
     */
    void setupPhysicsForBody(CelestialBodyToRigidWrapper *wrapper, double x, double y, double z);

    /** @brief Maps a star type to its corresponding visual color for 3D stars. */
    QColor getStarColorByType(Star::starType type);
};

#endif //GALAXYVIEW3D_H