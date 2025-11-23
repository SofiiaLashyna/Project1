#include "galaxyview.h"
#include "ui_GalaxyView.h"
#include "GalaxyEditDialog.h"
#include "EditStarSystemDialog.h"
#include "EditNebulaDialog.h"
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

GalaxyView::GalaxyView(QWidget *parent) : QWidget(parent), ui(new Ui::GalaxyView) {
    ui->setupUi(this);

    graphWidget = new GraphWidget(this);

    simulationTimer = new QTimer(this);
    if (ui->graphArea->layout() == nullptr) {
        QVBoxLayout *layout = new QVBoxLayout(ui->graphArea);
        layout->setContentsMargins(0, 0, 0, 0);
    }

    ui->graphArea->layout()->addWidget(graphWidget);

    paramsButton = new QPushButton("⚙️ Parameters", this);
    paramsButton->setStyleSheet("font-size: 14px; padding: 5px 10px;");

    switch3DButton = new QPushButton("🌌 3D View", this);
    switch3DButton->setStyleSheet("font-size: 14px; padding: 5px 10px;");
    connect(switch3DButton, &QPushButton::clicked, this, [this]() {
        is3DView = !is3DView;

        if (is3DView) {
            switch3DButton->setText("🪐 2D View");
            qDebug() << "Switched to 3D visualization";
        } else {
            switch3DButton->setText("🌕 3D View");
            qDebug() << "Switched to 2D visualization";
        }
    });


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
    connect(zoomOutButton, &QPushButton::clicked, this, &GalaxyView::on_zoomOutButton_clicked);
    connect(graphWidget, &GraphWidget::vertexDoubleClicked,
            this, &GalaxyView::on_vertexDoubleClicked);
    connect(editButton, &QPushButton::clicked, this, &GalaxyView::on_editButton_clicked);
    connect(simulationTimer, &QTimer::timeout, this, &GalaxyView::onPhysicsTimerTick);
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

        if (switch3DButton) {
            switch3DButton->resize(150, 35);
            int margin = 20;
            int btn_x = width() - paramsButton->width() - switch3DButton->width() - margin - 10;
            int btn_y = margin;
            switch3DButton->move(btn_x, btn_y);
        }

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
    initPhysicsSimulation();

    updateGraphDisplay();
    updateParametersWindow();
}

void GalaxyView::createPhysicsBody(CelestialObject* obj) {
    if (!physicsEngine || !physicsController) return;

    auto* wrapper = new CelestialBodyToRigidWrapper(obj, physicsEngine->getWorld());

    double radius = rngPtr->getDouble(100, 600);
    double angle = rngPtr->getDouble(0, 2 * M_PI);

    double physX = radius * std::cos(angle);
    double physY = radius * std::sin(angle);

    wrapper->setPosition(physX, physY, 0);

    double blackHoleMass = 50000.0;
    double G = 1.0;

    double dist = std::sqrt(physX*physX + physY*physY);
    double vMag = std::sqrt((G * blackHoleMass) / dist);

    btVector3 vel(-physY, physX, 0);
    vel.normalize();
    vel *= vMag;

    wrapper->getRigidBody()->setLinearVelocity(vel);

    physicsController->addCelestialBody(wrapper);

    vertexPositions.push_back(physicsToScreen(physX, physY));
}

void GalaxyView::initPhysicsSimulation() {
    if (!simulationTimer) {
        qDebug() << "CRITICAL ERROR: simulationTimer is null!";
        return;
    }
    simulationTimer->stop();

    if (physicsController) { delete physicsController; physicsController = nullptr; }
    if (physicsEngine) { delete physicsEngine; physicsEngine = nullptr; }

    physicsEngine = new PhysicsEngine();
    physicsController = new GalaxyPhysicsController(physicsEngine);

    double blackHoleMass = 50000.0;
    blackHoleField = new BlackHoleGravityField(blackHoleMass, 0, 0, 0);
    physicsController->addGravityField(blackHoleField);

    vertexPositions.clear();

    if (!galaxy) return;

    int nVerticesTotal = galaxy->getObject().size();
    for (int i = 0; i < nVerticesTotal; ++i) {
        CelestialObject* obj = galaxy->getObject()[i];

        if (galaxy->getGraph().getVertices()[i].getId() == -1) {
            vertexPositions.push_back(QPointF(0,0));
            continue;
        }

        createPhysicsBody(obj);
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
            CelestialObject* obj = galaxy->getObject()[i];

            if (galaxy->getGraph().getVertices()[i].getId() != -1) {
                createPhysicsBody(obj);
            } else {
                vertexPositions.push_back(QPointF(0,0));
            }
        }
    }
}

void GalaxyView::onPhysicsTimerTick() {
    if (!physicsController) return;

    physicsController->simulateStep(1.0 / 60.0);

    const auto& bodies = physicsController->getBodies();

    int bodyIndex = 0;
    for (int i = 0; i < vertexPositions.size(); ++i) {
         if (galaxy->getGraph().getVertices()[i].getId() == -1) continue;

         if (bodyIndex < bodies.size()) {
             auto* wrapper = bodies[bodyIndex];
             double px = wrapper->getX();
             double py = wrapper->getY();

             vertexPositions[i] = physicsToScreen(px, py);
             bodyIndex++;
         }
    }

    updateGraphDisplay();
}

QPointF GalaxyView::physicsToScreen(double x, double y) {
    double centerX = ui->graphArea->width() / 2.0;
    double centerY = ui->graphArea->height() / 2.0;

    double scale = 0.8;

    return QPointF(centerX + x * scale, centerY + y * scale);
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

    if (graphWidget) {
        graphWidget->zoomToVertex(vertexId);
    }
    if (zoomOutButton) {
        zoomOutButton->show();
        zoomOutButton->raise();
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
            for (Planet &planet : system->getPlanets()) {
                system->lifeExists(planet);
            }
            parametersText = QString::fromStdString(
                "Name: " + system->getName() + "\n" +
                "Type: Star System\n" +
                "Mass: " + QString::number(system->getMass(), 'e', 3).toStdString()+ " solar masses\n\n" +
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

void GalaxyView::show3DView() {
    qDebug() << "TODO: implement 3D view with Qt3D or QOpenGLWidget";
}
