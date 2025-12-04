#ifndef GALAXYPHYSICSCONTROLLER_H
#define GALAXYPHYSICSCONTROLLER_H

#include "PhysicsEngine.h"
#include "CelestialBodyToRigidWrapper.h"
#include "BlackHoleGravityField.h"
#include <vector>
struct PhysicsSpring {
    CelestialBodyToRigidWrapper* bodyA;
    CelestialBodyToRigidWrapper* bodyB;
    double targetLength;
    double stiffness;
};
/**
 * @class GalaxyPhysicsController
 * @brief Controls the physics simulation of a galaxy.
 */
class GalaxyPhysicsController {
public:
    GalaxyPhysicsController(PhysicsEngine* engine);
    ~GalaxyPhysicsController();

    void addCelestialBody(CelestialBodyToRigidWrapper* body);
    void addGravityField(BlackHoleGravityField* field);

    void simulateStep(double deltaTime); // One physics step

    const std::vector<CelestialBodyToRigidWrapper*>& getBodies() const { return bodies_; }

    void addSpring(CelestialBodyToRigidWrapper* a, CelestialBodyToRigidWrapper* b, double length);

    void clearSprings();
private:
    PhysicsEngine* engine_;
    std::vector<CelestialBodyToRigidWrapper*> bodies_;
    std::vector<BlackHoleGravityField*> gravityFields_;
    std::vector<PhysicsSpring> springs_;
    void applySpringForces(double deltaTime);
};

#endif // GALAXYPHYSICSCONTROLLER_H
