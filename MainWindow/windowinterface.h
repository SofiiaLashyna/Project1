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

/**
 * @file WindowInterface.h
 * @brief Defines the main window class for the application.
 * @note Ensure loadJsonData() is called before attempting to generate the galaxy.
 * @see GalaxyView
 */
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

/**
 * @class WindowInterface
 * @brief The main QWidget class that serves as the entry point of the UI.
 *
 * This class holds the main stacked widget, the start screen,
 * and the GalaxyView. It is responsible for loading the initial
 * JSON data and triggering the galaxy generation.
 */
class WindowInterface : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor for the main window.
     * @param parent The parent Qt widget (usually nullptr).
     */
    explicit WindowInterface(QWidget *parent = nullptr);
    /**
             * @brief Destructor.
             */
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
