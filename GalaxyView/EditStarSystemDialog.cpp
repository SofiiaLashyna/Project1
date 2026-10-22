#include"EditStarSystemDialog.h"
#include "StarSystem.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "GraphList.h"
#include "Galaxy.h"
#include "GalaxyFactory.h"

EditStarSystemDialog::EditStarSystemDialog(StarSystem *system,
                                           RandomGenerator *rng,
                                           const nlohmann::json *data,
                                           QWidget *parent)
    : QDialog(parent), starSystem(system), rngPtr(rng), dataPtr(data) {
    setWindowTitle("Edit Star System: " + QString::fromStdString(system->getName()));

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("System Name:", this));
    nameEdit = new QLineEdit(QString::fromStdString(starSystem->getName()), this);
    layout->addWidget(nameEdit);

    layout->addWidget(new QLabel("Star Type:", this));
    starTypeComboBox = new QComboBox(this);
    starTypeComboBox->addItem("Main_sequence_Star");
    starTypeComboBox->addItem("Red_Giant");
    starTypeComboBox->addItem("White_Dwarf");
    starTypeComboBox->addItem("Brown_Dwarf");
    starTypeComboBox->addItem("Neutron_Star");
    starTypeComboBox->addItem("Red_Dwarf");
    starTypeComboBox->setCurrentText(QString::fromStdString(starSystem->getStar().getStarTypeString()));
    layout->addWidget(starTypeComboBox);

    layout->addWidget(new QLabel("Number of Planets:", this));
    planetCountSpinBox = new QSpinBox(this);
    planetCountSpinBox->setRange(0, 10);
    planetCountSpinBox->setValue(static_cast<int>(starSystem->getPlanets().size()));
    layout->addWidget(planetCountSpinBox);

    QPushButton *saveButton = new QPushButton("Save", this);
    QPushButton *cancelButton = new QPushButton("Cancel", this);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    connect(saveButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}


void EditStarSystemDialog::saveChanges() {
    starSystem->setName(nameEdit->text().toStdString());
    starSystem->getStar().setStarType(starTypeComboBox->currentText().toStdString());

    int targetCount = planetCountSpinBox->value();
    int currentCount = static_cast<int>(starSystem->getPlanets().size());

    if (targetCount < currentCount) {
        int diff = currentCount - targetCount;
        for (int i = 0; i < diff; ++i) {
            starSystem->removePlanet();
        }
    } else if (targetCount > currentCount) {
        int diff = targetCount - currentCount;
        Galaxy<GraphList<CelestialObject *> > temp_galaxy;
        for (int i = 0; i < diff; ++i) {
            Planet *planet = GalaxyFactory::createPlanet(*dataPtr, *rngPtr);
            starSystem->addPlanet(planet);
            starSystem->lifeExists(*starSystem->getPlanets().back());

        }
    }
}
