#ifndef PLANET_H
#define PLANET_H
#include <QColor>

#include "CelestialObject.h"
/**
 * @file Planet.h
 * @brief Defines the Planet class, derived from CelestialObject.
 */

/**
 * @class Planet
 * @brief A concrete class representing a Planet.
 *
 * Implements the CelestialObject interface and adds
 * planet-specific properties like orbit, type, and habitability.
 */
class Planet : public CelestialObject {
private:
    double orbitRadius;
    double orbitSpeed;
    double inclination;
    bool habitable;
    QColor color = QColor(0, 0, 0);
    std::string texturePath;
    bool rings = false;
    double ringInnerRadiusFactor = 0.0;
    double ringOuterRadiusFactor = 0.0;
    QColor ringColor = Qt::transparent;
public:
    /// Enum representing all supported planet types.
    enum class planetType { Gas_Giant, Dwarf, Terrestrial_Planet };
    /**
         * @brief Constructor for the Planet class.
         * @param name The name of the planet.
         * @param mass The mass of the planet.
         * @param orbit The radius of the orbit.
         * @param speed The orbital speed.
         * @param angle The orbital inclination.
         * @param type The type of the planet (e.g., Gas_Giant).
         * @param life Whether the planet is habitable.
         * @throws std::invalid_argument If mass or orbit is zero or negative.
         */
    Planet(const std::string &name, double mass, double orbit, double speed, double angle, planetType type, bool life);
    /**
          * @brief Displays all information about the planet (overrides CelestialObject).
        */
    void displayInfo() const override;
    /**
         * @brief Gets the orbital radius of the planet.
         * @return The orbital radius.
         */
    double getOrbit() const;
    /**
         * @brief Sets the display color for the planet (e.g., in UI).
         * @param newColor The QColor to set.
         */
    void setColor(QColor newColor);

    QColor getColor() const;

    void setTexturePath(const std::string &path);

    std::string getTexturePath() const;

    bool isHabitable() const;
    /**
        * @brief Sets the habitability status of the planet.
        * @param life The new habitability status.
        */
    void setLifeExistence(bool life);
    /**
         * @brief Returns the specific type of the object (overrides CelestialObject).
         * @return The string "Planet".
       */
    std::string getType() const override;

    planetType getPlanetType() const;

    void setRings(bool hasRings, double innerFactor = 0.0, double outerFactor = 0.0, QColor rColor = Qt::transparent);
    bool hasRings() const;
    double getRingInnerFactor() const;
    double getRingOuterFactor() const;
    QColor getRingColor() const;
    double getInclination() const;

private:
    planetType planet_type; ///< Specific type of the planet (Gas_Giant, Terrestrial, Dwarf).
};

#endif //PLANET_H
