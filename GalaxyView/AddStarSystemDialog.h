#ifndef ADDSTARSYSTEMDIALOG_H
#define ADDSTARSYSTEMDIALOG_H
#include <QDialog>
#include <qlineedit.h>
#include <QSpinBox>
#include <QComboBox>
#include "RandomUtilities.h"
#include "nlohmann/json.hpp"
#include "StarSystem.h"
/**
 * @file AddStarSystemDialog.h
 * @brief Declares the AddStarSystemDialog class used for creating StarSystem objects.
 */

/**
 * @class AddStarSystemDialog
 * @brief A QDialog window that provides fields for a user
 * to define and create a new StarSystem.
 */
class AddStarSystemDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor for AddStarSystemDialog.
     * @param rng Reference to the random generator (for generating planets/star).
     * @param data Reference to the JSON config data.
     * @param parent The parent Qt widget.
     */
    explicit AddStarSystemDialog(RandomGenerator &rng, const nlohmann::json &data, QWidget *parent = nullptr);
    /**
     * @brief Creates a new StarSystem instance based on dialog input.
     * @param id The unique ID to assign to the created system.
     * @return A pointer to the newly created StarSystem. Ownership is passed to the caller.
     */
    StarSystem *getNewStarSystem(int id) const;

private:
    QLineEdit *nameEdit; ///< Input field for the system's name.
    QSpinBox *planetCountSpinBox; ///< Spin box to control the number of planets.
    QComboBox *starTypeComboBox; ///< Dropdown to select the central star's type.
    RandomGenerator &rng; ///< Reference to the random generator utility.
    const nlohmann::json &data; ///< Reference to the JSON configuration data.
};

#endif //ADDSTARSYSTEMDIALOG_H
