#ifndef EDITPLANETDIALOG_H
#define EDITPLANETDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <RandomUtilities.h>

#include "Planet.h"

/**
 * @file EditPlanetDialog.h
 * @brief Header file for the EditPlanetDialog class, a graphical interface for modifying planet properties.
 */

/**
 * @class EditPlanetDialog
 * @brief A custom dialog window used to edit the attributes of a Planet object.
 *
 * This class provides a set of Qt widgets (inputs, spin boxes, checkboxes) that
 * map to the physical and orbital properties of a Planet. It allows for manual
 * adjustment of celestial data within the application.
 * *
 */
class EditPlanetDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructs an EditPlanetDialog initialized with the data from a specific planet.
     * * The constructor populates all input fields with the current values of the
     * provided Planet pointer.
     *
     * @param planet Pointer to the Planet object to be edited.
     * @param rng Pointer to the RandomGenerator (useful if randomizing specific attributes within the UI).
     * @param parent Pointer to the parent QWidget (standard Qt memory management).
     */
    explicit EditPlanetDialog(Planet *planet, RandomGenerator *rng, QWidget *parent = nullptr);

    /**
     * @brief Persists the changes from the UI widgets back to the Planet object.
     * * This method reads the values currently held in the QLineEdit, QDoubleSpinBox,
     * and other widgets, and updates the state of the associated Planet instance.
     * Usually called when the user accepts the dialog (e.g., clicks "Save" or "OK").
     */
    void saveChanges();

private:
    Planet *planet;            ///< Pointer to the planet instance being modified.
    RandomGenerator *rngPtr;   ///< Pointer to the utility for random value generation.

    QLineEdit *nameEdit;                ///< Input field for the planet's name.
    QComboBox *typeComboBox;            ///< Selection for planet classification (e.g., Gas Giant, Terrestrial).
    QDoubleSpinBox *massSpinBox;        ///< Numerical input for planetary mass.
    QDoubleSpinBox *orbitRadiusSpinBox; ///< Numerical input for the distance from the star.
    QDoubleSpinBox *speedSpinBox;       ///< Numerical input for orbital velocity.
    QDoubleSpinBox *inclinationSpinBox; ///< Numerical input for the orbital plane inclination.
    QCheckBox *habitableCheckBox;       ///< Toggle for habitability status.
    QCheckBox *ringsCheckBox;           ///< Toggle for the presence of planetary rings.
};

#endif // EDITPLANETDIALOG_H