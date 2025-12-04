#ifndef ADDNEBULADIALOG_H
#define ADDNEBULADIALOG_H
#include <QDialog>
#include <qlineedit.h>
#include <QComboBox>
#include "RandomUtilities.h"
#include "nlohmann/json.hpp"
#include "Nebula.h"
/**
 * @file AddNebulaDialog.h
 * @brief Declares the AddNebulaDialog class used for creating custom Nebula objects.
 */

/**
 * @class AddNebulaDialog
 * @brief A QDialog that provides a user interface for defining parameters
 * and creating a new Nebula object.
 */
class AddNebulaDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor an AddNebulaDialog.
     * @param rng Reference to the random generator (used for default nebula generation).
     * @param data Reference to the JSON config data used for nebula templates.
     * @param parent The parent Qt widget.
     */
    explicit AddNebulaDialog(RandomGenerator &rng, const nlohmann::json &data, QWidget *parent = nullptr);
    /**
     * @brief Creates a new Nebula instance based on the current dialog input.
     * @return A pointer to the newly created Nebula. Ownership is passed to the caller.
     */
    Nebula *getNewNebula() const;

private:
    QLineEdit *nameEdit; ///< Input field for the nebula's name.
    QComboBox *nebulaTypeComboBox; ///< Dropdown to select the nebula's type.
    RandomGenerator &rng; ///< Reference to the random generator utility.
    const nlohmann::json &data; ///< Reference to the JSON configuration data.
};

#endif //ADDNEBULADIALOG_H
