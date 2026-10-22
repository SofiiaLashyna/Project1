#include "AddNebulaDialog.h"
#include "GraphList.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include "Galaxy.h"
#include "GalaxyFactory.h"

AddNebulaDialog::AddNebulaDialog(RandomGenerator &rngRef,
                                 const nlohmann::json &dataRef,
                                 QWidget *parent)
: QDialog(parent), rng(rngRef), data(dataRef) {
    setWindowTitle("Add Nebula");

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("Nebula Name:", this));
    nameEdit = new QLineEdit("New Nebula", this);
    layout->addWidget(nameEdit);

    layout->addWidget(new QLabel("Nebula Type:", this));
    nebulaTypeComboBox = new QComboBox(this);
    nebulaTypeComboBox->addItem("Emission");
    nebulaTypeComboBox->addItem("Reflection");
    nebulaTypeComboBox->addItem("Dark");
    nebulaTypeComboBox->addItem("Supernova");
    nebulaTypeComboBox->addItem("Planetary");
    layout->addWidget(nebulaTypeComboBox);

    QPushButton *okButton = new QPushButton("Add", this);
    QPushButton *cancelButton = new QPushButton("Cancel", this);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}
Nebula *AddNebulaDialog::getNewNebula() const {
    Galaxy<GraphList<CelestialObject *> > temp_galaxy;
    Nebula *nebula =  GalaxyFactory::createNebula(data, rng);


    if (!nebula) return nullptr;

    std::string newName = nameEdit->text().toStdString();
    nebula->setName(newName);

    std::string typeStr = nebulaTypeComboBox->currentText().toStdString();
    nebula->setNebulaType(typeStr);


    return nebula;
}
