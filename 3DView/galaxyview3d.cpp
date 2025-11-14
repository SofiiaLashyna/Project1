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

    const double galaxyRadius = 20000.0;

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

}

void GalaxyView3D::on_paramsButton_clicked() { qDebug() << "params clicked"; }
void GalaxyView3D::on_editButton_clicked() { qDebug() << "edit clicked"; }
void GalaxyView3D::on_vertexDoubleClicked(int vertexId) { qDebug() << "vertex" << vertexId; }
void GalaxyView3D::on_zoomOutButton_clicked() { qDebug() << "zoom out"; }
void GalaxyView3D::showObjectParameters(CelestialObject *obj) { qDebug() << "show params"; }
void GalaxyView3D::on_editObjectButton_clicked() { qDebug() << "edit obj"; }
void GalaxyView3D::editStarSystem(StarSystem* system) {
    //not added yet
    }
void GalaxyView3D::editNebula(Nebula* nebula) {
    //not added yet
    }