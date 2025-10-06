#ifndef CELESTIALOBJECTS_H
#define CELESTIALOBJECTS_H
#include <string>

class CelestialObjects {
public:
    std::string name;
    double mass;
    CelestialObjects(const std::string& name, double mass);
    virtual ~CelestialObjects() = default;

    virtual void displayInfo() const = 0;
    virtual std::string getType() const = 0;

    std::string getName();
    double getMass();
};

class Nebula : public CelestialObjects{
    enum class nebulaType{Planetary, Supernova, Emission, Reflection, Dark};
    nebulaType nebula_type;

public:
    Nebula(const std::string& name, double mass, nebulaType type);

    void displayInfo() const override;
    std::string getType() const override;
};
class Star : public CelestialObjects {
private:
    double temperature;
    enum class starType {Red_Giant, White_Dwarf, Neutron_Star, Red_Dwarf, Brown_Dwarf, Main_sequence_Star};
    starType star_type;
public:
    Star(const std::string& name, double mass, double temperature, starType type);

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
    Planet(const std::string& name, double mass, double orbit, double speed, double angle, planetType type);

    void displayInfo() const override;
    std::string getType() const override;
};
#endif //CELESTIALOBJECTS_H
