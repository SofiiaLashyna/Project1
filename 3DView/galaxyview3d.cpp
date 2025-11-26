// #include "galaxyview3d.h"
// #include "ui_GalaxyView3D.h"
// #include "CelestialObject3DModel.h"
// #include <QQuickWidget>
// #include <QQmlContext>
// #include <QQuickItem>
// #include <QDebug>
// #include <QMessageBox>
// #include <QTextEdit>
// #include <QTimer>
// #include <QVector3D>
//
// #include "EditNebulaDialog.h"
// #include "EditStarSystemDialog.h"
// #include "GalaxyEditDialog.h"
// static constexpr double PHYSICS_MASS_SCALE = 100.0;
//
// GalaxyView3D::GalaxyView3D(QWidget *parent)
//     : QWidget(parent), ui(new Ui::GalaxyView3D) {
//     ui->setupUi(this);
//
//     simulationTimer = new QTimer(this);
//     connect(simulationTimer, &QTimer::timeout, this, &GalaxyView3D::onPhysicsTimerTick);
//
//     galaxy = nullptr;
//     std::vector<CelestialObject*> emptyObjects;
//
//     quickWidget = new QQuickWidget(this);
//     quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
//
//     if (!ui->graphArea->layout()) {
//         ui->graphArea->setLayout(new QVBoxLayout());
//     }
//     ui->graphArea->layout()->addWidget(quickWidget);
//
//     celestialModelPtr = new CelestialObject3DModel(quickWidget, emptyObjects);
//
//     quickWidget->rootContext()->setContextProperty("celestialModel", celestialModelPtr);
//     quickWidget->setSource(QUrl("qrc:/Galaxy3DView.qml"));
//
//     paramsButton = new QPushButton("⚙️ Parameters", this);
//     paramsButton->setStyleSheet("font-size: 14px; padding: 5px 10px; background-color: rgba(50, 50, 50, 200); color: white; border: 1px solid #777; border-radius: 5px;");
//     paramsButton->show(); // Явно показуємо
//
//     zoomOutButton = new QPushButton("⬅ Zoom Out", this);
//     zoomOutButton->setStyleSheet("font-size: 14px; padding: 5px 10px; background-color: rgba(50, 50, 50, 200); color: white; border: 1px solid #777; border-radius: 5px;");
//     zoomOutButton->hide(); // Спочатку ховаємо
//
//     // Створення вікна параметрів
//     paramsWindow = new QWidget(this);
//     paramsWindow->setStyleSheet("background-color: rgba(30, 30, 30, 220); color: white; border: 1px solid #555; border-radius: 5px;");
//
//     QVBoxLayout *paramsLayout = new QVBoxLayout(paramsWindow);
//     paramsLayout->setContentsMargins(10, 10, 10, 10);
//     paramsLayout->setSpacing(5);
//
//     QLabel *titleLabel = new QLabel("Galaxy Parameters: \n", paramsWindow);
//     titleLabel->setStyleSheet("font-weight: bold; font-size: 16px; margin-bottom: 1px; background: transparent;");
//
//     QTextEdit *infoText = new QTextEdit(paramsWindow);
//     infoText->setObjectName("infoTextWidget");
//     infoText->setReadOnly(true);
//     infoText->setText("Galaxy not generated yet.");
//     infoText->setStyleSheet("background-color: transparent; color: #eee; border: none;");
//
//     editButton = new QPushButton("✎ Edit..", paramsWindow);
//     editButton->setStyleSheet("font-size: 14px; padding: 5px 10px; background-color: #555;");
//
//     paramsLayout->addWidget(titleLabel);
//     paramsLayout->addWidget(infoText);
//     paramsLayout->addWidget(editButton);
//     paramsWindow->setLayout(paramsLayout);
//
//     paramsWindow->resize(250, 210);
//     paramsWindow->hide();
//
//     // Підключаємо старі слоти
//     connect(paramsButton, &QPushButton::clicked, this, &GalaxyView3D::on_paramsButton_clicked);
//     connect(zoomOutButton, &QPushButton::clicked, this, &GalaxyView3D::on_zoomOutButton_clicked);
//     QQuickItem *rootObject = quickWidget->rootObject();
//     if (rootObject) {
//         connect(rootObject, SIGNAL(objectClicked(int)),
//                 this, SLOT(on_vertexDoubleClicked(int)));
//     }
//     connect(editButton, &QPushButton::clicked,
//             this, &GalaxyView3D::on_editButton_clicked);
//
// }
//
// GalaxyView3D::~GalaxyView3D() {
//     if (galaxy) {
//         for (CelestialObject* obj : galaxy->getObject()) {
//             delete obj;
//         }
//         delete galaxy;
//     }
//     delete ui;
// }
//
// void GalaxyView3D::resizeEvent(QResizeEvent *event) {
//     QWidget::resizeEvent(event);
//
//     // 1. Розтягуємо QQuickWidget на все вікно
//     if (quickWidget) {
//         quickWidget->resize(this->size());
//     }
//
//     // 2. Позиціонуємо кнопки (Parameters, Zoom Out) поверх QML
//     if (paramsButton && paramsWindow) {
//         int margin = 20;
//
//         // Кнопка параметрів справа зверху
//         int btn_x = width() - paramsButton->width() - margin;
//         int btn_y = margin;
//         paramsButton->move(btn_x, btn_y);
//         paramsButton->raise(); // Важливо! Піднімаємо кнопку над QQuickWidget
//
//         // Вікно параметрів під кнопкою
//         int win_x = width() - paramsWindow->width() - margin;
//         int win_y = btn_y + paramsButton->height() + 10;
//         paramsWindow->move(win_x, win_y);
//         paramsWindow->raise();
//
//         // Кнопка Zoom Out (зліва зверху)
//         if (zoomOutButton) {
//             zoomOutButton->resize(150, 35);
//             int zoom_x = margin;
//             int zoom_y = margin;
//             zoomOutButton->move(zoom_x, zoom_y);
//             zoomOutButton->raise();
//         }
//
//         // Кнопка Edit (у вікні параметрів) - вона рухається разом з вікном
//     }
// }
// void GalaxyView3D::generateAndDisplayGalaxy(const nlohmann::json &data, RandomGenerator &rng) {
//     qDebug() << "Generating and displaying galaxy in GalaxyView3D...";
//
//     if (galaxy) {
//         for (CelestialObject* obj : galaxy->getObject()) {
//             delete obj;
//         }
//         delete galaxy;
//         galaxy = nullptr;
//     }
//
//     std::string galaxyNameFile = "Unnamed";
//     if (data.contains("Galaxy") && data["Galaxy"].is_array() && !data["Galaxy"].empty()) {
//         galaxyNameFile = data["Galaxy"][0].get<std::string>();
//     }
//     std::string randomGalaxyName = rng.getRandomNameFromFile(galaxyNameFile);
//
//     galaxy = new Galaxy<GraphList<CelestialObject *> >(randomGalaxyName);
//     galaxy->generateGalaxy(data, rng);
//
//     this->dataPtr = const_cast<nlohmann::json *>(&data);
//     this->rngPtr = &rng;
//
//     vertexPositions3D.clear();
//     const int nVerticesTotal = static_cast<int>(galaxy->getObject().size());
//
//     const double galaxyRadius = 100000.0;
//
//     const double galaxyThickness = galaxyRadius * 0.01;
//     const double centerX = 0.0, centerY = 0.0, centerZ = 0.0;
//
//     int n_active = 0;
//     for (int i = 0; i < nVerticesTotal; ++i) {
//
//         // double radius = std::pow(rng.getDouble(0, 1.0), 2.0) * galaxyRadius;
//         double radius = rng.getDouble(0.1, 1.0) * galaxyRadius;
//         double theta = rng.getDouble(0, 2 * M_PI);
//
//         double angle = theta + (radius / (galaxyRadius * 0.2));
//
//         double y_factor = rng.getDouble(-1.0, 1.0) * rng.getDouble(-1.0, 1.0);
//         double y = centerY + y_factor * galaxyThickness;
//
//         double x = centerX + radius * std::cos(angle);
//         double z = centerZ + radius * std::sin(angle);
//
//         vertexPositions3D.emplace_back(x, y, z);
//         n_active++;
//     }
//
//     std::vector<CelestialObject*>& currentObjects = galaxy->getObject();
//
//     // Важливо: переконатися, що вектори розмірності співпадають
//     std::vector<double> xPos, yPos, zPos;
//     xPos.reserve(vertexPositions3D.size());
//     yPos.reserve(vertexPositions3D.size());
//     zPos.reserve(vertexPositions3D.size());
//
//     for (const auto& pos : vertexPositions3D) {
//         xPos.push_back(pos.x());
//         yPos.push_back(pos.y());
//         zPos.push_back(pos.z());
//     }
//
//     // 1. Оновлюємо список об'єктів (кольори, типи)
//     celestialModelPtr->updateObjects(currentObjects);
//     // 2. Оновлюємо позиції (щоб вони не були 0,0,0)
//     celestialModelPtr->updatePositions(xPos, yPos, zPos);
//
//     ui->galaxyNameLabel->setText(QString::fromStdString(randomGalaxyName));
//
//     // Тепер запускаємо фізику
//     initPhysicsSimulation();
// }
//
// void GalaxyView3D::initPhysicsSimulation() {
//     if (!simulationTimer) return;
//     simulationTimer->stop();
//
//     // Очищення
//     if (physicsController) { delete physicsController; physicsController = nullptr; }
//     if (physicsEngine) { delete physicsEngine; physicsEngine = nullptr; }
//     if (blackHoleField) { delete blackHoleField; blackHoleField = nullptr; }
//
//     physicsEngine = new PhysicsEngine();
//     physicsController = new GalaxyPhysicsController(physicsEngine);
//
//     double realBlackHoleMass = 1.3e12;
//     double simBlackHoleMass = realBlackHoleMass * PHYSICS_MASS_SCALE;
//
//     blackHoleField = new BlackHoleGravityField(simBlackHoleMass, 0, 0, 0);
//
//     if (!galaxy) return;
//
//     std::vector<CelestialBodyToRigidWrapper*> wrappersMap(galaxy->getObject().size(), nullptr);
//     int nVerticesTotal = galaxy->getObject().size();
//
//
//     const double galaxyRadius = 100000.0;
//     for (int i = 0; i < nVerticesTotal; ++i) {
//         CelestialObject* obj = galaxy->getObject()[i];
//         if (galaxy->getGraph().getVertices()[i].getId() == -1) continue;
//
//         auto* wrapper = new CelestialBodyToRigidWrapper(obj, physicsEngine->getWorld());
//         wrappersMap[i] = wrapper;
//
//         double radius = rngPtr->getDouble(0.1, 1.0) * galaxyRadius;
//         double angle = rngPtr->getDouble(0, 2 * M_PI);
//
//         double x = radius * std::cos(angle);
//         double z = radius * std::sin(angle);
//
//         double y = rngPtr->getDouble(-500.0, 500.0);
//
//         wrapper->setPosition(x, y, z);
//
//         wrapper->getRigidBody()->setDamping(0.95, 0.95);
//         wrapper->getRigidBody()->setActivationState(DISABLE_DEACTIVATION);
//
//         physicsController->addCelestialBody(wrapper);
//     }
//
//     // for (const auto& edge : galaxy->getGraph().getEdges()) {
//     //     if (!edge.isActive()) continue;
//     //     if (wrappersMap[edge.from] && wrappersMap[edge.to]) {
//     //         physicsController->addSpring(wrappersMap[edge.from], wrappersMap[edge.to], edge.weight);
//     //     }
//     // }
//
//     qDebug() << "3D Pre-calc...";
//     for(int i = 0; i < 500; ++i) {
//         physicsController->simulateStep(0.016);
//     }
//
//     physicsController->clearSprings();
//     physicsController->addGravityField(blackHoleField);
//
//     double G = 1.0;
//
//     // for (int i = 0; i < nVerticesTotal; ++i) {
//     //     if (!wrappersMap[i]) continue;
//     //
//     //     auto* wrapper = wrappersMap[i];
//     //     wrapper->updateFromPhysics();
//     //
//     //     wrapper->getRigidBody()->setLinearVelocity(btVector3(0,0,0));
//     //     wrapper->getRigidBody()->setDamping(0.0, 0.0);
//     //
//     //     double x = wrapper->getX();
//     //     double y = wrapper->getY();
//     //     double z = wrapper->getZ();
//     //
//     //     double distOnPlane = std::sqrt(x*x + z*z);
//     //     if (distOnPlane < 10.0) distOnPlane = 10.0;
//     //
//     //     double vMag = std::sqrt((G * simBlackHoleMass) / distOnPlane);
//     //
//     //     btVector3 vel(-z, 0, x);
//     //     vel.normalize();
//     //     vel *= vMag;
//     //
//     //     // Можна додати малесенький рух по Y для коливання, але обережно
//     //     // vel.setY(rngPtr->getDouble(-0.1, 0.1));
//     //
//     //     wrapper->getRigidBody()->setLinearVelocity(vel);
//     // }
//     for (int i = 0; i < nVerticesTotal; ++i) {
//         if (!wrappersMap[i]) continue;
//         auto* wrapper = wrappersMap[i];
//
//         // Скидаємо стару швидкість
//         wrapper->getRigidBody()->setLinearVelocity(btVector3(0,0,0));
//
//         double x = wrapper->getX();
//         double z = wrapper->getZ();
//
//         // Відстань
//         double r = std::sqrt(x*x + z*z);
//         if (r < 100.0) r = 100.0;
//
//         // ФОРМУЛА ОРБІТИ:
//         // Швидкість залежить від маси діри (simBlackHoleMass).
//         // Чим важча діра, тим швидше треба летіти, щоб не впасти.
//         double vOrbital = std::sqrt((G * simBlackHoleMass) / r);
//
//         // Вектор направлений по дотичній до кола
//         // (-z, 0, x) - це вектор перпендикулярний до радіусу (x, 0, z)
//         btVector3 vel(-z, 0, x);
//         vel.normalize();
//         vel *= vOrbital; // Задаємо правильну величину
//
//         wrapper->getRigidBody()->setLinearVelocity(vel);
//     }
//
//     simulationTimer->start(16);
// }
//
// void GalaxyView3D::onPhysicsTimerTick() {
//     if (!physicsController) return;
//
//     int stepsPerFrame = 10;
//     for(int i = 0; i < stepsPerFrame; ++i) {
//         physicsController->simulateStep(0.1);
//     }
//     std::vector<double> xPos, yPos, zPos;
//     const auto& bodies = physicsController->getBodies();
//
//     int bodyIdx = 0;
//     for (size_t i = 0; i < galaxy->getObject().size(); ++i) {
//         // Якщо це пустий об'єкт графа -> координати 0,0,0 (або не малюємо)
//         if (galaxy->getGraph().getVertices()[i].getId() == -1) {
//             xPos.push_back(0); yPos.push_back(0); zPos.push_back(0);
//             continue;
//         }
//
//         if (bodyIdx < bodies.size()) {
//             auto* wrapper = bodies[bodyIdx];
//             xPos.push_back(wrapper->getX());
//             yPos.push_back(wrapper->getY());
//             zPos.push_back(wrapper->getZ());
//             bodyIdx++;
//         } else {
//             // Should not happen
//             xPos.push_back(0); yPos.push_back(0); zPos.push_back(0);
//         }
//     }
//
//     // 3. Оновлюємо модель
//     if (celestialModelPtr) {
//         celestialModelPtr->updatePositions(xPos, yPos, zPos);
//     }
// }

