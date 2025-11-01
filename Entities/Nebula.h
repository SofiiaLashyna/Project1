#ifndef NEBULA_H
#define NEBULA_H
#include "CelestialObject.h"

class Nebula : public CelestialObject {
public:
    enum class nebulaType { Planetary, Supernova, Emission, Reflection, Dark };

    Nebula(const std::string &name, double mass, nebulaType type);

    void displayInfo() const override;

    std::string getType() const override;

    nebulaType getNebulaType();

    void setNebulaType(std::string newtype);
private:
    nebulaType nebula_type;
};
#endif //NEBULA_H
