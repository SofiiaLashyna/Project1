#ifndef EDITNEBULADIALOG_H
#define EDITNEBULADIALOG_H
#include <QDialog>
#include <qlineedit.h>
#include <QComboBox>
#include "Nebula.h"
/**
 * @file EditNebulaDialog.h
 * @brief Defines a dialog for editing an existing Nebula.
 */

/**
 * @class EditNebulaDialog
 * @brief A QDialog window that allows modifying a Nebula's
 * name and type.
 */
class EditNebulaDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor for the EditNebulaDialog.
     * @param nebula Non-owning pointer to the Nebula object to be modified.
     * @param parent The parent Qt widget.
     */
    explicit EditNebulaDialog(Nebula *nebula, QWidget *parent = nullptr);
    /**
             * @brief Applies the changes from the UI fields to the current Nebula object.
             * This is called after the dialog is accepted.
             */
    void saveChanges();

private:
    Nebula *nebula; ///< Non-owning pointer to the Nebula being edited.
    QLineEdit *nameEdit; ///< The text input field for the nebula's name.
    QComboBox *nebulaTypeComboBox; ///< Dropdown to select the nebula's type.
};

#endif //EDITNEBULADIALOG_H
