#ifndef GALAXYVIEW3D_H
#define GALAXYVIEW3D_H

#include <QWidget>
#include <QTimer>
#include <QVector3D>
#include <QVBoxLayout>
#include <QQuickWidget>
#include <QPushButton>
#include <QTextEdit>
#include "DijkstraPathList.h"
#include <QLabel>
#include "RandomUtilities.h"
#include "CelestialObject.h"
#include "Galaxy.h"
#include "GraphList.h"
#include "nlohmann/json.hpp"
#include "CelestialObject3DModel.h"
#include "PhysicsEngine.h"
#include "BlackHoleGravityField.h"
#include "CelestialBodyToRigidWrapper.h"
#include "GalaxyPhysicsController.h"
#include "GalaxyEditDialog.h"
#include "EditStarSystemDialog.h"
#include "EditNebulaDialog.h"
#include "PlanetarySystemModel.h"

class StarSystem;
class Nebula;

namespace Ui {
    class GalaxyView3D;
}

class GalaxyView3D : public QWidget {
    Q_OBJECT

protected:
    void resizeEvent(QResizeEvent *event) override;

public:
    explicit GalaxyView3D(QWidget *parent = nullptr);

    ~GalaxyView3D() override;

    std::vector<QVector3D> vertexPositions3D;

    void generateAndDisplayGalaxy(const nlohmann::json &data, RandomGenerator &rng);

private slots:
    void on_paramsButton_clicked();

    void on_editButton_clicked();

    void on_vertexDoubleClicked(int vertexId);

    void on_zoomOutButton_clicked();

    void on_editObjectButton_clicked();

    void onPhysicsTimerTick();

    void onVertexClicked(int vertexId);

    void onBackgroundClicked();

private:
    Ui::GalaxyView3D *ui;
    Galaxy<GraphList<CelestialObject *> > *galaxy = nullptr;

    int detailedVertexId = -1;
    QPushButton *paramsButton = nullptr;
    QWidget *paramsWindow = nullptr;
    QTextEdit *infoText = nullptr;
    QPushButton *editButton = nullptr;
    QPushButton *zoomOutButton = nullptr;

    RandomGenerator *rngPtr = nullptr;
    nlohmann::json *dataPtr = nullptr;

    QQuickWidget *quickWidget = nullptr;
    CelestialObject3DModel *celestialModelPtr = nullptr;

    PhysicsEngine *physicsEngine = nullptr;
    GalaxyPhysicsController *physicsController = nullptr;
    BlackHoleGravityField *blackHoleField = nullptr;
    QTimer *simulationTimer = nullptr;
    std::vector<CelestialBodyToRigidWrapper *> wrappersMap3D;
    double viewScale = 1.0;

    int startNodeId = -1;
    int endNodeId = -1;
    QWidget *pathInfoWidget = nullptr;
    QLabel *pathStatusLabel = nullptr;
    QLabel *pathDetailsLabel = nullptr;
    QLabel *pathDistanceLabel = nullptr;
    bool isPathActive = false;

    void setupPathInfoWidget();

    void resetPathSelection();

    void calculateShortestPath();


    void sendPathToQML(const std::vector<int> &pathIndices);

    QVector3D getObjectPosition(int index);

    void applySpaceStyle();

    void initPhysicsSimulation();

    void createPhysicsBody(CelestialObject *obj);

    void updateParametersWindow();

    void showObjectParameters(CelestialObject *obj);

    void editStarSystem(StarSystem *system);

    void editNebula(Nebula *nebula);

    void checkForNewObjects();

    void setupPhysicsForBody(CelestialBodyToRigidWrapper *wrapper, double x, double y, double z);

    QColor getStarColorByType(Star::starType type);

    PlanetarySystemModel *planetModelPtr = nullptr;
};

#endif //GALAXYVIEW3D_H
