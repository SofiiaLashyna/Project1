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
    double orbitRadius; ///< The radius of the planet's orbit around its star.
    double orbitSpeed; ///< The speed at which the planet orbits the star.
    double inclination; ///< The inclination angle of the planet's orbit.
    bool habitable; ///< Whether the planet can support life.
    QColor color = QColor(0,0,0); ///< Display color for UI representation.
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
    /**
         * @brief Gets the display color of the planet.
         * @return The planet's QColor.
         */
    QColor getColor();
    /**
        * @brief Checks if the planet is habitable.
        * @return True if habitable, false otherwise.
        */
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
    /**
         * @brief Gets the specific enum type of this planet.
         * @return The enum value (planetType) of the planet.
         */
    planetType getPlanetType();
    /// @brief Default virtual destructor.
    virtual ~Planet() = default;
private:
    planetType planet_type; ///< Specific type of the planet (Gas_Giant, Terrestrial, Dwarf).
};

#endif //PLANET_H
