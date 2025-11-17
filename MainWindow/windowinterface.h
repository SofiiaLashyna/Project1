#ifndef WINDOWINTERFACE_H
#define WINDOWINTERFACE_H

#include <QWidget>
#include "RandomUtilities.h"
#include "nlohmann/json.hpp"
#include <fstream>
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

private:
    Ui::WindowInterface *ui; ///< Pointer to the Qt-generated UI class.
    GalaxyView *galaxyView; ///< Pointer to the main galaxy visualization widget.
    RandomGenerator rng; ///< The main random number generator instance for the application.
    nlohmann::json data; ///< The JSON object that stores all configuration data loaded from file.
    bool dataLoaded = false; ///< A flag to ensure galaxy generation only starts if JSON was loaded successfully.
    /**
             * @brief Private helper method to load and parse the CelestialObjects.json file.
             * Populates the 'data' member and sets the 'dataLoaded' flag.
             */
    void loadJsonData();
};

#endif // WINDOWINTERFACE_H
