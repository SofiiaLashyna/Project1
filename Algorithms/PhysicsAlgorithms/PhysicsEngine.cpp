#include "PhysicsEngine.h"
#include <algorithm>
PhysicsEngine::PhysicsEngine() {
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    broadphase = new btDbvtBroadphase();
    solver = new btSequentialImpulseConstraintSolver();

    dynamicsWorld = new btDiscreteDynamicsWorld(
        dispatcher,
        broadphase,
        solver,
        collisionConfiguration
    );

    dynamicsWorld->setGravity(btVector3(0, 0, 0));
}

PhysicsEngine::~PhysicsEngine() {
    for (btRigidBody* body : bodies) {
        dynamicsWorld->removeRigidBody(body);
        delete body->getMotionState();
        delete body;
    }

    for (btCollisionShape* shape : shapes) {
        delete shape;
    }

    delete dynamicsWorld;
    delete solver;
    delete broadphase;
    delete dispatcher;
    delete collisionConfiguration;
}

void PhysicsEngine::stepSimulation(float deltaTime) {
    dynamicsWorld->stepSimulation(deltaTime);
}

void PhysicsEngine::addRigidBody(btRigidBody* body, btCollisionShape* shape) {
    dynamicsWorld->addRigidBody(body);
    bodies.push_back(body);
    shapes.push_back(shape);
}

void PhysicsEngine::removeRigidBody(btRigidBody* body) {
    dynamicsWorld->removeRigidBody(body);
    auto it = std::find(bodies.begin(), bodies.end(), body);
    if (it != bodies.end()) {
        bodies.erase(it);
    }
    }
