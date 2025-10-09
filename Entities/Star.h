#ifndef STAR_H
#define STAR_H
#include "CelestialObject.h"

class Star : public CelestialObject {
private:
    double temperature;

public:
    enum class starType { Red_Giant, White_Dwarf, Neutron_Star, Red_Dwarf, Brown_Dwarf, Main_sequence_Star };

    starType star_type;

    Star(const std::string &name, double mass, double temperature, starType type);

    void displayInfo() const override;

    std::string getType() const override;
};
#endif //STAR_H
