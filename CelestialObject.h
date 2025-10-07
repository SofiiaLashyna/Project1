#ifndef CELESTIALOBJECT_H
#define CELESTIALOBJECT_H
#include <string>

class CelestialObject {
public:
    std::string name;
    double mass;

    CelestialObject(const std::string &name, double mass);

    virtual ~CelestialObject() = default;

    virtual void displayInfo() const = 0;

    virtual std::string getType() const = 0;

    std::string getName();

    double getMass();
};


#endif //CELESTIALOBJECT_H
