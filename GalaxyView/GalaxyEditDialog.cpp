#include "GalaxyEditDialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "GraphList.h"
#include "Galaxy.h"
#include <algorithm>


GalaxyEditDialog::GalaxyEditDialog(Galaxy<GraphList<CelestialObject *> > *g,
                                   RandomGenerator *rng,
                                   const nlohmann::json *data,
                                   QWidget *parent)
    : QDialog(parent), galaxy(g), rngPtr(rng), dataPtr(data) {
    setWindowTitle("Edit Galaxy: " + QString::fromStdString(g->getName()));

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *nameLabel = new QLabel("Galaxy Name:", this);
    nameEdit = new QLineEdit(QString::fromStdString(galaxy->getName()), this);

    QPushButton *saveNameButton = new QPushButton("Save Name", this);

    QHBoxLayout *nameLayout = new QHBoxLayout;
    nameLayout->addWidget(nameEdit);
    nameLayout->addWidget(saveNameButton);

    layout->addWidget(nameLabel);
    layout->addLayout(nameLayout);

    layout->addWidget(new QWidget(this));

    QLabel *addLabel = new QLabel("Add New Object:", this);
    QPushButton *addStarSystem = new QPushButton("Add Star System", this);
    QPushButton *addNebula = new QPushButton("Add Nebula", this);

    layout->addWidget(addLabel);
    layout->addWidget(addStarSystem);
    layout->addWidget(addNebula);

    layout->addWidget(new QWidget(this));

    QPushButton *closeButton = new QPushButton("Close", this);
    layout->addWidget(closeButton);

    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);

    connect(addStarSystem, &QPushButton::clicked, this, &GalaxyEditDialog::on_addStarSystem_clicked);
    connect(addNebula, &QPushButton::clicked, this, &GalaxyEditDialog::on_addNebula_clicked);

    connect(saveNameButton, &QPushButton::clicked, this, [this]() {
        galaxy->setName(nameEdit->text().toStdString());
        setWindowTitle("Edit Galaxy: " + nameEdit->text());
    });
}

void GalaxyEditDialog::on_addStarSystem_clicked() {
    if (!rngPtr || !dataPtr) {
        qWarning() << "Generator or data not initialized!";
        return;
    }
    AddStarSystemDialog dlg(*rngPtr, *dataPtr, this);

    if (dlg.exec() == QDialog::Accepted) {
        int oldSize = static_cast<int>(galaxy->getObject().size());
        StarSystem *newSystem = dlg.getNewStarSystem(oldSize);

        if (newSystem) {
            galaxy->addObject(newSystem);
            int newObjectIdx = oldSize;

            int existingObjectCount = oldSize;
            if (existingObjectCount > 0) {
                int maxEdges = std::min(5, existingObjectCount);
                int edgesCount = rngPtr->getInt(1, maxEdges);
                std::vector<int> targets;
                for (int i = 0; i < existingObjectCount; ++i) targets.push_back(i);

                std::random_shuffle(targets.begin(), targets.end());

                for (int i = 0; i < edgesCount && i < targets.size(); ++i) {
                    int targetIdx = targets[i];
                    int dist = rngPtr->getInt(10, 1000);
                    galaxy->getGraph().addEdge(newObjectIdx, targetIdx, dist);
                }
            }
        }
        emit galaxyModified();
    }
}

void GalaxyEditDialog::on_addNebula_clicked() {
    if (!rngPtr || !dataPtr) {
        qWarning() << "Generator or data not initialized!";
        return;
    }
    AddNebulaDialog dlg(*rngPtr, *dataPtr, this);

    if (dlg.exec() == QDialog::Accepted) {
        Nebula *newNebula = dlg.getNewNebula();
        int oldSize = static_cast<int>(galaxy->getObject().size());

        if (newNebula) {
            galaxy->addObject(newNebula);
            int newObjectIdx = oldSize;
            int existingObjectCount = oldSize;

            if (existingObjectCount > 0) {
                int maxEdges = std::min(3, existingObjectCount);
                int edgesCount = rngPtr->getInt(1, maxEdges);

                std::vector<int> targets;
                for (int i = 0; i < existingObjectCount; ++i) targets.push_back(i);

                std::shuffle(targets.begin(), targets.end(), rngPtr->getEngine());

                for (int i = 0; i < edgesCount && i < targets.size(); ++i) {
                    int targetIdx = targets[i];
                    int dist = rngPtr->getInt(10, 1000);
                    galaxy->getGraph().addEdge(newObjectIdx, targetIdx, dist);
                }
            }
        }
        emit galaxyModified();
    }
}

QString GalaxyEditDialog::getNewGalaxyName() const {
    return nameEdit->text();
}
