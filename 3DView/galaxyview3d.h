#ifndef GALAXYVIEW3D_H
#define GALAXYVIEW3D_H

#include "RandomUtilities.h"
#include "CelestialObject.h"
#include "Galaxy.h"
#include "GraphList.h"
#include "nlohmann/json.hpp"
#include "GraphWidget.h"
#include <cmath>
#include <qboxlayout.h>
#include <QPushButton>
#include <QtQuickWidgets/QQuickWidget>
#include "CelestialObject3DModel.h"

class GalaxyEditDialog;
class AddStarSystemDialog;
class AddNebulaDialog;
class EditStarSystemDialog;
class EditNebulaDialog;
class StarSystem;
class Nebula;

QT_BEGIN_NAMESPACE

namespace Ui {
    class GalaxyView3D;
}

QT_END_NAMESPACE

class GalaxyView3D : public QWidget {
    Q_OBJECT

protected:
    void resizeEvent(QResizeEvent *event) override;

public:
    explicit GalaxyView3D(QWidget *parent = nullptr);

    ~GalaxyView3D() override;

    void generateAndDisplayGalaxy(const nlohmann::json &data, RandomGenerator &rng);

private slots:
    void on_paramsButton_clicked();

    void on_editButton_clicked();

    void on_vertexDoubleClicked(int vertexId);

    void on_zoomOutButton_clicked();

    void showObjectParameters(CelestialObject *obj);

    void on_editObjectButton_clicked();

private:
    Ui::GalaxyView3D *ui;
    Galaxy<GraphList<CelestialObject *> > *galaxy = nullptr;

    int detailedVertexId = -1;
    void updateParametersWindow();

    QPushButton *paramsButton = nullptr;
    QWidget *paramsWindow = nullptr;
    RandomGenerator *rngPtr = nullptr;
    nlohmann::json *dataPtr = nullptr;
    QPushButton *editButton = nullptr;
    QPushButton *zoomOutButton = nullptr;

    std::vector<QVector3D> vertexPositions3D;

    QQuickWidget *quickWidget = nullptr;
    CelestialObject3DModel *celestialModelPtr = nullptr;
    void editStarSystem(StarSystem* system);
    void editNebula(Nebula* nebula);
};

#endif //GALAXYVIEW3D_H