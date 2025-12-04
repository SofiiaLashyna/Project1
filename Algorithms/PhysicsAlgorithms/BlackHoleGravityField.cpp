#include "BlackHoleGravityField.h"

BlackHoleGravityField::BlackHoleGravityField(double mass, double x, double y, double z)
    : mass_(mass), posX_(x), posY_(y), posZ_(z) {}

void BlackHoleGravityField::setPosition(double x, double y, double z) {
    posX_ = x;
    posY_ = y;
    posZ_ = z;
}

void BlackHoleGravityField::setMass(double mass) {
    mass_ = mass;
}

void BlackHoleGravityField::applyGravity(std::vector<CelestialBodyToRigidWrapper*>& bodies, double deltaTime) {
    for (auto* body : bodies) {
        applyForce(body, deltaTime);
    }
}
void BlackHoleGravityField::applyForce(CelestialBodyToRigidWrapper* body, double deltaTime) {
    double dx = posX_ - body->getX();
    double dy = posY_ - body->getY();
    double dz = posZ_ - body->getZ();

    double distanceSq = dx*dx + dy*dy + dz*dz;

    if (distanceSq < 100.0) distanceSq = 100.0;

    double distance = std::sqrt(distanceSq);

    double forceMagnitude = (G * mass_ * 1.0) / distanceSq;

    double fx = forceMagnitude * dx / distance;
    double fy = forceMagnitude * dy / distance;
    double fz = forceMagnitude * dz / distance;

    btRigidBody* rb = body->getRigidBody();
    if (rb) {
        rb->activate(true);
        rb->applyCentralForce(btVector3(fx, fy, fz));
    }
}