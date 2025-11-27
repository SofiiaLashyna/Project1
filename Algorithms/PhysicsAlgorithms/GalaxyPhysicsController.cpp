#include "GalaxyPhysicsController.h"

GalaxyPhysicsController::GalaxyPhysicsController(PhysicsEngine* engine)
    : engine_(engine) {}

GalaxyPhysicsController::~GalaxyPhysicsController() {
    bodies_.clear();
    gravityFields_.clear();
}

void GalaxyPhysicsController::addCelestialBody(CelestialBodyToRigidWrapper* body) {
    if (body) bodies_.push_back(body);
}

void GalaxyPhysicsController::addGravityField(BlackHoleGravityField* field) {
    if (field) gravityFields_.push_back(field);
}

void GalaxyPhysicsController::simulateStep(double deltaTime) {
    for (auto* field : gravityFields_) {
        field->applyGravity(bodies_, deltaTime);
    }
    applySpringForces(deltaTime);
    if (engine_) {
        engine_->stepSimulation(deltaTime);
    }

    for (auto* body : bodies_) {
        body->updateFromPhysics();
    }
}

void GalaxyPhysicsController::addSpring(CelestialBodyToRigidWrapper* a, CelestialBodyToRigidWrapper* b, double length) {
    if (!a || !b) return;

    PhysicsSpring spring;
    spring.bodyA = a;
    spring.bodyB = b;
    spring.targetLength = length;

    spring.stiffness = 5.0;

    springs_.push_back(spring);
}

void GalaxyPhysicsController::clearSprings() {
    springs_.clear();
}

void GalaxyPhysicsController::applySpringForces(double deltaTime) {
    for (const auto& spring : springs_) {
        btRigidBody* rbA = spring.bodyA->getRigidBody();
        btRigidBody* rbB = spring.bodyB->getRigidBody();

        btVector3 posA = rbA->getWorldTransform().getOrigin();
        btVector3 posB = rbB->getWorldTransform().getOrigin();

        btVector3 diff = posB - posA;
        double currentDist = diff.length();

        if (currentDist < 0.001) continue;
        double delta = currentDist - spring.targetLength;

        double forceMagnitude = spring.stiffness * delta;

        btVector3 forceDir = diff.normalized();

        btVector3 force = forceDir * forceMagnitude;

        rbA->applyCentralImpulse(force * deltaTime);
        rbB->applyCentralImpulse(-force * deltaTime);
    }
}