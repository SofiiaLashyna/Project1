#ifndef GALAXYVIEW_H
#define GALAXYVIEW_H

#include "RandomUtilities.h"
#include "CelestialObject.h"
#include "Galaxy.h"
#include "GraphList.h"
#include "DijkstraList.h"
#include "nlohmann/json.hpp"
#include "GraphWidget.h"
#include <cmath>
#include <qboxlayout.h>
#include <QLabel>
#include <QPushButton>
#include "PhysicsEngine.h"
#include "BlackHoleGravityField.h"
#include "CelestialBodyToRigidWrapper.h"
#include "GalaxyPhysicsController.h"

// Forward declarations to avoid circular dependencies
class GalaxyEditDialog;
class AddStarSystemDialog;
class AddNebulaDialog;
class EditStarSystemDialog;
class EditNebulaDialog;
/**
 * @file GalaxyView.h
 * @brief Defines the GalaxyView class, the main Qt widget for galaxy visualization.
 */

QT_BEGIN_NAMESPACE

/**
 * @class GalaxyView
 * @brief The main UI widget, inheriting from QWidget.
 *
 * This class manages the `GraphWidget` display, floating parameter
 * windows, galaxy generation, and user interaction
 * (clicks, edits, zooming).
 */
namespace Ui {
    class GalaxyView;
}

QT_END_NAMESPACE

class GalaxyView : public QWidget {
    Q_OBJECT

protected:
    /**
     * @brief Overridden Qt event handler for window resize events.
     * Used to dynamically reposition floating buttons and windows.
     * @param event The resize event.
     */
    void resizeEvent(QResizeEvent *event) override;

public:
    /**
     * @brief Constructor for GalaxyView.
     * @param parent The parent widget (usually nullptr).
     */
    explicit GalaxyView(QWidget *parent = nullptr);

    /**
        * @brief Destructor.
        * Cleans up memory by deleting the main `galaxy` object and the `ui`.
        */
    ~GalaxyView() override;

    /**
             * @brief The main method to start the simulation.
             * Creates or replaces the internal `Galaxy` object, triggers procedural generation,
             * updates the graph layout, and refreshes the GraphWidget display.
             * @param data The JSON data with generation configuration.
             * @param rng A reference to the random number generator.
             */
    void generateAndDisplayGalaxy(const nlohmann::json &data, RandomGenerator &rng);

private slots:
    /**
     * @brief Qt Slot: Called when the "Parameters" button is clicked.
     * Toggles the visibility of the parameters window.
     */
    void on_paramsButton_clicked();

    /**
             * @brief Qt Slot: Called when the "Edit" button is clicked (in galaxy mode).
             * Opens the dialog to edit the entire galaxy.
             */
    void on_editButton_clicked();

    /**
             * @brief Qt Slot: Called when the GraphWidget emits a vertexDoubleClicked signal.
             * @param vertexId The ID (object index) of the clicked vertex.
             */
    void on_vertexDoubleClicked(int vertexId);

    /**
     * @brief Qt Slot: Handles a single click on a vertex.
     * Often used for pathfinding selection (start/end nodes).
     * @param vertexId The ID of the clicked vertex.
     */
    void onVertexClicked(int vertexId);

    /**
     * @brief Qt Slot: Handles clicks on the empty background of the widget.
     * Used to deselect objects or hide parameter windows.
     */
    void onBackgroundClicked();

    /**
             * @brief Qt Slot: Called when the "Zoom Out" button is clicked.
             * Resets the zoom level in the GraphWidget.
             */
    void on_zoomOutButton_clicked();

    /**
 * @brief Displays the parameters window for a specific object of galaxy.
 * Triggered internally after a vertex double-click event.
 */

    void showObjectParameters(CelestialObject *obj);
    /**
     * @brief Displays the parameters window for a specific planet of star system.
     * Triggered internally after a vertex double-click event if this event has already been triggered.
     */
    void showPlanetParameters(Planet *planet);

    /**
             * @brief Qt Slot: Called when "Edit" is clicked (in object-focus mode).
             * Opens the edit dialog for the specific StarSystem or Nebula.
             */
    void on_editObjectButton_clicked();