#include "galaxyview3d.h"
#include "ui_GalaxyView3D.h"
#include "CelestialObject3DModel.h"
#include <QQuickWidget>
#include <QQmlContext>
#include <QQuickItem>
#include <QDebug>
#include <QMessageBox>
#include <QTextEdit>
#include <QTimer>
#include <QVector3D>

#include "EditNebulaDialog.h"
#include "EditStarSystemDialog.h"
#include "GalaxyEditDialog.h"

static constexpr double PHYSICS_MASS_SCALE = 1.0e-7;


GalaxyView3D::GalaxyView3D(QWidget *parent)
    : QWidget(parent), ui(new Ui::GalaxyView3D) {
    ui->setupUi(this);

    simulationTimer = new QTimer(this);
    connect(simulationTimer, &QTimer::timeout, this, &GalaxyView3D::onPhysicsTimerTick);

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

    paramsButton = new QPushButton("⚙️ Parameters", this);
    paramsButton->setStyleSheet("font-size: 14px; padding: 5px 10px; background-color: rgba(50, 50, 50, 200); color: white; border: 1px solid #777; border-radius: 5px;");
    paramsButton->show();

    zoomOutButton = new QPushButton("⬅ Zoom Out", this);
    zoomOutButton->setStyleSheet("font-size: 14px; padding: 5px 10px; background-color: rgba(50, 50, 50, 200); color: white; border: 1px solid #777; border-radius: 5px;");
    zoomOutButton->hide();

    paramsWindow = new QWidget(this);
    paramsWindow->setStyleSheet("background-color: rgba(30, 30, 30, 220); color: white; border: 1px solid #555; border-radius: 5px;");

    QVBoxLayout *paramsLayout = new QVBoxLayout(paramsWindow);
    paramsLayout->setContentsMargins(10, 10, 10, 10);
    paramsLayout->setSpacing(5);
    QLabel *titleLabel = new QLabel("Galaxy Parameters: \n", paramsWindow);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 16px; margin-bottom: 1px; background: transparent;");
    QTextEdit *infoText = new QTextEdit(paramsWindow);
    infoText->setObjectName("infoTextWidget");
    infoText->setReadOnly(true);
    infoText->setText("Galaxy not generated yet.");
    infoText->setStyleSheet("background-color: transparent; color: #eee; border: none;");
    editButton = new QPushButton("✎ Edit..", paramsWindow);
    editButton->setStyleSheet("font-size: 14px; padding: 5px 10px; background-color: #555;");
    paramsLayout->addWidget(titleLabel);
    paramsLayout->addWidget(infoText);
    paramsLayout->addWidget(editButton);
    paramsWindow->setLayout(paramsLayout);
    paramsWindow->resize(250, 210);
    paramsWindow->hide();

    connect(paramsButton, &QPushButton::clicked, this, &GalaxyView3D::on_paramsButton_clicked);
    connect(zoomOutButton, &QPushButton::clicked, this, &GalaxyView3D::on_zoomOutButton_clicked);
    QQuickItem *rootObject = quickWidget->rootObject();
    if (rootObject) {
        connect(rootObject, SIGNAL(objectClicked(int)), this, SLOT(on_vertexDoubleClicked(int)));
    }
    connect(editButton, &QPushButton::clicked, this, &GalaxyView3D::on_editButton_clicked);
}

