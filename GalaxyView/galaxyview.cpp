#include "galaxyview.h"
#include "ui_GalaxyView.h"
#include <QApplication>
#include <QInputDialog>
#include <QPushButton>
#include <QTextEdit>
#include <QResizeEvent>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QMessageBox>

GalaxyView::GalaxyView(QWidget *parent) : QWidget(parent), ui(new Ui::GalaxyView) {
    ui->setupUi(this);

    graphWidget = new GraphWidget(this);

    if (ui->graphArea->layout() == nullptr) {
        QVBoxLayout *layout = new QVBoxLayout(ui->graphArea);
        layout->setContentsMargins(0, 0, 0, 0);
    }

    ui->graphArea->layout()->addWidget(graphWidget);

    paramsButton = new QPushButton("⚙️ Parameters", this);
    paramsButton->setStyleSheet("font-size: 14px; padding: 5px 10px;");

    paramsWindow = new QWidget(this);
    paramsWindow->setStyleSheet(
        "background-color: #333; color: white; border: 1px solid #555; border-radius: 5px;"
    );

    QVBoxLayout *paramsLayout = new QVBoxLayout(paramsWindow);
    paramsLayout->setContentsMargins(10, 10, 10, 10);
    paramsLayout->setSpacing(5);

    QLabel *titleLabel = new QLabel("Galaxy Parameters: \n", paramsWindow);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 16px; margin-bottom: 1px;");

    QTextEdit *infoText = new QTextEdit(paramsWindow);
    infoText->setObjectName("infoTextWidget");
    infoText->setReadOnly(true);

    infoText->setText("Galaxy not generated yet.");
    infoText->setStyleSheet("background-color: #444; color: #eee; border: none;");

    QPushButton *editButton = new QPushButton("✎ Edit..", paramsWindow);
    this->editButton = editButton;
    editButton->setStyleSheet("font-size: 14px; padding: 5px 10px;");

    zoomOutButton = new QPushButton("⬅ Zoom Out", this);
    zoomOutButton->setStyleSheet("font-size: 14px; padding: 5px 10px;");
    zoomOutButton->hide();

    paramsLayout->addWidget(titleLabel);
    paramsLayout->addWidget(infoText);
    paramsLayout->addWidget(editButton);
    paramsWindow->setLayout(paramsLayout);

    paramsButton->resize(150, 35);
    paramsWindow->resize(250, 210);

    connect(paramsButton, &QPushButton::clicked, this, &GalaxyView::on_paramsButton_clicked);
    // connect(editButton, &QPushButton::clicked, this, &GalaxyView::on_editButton_clicked);
    connect(zoomOutButton, &QPushButton::clicked, this, &GalaxyView::on_zoomOutButton_clicked);
    connect(graphWidget, &GraphWidget::vertexDoubleClicked,
            this, &GalaxyView::on_vertexDoubleClicked);
    connect(editButton, &QPushButton::clicked, this, &GalaxyView::on_editObjectButton_clicked);
    paramsWindow->hide();
}

GalaxyView::~GalaxyView() {
    if (galaxy) {
        delete galaxy;
        galaxy = nullptr;
    }
    delete ui;
}

void GalaxyView::on_paramsButton_clicked() {
    if (paramsWindow->isVisible()) {
        paramsWindow->hide();
    } else {
        paramsWindow->show();
        paramsWindow->raise();
    }
}

void GalaxyView::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    if (paramsButton && paramsWindow) {
        int margin = 20;
        int btn_x = width() - paramsButton->width() - margin;
        int btn_y = margin;
        paramsButton->move(btn_x, btn_y);

        int win_x = width() - paramsWindow->width() - margin;
        int win_y = btn_y + paramsButton->height() + 10;
        paramsWindow->move(win_x, win_y);

        if (zoomOutButton) {
            zoomOutButton->resize(150, 35);
            int zoom_x = margin;
            int zoom_y = margin;
            zoomOutButton->move(zoom_x, zoom_y);
        }
    }
}

