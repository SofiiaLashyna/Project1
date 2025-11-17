#ifndef EDITSTARSYSTEMDIALOG_H
#define EDITSTARSYSTEMDIALOG_H
#include <QDialog>
#include <qlineedit.h>
#include <QSpinBox>
#include <QComboBox>
#include "RandomUtilities.h"
#include "nlohmann/json.hpp"
#include "StarSystem.h"
/**
 * @file EditStarSystemDialog.h
 * @brief Defines a dialog for editing an existing StarSystem.
 */

/**
 * @class EditStarSystemDialog
 * @brief A QDialog window that allows modifying the properties
 * of a specific StarSystem (name, star type, planet count).
 */
class EditStarSystemDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor for the EditStarSystemDialog.
     * @param system The StarSystem object to be modified.
     * @param rng Non-owning pointer to the random generator (for new planets).
     * @param data Non-owning pointer to the JSON data (for new planets).
     * @param parent The parent Qt widget.
     */
    explicit EditStarSystemDialog(StarSystem *system, RandomGenerator *rng, const nlohmann::json *data, QWidget *parent = nullptr);
    /**
             * @brief Applies the changes from the UI fields to the current StarSystem object.
             * This is called after the dialog is accepted.
             */
    void saveChanges();

private:
    StarSystem *starSystem; ///< Non-owning pointer to the system being edited.
    QLineEdit *nameEdit; ///< Text field for the system's name.
    QSpinBox *planetCountSpinBox; ///< Spin box to control the number of planets.
    QComboBox *starTypeComboBox; ///< Dropdown to select the central star's type.
    RandomGenerator *rngPtr; ///< Non-owning pointer to the random generator.
    const nlohmann::json *dataPtr; ///< Non-owning pointer to the JSON config data.

    void updatePlanetCount();
};

#endif //EDITSTARSYSTEMDIALOG_H
