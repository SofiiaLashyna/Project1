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

    void onVertexClicked(int vertexId);

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

    void onPhysicsTimerTick();

private:
    double viewScale = 0.2;
    Ui::GalaxyView *ui;
    Galaxy<GraphList<CelestialObject *> > *galaxy = nullptr;

    void updateParametersWindow();

    /**
             * @brief Internal helper method to redraw the graph in the GraphWidget.
             */
    void updateGraphDisplay();

    GraphWidget *graphWidget = nullptr;
    QPushButton *paramsButton = nullptr;
    QWidget *paramsWindow = nullptr;
    RandomGenerator *rngPtr = nullptr;
    nlohmann::json *dataPtr = nullptr;
    QPushButton *editButton = nullptr;
    QPushButton *zoomOutButton = nullptr;
    std::vector<QPointF> vertexPositions;

    void editStarSystem(StarSystem *system);

    void editNebula(Nebula *nebula);

    void applySpaceStyle();

    PhysicsEngine *physicsEngine = nullptr;
    GalaxyPhysicsController *physicsController = nullptr;
    BlackHoleGravityField *blackHoleField = nullptr;
    QTimer *simulationTimer = nullptr;

    void initPhysicsSimulation();

    void checkForNewObjects();

    void createPhysicsBody(CelestialObject *obj);

    QPointF physicsToScreen(double x, double y);

    int startNodeId = -1;
    int endNodeId = -1;

    std::vector<std::pair<int, int> > pathEdges;

    void resetPathSelection();

    void calculateShortestPath();

    QWidget *pathInfoWidget = nullptr;
    QLabel *pathStatusLabel = nullptr;
    QLabel *pathDetailsLabel = nullptr;
    QLabel *pathDistanceLabel = nullptr;

    void setupPathInfoWidget();

    void updatePathDistanceText();
};

#endif //GALAXYVIEW_H