    /**
     * @brief Qt Slot: Called on each physics timer tick.
     * Updates the positions of objects based on physics calculations.
     */
    void onPhysicsTimerTick();

private:
    double viewScale = 0.2; ///< Current scale for rendering.
    Ui::GalaxyView *ui; ///< Pointer to the UI namespace object.
    Galaxy<GraphList<CelestialObject *> > *galaxy = nullptr; ///< Pointer to the core Galaxy data structure.

    /**
     * @brief Synchronizes the data in the parameters window with the currently selected object.
     */
    void updateParametersWindow();

    /**
             * @brief Internal helper method to redraw the graph in the GraphWidget.
             */
    void updateGraphDisplay();

    GraphWidget *graphWidget = nullptr; ///< Widget responsible for drawing the graph nodes and edges.
    QPushButton *paramsButton = nullptr; ///< UI button to open parameters.
    QWidget *paramsWindow = nullptr; ///< Floating window for displaying object details.
    RandomGenerator *rngPtr = nullptr; ///< Pointer to the shared random generator.
    nlohmann::json *dataPtr = nullptr; ///< Pointer to the loaded configuration data.
    QPushButton *editButton = nullptr; ///< UI button for editing.
    QPushButton *zoomOutButton = nullptr; ///< UI button for resetting zoom.
    std::vector<QPointF> vertexPositions; ///< Cached screen positions of vertices.

    /**
     * @brief Opens a dialog to edit a specific Star System.
     * @param system Pointer to the StarSystem object.
     */
    void editStarSystem(StarSystem *system);

    /**
     * @brief Opens a dialog to edit a specific Nebula.
     * @param nebula Pointer to the Nebula object.
     */
    void editNebula(Nebula *nebula);

    /**
     * @brief Applies a dark "space" theme to the UI components.
     */
    void applySpaceStyle();

    PhysicsEngine *physicsEngine = nullptr; ///< The core physics simulation engine.
    GalaxyPhysicsController *physicsController = nullptr; ///< High-level controller for galaxy-specific physics.
    BlackHoleGravityField *blackHoleField = nullptr; ///< Special gravity field representing the galactic center.
    QTimer *simulationTimer = nullptr; ///< Timer that drives the physics update loop.

    /**
     * @brief Initializes the Bullet physics simulation and gravity fields.
     */
    void initPhysicsSimulation();

    /**
     * @brief Scans the galaxy for new objects that don't have a physics body yet.
     */
    void checkForNewObjects();

    /**
     * @brief Wraps a CelestialObject in a rigid body and adds it to the physics world.
     * @param obj The object to add to physics.
     */
    void createPhysicsBody(CelestialObject *obj);

    /**
     * @brief Maps physics world coordinates to screen pixel coordinates.
     * @param x World X coordinate.
     * @param y World Y coordinate.
     * @return QPointF The mapped screen position.
     */
    QPointF physicsToScreen(double x, double y);

    int startNodeId = -1; ///< ID of the starting node for pathfinding.
    int endNodeId = -1; ///< ID of the destination node for pathfinding.

    std::vector<std::pair<int, int> > pathEdges; ///< List of edge pairs forming the shortest path.

    /**
     * @brief Resets current pathfinding selection and highlights.
     */
    void resetPathSelection();

    /**
     * @brief Calculates the shortest path between startNodeId and endNodeId using Dijkstra.
     */
    void calculateShortestPath();

    QWidget *pathInfoWidget = nullptr; ///< UI container for pathfinding information.
    QLabel *pathStatusLabel = nullptr; ///< Label showing current status (e.g., "Select target").
    QLabel *pathDetailsLabel = nullptr; ///< Label showing path sequence or errors.
    QLabel *pathDistanceLabel = nullptr; ///< Label showing the total cost of the path.

    /**
     * @brief Creates and initializes the path information UI elements.
     */
    void setupPathInfoWidget();

    /**
     * @brief Updates the distance text label based on current Dijkstra results.
     */
    void updatePathDistanceText();
};

#endif //GALAXYVIEW_H