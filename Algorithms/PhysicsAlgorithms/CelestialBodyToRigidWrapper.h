#ifndef CELESTIALBODYTORIGIDWRAPPER_H
#define CELESTIALBODYTORIGIDWRAPPER_H

#include <btBulletDynamicsCommon.h>
#include "../../Entities/CelestialObject.h"

/**
 * @class CelestialBodyToRigidWrapper
 * @brief Converts a CelestialObject into a Bullet btRigidBody for physics simulation.
 */
class CelestialBodyToRigidWrapper {
public:
    CelestialBodyToRigidWrapper(CelestialObject* object, btDiscreteDynamicsWorld* world);
    ~CelestialBodyToRigidWrapper();

    btRigidBody* getRigidBody() const { return rigidBody_; }
    CelestialObject* getCelestial() const { return celestial_; }

    void updateFromPhysics();     // btRigidBody → CelestialObject
    void updateToPhysics();       // CelestialObject → btRigidBody

    double getX() const;
    double getY() const;
    double getZ() const;

    void setPosition(double x, double y, double z);

private:
    double posX_ = 0;
    double posY_ = 0;
    double posZ_ = 0;
    CelestialObject* celestial_;
    btRigidBody* rigidBody_;
    btCollisionShape* shape_;
    btDefaultMotionState* motionState_;
    btDiscreteDynamicsWorld* world_;

    btRigidBody* buildRigidBody();
};

#endif // CELESTIALBODYTORIGIDWRAPPER_H
