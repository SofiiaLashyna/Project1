#ifndef GALAXYPHYSICSCONTROLLER_H
#define GALAXYPHYSICSCONTROLLER_H

#include "PhysicsEngine.h"
#include "CelestialBodyToRigidWrapper.h"
#include "BlackHoleGravityField.h"
#include <vector>

/**
 * @struct PhysicsSpring
 * @brief Represents a restorative force between two physical bodies.
 * * Used to maintain a target distance between celestial objects,
 * effectively acting like a structural bond or a graph edge constraint.
 */
struct PhysicsSpring {
    CelestialBodyToRigidWrapper* bodyA; ///< First connected body.
    CelestialBodyToRigidWrapper* bodyB; ///< Second connected body.
    double targetLength;               ///< The ideal distance between bodies.
    double stiffness;                  ///< The force constant (Hooke's Law).
};

/**
 * @file GalaxyPhysicsController.h
 * @brief High-level coordinator for galactic physics, including gravity and springs.
 */

/**
 * @class GalaxyPhysicsController
 * @brief Orchestrates the entire physics simulation within the galaxy.
 *
 * This controller acts as a facade for the `PhysicsEngine`. It manages the
 * lifecycle of physical entities, applies global forces (like black hole gravity),
 * and handles internal constraints (springs) between systems.
 * *

[Image of mass-spring system diagram]

 */
class GalaxyPhysicsController {
public:
    /**
     * @brief Constructs the controller tied to a specific physics engine.
     * @param engine Pointer to the core PhysicsEngine instance.
     */
    GalaxyPhysicsController(PhysicsEngine* engine);

    /**
     * @brief Destructor.
     * @note Does not necessarily delete bodies, as they might be owned by the Galaxy.
     */
    ~GalaxyPhysicsController();

    /**
     * @brief Registers a new celestial body into the physics simulation loop.
     * @param body The physics-wrapped celestial object.
     */
    void addCelestialBody(CelestialBodyToRigidWrapper* body);

    /**
     * @brief Adds a gravity source (e.g., a Black Hole) to the simulation.
     * @param field Pointer to the gravity field to be applied every step.
     */
    void addGravityField(BlackHoleGravityField* field);

    /**
     * @brief Performs a single simulation step.
     * * This method coordinates:
     * 1. Application of gravitational forces from all fields.
     * 2. Calculation of spring/constraint forces.
     * 3. Stepping the underlying PhysicsEngine.
     * 4. Synchronizing visual objects with physical results.
     * * @param deltaTime The time increment for this simulation step.
     */
    void simulateStep(double deltaTime);

    /** @brief Returns the list of currently managed physical bodies. */
    const std::vector<CelestialBodyToRigidWrapper*>& getBodies() const { return bodies_; }

    /**
     * @brief Creates a physical constraint between two bodies.
     * Useful for visual graph layout or binary star system modeling.
     * @param a First body.
     * @param b Second body.
     * @param length The desired rest length of the connection.
     */
    void addSpring(CelestialBodyToRigidWrapper* a, CelestialBodyToRigidWrapper* b, double length);

    /** @brief Removes all active spring constraints. */
    void clearSprings();

private:
    PhysicsEngine* engine_;                                 ///< Core physics engine (e.g., Bullet wrapper).
    std::vector<CelestialBodyToRigidWrapper*> bodies_;      ///< Managed physical bodies.
    std::vector<BlackHoleGravityField*> gravityFields_;    ///< Active gravitational sources.
    std::vector<PhysicsSpring> springs_;                    ///< Active spring constraints.

    /**
     * @brief Internal logic to apply Hooke's Law forces to connected bodies.
     * @param deltaTime Time step for force application.
     */
    void applySpringForces(double deltaTime);
};

#endif // GALAXYPHYSICSCONTROLLER_H