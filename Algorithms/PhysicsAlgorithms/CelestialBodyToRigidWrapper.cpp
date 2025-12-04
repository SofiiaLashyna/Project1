#include "CelestialBodyToRigidWrapper.h"

CelestialBodyToRigidWrapper::CelestialBodyToRigidWrapper(CelestialObject* object,
                                                         btDiscreteDynamicsWorld* world)
    : celestial_(object), world_(world)
{
    rigidBody_ = buildRigidBody();
    world_->addRigidBody(rigidBody_);
}

CelestialBodyToRigidWrapper::~CelestialBodyToRigidWrapper() {
    if (rigidBody_) {
        world_->removeRigidBody(rigidBody_);
        delete rigidBody_->getMotionState();
        delete rigidBody_;
    }
    delete shape_;
}

btRigidBody* CelestialBodyToRigidWrapper::buildRigidBody() {
    shape_ = new btSphereShape(btScalar(1.0));

    btVector3 inertia(0, 0, 0);
    btScalar mass = 1.0;
    if (mass != 0)
        shape_->calculateLocalInertia(mass, inertia);

    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(btVector3(0, 0, 0));

    motionState_ = new btDefaultMotionState(transform);

    btRigidBody::btRigidBodyConstructionInfo info(mass, motionState_, shape_, inertia);
    info.m_friction = 0.5;
    info.m_restitution = 0.1;

    return new btRigidBody(info);
}

void CelestialBodyToRigidWrapper::updateFromPhysics() {
    btTransform transform;
    rigidBody_->getMotionState()->getWorldTransform(transform);

    posX_ = transform.getOrigin().x();
    posY_ = transform.getOrigin().y();
    posZ_ = transform.getOrigin().z();
}


void CelestialBodyToRigidWrapper::updateToPhysics() {
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(btVector3(posX_, posY_, posZ_));
    rigidBody_->setWorldTransform(transform);
}

double CelestialBodyToRigidWrapper::getX() const { return posX_; }
double CelestialBodyToRigidWrapper::getY() const { return posY_; }
double CelestialBodyToRigidWrapper::getZ() const { return posZ_; }

void CelestialBodyToRigidWrapper::setPosition(double x, double y, double z) {
    posX_ = x;
    posY_ = y;
    posZ_ = z;
    updateToPhysics();
}