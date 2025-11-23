#ifndef BLACKHOLEGRAVITYFIELD_H
#define BLACKHOLEGRAVITYFIELD_H

#include "CelestialBodyToRigidWrapper.h"
#include <vector>
#include <cmath>

/**
 * @class BlackHoleGravityField
 * @brief Calculates gravitational forces of a black hole on celestial bodies.
 */
class BlackHoleGravityField {
public:
    BlackHoleGravityField(double mass, double x = 0, double y = 0, double z = 0);

    void applyGravity(std::vector<CelestialBodyToRigidWrapper*>& bodies, double deltaTime);

    void setPosition(double x, double y, double z);
    void setMass(double mass);

private:
    double mass_;  // Mass of the black hole
    double posX_, posY_, posZ_;  // Position of the black hole

    static constexpr double G = 1.0;

    void applyForce(CelestialBodyToRigidWrapper* body, double deltaTime);
};

#endif // BLACKHOLEGRAVITYFIELD_H
