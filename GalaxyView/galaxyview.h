#ifndef GALAXYVIEW_H
#define GALAXYVIEW_H

#include "RandomUtilities.h"
#include "CelestialObject.h"
#include "Galaxy.h"
#include "GraphList.h"
#include "nlohmann/json.hpp"
#include "GraphWidget.h"
#include <cmath>
#include <qboxlayout.h>
#include <QDialog>
#include <qlineedit.h>
#include <QPushButton>
#include <QSpinBox>

#include <QComboBox>

QT_BEGIN_NAMESPACE
namespace Ui { class GalaxyView; }
QT_END_NAMESPACE

class GalaxyView : public QWidget {
    Q_OBJECT
    protected:
    void resizeEvent(QResizeEvent *event) override;
public:
    explicit GalaxyView(QWidget *parent = nullptr);
    ~GalaxyView() override;
    void generateAndDisplayGalaxy(const nlohmann::json& data, RandomGenerator& rng);
private slots:
        void on_paramsButton_clicked();
        void on_editButton_clicked();
private:
    Ui::GalaxyView *ui;
    Galaxy<GraphList<CelestialObject*>>* galaxy = nullptr;
    void updateParametersWindow();
    void updateGraphDisplay();
    GraphWidget *graphWidget = nullptr;
    QPushButton *paramsButton = nullptr;
    QWidget *paramsWindow = nullptr;
    RandomGenerator* rngPtr = nullptr;
    nlohmann::json* dataPtr = nullptr;
    QPushButton *editButton = nullptr;
    std::vector<QPointF> vertexPositions;
};

class GalaxyEditDialog : public QDialog {
    Q_OBJECT
public:
    explicit GalaxyEditDialog(Galaxy<GraphList<CelestialObject*>>* g,
                              RandomGenerator* rng,
                              const nlohmann::json* data,
                              QWidget* parent = nullptr);


    QString getNewGalaxyName() const;
    private slots:
        void on_addStarSystem_clicked();
    void on_addNebula_clicked();
    signals:
    void galaxyModified();
private:
    Galaxy<GraphList<CelestialObject*>>* galaxy;
    QLineEdit* nameEdit;

    RandomGenerator* rngPtr;
    const nlohmann::json* dataPtr;
};

class AddStarSystemDialog : public QDialog {
    Q_OBJECT
public:
    explicit AddStarSystemDialog(RandomGenerator& rng, const nlohmann::json& data, QWidget* parent = nullptr);
    StarSystem* getNewStarSystem(int id) const;
private:
    QLineEdit* nameEdit;
    QSpinBox* planetCountSpinBox;
    QComboBox* starTypeComboBox;
    RandomGenerator& rng;
    const nlohmann::json& data;
};

class AddNebulaDialog : public QDialog {
    Q_OBJECT
public:
    explicit AddNebulaDialog(RandomGenerator& rng, const nlohmann::json& data, QWidget* parent = nullptr);
    Nebula* getNewNebula() const;
private:
    QLineEdit* nameEdit;
    QComboBox* nebulaTypeComboBox;
    RandomGenerator& rng;
    const nlohmann::json& data;
};


#endif //GALAXYVIEW_H