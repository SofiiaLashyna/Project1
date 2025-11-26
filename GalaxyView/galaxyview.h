#ifndef GALAXYVIEW_H
#define GALAXYVIEW_H

#include "RandomUtilities.h"
#include "CelestialObject.h"
#include "Galaxy.h"
#include "GraphList.h"
#include "DijkstraList.h"
#include "nlohmann/json.hpp"
#include "GraphWidget.h"
#include <cmath>
#include <qboxlayout.h>
#include <QLabel>
#include <QPushButton>
#include "PhysicsEngine.h"
#include "BlackHoleGravityField.h"
#include "CelestialBodyToRigidWrapper.h"
#include "GalaxyPhysicsController.h"

class GalaxyEditDialog;
class AddStarSystemDialog;
class AddNebulaDialog;
class EditStarSystemDialog;
class EditNebulaDialog;

QT_BEGIN_NAMESPACE

namespace Ui {
    class GalaxyView;
}

QT_END_NAMESPACE

class GalaxyView : public QWidget {
    Q_OBJECT

protected:
    void resizeEvent(QResizeEvent *event) override;

public:
    explicit GalaxyView(QWidget *parent = nullptr);

    ~GalaxyView() override;

    void generateAndDisplayGalaxy(const nlohmann::json &data, RandomGenerator &rng);

    private slots:
    void on_paramsButton_clicked();

    void on_editButton_clicked();

    void on_vertexDoubleClicked(int vertexId);

    void onVertexClicked(int vertexId);

    void onBackgroundClicked();

    void on_zoomOutButton_clicked();

    void showObjectParameters(CelestialObject *obj);

    void on_editObjectButton_clicked();

    void onPhysicsTimerTick();

private:
    double viewScale = 0.2;
    Ui::GalaxyView *ui;
    Galaxy<GraphList<CelestialObject *> > *galaxy = nullptr;

    void updateParametersWindow();

    void updateGraphDisplay();

    GraphWidget *graphWidget = nullptr;
    QPushButton *paramsButton = nullptr;
    QWidget *paramsWindow = nullptr;
    RandomGenerator *rngPtr = nullptr;
    nlohmann::json *dataPtr = nullptr;
    QPushButton *editButton = nullptr;
    QPushButton *zoomOutButton = nullptr;
    std::vector<QPointF> vertexPositions;
    void editStarSystem(StarSystem* system);
    void editNebula(Nebula* nebula);

    PhysicsEngine* physicsEngine = nullptr;
    GalaxyPhysicsController* physicsController = nullptr;
    BlackHoleGravityField* blackHoleField = nullptr;
    QTimer* simulationTimer = nullptr;

    void initPhysicsSimulation();

    void checkForNewObjects();

    void createPhysicsBody(CelestialObject* obj);

    QPointF physicsToScreen(double x, double y);

    int startNodeId = -1;
    int endNodeId = -1;

    std::vector<std::pair<int, int>> pathEdges;

    void resetPathSelection();

    void calculateShortestPath();

    QWidget* pathInfoWidget = nullptr;
    QLabel* pathStatusLabel = nullptr;
    QLabel* pathDetailsLabel = nullptr;
    QLabel* pathDistanceLabel = nullptr;

    void setupPathInfoWidget();
    void updatePathDistanceText();

};

#endif //GALAXYVIEW_H