void GalaxyView::updateGraphDisplay() {
    if (!galaxy || !graphWidget) return;
    if (!rngPtr) {
        qDebug() << "Error: rngPtr is null. Cannot generate new coordinates.";
        return;
    }

    int currentPositionsSize = static_cast<int>(vertexPositions.size());
    int nVerticesTotal = static_cast<int>(galaxy->getObject().size());

    if (nVerticesTotal > currentPositionsSize) {
        for (int i = currentPositionsSize; i < nVerticesTotal; ++i) {
            int cx = 550;
            int cy = 350;
            double radius = rngPtr->getDouble(150, 250);

            double angle = rngPtr->getDouble(0, 2 * M_PI);

            QPointF newPos(
                cx + radius * std::cos(angle),
                cy + radius * std::sin(angle)
            );

            vertexPositions.push_back(newPos);
            qDebug() << "Generated position for new object at index" << i;
        }
    }
    std::vector<W_Vertex> vertices;
    vertices.resize(nVerticesTotal);

    for (int i = 0; i < nVerticesTotal; ++i) {
        const auto &v = galaxy->getGraph().getVertices()[i];
        if (v.getId() != -1) {
            QPointF pos = vertexPositions[i];
            vertices[i] = {
                i,
                static_cast<int>(pos.x()),
                static_cast<int>(pos.y()),
                QString::fromStdString(galaxy->getObject()[i]->getName())
            };
        } else {
            vertices[i] = {-1, 0, 0, QString()};
        }
    }

    std::vector<W_Edge> edges;
    for (const auto &e: galaxy->getGraph().getEdges()) {
        if (!e.isActive()) continue;
        if (e.from < 0 || e.from >= nVerticesTotal) continue;
        if (e.to < 0 || e.to >= nVerticesTotal) continue;
        if (vertices[e.from].id == -1 || vertices[e.to].id == -1) continue;
        edges.push_back({e.from, e.to, e.weight});
    }

    if (graphWidget) {
        graphWidget->setGraph(vertices, edges, &galaxy->getObject());
        graphWidget->update();
    }
}

void GalaxyView::generateAndDisplayGalaxy(const nlohmann::json &data, RandomGenerator &rng) {
    qDebug() << "Generating and displaying galaxy in GalaxyView...";

    if (galaxy) {
        delete galaxy;
        galaxy = nullptr;
    }

    std::string galaxyNameFile = "Unnamed";
    if (data.contains("Galaxy") && data["Galaxy"].is_array() && !data["Galaxy"].empty()) {
        galaxyNameFile = data["Galaxy"][0].get<std::string>();
    }
    std::string randomGalaxyName = rng.getRandomNameFromFile(galaxyNameFile);

    galaxy = new Galaxy<GraphList<CelestialObject *> >(randomGalaxyName);

    galaxy->generateGalaxy(data, rng);

    this->dataPtr = const_cast<nlohmann::json *>(&data);
    this->rngPtr = &rng;

    qDebug() << "Galaxy successfully generated:" << QString::fromStdString(randomGalaxyName);

    ui->galaxyNameLabel->setText(QString("Created Galaxy: ") + QString::fromStdString(randomGalaxyName));

    vertexPositions.clear();
    int nVerticesTotal = galaxy->getObject().size();

    int n_active = 0;
    for (int i = 0; i < nVerticesTotal; ++i) {
        const auto &v = galaxy->getGraph().getVertices()[i];
        if (v.getId() != -1) {
            int cx = 550;
            int cy = 350;
            double radius = rng.getDouble(150, 250);

            double angle = 2 * M_PI * n_active / nVerticesTotal;

            QPointF pos(
                cx + radius * std::cos(angle),
                cy + radius * std::sin(angle)
            );

            vertexPositions.push_back(pos);
            n_active++;
        }
    }
    updateGraphDisplay();
    updateParametersWindow();
}

void GalaxyView::updateParametersWindow() {
    QTextEdit *infoText = paramsWindow->findChild<QTextEdit *>("infoTextWidget");

    if (infoText && galaxy) {
        infoText->setText(galaxy->getGalaxyParameters());
    } else if (infoText) {
        infoText->setText("Cannot load parameters: Galaxy object is missing.");
    }
}


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
                int maxEdges = existingObjectCount;
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

                std::random_shuffle(targets.begin(), targets.end()); // Перемішуємо

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


