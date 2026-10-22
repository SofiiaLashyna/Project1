#include "EditPlanetDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

EditPlanetDialog::EditPlanetDialog(Planet *planet, RandomGenerator *rng, QWidget *parent)
    : QDialog(parent), planet(planet), rngPtr(rng) {
    setWindowTitle("Edit Planet: " + QString::fromStdString(planet->getName()));
    resize(300, 400);

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("Planet Name:", this));
    nameEdit = new QLineEdit(QString::fromStdString(planet->getName()), this);
    layout->addWidget(nameEdit);

    layout->addWidget(new QLabel("Planet Type:", this));
    typeComboBox = new QComboBox(this);

    typeComboBox->addItem("Terrestrial Planet", "Terrestrial_Planet");
    typeComboBox->addItem("Gas Giant", "Gas_Giant");
    typeComboBox->addItem("Dwarf Planet", "Dwarf");

    int typeIndex = typeComboBox->findData(QString::fromStdString(planet->getPlanetTypeString())); // Твій геттер
    if (typeIndex != -1) {
        typeComboBox->setCurrentIndex(typeIndex);
    }
    layout->addWidget(typeComboBox);

    layout->addWidget(new QLabel("Mass (Earths):", this));
    massSpinBox = new QDoubleSpinBox(this);
    massSpinBox->setRange(0.0001, 10000.0);
    massSpinBox->setValue(planet->getMass());
    layout->addWidget(massSpinBox);

    layout->addWidget(new QLabel("Orbit Radius (AU):", this));
    orbitRadiusSpinBox = new QDoubleSpinBox(this);
    orbitRadiusSpinBox->setRange(0.1, 500.0);
    orbitRadiusSpinBox->setValue(planet->getOrbit());
    layout->addWidget(orbitRadiusSpinBox);

    layout->addWidget(new QLabel("Orbit Speed:", this));
    speedSpinBox = new QDoubleSpinBox(this);
    speedSpinBox->setRange(0.0, 1000.0);
    speedSpinBox->setValue(planet->getSpeed());
    layout->addWidget(speedSpinBox);

    layout->addWidget(new QLabel("Axial Tilt (degrees):", this));
    inclinationSpinBox = new QDoubleSpinBox(this);
    inclinationSpinBox->setRange(-180.0, 180.0);
    inclinationSpinBox->setValue(planet->getInclination());
    layout->addWidget(inclinationSpinBox);

    habitableCheckBox = new QCheckBox("Is Habitable?", this);
    habitableCheckBox->setChecked(planet->isHabitable());
    layout->addWidget(habitableCheckBox);

    ringsCheckBox = new QCheckBox("Has Rings?", this);
    ringsCheckBox->setChecked(planet->hasRings());
    layout->addWidget(ringsCheckBox);

    QPushButton *saveButton = new QPushButton("Save", this);
    QPushButton *cancelButton = new QPushButton("Cancel", this);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    connect(saveButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void EditPlanetDialog::saveChanges() {
    planet->setName(nameEdit->text().toStdString());

    std::string typeStr = typeComboBox->currentData().toString().toStdString();
    bool typeChanged = (typeStr != planet->getPlanetTypeString());
    planet->setPlanetType(typeStr);

    if (typeChanged && rngPtr) {
        QColor pColor;
        if (typeStr == "Gas_Giant") {
            int choice = rngPtr->getInt(0, 2);
            if (choice == 0) pColor = QColor::fromHsv(rngPtr->getInt(20, 40), rngPtr->getInt(50, 150),
                                                      rngPtr->getInt(200, 255));

            else if (choice == 1)
                pColor = QColor::fromHsv(rngPtr->getInt(180, 220), rngPtr->getInt(30, 100),
                                         rngPtr->getInt(200, 255)); // Icy Blue
            else pColor = QColor::fromHsv(rngPtr->getInt(0, 30), rngPtr->getInt(100, 200), rngPtr->getInt(150, 230));

        } else if (typeStr == "Terrestrial_Planet") {
            int choice = rngPtr->getInt(0, 3);
            if (choice == 0) pColor = QColor::fromHsv(rngPtr->getInt(100, 140), rngPtr->getInt(100, 255),
                                                      rngPtr->getInt(100, 255));

            else if (choice == 1)
                pColor = QColor::fromHsv(rngPtr->getInt(200, 240), rngPtr->getInt(100, 200),
                                         rngPtr->getInt(150, 255)); // Water Blue
            else if (choice == 2)
                pColor = QColor::fromHsv(rngPtr->getInt(0, 20), rngPtr->getInt(150, 255),
                                         rngPtr->getInt(100, 200)); // Mars Red
            else pColor = QColor::fromHsv(rngPtr->getInt(0, 360), 0, rngPtr->getInt(100, 200)); // Rocky Gray
        } else {
            int choice = rngPtr->getInt(0, 1);
            if (choice == 0) pColor = QColor::fromHsv(rngPtr->getInt(180, 260), rngPtr->getInt(10, 50),
                                                      rngPtr->getInt(200, 255));

            else pColor = QColor::fromHsv(0, 0, rngPtr->getInt(100, 200)); // Rock
        }

        planet->setColor(pColor.red(), pColor.green(), pColor.blue());
    }

    planet->setMass(massSpinBox->value());
    planet->setOrbit(orbitRadiusSpinBox->value());
    planet->setSpeed(speedSpinBox->value());
    planet->setInclination(inclinationSpinBox->value());
    planet->setLifeExistence(habitableCheckBox->isChecked());

    bool newHasRings = ringsCheckBox->isChecked();
    if (newHasRings && !planet->hasRings()) {
        planet->setRings(true, 1.4, 2.2, 200, 200, 200);
    } else if (!newHasRings) {
        planet->setRings(false, 0, 0, 0, 0, 0);
    }
}
