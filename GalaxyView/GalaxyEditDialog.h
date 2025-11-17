#ifndef GALAXYEDITDIALOG_H
#define GALAXYEDITDIALOG_H
#include <qlineedit.h>
#include "RandomUtilities.h"
#include "nlohmann/json.hpp"
#include "Galaxy.h"
#include "GraphList.h"
#include "CelestialObject.h"
#include "AddNebulaDialog.h"
#include "AddStarSystemDialog.h"
/**
 * @file GalaxyEditDialog.h
 * @brief Defines the GalaxyEditDialog class for editing galaxy properties.
 */

/**
 * @class GalaxyEditDialog
 * @brief A QDialog window that allows editing the galaxy's name
 * and adding new celestial objects.
 */
class GalaxyEditDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor for GalaxyEditDialog.
     * @param g Pointer to the Galaxy object to be modified.
     * @param rng Pointer to the random number generator (for new objects).
     * @param data Pointer to the JSON configuration data (for new objects).
     * @param parent The parent Qt widget.
     */
    explicit GalaxyEditDialog(Galaxy<GraphList<CelestialObject *> > *g,
                              RandomGenerator *rng,
                              const nlohmann::json *data,
                              QWidget *parent = nullptr);

    /**
             * @brief Gets the new name of the galaxy from the text field.
             * @return QString containing the (potentially modified) galaxy name.
             */
    QString getNewGalaxyName() const;

private slots:
    /**
   * @brief Qt Slot: Handles the creation of a new StarSystem.
   *
   * Opens the AddStarSystemDialog, retrieves the newly created StarSystem
   * (if accepted), adds it to the galaxy, and automatically connects it
   * to several existing celestial objects using randomly selected indices.
   *
   * Emits galaxyModified() after a successful addition.
   */
    void on_addStarSystem_clicked();

    /**
   * @brief Qt Slot: Handles the creation of a new Nebula.
   *
   * Opens the AddNebulaDialog, retrieves the created Nebula (if accepted),
   * adds it to the galaxy, and automatically generates a set of edges
   * connecting it to random existing objects.
   *
   * Emits galaxyModified() once the nebula is added.
   */
    void on_addNebula_clicked();

signals:
    void galaxyModified();

private:
    /**
           * @brief Qt Signal: Emitted whenever a change is made (new object added).
           * Used for live-updating the main graph view.
           */
    Galaxy<GraphList<CelestialObject *> > *galaxy; ///< Pointer to the main Galaxy object (non-owning).
    QLineEdit *nameEdit; ///< The text input field for the galaxy's name.

    RandomGenerator *rngPtr; ///< Non-owning pointer to the random number generator.
    const nlohmann::json *dataPtr; ///< Non-owning pointer to the JSON configuration data.
};
#endif //GALAXYEDITDIALOG_H
