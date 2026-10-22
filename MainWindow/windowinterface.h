#ifndef WINDOWINTERFACE_H
#define WINDOWINTERFACE_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include "RandomUtilities.h"
#include "nlohmann/json.hpp"
#include <vector>

/**
 * @file WindowInterface.h
 * @brief Header file for the main application window and initial user interface.
 * @note This class manages the transition from the main menu to the simulation views.
 */

QT_BEGIN_NAMESPACE
namespace Ui {
    class WindowInterface;
}
QT_END_NAMESPACE

class GalaxyView;
class GalaxyView3D;

/**
 * @struct MenuStar
 * @brief Represents a single star in the animated main menu background.
 * * Used to create a parallax or shimmering effect on the entry screen.
 */
struct MenuStar {
    QPointF pos;    ///< Position on the screen coordinate system.
    double size;    ///< Diameter of the star.
    int alpha;      ///< Transparency level (0-255) for twinkling effects.
    double speed;   ///< Movement speed for background animation.
};

/**
 * @class WindowInterface
 * @brief The primary container and manager for the application's user interface.
 *
 * WindowInterface acts as a manager for the different views of the application
 * (2D View, 3D View, and Main Menu). It handles high-level tasks such as:
 * - **Resource Management:** Loading configuration files (JSON).
 * - **View Switching:** Managing a QStackedWidget to swap between the menu and the galaxy simulation.
 * - **Visual Polish:** Rendering an animated starfield background and applying global styles.
 */
class WindowInterface : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructs the main window interface.
     * Initializes the UI, triggers JSON loading, and starts the background animation timer.
     * @param parent Pointer to the parent widget.
     */
    explicit WindowInterface(QWidget *parent = nullptr);

    /**
     * @brief Cleans up UI resources and background animation timers.
     */
    ~WindowInterface() override;

protected:
    /**
     * @brief Custom paint handler for the animated star background.
     * Draws the `MenuStar` collection when `isMenuMode` is active.
     */
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::WindowInterface *ui;            ///< Pointer to the Qt UI setup.
    GalaxyView *galaxyView;             ///< Instance of the 2D galaxy simulation view.
    GalaxyView3D *galaxyView3D;         ///< Instance of the 3D galaxy simulation view.

    RandomGenerator rng;                ///< Global random generator for consistent seeding.
    nlohmann::json data;                ///< Root configuration object loaded from file.
    bool dataLoaded = false;            ///< Flag to track if the JSON was successfully parsed.

    /**
     * @brief Parses the 'universe_config.json' file to initialize generation parameters.
     * Sets `dataLoaded` to true upon success.
     */
    void loadJsonData();

    /**
     * @brief Applies global CSS-like styles (QSS) to the window and its children.
     * Configures dark themes and translucent effects.
     */
    void setupStyle();

    /**
     * @brief Initializes the collection of `MenuStar` objects with random properties.
     */
    void generateMenuStars();

    std::vector<MenuStar> stars;        ///< Container for background animation elements.
    QTimer *backgroundTimer;            ///< Drives the frame rate for the menu's star animation.
    double offset = 0.0;                ///< Current animation offset for moving backgrounds.
    bool isMenuMode = true;             ///< Determines whether to render the menu background and overlay.
};

#endif // WINDOWINTERFACE_H