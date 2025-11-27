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

    galaxy = nullptr;
    std::vector<CelestialObject*> emptyObjects;

    quickWidget = new QQuickWidget(this);
    quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

    if (!ui->graphArea->layout()) {
        ui->graphArea->setLayout(new QVBoxLayout());
    }
    ui->graphArea->layout()->addWidget(quickWidget);

    celestialModelPtr = new CelestialObject3DModel(quickWidget, emptyObjects);

    quickWidget->rootContext()->setContextProperty("celestialModel", celestialModelPtr);
    quickWidget->setSource(QUrl("qrc:/Galaxy3DView.qml"));

    QQuickItem *rootObject = quickWidget->rootObject();
    if (rootObject) {
        connect(rootObject, SIGNAL(objectClicked(int)),
                this, SLOT(on_vertexDoubleClicked(int)));
    }

    simulationTimer = new QTimer(this);
    connect(simulationTimer, &QTimer::timeout, this, &GalaxyView3D::onPhysicsTimerTick);

    paramsButton = new QPushButton("⚙️ Parameters", this);
    paramsButton->setStyleSheet("font-size: 14px; padding: 5px 10px;");

    paramsWindow = new QWidget(this);
    paramsWindow->setStyleSheet(
        "background-color: #333; color: white; border: 1px solid #555; border-radius: 5px;"
    );
    paramsWindow->hide();

    QVBoxLayout *paramsLayout = new QVBoxLayout(paramsWindow);
    paramsLayout->setContentsMargins(10, 10, 10, 10);

    QLabel *titleLabel = new QLabel("Object Info: \n", paramsWindow);
    titleLabel->setObjectName("titleLabel");
    titleLabel->setStyleSheet("font-weight: bold; font-size: 16px; margin-bottom: 1px;");

    infoText = new QTextEdit(paramsWindow);
    infoText->setReadOnly(true);
    infoText->setStyleSheet("background-color: #444; color: #eee; border: none;");

    editButton = new QPushButton("✎ Edit..", paramsWindow);
    editButton->setStyleSheet("font-size: 14px; padding: 5px 10px;");

    paramsLayout->addWidget(titleLabel);
    paramsLayout->addWidget(infoText);
    paramsLayout->addWidget(editButton);
    paramsWindow->setLayout(paramsLayout);

    paramsButton->resize(150, 35);
    paramsWindow->resize(250, 210);

    zoomOutButton = new QPushButton("⬅ Zoom Out", this);
    zoomOutButton->setStyleSheet("font-size: 14px; padding: 5px 10px;");
    zoomOutButton->hide();

    connect(paramsButton, &QPushButton::clicked, this, &GalaxyView3D::on_paramsButton_clicked);
    connect(zoomOutButton, &QPushButton::clicked, this, &GalaxyView3D::on_zoomOutButton_clicked);
    connect(editButton, &QPushButton::clicked, this, &GalaxyView3D::on_editButton_clicked);
    connect(simulationTimer, &QTimer::timeout, this, &GalaxyView3D::onPhysicsTimerTick);
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

void GalaxyView3D::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    if (paramsButton && paramsWindow) {
        int margin = 20;
        int btn_x = width() - paramsButton->width() - margin;
        int btn_y = margin;
        paramsButton->move(btn_x, btn_y);

        int win_x = width() - paramsWindow->width() - margin;
        int win_y = btn_y + paramsButton->height() + 10;
        paramsWindow->move(win_x, win_y);
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

        double radius = std::pow(rng.getDouble(0, 1.0), 2.0) * galaxyRadius;

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


    // for (const auto &edge : galaxy->getGraph().getEdges()) {
    //     if (!edge.isActive()) continue;
    //
    //     int u = edge.from;
    //     int v = edge.to;
    //
    //     if (u < wrappersMap.size() && v < wrappersMap.size() && wrappersMap[u] && wrappersMap[v]) {
    //
    //         double dx = wrappersMap[u]->getX() - wrappersMap[v]->getX();
    //         double dy = wrappersMap[u]->getY() - wrappersMap[v]->getY();
    //         double currentDist = std::sqrt(dx*dx + dy*dy);
    //
    //         physicsController->addSpring(
    //             wrappersMap[u],
    //             wrappersMap[v],
    //             currentDist
    //         );
    //     }
    // }

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
}

void GalaxyView3D::on_vertexDoubleClicked(int vertexId) {
    if (!galaxy || vertexId < 0 || vertexId >= galaxy->getObject().size()) return;

    CelestialObject *obj = galaxy->getObject()[vertexId];

    qDebug() << "Selected 3D object:" << QString::fromStdString(obj->getName());

    showObjectParameters(obj);
    //
    // if (paramsWindow) {
    //     paramsWindow->hide();
    // }

    ui->galaxyNameLabel->hide();
    disconnect(editButton, &QPushButton::clicked, this, &GalaxyView3D::on_editButton_clicked);
    connect(editButton, &QPushButton::clicked, this, &GalaxyView3D::on_editObjectButton_clicked);
}

void GalaxyView3D::showObjectParameters(CelestialObject *obj) {
    if (!obj || !infoText) return;

    QString parametersText;

    if (obj->getType() == "StarSystem") {
        StarSystem *system = dynamic_cast<StarSystem *>(obj);
        if (system) {
            for (Planet &planet: system->getPlanets()) {
                system->lifeExists(planet);
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
}
void GalaxyView3D::on_paramsButton_clicked() {
    if (paramsWindow->isVisible()) {
        paramsWindow->hide();
    } else {
        paramsWindow->show();
        paramsWindow->raise();
    }
}

void GalaxyView3D::on_editButton_clicked() { qDebug() << "edit clicked"; }
void GalaxyView3D::on_zoomOutButton_clicked() { qDebug() << "zoom out"; }
void GalaxyView3D::on_editObjectButton_clicked() { qDebug() << "edit obj"; }
void GalaxyView3D::editStarSystem(StarSystem* system) {
    //not added yet
    }
void GalaxyView3D::editNebula(Nebula* nebula) {
    //not added yet
    }