Nebula *AddNebulaDialog::getNewNebula() const {
    Galaxy<GraphList<CelestialObject *> > temp_galaxy;

    Nebula *nebula = temp_galaxy.generateNebula(rng, data);

    if (!nebula) return nullptr;

    std::string newName = nameEdit->text().toStdString();
    nebula->setName(newName);

    std::string typeStr = nebulaTypeComboBox->currentText().toStdString();
    nebula->setNebulaType(typeStr);


    return nebula;
}


void GalaxyView::on_editButton_clicked() {
    if (!galaxy) {
        QMessageBox::warning(this, "Error", "Can't generate Galaxy!");
        return;
    }

    GalaxyEditDialog dlg(galaxy, rngPtr, dataPtr, this);

    connect(&dlg, &GalaxyEditDialog::galaxyModified, this,
            [this]() {
                updateParametersWindow();
                updateGraphDisplay();
                QApplication::processEvents();
            },
            Qt::DirectConnection);

    if (dlg.exec() == QDialog::Accepted) {
        updateParametersWindow();
        ui->galaxyNameLabel->setText(QString("Created Galaxy: ") + dlg.getNewGalaxyName());

        qDebug() << "New object added. Reloading graph display is needed.";
        updateGraphDisplay();
        QApplication::processEvents();
    }
}

void GalaxyView::on_vertexDoubleClicked(int vertexId) {
    if (!galaxy || vertexId < 0 || vertexId >= galaxy->getObject().size()) {
        qWarning() << "Invalid galaxy object index clicked:" << vertexId;
        return;
    }
    CelestialObject *obj = galaxy->getObject()[vertexId];

    if (graphWidget) {
        graphWidget->zoomToVertex(vertexId);
    }
    if (zoomOutButton) {
        zoomOutButton->show();
        zoomOutButton->raise();
    }

    paramsButton->show();
    paramsButton->raise();
    ui->galaxyNameLabel->hide();
    showObjectParameters(obj);
    if (editButton) {
        editButton->show();
        editButton->raise();
    }
    disconnect(editButton, &QPushButton::clicked, this, &GalaxyView::on_editButton_clicked);
    connect(editButton, &QPushButton::clicked, this, &GalaxyView::on_editObjectButton_clicked);
}

void GalaxyView::on_zoomOutButton_clicked() {
    if (graphWidget) {
        graphWidget->resetZoom();
    }
    zoomOutButton->hide();
    paramsButton->show();
    ui->galaxyNameLabel->show();
    if (editButton) {
        editButton->show();
    }
    disconnect(editButton, &QPushButton::clicked, this, &GalaxyView::on_editObjectButton_clicked);
    connect(editButton, &QPushButton::clicked, this, &GalaxyView::on_editButton_clicked);
    updateParametersWindow();
}

void GalaxyView::showObjectParameters(CelestialObject *obj) {
    if (!obj || !paramsWindow) return;

    QTextEdit *infoText = paramsWindow->findChild<QTextEdit *>("infoTextWidget");
    if (!infoText) return;

    QString parametersText;

    if (obj->getType() == "StarSystem") {
        StarSystem *system = dynamic_cast<StarSystem *>(obj);
        if (system) {
            parametersText = QString::fromStdString(
                "Name: " + system->getName() + "\n" +
                "Type: Star System\n" +
                "Mass: " + std::to_string(system->getMass()) + " solar masses\n\n" +
                "Star Information:\n" +
                "  Type: " + system->getStar().getStarTypeString() + "\n" +
                "  Mass: " + std::to_string(system->getStar().getMass()) + " solar masses\n" +
                "  Temperature: " + std::to_string(system->getStar().getTemperature()) + " K\n\n" +
                "Planets: " + std::to_string(system->getPlanets().size()) + "\n"
            );

            for (size_t i = 0; i < system->getPlanets().size(); ++i) {
                Planet &planet = system->getPlanets()[i];
                parametersText += QString::fromStdString(
                    "\nPlanet " + std::to_string(i + 1) + ":\n" +
                    "  Name: " + planet.getName() + "\n" +
                    "  Mass: " + std::to_string(planet.getMass()) + " Earth masses\n" +
                    "  Orbit: " + std::to_string(planet.getOrbit()) + " AU\n" +
                    "  Habitable: " + (planet.isHabitable() ? "Yes" : "No") + "\n"
                );
            }
        }
    } else if (obj->getType() == "Nebula") {
        Nebula *nebula = dynamic_cast<Nebula *>(obj);
        if (nebula) {
            parametersText = QString::fromStdString(
                "Name: " + nebula->getName() + "\n" +
                "Type: Nebula\n" +
                "Nebula Type: " + nebula->getNebulaTypeString() + "\n" +
                "Mass: " + std::to_string(nebula->getMass()) + " solar masses\n"
            );
        }
    } else {
        parametersText = QString::fromStdString(
            "Name: " + obj->getName() + "\n" +
            "Type: " + obj->getType() + "\n" +
            "Mass: " + std::to_string(obj->getMass()) + "\n"
        );
    }

    infoText->setText(parametersText);

    QLabel *titleLabel = paramsWindow->findChild<QLabel *>();
    if (titleLabel) {
        titleLabel->setText(QString::fromStdString(obj->getType()) + ": " +
                            QString::fromStdString(obj->getName()));
    }

    paramsWindow->show();
    paramsWindow->raise();
}

