#include "EditNebulaDialog.h"
#include "Nebula.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
EditNebulaDialog::EditNebulaDialog(Nebula *nebulaRef, QWidget *parent)
    : QDialog(parent), nebula(nebulaRef) {
    setWindowTitle("Edit Nebula: " + QString::fromStdString(nebula->getName()));

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel("Nebula Name:", this));
    nameEdit = new QLineEdit(QString::fromStdString(nebula->getName()), this);
    layout->addWidget(nameEdit);

    layout->addWidget(new QLabel("Nebula Type:", this));
    nebulaTypeComboBox = new QComboBox(this);
    nebulaTypeComboBox->addItem("Emission");
    nebulaTypeComboBox->addItem("Reflection");
    nebulaTypeComboBox->addItem("Dark");
    nebulaTypeComboBox->addItem("Supernova");
    nebulaTypeComboBox->addItem("Planetary");
    nebulaTypeComboBox->setCurrentText(QString::fromStdString(nebula->getNebulaTypeString()));
    layout->addWidget(nebulaTypeComboBox);

    QPushButton *saveButton = new QPushButton("Save", this);
    QPushButton *cancelButton = new QPushButton("Cancel", this);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    connect(saveButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void EditNebulaDialog::saveChanges() {
    nebula->setName(nameEdit->text().toStdString());
    nebula->setNebulaType(nebulaTypeComboBox->currentText().toStdString());
}
