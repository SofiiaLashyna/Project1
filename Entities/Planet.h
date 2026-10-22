#ifndef PLANET_H
#define PLANET_H

#include "CelestialObject.h"

/**
 * @file Planet.h
 * @brief Defines the Planet class and associated structures for planetary data.
 */

/**
 * @struct RGBColor
 * @brief A simple structure to represent RGBA color values.
 * Used for rendering planets and their rings in both 2D and 3D views.
 */
struct RGBColor {
    int r, g, b;    ///< Red, Green, Blue components (0-255).
    int a = 255;    ///< Alpha (opacity) component (0-255, default is opaque).
};

/**
 * @class Planet
 * @brief A concrete class representing a Planet within a star system.
 *
 * This class extends CelestialObject by adding orbital mechanics (radius, speed, inclination),
 * physical classifications (Gas Giant, Terrestrial, etc.), and visual properties like
 * textures and planetary rings.
 * *
 */
class Planet : public CelestialObject {
private:
    double orbitRadius;      ///< Distance from the host star.
    double orbitSpeed;       ///< Velocity of the planet along its orbit.
    double inclination;      ///< Orbital tilt relative to the system's ecliptic plane.
    bool habitable;          ///< Flag indicating if the planet can support life.
    RGBColor color = {0, 0, 0}; ///< Base display color for the planet.
    std::string texturePath; ///< Path to the texture image file for 3D rendering.

    // Ring system properties
    bool rings = false;               ///< Flag indicating if the planet has a ring system.
    double ringInnerRadiusFactor = 0.0; ///< Inner edge of rings (multiplier of planet radius).
    double ringOuterRadiusFactor = 0.0; ///< Outer edge of rings (multiplier of planet radius).
    RGBColor ringColor = {0,0,0};      ///< Color and transparency of the planetary rings.

public:
    /**
     * @enum planetType
     * @brief Supported classifications for planets based on their physical composition.
     */
    enum class planetType {
        Gas_Giant,          ///< Large planets composed mostly of hydrogen and helium.
        Dwarf,              ///< Small planetary-mass objects (e.g., Pluto-like).
        Terrestrial_Planet  ///< Rocky planets with solid surfaces (e.g., Earth, Mars).
    };

    /**
     * @brief Constructor for the Planet class.
     * @param name Name of the planet.
     * @param mass Physical mass of the body.
     * @param orbit Initial orbital radius.
     * @param speed Speed of orbital rotation.
     * @param angle Orbital inclination in degrees.
     * @param type Classification (Gas Giant, etc.).
     * @param life Initial habitability status.
     * @throws std::invalid_argument If mass or orbit is zero or negative.
     */
    Planet(const std::string &name, double mass, double orbit, double speed, double angle, planetType type, bool life);

    /**
     * @brief Outputs planetary statistics and properties to the console.
     * Overrides CelestialObject::displayInfo().
     */
    void displayInfo() const override;

    /** @brief Gets the current orbital radius. */
    double getOrbit() const;

    /** @brief Updates the orbital radius. */
    void setOrbit(double newOrbit);

    /** @brief Sets the RGBA color for UI and 2D rendering. */
    void setColor(int r, int g, int b, int a = 255);

    /** @brief Returns the planet's base color structure. */
    RGBColor getColor() const;

    /** @brief Sets the file path for the planet's surface texture. */
    void setTexturePath(const std::string &path);

    /** @brief Returns the current texture file path. */
    std::string getTexturePath() const;

    /** @brief Checks if the planet is marked as habitable. */
    bool isHabitable() const;

    /** @brief Sets whether the planet supports life. */
    void setLifeExistence(bool life);

    /** @return Always returns "Planet" to identify the class type. */
    std::string getType() const override;

    /** @brief Gets the enum-based planet type. */
    planetType getPlanetType() const;

    /** @brief Returns a string representation of the planet type (e.g., "Gas Giant"). */
    std::string getPlanetTypeString() const;

    /** @brief Updates the planet type from a string identifier. */
    void setPlanetType(std::string type);

    /**
     * @brief Configures the planetary ring system.
     * @param hasRings Enable/disable rings.
     * @param innerFactor Ratio for the inner ring boundary.
     * @param outerFactor Ratio for the outer ring boundary.
     * @param r, g, b, a Color components for the rings.
     */
    void setRings(bool hasRings, double innerFactor = 0.0, double outerFactor = 0.0, int r = 0, int g = 0, int b = 0, int a = 150);

    /** @brief Checks for the presence of rings. */
    bool hasRings() const;

    /** @brief Gets the inner radius multiplier for rings. */
    double getRingInnerFactor() const;

    /** @brief Gets the outer radius multiplier for rings. */
    double getRingOuterFactor() const;

    /** @brief Returns the color structure of the ring system. */
    RGBColor getRingColor() const;

    /** @brief Gets the orbital inclination angle. */
    double getInclination() const;

    /** @brief Sets the orbital inclination angle. */
    void setInclination(double angle);

    /** @brief Gets the orbital speed. */
    double getSpeed() const;

    /** @brief Updates the orbital speed. */
    void setSpeed(double newSpeed);

private:
    planetType planet_type; ///< Internal storage for the planet's classification.
};

#endif //PLANET_H