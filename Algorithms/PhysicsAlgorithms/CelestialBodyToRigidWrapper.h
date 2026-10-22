#ifndef CELESTIALBODYTORIGIDWRAPPER_H
#define CELESTIALBODYTORIGIDWRAPPER_H

#include <btBulletDynamicsCommon.h>
#include "../../Entities/CelestialObject.h"

/**
 * @file CelestialBodyToRigidWrapper.h
 * @brief Bridge between domain-specific celestial objects and the Bullet Physics engine.
 */

/**
 * @class CelestialBodyToRigidWrapper
 * @brief Wrapper class that integrates a CelestialObject into the Bullet Physics simulation.
 *
 * This class acts as an adapter, creating and managing a `btRigidBody` for a given
 * `CelestialObject`. It ensures that physical forces (like gravity or collisions)
 * calculated by Bullet are reflected back in the celestial object's coordinates
 * and vice versa.
 */
class CelestialBodyToRigidWrapper {
public:
    /**
     * @brief Constructs a physics wrapper for a celestial object.
     * * Initializes the collision shape, motion state, and rigid body, then
     * adds the resulting body to the Bullet dynamics world.
     * * @param object Pointer to the CelestialObject to be simulated.
     * @param world Pointer to the Bullet dynamics world where the physics take place.
     */
    CelestialBodyToRigidWrapper(CelestialObject* object, btDiscreteDynamicsWorld* world);

    /**
     * @brief Destructor that safely removes the rigid body from the physics world.
     * * Cleans up the motion state, collision shape, and the rigid body instance
     * to prevent memory leaks in the physics simulation.
     */
    ~CelestialBodyToRigidWrapper();

    /** @brief Provides access to the underlying Bullet rigid body. */
    btRigidBody* getRigidBody() const { return rigidBody_; }

    /** @brief Provides access to the associated celestial object data. */
    CelestialObject* getCelestial() const { return celestial_; }

    /**
     * @brief Synchronizes the CelestialObject with the current physics state.
     * * Fetches the transform (position/rotation) from the `btRigidBody` and
     * updates the internal coordinates of the `CelestialObject`.
     * Call this after the physics world steps.
     */
    void updateFromPhysics();

    /**
     * @brief Synchronizes the physics body with the CelestialObject's data.
     * * Updates the `btRigidBody` transform based on the manual changes
     * made to the `CelestialObject`.
     */
    void updateToPhysics();

    /** @brief Gets the current X-coordinate from the physics simulation. */
    double getX() const;
    /** @brief Gets the current Y-coordinate from the physics simulation. */
    double getY() const;
    /** @brief Gets the current Z-coordinate from the physics simulation. */
    double getZ() const;

    /**
     * @brief Manually teleports the physics body to a new position.
     * @param x, y, z New coordinates in world space.
     */
    void setPosition(double x, double y, double z);

private:
    double posX_ = 0; ///< Internal cache for the X position.
    double posY_ = 0; ///< Internal cache for the Y position.
    double posZ_ = 0; ///< Internal cache for the Z position.

    CelestialObject* celestial_; ///< Pointer to the high-level celestial entity.
    btRigidBody* rigidBody_;    ///< The actual physical body in the Bullet engine.
    btCollisionShape* shape_;   ///< The geometric shape used for collision detection.
    btDefaultMotionState* motionState_; ///< Handles interpolation between physics steps.
    btDiscreteDynamicsWorld* world_;    ///< Reference to the world containing this body.

    /**
     * @brief Internal factory method to configure the btRigidBody.
     * * Calculates inertia based on the CelestialObject's mass and creates
     * the physical representation.
     * @return A fully initialized btRigidBody pointer.
     */
    btRigidBody* buildRigidBody();
};

#endif // CELESTIALBODYTORIGIDWRAPPER_H