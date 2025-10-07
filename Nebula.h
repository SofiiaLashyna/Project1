#ifndef NEBULA_H
#define NEBULA_H
#include "CelestialObject.h"

class Nebula : public CelestialObject {
public:
    enum class nebulaType { Planetary, Supernova, Emission, Reflection, Dark };

    nebulaType nebula_type;

    Nebula(const std::string &name, double mass, nebulaType type);

    void displayInfo() const override;

    std::string getType() const override;
};
#endif //NEBULA_H