GalaxyView3D::~GalaxyView3D() {
    if (galaxy) {
        for (CelestialObject* obj : galaxy->getObject()) delete obj;
        delete galaxy;
    }
    delete ui;
}

void GalaxyView3D::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    if (quickWidget) quickWidget->resize(this->size());
    if (paramsButton && paramsWindow) {
        int margin = 20;
        paramsButton->move(width() - paramsButton->width() - margin, margin);
        paramsButton->raise();
        paramsWindow->move(width() - paramsWindow->width() - margin, margin + paramsButton->height() + 10);
        paramsWindow->raise();
        if (zoomOutButton) {
            zoomOutButton->move(margin, margin);
            zoomOutButton->raise();
        }
    }
}

void GalaxyView3D::generateAndDisplayGalaxy(const nlohmann::json &data, RandomGenerator &rng) {
    qDebug() << "Generating 3D Galaxy...";
    if (galaxy) {
        for (CelestialObject* obj : galaxy->getObject()) delete obj;
        delete galaxy;
        galaxy = nullptr;
    }

    std::string galaxyNameFile = "Unnamed";
    if (data.contains("Galaxy") && data["Galaxy"].is_array() && !data["Galaxy"].empty()) {
        galaxyNameFile = data["Galaxy"][0].get<std::string>();
    }
    galaxy = new Galaxy<GraphList<CelestialObject *> >(rng.getRandomNameFromFile(galaxyNameFile));
    galaxy->generateGalaxy(data, rng);

    this->dataPtr = const_cast<nlohmann::json *>(&data);
    this->rngPtr = &rng;

    const double displayRadius = 4000.0;
    vertexPositions3D.clear();

    for (size_t i = 0; i < galaxy->getObject().size(); ++i) {
        double radius = rng.getDouble(0.2, 1.0) * displayRadius;
        double theta = rng.getDouble(0, 2 * M_PI);
        double angle = theta + (radius / (displayRadius * 0.2));

        double x = radius * std::cos(angle);
        double z = radius * std::sin(angle);
        double y = rng.getDouble(-300.0, 300.0);

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
    ui->galaxyNameLabel->setText(QString::fromStdString(galaxy->getName()));

    initPhysicsSimulation();
}

void GalaxyView3D::initPhysicsSimulation() {
    if (!simulationTimer) return;
    simulationTimer->stop();

    if (physicsController) { delete physicsController; physicsController = nullptr; }
    if (physicsEngine) { delete physicsEngine; physicsEngine = nullptr; }
    if (blackHoleField) { delete blackHoleField; blackHoleField = nullptr; }

    physicsEngine = new PhysicsEngine();
    physicsController = new GalaxyPhysicsController(physicsEngine);

    //double realBlackHoleMass = 1.3e10; // Середня маса
    //double simBlackHoleMass = realBlackHoleMass * PHYSICS_MASS_SCALE;
    double simBlackHoleMass = 500000.0;
    blackHoleField = new BlackHoleGravityField(simBlackHoleMass, 0, 0, 0);

    std::vector<CelestialBodyToRigidWrapper*> wrappersMap(galaxy->getObject().size(), nullptr);
    int nVerticesTotal = galaxy->getObject().size();

    const double physicsRadius = 4000.0;

    for (int i = 0; i < nVerticesTotal; ++i) {
        CelestialObject* obj = galaxy->getObject()[i];
        if (galaxy->getGraph().getVertices()[i].getId() == -1) continue;

        auto* wrapper = new CelestialBodyToRigidWrapper(obj, physicsEngine->getWorld());
        wrappersMap[i] = wrapper;

        double radius = rngPtr->getDouble(0.2, 1.0) * physicsRadius;
        double angle = rngPtr->getDouble(0, 2 * M_PI);

        double x = radius * std::cos(angle);
        double z = radius * std::sin(angle);
        double y = rngPtr->getDouble(-300.0, 300.0);

        wrapper->setPosition(x, y, z);

        wrapper->getRigidBody()->setDamping(0.9, 0.9);
        wrapper->getRigidBody()->setActivationState(DISABLE_DEACTIVATION);

        physicsController->addCelestialBody(wrapper);
    }

    double springScale3D = 15.0;
    for (const auto& edge : galaxy->getGraph().getEdges()) {
        if (!edge.isActive()) continue;
        if (wrappersMap[edge.from] && wrappersMap[edge.to]) {
            physicsController->addSpring(wrappersMap[edge.from], wrappersMap[edge.to], edge.weight * springScale3D);
        }
    }

    for (auto* wrapper : physicsController->getBodies()) {
        wrapper->getRigidBody()->setDamping(0.9, 0.9);
    }
    qDebug() << "Stabilizing springs...";
    for(int i = 0; i < 300; ++i) {
        physicsController->simulateStep(0.016);
    }

    physicsController->clearSprings();
    physicsController->addGravityField(blackHoleField);

    double G = 1.0;

    for (int i = 0; i < nVerticesTotal; ++i) {
        if (!wrappersMap[i]) continue;
        auto* wrapper = wrappersMap[i];

        wrapper->updateFromPhysics();

        wrapper->getRigidBody()->setLinearVelocity(btVector3(0,0,0));
        wrapper->getRigidBody()->setDamping(0.0, 0.0);
        double x = wrapper->getX();
        double z = wrapper->getZ();

        double r = std::sqrt(x*x + z*z);
        if (r < 50.0) r = 50.0;

        double vOrbital = std::sqrt((G * simBlackHoleMass) / r);

        btVector3 vel(-z, 0, x);
        vel.normalize();
        vel *= vOrbital;

        wrapper->getRigidBody()->setLinearVelocity(vel);
    }

    simulationTimer->start(16);
}

void GalaxyView3D::onPhysicsTimerTick() {
    if (!physicsController) return;

    int stepsPerFrame = 1;

    for(int i = 0; i < stepsPerFrame; ++i) {
        physicsController->simulateStep(0.016);
    }
    std::vector<double> xPos, yPos, zPos;
    const auto& bodies = physicsController->getBodies();

    int bodyIdx = 0;
    for (size_t i = 0; i < galaxy->getObject().size(); ++i) {
        if (galaxy->getGraph().getVertices()[i].getId() == -1) {
            xPos.push_back(0); yPos.push_back(0); zPos.push_back(0);
            continue;
        }
        if (bodyIdx < bodies.size()) {
            auto* wrapper = bodies[bodyIdx];
            xPos.push_back(wrapper->getX());
            yPos.push_back(wrapper->getY());
            zPos.push_back(wrapper->getZ());
            bodyIdx++;
        } else {
            xPos.push_back(0); yPos.push_back(0); zPos.push_back(0);
        }
    }

    if (celestialModelPtr) {
        celestialModelPtr->updatePositions(xPos, yPos, zPos);
    }
}

void GalaxyView3D::updateParametersWindow() {
    QTextEdit *infoText = paramsWindow->findChild<QTextEdit *>("infoTextWidget");

    if (infoText && galaxy) {
        infoText->setText(galaxy->getGalaxyParameters());
    } else if (infoText) {
        infoText->setText("Cannot load parameters.");
    }
}

void GalaxyView3D::updateGraphDisplay() {
    if (celestialModelPtr && galaxy) {
        celestialModelPtr->updateObjects(galaxy->getObject());
    }
}
void GalaxyView3D::on_vertexDoubleClicked(int vertexId) {
    if (!galaxy || vertexId < 0) return;

    detailedVertexId = vertexId;

    if (zoomOutButton) {
        zoomOutButton->show();
        zoomOutButton->raise();
    }

    if (paramsButton) {
        paramsButton->show();
        paramsButton->raise();
    }

    CelestialObject* obj = galaxy->getObject()[vertexId];
    showObjectParameters(obj);
    disconnect(editButton, &QPushButton::clicked, this, &GalaxyView3D::on_editButton_clicked);
    connect(editButton, &QPushButton::clicked, this, &GalaxyView3D::on_editObjectButton_clicked);
}
void GalaxyView3D::onObjectClicked(int index) {
    qDebug() << "3D Object clicked:" << index;
    on_vertexDoubleClicked(index);
}

void GalaxyView3D::on_zoomOutButton_clicked() {
    detailedVertexId = -1;
    zoomOutButton->hide();

    disconnect(editButton, &QPushButton::clicked, this, &GalaxyView3D::on_editObjectButton_clicked);
    connect(editButton, &QPushButton::clicked, this, &GalaxyView3D::on_editButton_clicked);

    QLabel *titleLabel = paramsWindow->findChild<QLabel *>();
    if (titleLabel) titleLabel->setText("Galaxy Parameters: \n");
    updateParametersWindow();

}

void GalaxyView3D::showObjectParameters(CelestialObject *obj) {
    if (!obj || !paramsWindow) return;

    QTextEdit *infoText = paramsWindow->findChild<QTextEdit *>("infoTextWidget");
    if (!infoText) return;

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

    QLabel *titleLabel = paramsWindow->findChild<QLabel *>();
    if (titleLabel) {
        titleLabel->setText(QString::fromStdString(obj->getType()) + ": " +
                            QString::fromStdString(obj->getName()));
    }

    paramsWindow->show();
    paramsWindow->raise();
}

void GalaxyView3D::on_editObjectButton_clicked() {
    if (!galaxy) return;

    if (detailedVertexId < 0 || detailedVertexId >= galaxy->getObject().size()) return;

    CelestialObject *obj = galaxy->getObject()[detailedVertexId];

    if (obj->getType() == "StarSystem") {
        editStarSystem(dynamic_cast<StarSystem *>(obj));
    } else if (obj->getType() == "Nebula") {
        editNebula(dynamic_cast<Nebula *>(obj));
    }
}
void GalaxyView3D::editStarSystem(StarSystem *system) {
    if (!system || !rngPtr || !dataPtr) return;

    EditStarSystemDialog dlg(system, rngPtr, dataPtr, this);

    if (dlg.exec() == QDialog::Accepted) {
        dlg.saveChanges();
        showObjectParameters(system);
        celestialModelPtr->updateObjects(galaxy->getObject());
        QApplication::processEvents();
    }
}

void GalaxyView3D::editNebula(Nebula *nebula) {
    if (!nebula) return;

    EditNebulaDialog dlg(nebula, this);

    if (dlg.exec() == QDialog::Accepted) {
        dlg.saveChanges();
        showObjectParameters(nebula);
        celestialModelPtr->updateObjects(galaxy->getObject());
        QApplication::processEvents();
    }
}

void GalaxyView3D::on_paramsButton_clicked() {
    if (paramsWindow->isVisible()) {
        paramsWindow->hide();
    } else {
        paramsWindow->show();
        paramsWindow->raise();
    }
}

void GalaxyView3D::on_editButton_clicked() {
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

        updateGraphDisplay();

        QApplication::processEvents();
    }
}