void GalaxyView::on_editObjectButton_clicked() {
    if (!graphWidget || !galaxy) return;

    int vertexId = graphWidget->getDetailedVertexId();
    if (vertexId < 0 || vertexId >= galaxy->getObject().size()) return;

    CelestialObject *obj = galaxy->getObject()[vertexId];

    if (obj->getType() == "StarSystem") {
        editStarSystem(dynamic_cast<StarSystem *>(obj));
    } else if (obj->getType() == "Nebula") {
        editNebula(dynamic_cast<Nebula *>(obj));
    } else {
        QMessageBox::information(this, "Edit", "Editing of this object is not implemented yet");
    }
}

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
        // Add new planets
        int diff = targetCount - currentCount;
        Galaxy<GraphList<CelestialObject *> > temp_galaxy; // Temporary galaxy to use generator
        for (int i = 0; i < diff; ++i) {
            Planet *planet = temp_galaxy.generatePlanet(*rngPtr, *dataPtr);
            starSystem->addPlanet(planet);
            starSystem->lifeExists(*starSystem->getPlanets().rbegin()); // Check habitability for new planet
        }
    }
}

EditNebulaDialog::EditNebulaDialog(Nebula *nebulaRef, QWidget *parent)
    : QDialog(parent), nebula(nebulaRef) {
    setWindowTitle("Edit Nebula: " + QString::fromStdString(nebula->getName()));

    QVBoxLayout *layout = new QVBoxLayout(this);

    // Name
    layout->addWidget(new QLabel("Nebula Name:", this));
    nameEdit = new QLineEdit(QString::fromStdString(nebula->getName()), this);
    layout->addWidget(nameEdit);

    // Nebula Type
    layout->addWidget(new QLabel("Nebula Type:", this));
    nebulaTypeComboBox = new QComboBox(this);
    nebulaTypeComboBox->addItem("Emission");
    nebulaTypeComboBox->addItem("Reflection");
    nebulaTypeComboBox->addItem("Dark");
    nebulaTypeComboBox->addItem("Supernova");
    nebulaTypeComboBox->addItem("Planetary");
    nebulaTypeComboBox->setCurrentText(QString::fromStdString(nebula->getNebulaTypeString()));
    // Assume you add getNebulaTypeString()
    layout->addWidget(nebulaTypeComboBox);

    // Buttons
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

void GalaxyView::editStarSystem(StarSystem *system) {
    if (!system || !rngPtr || !dataPtr) return;

    EditStarSystemDialog dlg(system, rngPtr, dataPtr, this);

    if (dlg.exec() == QDialog::Accepted) {
        dlg.saveChanges();
        showObjectParameters(system);
        updateGraphDisplay();
        QApplication::processEvents();
    }
}

void GalaxyView::editNebula(Nebula *nebula) {
    if (!nebula) return;

    EditNebulaDialog dlg(nebula, this);

    if (dlg.exec() == QDialog::Accepted) {
        dlg.saveChanges();
        showObjectParameters(nebula);
        updateGraphDisplay();
        QApplication::processEvents();
    }
}
