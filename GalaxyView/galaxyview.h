#ifndef GALAXYVIEW_H
#define GALAXYVIEW_H

#include "RandomUtilities.h"
#include "CelestialObject.h"
#include "Galaxy.h"
#include "GraphList.h"
#include "nlohmann/json.hpp"
#include "GraphWidget.h"
#include <cmath>
#include <qboxlayout.h>
#include <QPushButton>
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
             * @brief Qt Slot: Called when the "Zoom Out" button is clicked.
             * Resets the zoom level in the GraphWidget.
             */
    void on_zoomOutButton_clicked();

    /**
 * @brief Displays the parameters window for a specific object.
 * Triggered internally after a vertex double-click event.
 */

    void showObjectParameters(CelestialObject *obj);

    /**
             * @brief Qt Slot: Called when "Edit" is clicked (in object-focus mode).
             * Opens the edit dialog for the specific StarSystem or Nebula.
             */
    void on_editObjectButton_clicked();

private:
    Ui::GalaxyView *ui; ///< Pointer to the Qt-generated UI class (holds all elements from the .ui file).
    Galaxy<GraphList<CelestialObject *> > *galaxy = nullptr; /// The main data object containing the entire galaxy.
    /**
            * @brief Internal helper method to update the text in the parameters window.
            */
    void updateParametersWindow();

    /**
             * @brief Internal helper method to redraw the graph in the GraphWidget.
             */
    void updateGraphDisplay();

    GraphWidget *graphWidget = nullptr;
    ////< Pointer to the custom widget responsible for drawing and animating the galaxy graph.
    QPushButton *paramsButton = nullptr; ///< Pointer to the "Parameters" button.
    QWidget *paramsWindow = nullptr; ///< Pointer to the floating parameters window.
    RandomGenerator *rngPtr = nullptr; ///< Non-owning pointer to the random number generator.
    nlohmann::json *dataPtr = nullptr; ///< Non-owning pointer to the JSON data.
    QPushButton *editButton = nullptr; ///< Pointer to the "Edit" button.
    QPushButton *zoomOutButton = nullptr; ///< Pointer to the "Zoom Out" button.
    std::vector<QPointF> vertexPositions; ///< Stores the 2D coordinates for each graph vertex.
    /**
         * @brief Opens the edit dialog for a StarSystem.
         * @param system Pointer to the system to be edited.
         */
    void editStarSystem(StarSystem *system);

    /**
     * @brief Opens the edit dialog for a Nebula.
     * @param nebula Pointer to the nebula to be edited.
     */
    void editNebula(Nebula *nebula);
};

#endif //GALAXYVIEW_H
