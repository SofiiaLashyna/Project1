#include "galaxyview3d.h"
#include "ui_GalaxyView3D.h"
#include "CelestialObject3DModel.h"
#include <QQuickWidget>
#include <QQmlContext>
#include <QQuickItem>
#include <QDebug>
#include <QTimer>
#include <QVector3D>

GalaxyView3D::GalaxyView3D(QWidget *parent)
    : QWidget(parent), ui(new Ui::GalaxyView3D) {
    ui->setupUi(this);
    detailedVertexId = -1;
    galaxy = nullptr;
    std::vector<CelestialObject*> emptyObjects;

    quickWidget = new QQuickWidget(this);
    quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

    if (!ui->graphArea->layout()) {
        ui->graphArea->setLayout(new QVBoxLayout());
    }
    ui->graphArea->layout()->addWidget(quickWidget);

    celestialModelPtr = new CelestialObject3DModel(quickWidget, emptyObjects);

    planetModelPtr = new PlanetarySystemModel(this);
    quickWidget->rootContext()->setContextProperty("planetModel", planetModelPtr);
    quickWidget->rootContext()->setContextProperty("celestialModel", celestialModelPtr);
    quickWidget->setSource(QUrl("qrc:/Galaxy3DView.qml"));

    QQuickItem *rootObject = quickWidget->rootObject();

    simulationTimer = new QTimer(this);
    connect(simulationTimer, &QTimer::timeout, this, &GalaxyView3D::onPhysicsTimerTick);

    paramsButton = new QPushButton("Parameters", this);

    paramsWindow = new QWidget(this);
    paramsWindow->hide();

    QVBoxLayout *paramsLayout = new QVBoxLayout(paramsWindow);
    paramsLayout->setContentsMargins(15, 15, 15, 15);

    QLabel *titleLabel = new QLabel("Object Info:", paramsWindow);
    titleLabel->setObjectName("titleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);

    infoText = new QTextEdit(paramsWindow);
    infoText->setObjectName("infoTextWidget");
    infoText->setReadOnly(true);

    editButton = new QPushButton("Edit Object", paramsWindow);

    paramsLayout->addWidget(titleLabel);
    paramsLayout->addWidget(infoText);
    paramsLayout->addWidget(editButton);
    paramsWindow->setLayout(paramsLayout);

    paramsButton->resize(160, 40);
    paramsWindow->resize(240, 220);

    zoomOutButton = new QPushButton("Back to Galaxy", this);
    zoomOutButton->hide();

    setupPathInfoWidget();

    applySpaceStyle();

    connect(paramsButton, &QPushButton::clicked, this, &GalaxyView3D::on_paramsButton_clicked);
    connect(zoomOutButton, &QPushButton::clicked, this, &GalaxyView3D::on_zoomOutButton_clicked);
    connect(editButton, &QPushButton::clicked, this, &GalaxyView3D::on_editButton_clicked);
    connect(simulationTimer, &QTimer::timeout, this, &GalaxyView3D::onPhysicsTimerTick);

    if (rootObject) {
        connect(rootObject, SIGNAL(objectClicked(int)),
                this, SLOT(onVertexClicked(int)));

        connect(rootObject, SIGNAL(objectDoubleClicked(int)),
                this, SLOT(on_vertexDoubleClicked(int)));

        connect(rootObject, SIGNAL(backgroundClicked()),
                this, SLOT(onBackgroundClicked()));
    }


}

GalaxyView3D::~GalaxyView3D() {
    if (galaxy) {
        for (CelestialObject* obj : galaxy->getObject()) {
            delete obj;
        }
        delete galaxy;
    }
    delete ui;
}

void GalaxyView3D::setupPathInfoWidget() {
    pathInfoWidget = new QWidget(this);

    // Стиль для вікна дистанції
    QString pathWindowStyle =
        "QWidget {"
        "  background-color: rgba(10, 10, 25, 230);"
        "  border: 2px solid #00aaff;" // Яскравіша рамка
        "  border-radius: 15px;"
        "}";
    pathInfoWidget->setStyleSheet(pathWindowStyle);

    QVBoxLayout* layout = new QVBoxLayout(pathInfoWidget);

    pathStatusLabel = new QLabel("Select Start Point", pathInfoWidget);
    pathStatusLabel->setStyleSheet(
        "background: transparent; border: none; color: #00aaff; font-family: 'Ravie'; font-size: 14px;"
    );
    pathStatusLabel->setAlignment(Qt::AlignCenter);

    pathDetailsLabel = new QLabel("", pathInfoWidget);
    pathDetailsLabel->setStyleSheet(
        "background: transparent; border: none; color: white; font-family: 'Segoe UI'; font-size: 12px;"
    );
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

void GalaxyView3D::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    int margin = 20;
    if (paramsButton && paramsWindow) {

        int btn_x = width() - paramsButton->width() - margin;
        int btn_y = margin;
        paramsButton->move(btn_x, btn_y);

        int win_x = width() - paramsWindow->width() - margin;
        int win_y = btn_y + paramsButton->height() + 10;
        paramsWindow->move(win_x, win_y);
    }
    if (zoomOutButton) {
        int zoomBtnX = margin;

        int zoomBtnY = margin + 15;

        zoomOutButton->move(zoomBtnX, zoomBtnY);
    }
    if (pathInfoWidget) {
        int margin = 20;
        int px = width() - pathInfoWidget->width() - margin;
        int py = height() - pathInfoWidget->height() - margin;
        pathInfoWidget->move(px, py);
    }
}
void GalaxyView3D::generateAndDisplayGalaxy(const nlohmann::json &data, RandomGenerator &rng) {
    qDebug() << "Generating and displaying galaxy in GalaxyView3D...";

    if (galaxy) {
        for (CelestialObject* obj : galaxy->getObject()) {
            delete obj;
        }
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

    vertexPositions3D.clear();
    const int nVerticesTotal = static_cast<int>(galaxy->getObject().size());

    const double galaxyRadius = 4000.0;

    const double galaxyThickness = galaxyRadius * 0.01;
    const double centerX = 0.0, centerY = 0.0, centerZ = 0.0;

    int n_active = 0;
    for (int i = 0; i < nVerticesTotal; ++i) {

        double minSpawnRadius = 300.0;
        double rFactor = std::pow(rng.getDouble(0, 1.0), 2.0);
        double radius = minSpawnRadius + rFactor * (galaxyRadius - minSpawnRadius);

        double theta = rng.getDouble(0, 2 * M_PI);

        double angle = theta + (radius / (galaxyRadius * 0.2));

        double y_factor = rng.getDouble(-1.0, 1.0) * rng.getDouble(-1.0, 1.0);
        double y = centerY + y_factor * galaxyThickness;

        double x = centerX + radius * std::cos(angle);
        double z = centerZ + radius * std::sin(angle);

        vertexPositions3D.emplace_back(x, y, z);
        n_active++;
    }

    std::vector<double> xPos, yPos, zPos;
    for (const auto& pos : vertexPositions3D) {
        xPos.push_back(pos.x());
        yPos.push_back(pos.y());
        zPos.push_back(pos.z());
    }

    std::vector<CelestialObject*>& currentObjects = galaxy->getObject();
    celestialModelPtr->updateObjects(currentObjects);
    celestialModelPtr->updatePositions(xPos, yPos, zPos);

    ui->galaxyNameLabel->setText(QString::fromStdString(randomGalaxyName));

    celestialModelPtr->updateObjects(galaxy->getObject());

    initPhysicsSimulation();

}
void GalaxyView3D::initPhysicsSimulation() {
    if (!simulationTimer) return;
    simulationTimer->stop();


    for (auto *w : wrappersMap3D) {
        if (w) {
            delete w;
        }
    }
    wrappersMap3D.clear();

    delete physicsController; physicsController = nullptr;
    delete physicsEngine; physicsEngine = nullptr;
    delete blackHoleField; blackHoleField = nullptr;

    physicsEngine = new PhysicsEngine();
    physicsController = new GalaxyPhysicsController(physicsEngine);

    double realBlackHoleMass = 1.3e12;
    double simBlackHoleMass = realBlackHoleMass * 1.0e-7;

    blackHoleField = new BlackHoleGravityField(simBlackHoleMass, 0, 0, 0);
    physicsController->addGravityField(blackHoleField);


if (!galaxy) return;

    wrappersMap3D.clear();
    wrappersMap3D.resize(galaxy->getObject().size(), nullptr);

int nVerticesTotal = static_cast<int>(galaxy->getObject().size());

const double G_const = 1.0;
const double velFactor = 1.0;
const double minRadHoriz = 10.0;

for (int i = 0; i < nVerticesTotal; ++i) {
    CelestialObject *obj = galaxy->getObject()[i];
    if (galaxy->getGraph().getVertices()[i].getId() == -1) continue;

    auto *wrapper = new CelestialBodyToRigidWrapper(obj, physicsEngine->getWorld());
    wrappersMap3D[i] = wrapper;

    double startX = 0.0, startY = 0.0, startZ = 0.0;
    if (i < vertexPositions3D.size()) {
        startX = vertexPositions3D[i].x();
        startY = vertexPositions3D[i].y();
        startZ = vertexPositions3D[i].z();
    } else {
        double radius = rngPtr->getDouble(500, 2400);
        double angle = rngPtr->getDouble(0, 2 * M_PI);
        startX = radius * std::cos(angle);
        startY = rngPtr->getDouble(-50.0, 50.0);
        startZ = radius * std::sin(angle);
    }

    wrapper->setPosition(startX, startY, startZ);

    btRigidBody* rb = wrapper->getRigidBody();
    rb->setActivationState(DISABLE_DEACTIVATION);
    rb->setDamping(0.0, 0.0);
    rb->setCollisionFlags(rb->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
    rb->setLinearFactor(btVector3(1, 1, 1));

    double rHoriz = std::sqrt(startX * startX + startZ * startZ);
    if (rHoriz < minRadHoriz) rHoriz = minRadHoriz;

    double vMag = std::sqrt((G_const * simBlackHoleMass) / rHoriz) * velFactor;

    btVector3 radial((btScalar)startX, (btScalar)startY, (btScalar)startZ);

    btVector3 tangential(-startZ, 0, startX);
    tangential.normalize();
    tangential *= vMag;
    rb->setLinearVelocity(tangential);

    if (tangential.length2() < 1e-6) {
        tangential = btVector3(1, 0, 0).cross(radial);
    }

    tangential.normalize();
    tangential *= vMag;

    rb->setLinearVelocity(tangential);

    physicsController->addCelestialBody(wrapper);
}

    simulationTimer->start(16);
}

void GalaxyView3D::onPhysicsTimerTick() {
    if (!physicsController || !celestialModelPtr) return;

    const double dt = 1.0 / 60.0;
    for (int i = 0; i < 2; ++i) {
        physicsController->simulateStep(dt);
    }

    std::vector<double> xPos, yPos, zPos;
    int nObjects = static_cast<int>(galaxy ? galaxy->getObject().size() : 0);

    xPos.resize(nObjects);
    yPos.resize(nObjects);
    zPos.resize(nObjects);

    for (int i = 0; i < nObjects; ++i) {
        CelestialBodyToRigidWrapper* w = nullptr;
        if (i < static_cast<int>(wrappersMap3D.size())) w = wrappersMap3D[i];

        if (w) {
            double physX = w->getX();
            double physY = w->getY();
            double physZ = w->getZ();

            xPos[i] = physX * viewScale;
            yPos[i] = physY * viewScale;
            zPos[i] = physZ * viewScale;
        }
        else {
            if (i < static_cast<int>(vertexPositions3D.size())) {
                xPos[i] = vertexPositions3D[i].x() * viewScale;
                yPos[i] = vertexPositions3D[i].y() * viewScale;
                zPos[i] = vertexPositions3D[i].z() * viewScale;
            }
        }
    }

    if (celestialModelPtr) {
        celestialModelPtr->updatePositions(xPos, yPos, zPos);
    }
    if (detailedVertexId != -1 && quickWidget && quickWidget->rootObject()) {
        double targetX = 0, targetY = 0, targetZ = 0;

        if (detailedVertexId < static_cast<int>(wrappersMap3D.size()) && wrappersMap3D[detailedVertexId]) {
            targetX = wrappersMap3D[detailedVertexId]->getX() * viewScale;
            targetY = wrappersMap3D[detailedVertexId]->getY() * viewScale;
            targetZ = wrappersMap3D[detailedVertexId]->getZ() * viewScale;
        } else if (detailedVertexId < static_cast<int>(vertexPositions3D.size())) {
            targetX = vertexPositions3D[detailedVertexId].x() * viewScale;
            targetY = vertexPositions3D[detailedVertexId].y() * viewScale;
            targetZ = vertexPositions3D[detailedVertexId].z() * viewScale;
        }

        const double qmlScale = 0.3;
        targetX *= qmlScale;
        targetY *= qmlScale;
        targetZ *= qmlScale;

        QMetaObject::invokeMethod(quickWidget->rootObject(), "updateTargetPosition",
            Q_ARG(QVariant, targetX),
            Q_ARG(QVariant, targetY),
            Q_ARG(QVariant, targetZ));
    }
    if (isPathActive && startNodeId != -1 && endNodeId != -1) {
        calculateShortestPath();
    }

}

void GalaxyView3D::on_vertexDoubleClicked(int vertexId) {
    if (!galaxy || vertexId < 0 || vertexId >= galaxy->getObject().size()) return;

    detailedVertexId = vertexId;

    CelestialObject *obj = galaxy->getObject()[vertexId];
    qDebug() << "Selected 3D object:" << QString::fromStdString(obj->getName());

    double targetX = 0, targetY = 0, targetZ = 0;

    if (vertexId < wrappersMap3D.size() && wrappersMap3D[vertexId]) {
        targetX = wrappersMap3D[vertexId]->getX() * viewScale;
        targetY = wrappersMap3D[vertexId]->getY() * viewScale;
        targetZ = wrappersMap3D[vertexId]->getZ() * viewScale;
    } else if (vertexId < vertexPositions3D.size()) {
        targetX = vertexPositions3D[vertexId].x() * viewScale;
        targetY = vertexPositions3D[vertexId].y() * viewScale;
        targetZ = vertexPositions3D[vertexId].z() * viewScale;
    }

    const double qmlScale = 0.3;
    targetX *= qmlScale;
    targetY *= qmlScale;
    targetZ *= qmlScale;

    QMetaObject::invokeMethod(quickWidget->rootObject(), "cameraZoomTo",
        Q_ARG(QVariant, targetX),
        Q_ARG(QVariant, targetY),
        Q_ARG(QVariant, targetZ));

    QColor objColor = Qt::white;
    int objType = 0;
    QString texturePath = "";

    if (obj->getType() == "StarSystem") {
        objType = 1;
        StarSystem* system = dynamic_cast<StarSystem*>(obj);
        planetModelPtr->updateSystem(system);
        objColor = getStarColorByType(system->getStar().getStarType());
        texturePath = "";

    } else if (obj->getType() == "Nebula") {
        objType = 2;
        Nebula* nebula = dynamic_cast<Nebula*>(obj);
        planetModelPtr->clear();
        switch(nebula->getNebulaType()) {
            case Nebula::nebulaType::Emission: objColor = QColor(255, 0, 127); break;
            case Nebula::nebulaType::Reflection: objColor = QColor(100, 149, 237); break;
            case Nebula::nebulaType::Dark: objColor = QColor(40, 40, 40); break;
            case Nebula::nebulaType::Supernova: objColor = QColor(255, 69, 0); break;
            case Nebula::nebulaType::Planetary: objColor = QColor(0, 255, 127); break;
            default: objColor = QColor("purple"); break;
        }
        texturePath = "qrc:/3DView/textures/nebula.png";
    }
    QMetaObject::invokeMethod(quickWidget->rootObject(), "setCurrentSystemType",
        Q_ARG(QVariant, objType));

    QMetaObject::invokeMethod(quickWidget->rootObject(), "setStarColor",
        Q_ARG(QVariant, objColor.redF()),
        Q_ARG(QVariant, objColor.greenF()),
        Q_ARG(QVariant, objColor.blueF()));

    QMetaObject::invokeMethod(quickWidget->rootObject(), "setCentralTexture",
    Q_ARG(QVariant, texturePath));

    // showObjectParameters(obj);
    ui->galaxyNameLabel->hide();

    if (zoomOutButton) {
        zoomOutButton->show();
        zoomOutButton->raise();
    }

    disconnect(editButton, &QPushButton::clicked, this, &GalaxyView3D::on_editButton_clicked);
    connect(editButton, &QPushButton::clicked, this, &GalaxyView3D::on_editObjectButton_clicked);
}

void GalaxyView3D::showObjectParameters(CelestialObject *obj) {
    if (!obj || !infoText) return;

    QString parametersText;

    if (obj->getType() == "StarSystem") {
        StarSystem *system = dynamic_cast<StarSystem *>(obj);
        if (system) {
            for (Planet* planet : system->getPlanets()) {
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
                Planet *planet = system->getPlanets()[i];
                parametersText += QString::fromStdString(
                    "\nPlanet " + std::to_string(i + 1) + ":\n" +
                    "  Name: " + planet->getName() + "\n" +
                    "  Mass: " + std::to_string(planet->getMass()) + " Earth masses\n" +
                    "  Orbit: " + std::to_string(planet->getOrbit()) + " AU\n" +
                    "  Habitable: " + (planet->isHabitable() ? "Yes" : "No") + "\n"
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

    QLabel *titleLabel = paramsWindow->findChild<QLabel *>("titleLabel");
    if (titleLabel) {
        titleLabel->setText(QString::fromStdString(obj->getType()) + ": " +
                            QString::fromStdString(obj->getName()));
    }

    paramsWindow->show();
    paramsWindow->raise();

    if (editButton) {
        disconnect(editButton, &QPushButton::clicked, nullptr, nullptr);

        connect(editButton, &QPushButton::clicked, this, &GalaxyView3D::on_editObjectButton_clicked);
    }
}
void GalaxyView3D::on_paramsButton_clicked() {
    if (paramsWindow->isVisible()) {
        paramsWindow->hide();
    } else {
        if (detailedVertexId != -1) {
            CelestialObject* obj = galaxy->getObject()[detailedVertexId];
            showObjectParameters(obj);

            if (zoomOutButton) zoomOutButton->show();
        }
        else {
            updateParametersWindow();

            if (zoomOutButton) zoomOutButton->hide();
        }

        paramsWindow->show();
        paramsWindow->raise();
    }
}

void GalaxyView3D::on_editButton_clicked() {
    if (!galaxy) return;

    GalaxyEditDialog dlg(galaxy, rngPtr, dataPtr, this);

    connect(&dlg, &GalaxyEditDialog::galaxyModified, this, [this]() {
        updateParametersWindow();
        checkForNewObjects();
        ui->galaxyNameLabel->setText(QString::fromStdString(galaxy->getName()));
    });

    if (dlg.exec() == QDialog::Accepted) {
        updateParametersWindow();
        checkForNewObjects();
        ui->galaxyNameLabel->setText(QString::fromStdString(galaxy->getName()));
    }
}

void GalaxyView3D::checkForNewObjects() {
    if (!galaxy || !physicsController) return;

    int totalObjects = static_cast<int>(galaxy->getObject().size());
    int knownObjects = static_cast<int>(wrappersMap3D.size());

    if (totalObjects > knownObjects) {
        qDebug() << "Found new objects in 3D View! Adding physics...";

        wrappersMap3D.resize(totalObjects, nullptr);

        for (int i = knownObjects; i < totalObjects; ++i) {
            CelestialObject *obj = galaxy->getObject()[i];

            auto *wrapper = new CelestialBodyToRigidWrapper(obj, physicsEngine->getWorld());
            wrappersMap3D[i] = wrapper;

            const double galaxyRadius = 4000.0;
            const double minSpawnRadius = 300.0;

            double rFactor = std::pow(rngPtr->getDouble(0, 1.0), 2.0);
            double r = minSpawnRadius + rFactor * (galaxyRadius - minSpawnRadius);
            double theta = rngPtr->getDouble(0, 2 * M_PI);
            double angle = theta + (r / (galaxyRadius * 0.2));

            double y = (rngPtr->getDouble(-1.0, 1.0) * rngPtr->getDouble(-1.0, 1.0)) * 40.0; // Трохи товщини
            double x = r * std::cos(angle);
            double z = r * std::sin(angle);

            setupPhysicsForBody(wrapper, x, y, z);

            physicsController->addCelestialBody(wrapper);

            vertexPositions3D.emplace_back(x, y, z);
        }

        std::vector<double> xPos, yPos, zPos;
        for (const auto& pos : vertexPositions3D) {
            xPos.push_back(pos.x());
            yPos.push_back(pos.y());
            zPos.push_back(pos.z());
        }

        celestialModelPtr->updateObjects(galaxy->getObject());
        celestialModelPtr->updatePositions(xPos, yPos, zPos);
    }
}

void GalaxyView3D::setupPhysicsForBody(CelestialBodyToRigidWrapper* wrapper, double x, double y, double z) {
    wrapper->setPosition(x, y, z);

    btRigidBody* rb = wrapper->getRigidBody();
    rb->setActivationState(DISABLE_DEACTIVATION);
    rb->setDamping(0.0, 0.0);
    rb->setCollisionFlags(rb->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
    rb->setLinearFactor(btVector3(1, 1, 1));

    double realBlackHoleMass = 1.3e12;
    double simBlackHoleMass = realBlackHoleMass * 1.0e-7;
    const double G_const = 1.0;
    const double velFactor = 0.85;

    double rHoriz = std::sqrt(x * x + z * z);
    if (rHoriz < 10.0) rHoriz = 10.0;

    double vMag = std::sqrt((G_const * simBlackHoleMass) / rHoriz) * velFactor;

    btVector3 tangential(-z, 0, x);
    tangential.normalize();
    tangential *= vMag;

    if (tangential.length2() < 1e-6) {
        btVector3 radial((btScalar)x, (btScalar)y, (btScalar)z);
        tangential = btVector3(1, 0, 0).cross(radial);
        tangential.normalize();
        tangential *= vMag;
    }

    rb->setLinearVelocity(tangential);
}
void GalaxyView3D::on_zoomOutButton_clicked() {
    qDebug() << "Zoom out clicked";

    QMetaObject::invokeMethod(quickWidget->rootObject(), "cameraReset");

    zoomOutButton->hide();

    disconnect(editButton, &QPushButton::clicked, this, &GalaxyView3D::on_editObjectButton_clicked);
    connect(editButton, &QPushButton::clicked, this, &GalaxyView3D::on_editButton_clicked);

    if (paramsWindow) paramsWindow->hide();
    ui->galaxyNameLabel->show();

    detailedVertexId = -1;
    planetModelPtr->clear();
}
void GalaxyView3D::on_editObjectButton_clicked() {
    if (!galaxy || detailedVertexId < 0 || detailedVertexId >= galaxy->getObject().size()) return;

    CelestialObject *obj = galaxy->getObject()[detailedVertexId];

    if (obj->getType() == "StarSystem") {
        editStarSystem(dynamic_cast<StarSystem*>(obj));
    }
    else if (obj->getType() == "Nebula") {
        editNebula(dynamic_cast<Nebula*>(obj));
    }
    else {
        qDebug() << "Editing not supported for type:" << QString::fromStdString(obj->getType());
    }
}

void GalaxyView3D::editStarSystem(StarSystem* system) {
    if (!system || !rngPtr || !dataPtr) return;

    EditStarSystemDialog dlg(system, rngPtr, dataPtr, this);

    if (dlg.exec() == QDialog::Accepted) {
        dlg.saveChanges();

        showObjectParameters(system);
        celestialModelPtr->updateObjects(galaxy->getObject());
        planetModelPtr->updateSystem(system);

        QColor sColor = getStarColorByType(system->getStar().getStarType());
        QString texturePath = "";

        QMetaObject::invokeMethod(quickWidget->rootObject(), "setStarColor",
            Q_ARG(QVariant, sColor.redF()),
            Q_ARG(QVariant, sColor.greenF()),
            Q_ARG(QVariant, sColor.blueF()));

        QMetaObject::invokeMethod(quickWidget->rootObject(), "setCentralTexture",
            Q_ARG(QVariant, texturePath));
    }
}

void GalaxyView3D::editNebula(Nebula* nebula) {
    if (!nebula) return;

    EditNebulaDialog dlg(nebula, this);

    if (dlg.exec() == QDialog::Accepted) {
        dlg.saveChanges();

        showObjectParameters(nebula);
        celestialModelPtr->updateObjects(galaxy->getObject());

        QColor objColor = Qt::white;
        QString texturePath = "qrc:/3DView/textures/nebula.png";

        switch(nebula->getNebulaType()) {
            case Nebula::nebulaType::Emission: objColor = QColor(255, 0, 127); break;
            case Nebula::nebulaType::Reflection: objColor = QColor(100, 149, 237); break;
            case Nebula::nebulaType::Dark: objColor = QColor(40, 40, 40); break;
            case Nebula::nebulaType::Supernova: objColor = QColor(255, 69, 0); break;
            case Nebula::nebulaType::Planetary: objColor = QColor(0, 255, 127); break;
            default: objColor = QColor("purple"); break;
        }

        QMetaObject::invokeMethod(quickWidget->rootObject(), "setStarColor",
            Q_ARG(QVariant, objColor.redF()),
            Q_ARG(QVariant, objColor.greenF()),
            Q_ARG(QVariant, objColor.blueF()));

        QMetaObject::invokeMethod(quickWidget->rootObject(), "setCentralTexture",
            Q_ARG(QVariant, texturePath));
    }
}
void GalaxyView3D::updateParametersWindow() {
    QTextEdit *infoTextWidget = paramsWindow->findChild<QTextEdit *>("infoTextWidget");

    if (!infoTextWidget) return;

    if (galaxy) {
        infoTextWidget->setText(galaxy->getGalaxyParameters());
    } else {
        infoTextWidget->setText("Cannot load parameters: Galaxy object is missing.");
    }

    QLabel *titleLabel = paramsWindow->findChild<QLabel *>("titleLabel");
    if (titleLabel) {
        titleLabel->setText("Galaxy Parameters:");
    }


    if (editButton) {
        disconnect(editButton, &QPushButton::clicked, nullptr, nullptr);

        connect(editButton, &QPushButton::clicked, this, &GalaxyView3D::on_editButton_clicked);
    }
}

QColor GalaxyView3D::getStarColorByType(Star::starType type) {
    switch (type) {
        case Star::starType::Red_Giant: return QColor(255, 60, 60);
        case Star::starType::Red_Dwarf: return QColor(255, 100, 50);
        case Star::starType::White_Dwarf: return QColor(200, 240, 255);
        case Star::starType::Neutron_Star: return QColor(100, 150, 255);
        case Star::starType::Brown_Dwarf: return QColor(120, 60, 20);
        case Star::starType::Main_sequence_Star: return QColor(255, 220, 100);
        default: return QColor(255, 255, 255);
    }
}

void GalaxyView3D::onVertexClicked(int vertexId) {
    if (!galaxy || vertexId < 0) return;

    std::string objName = galaxy->getObject()[vertexId]->getName();

    if (startNodeId != -1 && endNodeId != -1) {
        resetPathSelection();
        startNodeId = vertexId;
        pathInfoWidget->show();
        pathStatusLabel->setText("Start Selected");
        pathDetailsLabel->setText("Start: " + QString::fromStdString(objName) + "\nChoose Destination...");
        pathDistanceLabel->setText("");
        return;
    }

    if (startNodeId == -1) {
        startNodeId = vertexId;
        pathInfoWidget->show();
        pathStatusLabel->setText("Start Selected");
        pathDetailsLabel->setText("Start: " + QString::fromStdString(objName) + "\nChoose Destination...");
        pathDistanceLabel->clear();
        isPathActive = false;
    } else {
        if (vertexId == startNodeId) return;
        endNodeId = vertexId;
        std::string startName = galaxy->getObject()[startNodeId]->getName();

        pathStatusLabel->setText("Path Calculated");
        pathDetailsLabel->setText(QString("From: %1\nTo: %2").arg(QString::fromStdString(startName), QString::fromStdString(objName)));

        calculateShortestPath();
        isPathActive = true;
    }
}
void GalaxyView3D::calculateShortestPath() {
    DijkstraPathList<CelestialObject*> solver;
    std::vector<int> pathIndices = solver.findShortestPath(galaxy->getGraph(), startNodeId, endNodeId);

    if (pathIndices.empty()) {
        pathStatusLabel->setText("No Path Found");
        return;
    }

    double realTimeDistance = 0.0;
    QVariantList qmlSegments;

    for (size_t i = 0; i < pathIndices.size() - 1; ++i) {
        int u = pathIndices[i];
        int v = pathIndices[i+1];

        QVector3D p1 = getObjectPosition(u);
        QVector3D p2 = getObjectPosition(v);

        realTimeDistance += p1.distanceToPoint(p2);

        QVariantMap seg;
        seg["x1"] = p1.x(); seg["y1"] = p1.y(); seg["z1"] = p1.z();
        seg["x2"] = p2.x(); seg["y2"] = p2.y(); seg["z2"] = p2.z();
        qmlSegments.append(seg);
    }

    pathDistanceLabel->setText("Distance: " + QString::number((int)realTimeDistance));

    QMetaObject::invokeMethod(quickWidget->rootObject(), "drawPath", Q_ARG(QVariant, qmlSegments));
}
void GalaxyView3D::resetPathSelection() {
    startNodeId = -1;
    endNodeId = -1;
    isPathActive = false;
    if (pathInfoWidget) pathInfoWidget->hide();

    QMetaObject::invokeMethod(quickWidget->rootObject(), "clearPath");
}

void GalaxyView3D::onBackgroundClicked() {
    resetPathSelection();
}

QVector3D GalaxyView3D::getObjectPosition(int index) {
    double x = 0, y = 0, z = 0;

    if (index < wrappersMap3D.size() && wrappersMap3D[index]) {
        x = wrappersMap3D[index]->getX();
        y = wrappersMap3D[index]->getY();
        z = wrappersMap3D[index]->getZ();
    }
    else if (index < vertexPositions3D.size()) {
        x = vertexPositions3D[index].x();
        y = vertexPositions3D[index].y();
        z = vertexPositions3D[index].z();
    }

    return QVector3D(x * viewScale, y * viewScale, z * viewScale);
}

void GalaxyView3D::applySpaceStyle() {
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

    paramsButton->setStyleSheet(btnStyle);
    editButton->setStyleSheet(btnStyle);
    zoomOutButton->setStyleSheet(btnStyle);

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

    paramsWindow->setStyleSheet(windowStyle);

    editButton->setStyleSheet(btnStyle);
}