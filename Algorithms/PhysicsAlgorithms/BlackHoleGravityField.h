#ifndef BLACKHOLEGRAVITYFIELD_H
#define BLACKHOLEGRAVITYFIELD_H

#include "CelestialBodyToRigidWrapper.h"
#include <vector>
#include <cmath>

/**
 * @file BlackHoleGravityField.h
 * @brief Logic for gravitational influence of a central singularity on galactic objects.
 */

/**
 * @class BlackHoleGravityField
 * @brief Simulates a point-mass gravitational field, typically representing a Supermassive Black Hole (SMBH).
 *
 * This class calculates and applies gravitational forces to a collection of celestial bodies.
 * It uses a simplified Newtonian gravity model where the force follows the inverse-square law:
 * $$F = G \frac{m_1 m_2}{r^2}$$
 *
 *
 */
class BlackHoleGravityField {
public:
    /**
     * @brief Constructs a gravity field with a specific mass and spatial coordinates.
     * @param mass The gravitational mass of the black hole (influences pull strength).
     * @param x, y, z Initial position coordinates in the simulation space.
     */
    BlackHoleGravityField(double mass, double x = 0, double y = 0, double z = 0);

    /**
     * @brief Iterates through a list of bodies and applies the calculated gravity vector to each.
     * @param bodies A vector of wrappers that link celestial data to rigid body physics.
     * @param deltaTime The time step of the current simulation frame (used for impulse calculation).
     */
    void applyGravity(std::vector<CelestialBodyToRigidWrapper*>& bodies, double deltaTime);

    /** @brief Updates the center of the gravity field. */
    void setPosition(double x, double y, double z);

    /** @brief Updates the mass, effectively changing the gravitational pull of the field. */
    void setMass(double mass);

private:
    double mass_;                ///< Mass of the black hole.
    double posX_, posY_, posZ_;  ///< Spatial coordinates (center of the field).

    /** * @brief Gravitational constant adjusted for the simulation's scale.
     * @note Set to 1.0 for simplified unit calculations.
     */
    static constexpr double G = 1.0;

    /**
     * @brief Internal helper to calculate and apply force to a single body.
     * Computes the distance vector, normalization, and the resulting acceleration.
     * @param body The target body to be influenced.
     * @param deltaTime The duration of the force application.
     */
    void applyForce(CelestialBodyToRigidWrapper* body, double deltaTime);
};

#endif // BLACKHOLEGRAVITYFIELD_H