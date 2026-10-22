#include "galaxyview.h"
#include "ui_GalaxyView.h"
#include "GalaxyEditDialog.h"
#include "EditStarSystemDialog.h"
#include "EditNebulaDialog.h"
#include "EditPlanetDialog.h"
#include "AddNebulaDialog.h"
#include "AddStarSystemDialog.h"
#include <QApplication>
#include <QPushButton>
#include <QTextEdit>
#include <QResizeEvent>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QMessageBox>
#include <algorithm>

#include "DijkstraPathList.h"
#include "GalaxyFactory.h"
static constexpr double PHYSICS_MASS_SCALE = 1.0e-7;

GalaxyView::GalaxyView(QWidget *parent) : QWidget(parent), ui(new Ui::GalaxyView) {
    ui->setupUi(this);

    graphWidget = new GraphWidget(this);

    simulationTimer = new QTimer(this);
    if (ui->graphArea->layout() == nullptr) {
        QVBoxLayout *layout = new QVBoxLayout(ui->graphArea);
        layout->setContentsMargins(0, 0, 0, 0);
    }

    ui->graphArea->layout()->addWidget(graphWidget);

    paramsButton = new QPushButton("Parameters", this);

    paramsWindow = new QWidget(this);

    QVBoxLayout *paramsLayout = new QVBoxLayout(paramsWindow);
    paramsLayout->setContentsMargins(15, 15, 15, 15);
    paramsLayout->setSpacing(5);

    QLabel *titleLabel = new QLabel("Galaxy Parameters:", paramsWindow);
    titleLabel->setObjectName("titleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);

    QTextEdit *infoText = new QTextEdit(paramsWindow);
    infoText->setObjectName("infoTextWidget");
    infoText->setReadOnly(true);
    infoText->setText("Galaxy not generated yet.");

    QPushButton *editButtonLocal = new QPushButton("Edit Object", paramsWindow);
    this->editButton = editButtonLocal;

    zoomOutButton = new QPushButton("Back to Galaxy", this);
    zoomOutButton->hide();

    paramsLayout->addWidget(titleLabel);
    paramsLayout->addWidget(infoText);
    paramsLayout->addWidget(this->editButton);
    paramsWindow->setLayout(paramsLayout);

    paramsButton->resize(160, 40);
    paramsWindow->resize(240, 220);

    connect(paramsButton, &QPushButton::clicked, this, &GalaxyView::on_paramsButton_clicked);
    connect(zoomOutButton, &QPushButton::clicked, this, &GalaxyView::on_zoomOutButton_clicked);
    connect(graphWidget, &GraphWidget::vertexDoubleClicked, this, &GalaxyView::on_vertexDoubleClicked);
    connect(graphWidget, &GraphWidget::vertexClicked, this, &GalaxyView::onVertexClicked);
    connect(graphWidget, &GraphWidget::backgroundClicked, this, &GalaxyView::onBackgroundClicked);
    connect(editButton, &QPushButton::clicked, this, &GalaxyView::on_editButton_clicked);
    connect(simulationTimer, &QTimer::timeout, this, &GalaxyView::onPhysicsTimerTick);
    connect(graphWidget, &GraphWidget::planetDoubleClicked, this, [this](int index) {
        int sysId = graphWidget->getDetailedVertexId();
        if (sysId < 0 || !galaxy) return;

        CelestialObject* obj = galaxy->getObject()[sysId];
        StarSystem* system = dynamic_cast<StarSystem*>(obj);

        if (system && index >= 0 && index < system->getPlanets().size()) {
            Planet* planet = system->getPlanets()[index];
            graphWidget->setShowAxialTilt(false);
            showPlanetParameters(planet);
            if (paramsWindow) {
                paramsWindow->hide();
            }
            zoomOutButton->setText("Back to System");
        }
    });
    setupPathInfoWidget();
    applySpaceStyle();
    paramsWindow->hide();
}

GalaxyView::~GalaxyView() {
    if (galaxy) {
        delete galaxy;
        galaxy = nullptr;
    }
    delete physicsController;
    delete physicsEngine;
    delete blackHoleField;
    delete ui;
}

void GalaxyView::on_paramsButton_clicked() {
    if (paramsWindow->isVisible()) {
        paramsWindow->hide();
        if (graphWidget) graphWidget->setShowAxialTilt(false);
    } else {
        paramsWindow->show();
        paramsWindow->raise();
        if (graphWidget && graphWidget->isInPlanetMode()) {
            graphWidget->setShowAxialTilt(true);
        }
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
    if (pathInfoWidget) {
        int margin = 20;
        int px = width() - pathInfoWidget->width() - margin;
        int py = height() - pathInfoWidget->height() - margin;
        pathInfoWidget->move(px, py);
    }
}

void GalaxyView::updateGraphDisplay() {
    if (!galaxy || !graphWidget) return;
    if (!rngPtr) {
        qDebug() << "Error: rngPtr is null. Cannot generate new coordinates.";
        return;
    }

    int nVerticesTotal = static_cast<int>(galaxy->getObject().size());


    std::vector<W_Vertex> vertices;
    vertices.resize(nVerticesTotal);

    for (int i = 0; i < nVerticesTotal; ++i) {
        const auto &v = galaxy->getGraph().getVertices()[i];
        if (v.getId() != -1) {
            QPointF pos = vertexPositions[i];
            vertices[i] = {
                i,
                pos.x(),
                pos.y(),
                QString::fromStdString(galaxy->getObject()[i]->getName())
            };
        } else {
            vertices[i] = {-1, 0, 0, QString()};
        }
    }

    std::vector<W_Edge> edgesToDraw;

    if (!pathEdges.empty()) {
        auto &allEdges = galaxy->getGraph().getEdges();

        for (const auto &pair: pathEdges) {
            int u = pair.first;
            int v = pair.second;

            for (const auto &realEdge: allEdges) {
                if (!realEdge.isActive()) continue;

                if ((realEdge.from == u && realEdge.to == v) ||
                    (realEdge.from == v && realEdge.to == u)) {
                    edgesToDraw.push_back({realEdge.from, realEdge.to, realEdge.weight});
                    break;
                }
            }
        }
    }

    if (graphWidget) {
        graphWidget->setGraph(vertices, edgesToDraw, &galaxy->getObject());
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

    GalaxyFactory::populateGalaxy(*galaxy, data, rng);

    this->dataPtr = const_cast<nlohmann::json *>(&data);
    this->rngPtr = &rng;

    qDebug() << "Galaxy successfully generated:" << QString::fromStdString(randomGalaxyName);

    ui->galaxyNameLabel->setText(QString("Created Galaxy: ") + QString::fromStdString(randomGalaxyName));
    initPhysicsSimulation();

    updateGraphDisplay();
    updateParametersWindow();
}

void GalaxyView::createPhysicsBody(CelestialObject *obj) {
    if (!physicsEngine || !physicsController) return;

    auto *wrapper = new CelestialBodyToRigidWrapper(obj, physicsEngine->getWorld());

    double minScreenDimension = std::min(ui->graphArea->width(), ui->graphArea->height());
    double maxScreenRadius = (minScreenDimension / 2.0) - 50.0;
    double maxPhysicsRadius = maxScreenRadius / viewScale;

    if (maxPhysicsRadius < 150) maxPhysicsRadius = 200;

    double radius = rngPtr->getDouble(50, maxPhysicsRadius);

    double angle = rngPtr->getDouble(0, 2 * M_PI);
    double physX = radius * std::cos(angle);
    double physY = radius * std::sin(angle);

    wrapper->setPosition(physX, physY, 0);

    double realBlackHoleMass = 1.3e12;
    double simBlackHoleMass = realBlackHoleMass * PHYSICS_MASS_SCALE;
    double G = 1.0;

    double dist = std::sqrt(physX * physX + physY * physY);
    double vMag = std::sqrt((1.0 * simBlackHoleMass) / dist);
    btVector3 vel(-physY, physX, 0);
    vel.normalize();
    vel *= vMag;

    wrapper->getRigidBody()->setLinearVelocity(vel);

    physicsController->addCelestialBody(wrapper);

    vertexPositions.push_back(physicsToScreen(physX, physY));
}

void GalaxyView::initPhysicsSimulation() {
    if (!simulationTimer) return;
    simulationTimer->stop();

    if (physicsController) {
        delete physicsController;
        physicsController = nullptr;
    }
    if (physicsEngine) {
        delete physicsEngine;
        physicsEngine = nullptr;
    }
    if (blackHoleField) {
        delete blackHoleField;
        blackHoleField = nullptr;
    }

    physicsEngine = new PhysicsEngine();
    physicsController = new GalaxyPhysicsController(physicsEngine);


    double realBlackHoleMass = 1.3e12;

    double simBlackHoleMass = realBlackHoleMass * PHYSICS_MASS_SCALE;

    blackHoleField = new BlackHoleGravityField(simBlackHoleMass, 0, 0, 0);

    vertexPositions.clear();
    if (!galaxy) return;

    std::vector<CelestialBodyToRigidWrapper *> wrappersMap(galaxy->getObject().size(), nullptr);
    int nVerticesTotal = galaxy->getObject().size();

    for (int i = 0; i < nVerticesTotal; ++i) {
        CelestialObject *obj = galaxy->getObject()[i];
        if (galaxy->getGraph().getVertices()[i].getId() == -1) {
            vertexPositions.push_back(QPointF(0, 0));
            continue;
        }

        auto *wrapper = new CelestialBodyToRigidWrapper(obj, physicsEngine->getWorld());
        wrappersMap[i] = wrapper;

        double minScreenDimension = std::min(ui->graphArea->width(), ui->graphArea->height());

        double maxScreenRadius = (minScreenDimension / 2.0) - 50.0;

        double maxPhysicsRadius = maxScreenRadius / viewScale;

        if (maxPhysicsRadius < 200) maxPhysicsRadius = 300;

        double radius = rngPtr->getDouble(100, maxPhysicsRadius);

        double angle = rngPtr->getDouble(0, 2 * M_PI);
        wrapper->setPosition(radius * std::cos(angle), radius * std::sin(angle), 0);
        wrapper->getRigidBody()->setDamping(0.95, 0.95);
        wrapper->getRigidBody()->setActivationState(DISABLE_DEACTIVATION);

        physicsController->addCelestialBody(wrapper);
    }

    double radiusMultiplying = 5;
    for (const auto &edge: galaxy->getGraph().getEdges()) {
        if (!edge.isActive()) continue;
        if (wrappersMap[edge.from] && wrappersMap[edge.to]) {
            physicsController->addSpring(wrappersMap[edge.from], wrappersMap[edge.to], edge.weight * radiusMultiplying);
        }
    }

    qDebug() << "Pre-calculating positions...";
    for (int i = 0; i < 500; ++i) {
        physicsController->simulateStep(0.016);
    }

    physicsController->clearSprings();
    physicsController->addGravityField(blackHoleField);

    double G = 1.0;

    for (int i = 0; i < nVerticesTotal; ++i) {
        if (!wrappersMap[i]) {
            if (i >= vertexPositions.size()) vertexPositions.push_back(QPointF(0, 0));
            continue;
        }

        auto *wrapper = wrappersMap[i];
        wrapper->updateFromPhysics();

        wrapper->getRigidBody()->setLinearVelocity(btVector3(0, 0, 0));
        wrapper->getRigidBody()->setDamping(0.0, 0.0);

        double x = wrapper->getX();
        double y = wrapper->getY();
        double dist = std::sqrt(x * x + y * y);
        if (dist < 10.0) dist = 10.0;

        double vMag = std::sqrt((G * simBlackHoleMass) / dist);

        btVector3 vel(-y, x, 0);
        vel.normalize();
        vel *= vMag;

        wrapper->getRigidBody()->setLinearVelocity(vel);

        if (i < vertexPositions.size()) vertexPositions[i] = physicsToScreen(x, y);
        else vertexPositions.push_back(physicsToScreen(x, y));
    }

    simulationTimer->start(16);
}

void GalaxyView::checkForNewObjects() {
    if (!galaxy || !physicsController) return;

    int totalObjectsInGalaxy = galaxy->getObject().size();

    int knownObjects = vertexPositions.size();

    if (totalObjectsInGalaxy > knownObjects) {
        qDebug() << "Found new objects! Adding to physics engine...";

        for (int i = knownObjects; i < totalObjectsInGalaxy; ++i) {
            CelestialObject *obj = galaxy->getObject()[i];

            if (galaxy->getGraph().getVertices()[i].getId() != -1) {
                createPhysicsBody(obj);
            } else {
                vertexPositions.push_back(QPointF(0, 0));
            }
        }
    }
}

void GalaxyView::onPhysicsTimerTick() {
    if (!physicsController) return;

    for (int i = 0; i < 3; ++i) {
        physicsController->simulateStep(1.0 / 60.0);
    }
    const auto &bodies = physicsController->getBodies();

    int bodyIndex = 0;
    for (int i = 0; i < vertexPositions.size(); ++i) {
        if (galaxy->getGraph().getVertices()[i].getId() == -1) continue;

        if (bodyIndex < bodies.size()) {
            auto *wrapper = bodies[bodyIndex];
            double px = wrapper->getX();
            double py = wrapper->getY();

            vertexPositions[i] = physicsToScreen(px, py);
            bodyIndex++;
        }
    }

    auto &edges = galaxy->getGraph().getEdges();

    for (auto &edge: edges) {
        if (!edge.isActive()) continue;

        int idA = edge.from;
        int idB = edge.to;

        if (idA >= 0 && idA < vertexPositions.size() &&
            idB >= 0 && idB < vertexPositions.size()) {
            QPointF pA = vertexPositions[idA];
            QPointF pB = vertexPositions[idB];

            double dx = pA.x() - pB.x();
            double dy = pA.y() - pB.y();
            double realDistance = std::sqrt(dx * dx + dy * dy);

            edge.changeWeight(static_cast<int>(realDistance));
        }
    }

    updateGraphDisplay();

    if (pathInfoWidget && pathInfoWidget->isVisible() && !pathEdges.empty()) {
        double totalDistance = 0;

        auto &graphEdges = galaxy->getGraph().getEdges();
        for (const auto &pair: pathEdges) {
            int u = pair.first;
            int v = pair.second;

            for (const auto &edge: graphEdges) {
                if (!edge.isActive()) continue;
                if ((edge.from == u && edge.to == v) || (edge.from == v && edge.to == u)) {
                    totalDistance += edge.weight;
                    break;
                }
            }
        }

        pathDistanceLabel->setText("Total Distance: " + QString::number((int) totalDistance));
    }
}

QPointF GalaxyView::physicsToScreen(double x, double y) {
    double centerX = ui->graphArea->width() / 2.0;
    double centerY = ui->graphArea->height() / 2.0;
    return QPointF(centerX + x * viewScale, centerY + y * viewScale);
}

void GalaxyView::updateParametersWindow() {
    QTextEdit *infoText = paramsWindow->findChild<QTextEdit *>("infoTextWidget");

    if (infoText && galaxy) {
        infoText->setText(galaxy->getGalaxyParameters());
    } else if (infoText) {
        infoText->setText("Cannot load parameters: Galaxy object is missing.");
    }
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
                checkForNewObjects();
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

    if (pathInfoWidget) resetPathSelection();
    if (graphWidget) {
        graphWidget->zoomToVertex(vertexId);
    }
    if (zoomOutButton) {
        zoomOutButton->show();
        zoomOutButton->raise();
        zoomOutButton->setText("Back to Galaxy");
    }

    paramsButton->show();
    paramsButton->raise();

    showObjectParameters(obj);
    if (paramsWindow) {
        paramsWindow->hide();
    }

    ui->galaxyNameLabel->hide();
    disconnect(editButton, &QPushButton::clicked, this, &GalaxyView::on_editButton_clicked);
    connect(editButton, &QPushButton::clicked, this, &GalaxyView::on_editObjectButton_clicked);
}

void GalaxyView::onVertexClicked(int vertexId) {
    if (!galaxy || vertexId < 0) return;

    std::string objName = galaxy->getObject()[vertexId]->getName();

    if (startNodeId != -1 && endNodeId != -1) {
        resetPathSelection();
        startNodeId = vertexId;
        pathInfoWidget->show();
        pathStatusLabel->setText("Start Selected");
        pathDetailsLabel->setText("Start: " + QString::fromStdString(objName) + "\nChoose Destination...");
        pathDistanceLabel->setText("");
        if (graphWidget) graphWidget->setHighlightedNodes({startNodeId});
        return;
    }

    if (startNodeId == -1) {
        startNodeId = vertexId;
        pathInfoWidget->show();
        pathStatusLabel->setText("Start Selected");
        pathDetailsLabel->setText("Start: " + QString::fromStdString(objName) + "\nChoose Destination...");
        pathDistanceLabel->clear();
        if (graphWidget) graphWidget->setHighlightedNodes({startNodeId});
    } else {
        if (vertexId == startNodeId) return;

        endNodeId = vertexId;
        std::string startName = galaxy->getObject()[startNodeId]->getName();

        pathStatusLabel->setText("Path Calculated");
        pathDetailsLabel->setText(
            QString("From: %1\nTo: %2").arg(QString::fromStdString(startName), QString::fromStdString(objName)));

        calculateShortestPath();
    }
}

void GalaxyView::onBackgroundClicked() {
    if (zoomOutButton->isVisible()) return;
    resetPathSelection();
}

void GalaxyView::resetPathSelection() {
    startNodeId = -1;
    endNodeId = -1;
    pathEdges.clear();

    if (pathInfoWidget) pathInfoWidget->hide();

    ui->galaxyNameLabel->show();
    if (graphWidget) {
        graphWidget->setHighlightedNodes({});
    }
    updateGraphDisplay();
}

void GalaxyView::calculateShortestPath() {
    DijkstraPathList<CelestialObject *> solver;

    std::vector<int> pathIndices = solver.findShortestPath(galaxy->getGraph(), startNodeId, endNodeId);

    pathEdges.clear();
    if (pathIndices.empty()) {
    } else {
        for (size_t i = 0; i < pathIndices.size() - 1; ++i) {
            pathEdges.push_back({pathIndices[i], pathIndices[i + 1]});
        }
    }
    if (graphWidget) {
        graphWidget->setHighlightedNodes(pathIndices);
    }
    updateGraphDisplay();
}

void GalaxyView::setupPathInfoWidget() {
    pathInfoWidget = new QWidget(this);

    QString pathWindowStyle =
            "QWidget {"
            "  background-color: rgba(10, 10, 25, 230);"
            "  border: 2px solid #00aaff;"
            "  border-radius: 15px;"
            "}";
    pathInfoWidget->setStyleSheet(pathWindowStyle);

    QVBoxLayout *layout = new QVBoxLayout(pathInfoWidget);

    pathStatusLabel = new QLabel("Select Start Point", pathInfoWidget);
    pathStatusLabel->setStyleSheet(
        "background: transparent; border: none; color: #00aaff; font-family: 'Ravie'; font-size: 14px;"
    );
    pathStatusLabel->setAlignment(Qt::AlignCenter);

    pathDetailsLabel = new QLabel("", pathInfoWidget);
    pathDetailsLabel->setStyleSheet(
        "background: transparent; border: none; color: white; font-family: 'Segoe UI'; font-size: 12px;"
    );
    pathDetailsLabel->setWordWrap(true);
    pathDetailsLabel->setAlignment(Qt::AlignCenter);

    pathDistanceLabel = new QLabel("", pathInfoWidget);
    pathDistanceLabel->setStyleSheet(
        "background: transparent; border: none; color: #00ff00; font-family: 'Ravie'; font-size: 13px;"
    );
    pathDistanceLabel->setAlignment(Qt::AlignCenter);

    layout->addWidget(pathStatusLabel);
    layout->addWidget(pathDetailsLabel);
    layout->addWidget(pathDistanceLabel);

    pathInfoWidget->setLayout(layout);
    pathInfoWidget->resize(240, 140);
    pathInfoWidget->hide();
}

void GalaxyView::on_zoomOutButton_clicked() {
    if (graphWidget->isInPlanetMode()) {
        graphWidget->resetPlanetZoom();
        graphWidget->setShowAxialTilt(false);
        int sysId = graphWidget->getDetailedVertexId();
        if (sysId >= 0) {
            CelestialObject *obj = galaxy->getObject()[sysId];
            showObjectParameters(obj);
            updateGraphDisplay();
            QApplication::processEvents();
        }
        zoomOutButton->setText("Back to Galaxy");
        if (paramsWindow) {
            paramsWindow->hide();
        }
        return;
    }
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
    QLabel *titleLabel = paramsWindow->findChild<QLabel *>();
    if (titleLabel) {
        titleLabel->setText("Galaxy Parameters: \n");
    }
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
            for (Planet *planet: system->getPlanets()) {
                system->lifeExists(*planet);
            }
            parametersText = QString::fromStdString(
                "Name: " + system->getName() + "\n" +
                "Type: Star System\n" +
                "Mass: " + QString::number(system->getMass(), 'e', 3).toStdString() + " solar masses\n\n" +
                "Star Information:\n" +
                "  Type: " + system->getStar().getStarTypeString() + "\n" +
                "  Mass: " + QString::number(system->getStar().getMass(), 'e', 3).toStdString() + " solar masses\n" +
                "  Temperature: " + std::to_string(system->getStar().getTemperature()) + " K\n\n" +
                "Planets: " + std::to_string(system->getPlanets().size()) + "\n"
            );

            for (size_t i = 0; i < system->getPlanets().size(); ++i) {
                Planet *planetPtr = system->getPlanets()[i];
                Planet &planet = *planetPtr;
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
                "Mass: " + QString::number(nebula->getMass(), 'e', 3).toStdString() + " solar masses\n"
            );
        }
    } else {
        parametersText = QString::fromStdString(
            "Name: " + obj->getName() + "\n" +
            "Type: " + obj->getType() + "\n" +
            "Mass: " + QString::number(obj->getMass(), 'e', 3).toStdString() + "\n"
        );
    }

    infoText->setText(parametersText);

    QLabel *titleLabel = paramsWindow->findChild<QLabel *>();
    if (titleLabel) {
        titleLabel->setText(QString::fromStdString(obj->getType()) + " info: ");
    }

    paramsWindow->show();
    paramsWindow->raise();
}
void GalaxyView::showPlanetParameters(Planet* planet) {
    if (!planet || !paramsWindow) return;

    QTextEdit *infoTextWidget = paramsWindow->findChild<QTextEdit *>("infoTextWidget");
    if (!infoTextWidget) return;

    QString typeStr;
    switch(planet->getPlanetType()) {
        case Planet::planetType::Gas_Giant: typeStr = "Gas Giant"; break;
        case Planet::planetType::Terrestrial_Planet: typeStr = "Terrestrial"; break;
        case Planet::planetType::Dwarf: typeStr = "Dwarf Planet"; break;
    }

    QString parametersText = QString::fromStdString(
        "Name: " + planet->getName() + "\n" +
        "Type: " + typeStr.toStdString() + "\n\n" +
        "Mass: " + std::to_string(planet->getMass()) + " Earths\n" +
        "Orbit Radius: " + std::to_string(planet->getOrbit()) + " AU\n" +
        "Orbit Speed: " + std::to_string(planet->getSpeed()) + " km/s\n" +
        "Axial Tilt: " + QString::number(planet->getInclination(), 'f', 2).toStdString() + "°\n" +
        "Habitable: " + (planet->isHabitable() ? "YES" : "NO") + "\n"
    );

    infoTextWidget->setText(parametersText);

    QLabel *titleLabel = paramsWindow->findChild<QLabel *>("titleLabel");
    if (titleLabel) {
        titleLabel->setText("Planet Info:");
    }

    paramsWindow->show();
    paramsWindow->raise();
}
void GalaxyView::on_editObjectButton_clicked() {
    if (!graphWidget || !galaxy) return;

    if (graphWidget->isInPlanetMode()) {
        int sysId = graphWidget->getDetailedVertexId();
        if (sysId < 0) return;

        CelestialObject *obj = galaxy->getObject()[sysId];
        StarSystem *system = dynamic_cast<StarSystem *>(obj);

        int pIndex = graphWidget->getDetailedPlanetIndex();

        if (system && pIndex >= 0 && pIndex < system->getPlanets().size()) {
            Planet* planet = system->getPlanets()[pIndex];

            EditPlanetDialog dlg(planet, this->rngPtr, this);

            if (dlg.exec() == QDialog::Accepted) {
                dlg.saveChanges();
                showPlanetParameters(planet);
                graphWidget->update();
            }
            return;
        }
    }

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

void GalaxyView::applySpaceStyle() {
    QString btnStyle =
            "QPushButton {"
            "  background-color: rgba(20, 20, 40, 200);"
            "  border: 2px solid #00aaff;"
            "  border-radius: 10px;"
            "  color: white;"
            "  font-family: 'Ravie';"
            "  font-size: 12px;"
            "  padding: 5px;"
            "}"
            "QPushButton:hover {"
            "  background-color: rgba(0, 170, 255, 50);"
            "  border: 2px solid #ffffff;"
            "  color: #00ffff;"
            "}"
            "QPushButton:pressed {"
            "  background-color: rgba(0, 170, 255, 100);"
            "  border: 2px solid #0088cc;"
            "}";

    if (paramsButton) paramsButton->setStyleSheet(btnStyle);
    if (editButton) editButton->setStyleSheet(btnStyle);
    if (zoomOutButton) zoomOutButton->setStyleSheet(btnStyle);

    QString windowStyle =
            "QWidget {"
            "  background-color: rgba(10, 10, 20, 240);"
            "  border: 1px solid #00aaff;"
            "  border-radius: 15px;"
            "}"
            "QLabel#titleLabel {"
            "  background: transparent;"
            "  border: none;"
            "  color: #00ffff;"
            "  font-family: 'Ravie';"
            "  font-size: 14px;"
            "  margin-bottom: 5px;"
            "}"
            "QTextEdit {"
            "  background-color: rgba(0, 0, 0, 50);"
            "  border: none;"
            "  color: #d0e0ff;"
            "  font-family: 'Segoe UI', sans-serif;"
            "  font-size: 13px;"
            "}";

    if (paramsWindow) paramsWindow->setStyleSheet(windowStyle);
}
