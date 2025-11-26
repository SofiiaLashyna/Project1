#ifndef WINDOWINTERFACE_H
#define WINDOWINTERFACE_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include "RandomUtilities.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <vector>

QT_BEGIN_NAMESPACE

namespace Ui {
    class WindowInterface;
}

QT_END_NAMESPACE

class GalaxyView;
class GalaxyView3D;

struct MenuStar {
    QPointF pos;
    double size;
    int alpha;
    double speed;
};

class WindowInterface : public QWidget {
    Q_OBJECT

public:
    explicit WindowInterface(QWidget *parent = nullptr);

    ~WindowInterface() override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::WindowInterface *ui;
    GalaxyView *galaxyView;
    GalaxyView3D *galaxyView3D;
    RandomGenerator rng;
    nlohmann::json data;
    bool dataLoaded = false;

    void loadJsonData();

    void setupStyle();

    void generateMenuStars();

    std::vector<MenuStar> stars;
    QTimer *backgroundTimer;
    double offset = 0.0;
    bool isMenuMode = true;
};

#endif // WINDOWINTERFACE_H
