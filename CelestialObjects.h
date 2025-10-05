#ifndef CELESTIALOBJECTS_H
#define CELESTIALOBJECTS_H
#include <string>

class CelestialObjects {
    std::string name;
    double mass;
public:
    CelestialObjects(std::string& name, double mass);
    virtual ~CelestialObjects() = default;

    virtual void displayInfo() const = 0;
    virtual std::string getType() const = 0;

    std::string getName();
    double getMass();
};
class Nebula {
    std::string nebulaName;
    enum class nebulaType{Planetary, Supeernova};
};
class Star : public CelestialObjects {
private:
    double temperature;
    enum class starType {Red_Giant, White_Dwarf, Neutron_Star, Red_Dwarf, Brown_Dwarf, Main_sequence_Star};
    starType star_type;
public:
    Star(std::string& name, double mass, double temperture, starType type);

    void displayInfo() const override;
    std::string getType() const override;
};
class Planet : public CelestialObjects{
private:
    enum class planetType {Gas_Giant, Dwarf, Terrestrial_Planet};
    planetType planet_type;
    double orbitRadius;
    double orbitSpeed;
    double currentAngle;
public:
    Planet(std::string& name, double mass, double orbit, double speed, planetType type);

    void orbitStep(double deltaTime);
    void displayInfo() const override;
    std::string getType() const override;
};
// class Nat_satellite : public CelestialObjects{
//
// };
//
// class Orbit {
// private:
//     double radius;
//     double angularVelocity;
// public:
//     Orbit(double radius, double angularVelocity);
//
//     double getRadius() const;
//     double getAngularVelocity() const;
// };
#endif //CELESTIALOBJECTS_H
