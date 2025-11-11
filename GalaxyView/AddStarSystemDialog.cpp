#include "AddStarSystemDialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "GraphList.h"
#include "Galaxy.h"

AddStarSystemDialog::AddStarSystemDialog(RandomGenerator &rngRef,
const nlohmann::json &dataRef,
QWidget *parent)
: QDialog(parent), rng(rngRef), data(dataRef) {
    setWindowTitle("Add Star System");

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("System Name:", this));
    nameEdit = new QLineEdit("New Star System", this);
    layout->addWidget(nameEdit);

    layout->addWidget(new QLabel("Number of Planets:", this));
    planetCountSpinBox = new QSpinBox(this);
    planetCountSpinBox->setRange(0, 10);
    planetCountSpinBox->setValue(3);
    layout->addWidget(planetCountSpinBox);

    layout->addWidget(new QLabel("Star Type:", this));
    starTypeComboBox = new QComboBox(this);
    starTypeComboBox->addItem("Main_sequence_Star");
    starTypeComboBox->addItem("Red_Giant");
    starTypeComboBox->addItem("White_Dwarf");
    starTypeComboBox->addItem("Brown_Dwarf");
    starTypeComboBox->addItem("Neutron_Star");
    starTypeComboBox->addItem("Red_Dwarf");
    layout->addWidget(starTypeComboBox);

    QPushButton *okButton = new QPushButton("Add", this);
    QPushButton *cancelButton = new QPushButton("Cancel", this);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}
StarSystem *AddStarSystemDialog::getNewStarSystem(int id) const {
    Galaxy<GraphList<CelestialObject *> > temp_galaxy;

    StarSystem *system = temp_galaxy.generateStarSystem(id, rng, data);

    if (!system) return nullptr;

    std::string newName = nameEdit->text().toStdString();
    system->setName(newName);

    std::string starTypeStr = starTypeComboBox->currentText().toStdString();

    system->getStar().setStarType(starTypeStr);


    int targetPlanetCount = planetCountSpinBox->value();
    if (targetPlanetCount < system->getPlanets().size()) {
        int dif = system->getPlanets().size() - targetPlanetCount;
        for (auto i = 0; i != dif; ++i) {
            system->removePlanet();
        }
    } else if (targetPlanetCount > system->getPlanets().size()) {
        int dif = targetPlanetCount - system->getPlanets().size();
        for (auto i = 0; i != dif; ++i) {
            Planet *planet = temp_galaxy.generatePlanet(rng, data);
            system->addPlanet(planet);
        }
    }
    return system;
}

