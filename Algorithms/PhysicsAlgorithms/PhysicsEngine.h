#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include <btBulletDynamicsCommon.h>
#include <vector>

/**
 * @file PhysicsEngine.h
 * @brief Core physics engine wrapper for the Bullet Dynamics library.
 */

/**
 * @class PhysicsEngine
 * @brief Encapsulates the Bullet Physics pipeline and dynamics world.
 *
 * This class is responsible for the low-level initialization of the physics world,
 * including collision detection, impulse solving, and step integration. It acts
 * as a container for all physical entities and their geometric shapes.
 */
class PhysicsEngine {
private:
    /** @brief The main container for all rigid bodies and constraints. */
    btDiscreteDynamicsWorld* dynamicsWorld;

    /** @brief Handles how bodies react to collisions and constraints (impulses). */
    btSequentialImpulseConstraintSolver* solver;

    /** @brief The "Broadphase" algorithm used to quickly filter out pairs of bodies that cannot collide. */
    btBroadphaseInterface* broadphase;

    /** @brief The "Narrowphase" dispatcher that calculates exact collision points and normals. */
    btCollisionDispatcher* dispatcher;

    /** @brief Global settings for collision memory allocation and algorithms. */
    btDefaultCollisionConfiguration* collisionConfiguration;

    /** @brief Internal registry of collision shapes to ensure proper memory cleanup. */
    std::vector<btCollisionShape*> shapes;

    /** @brief Internal registry of rigid bodies managed by this engine. */
    std::vector<btRigidBody*> bodies;

public:
    /**
     * @brief Constructor: Initializes the complete Bullet physics pipeline.
     * * Sets up the default collision configuration, dispatcher, broadphase,
     * solver, and finally the discrete dynamics world with Earth-like or custom gravity.
     */
    PhysicsEngine();

    /**
     * @brief Destructor: Performs a deep cleanup of the physics world.
     * * Explicitly deletes all bodies, shapes, and the dynamics world components
     * in the correct order to prevent dangling pointers and memory leaks.
     */
    ~PhysicsEngine();

    /**
     * @brief Advances the physics simulation by a given time interval.
     * * @param deltaTime The time step (usually from a QTimer or frame clock).
     * Bullet will automatically handle "sub-stepping" to maintain stability.
     */
    void stepSimulation(float deltaTime);

    /**
     * @brief Registers a rigid body and its collision shape in the dynamics world.
     * @param body The Bullet rigid body instance.
     * @param shape The collision geometry associated with the body.
     */
    void addRigidBody(btRigidBody* body, btCollisionShape* shape);

    /**
     * @brief Removes a rigid body from the simulation.
     * @param body The body to be removed from the dynamics world.
     */
    void removeRigidBody(btRigidBody* body);

    /** @brief Provides direct access to the Bullet world for advanced configurations. */
    btDiscreteDynamicsWorld* getWorld() const { return dynamicsWorld; }
};

#endif // PHYSICSENGINE_H