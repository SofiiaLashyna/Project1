#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include <btBulletDynamicsCommon.h>
#include <vector>

class PhysicsEngine {
private:
    btDiscreteDynamicsWorld* dynamicsWorld;
    btSequentialImpulseConstraintSolver* solver;
    btBroadphaseInterface* broadphase;
    btCollisionDispatcher* dispatcher;
    btDefaultCollisionConfiguration* collisionConfiguration;

    std::vector<btCollisionShape*> shapes;
    std::vector<btRigidBody*> bodies;

public:
    PhysicsEngine();
    ~PhysicsEngine();

    void stepSimulation(float deltaTime);

    void addRigidBody(btRigidBody* body, btCollisionShape* shape);
    void removeRigidBody(btRigidBody* body);

    btDiscreteDynamicsWorld* getWorld() const { return dynamicsWorld; }
};

#endif // PHYSICSENGINE_H
