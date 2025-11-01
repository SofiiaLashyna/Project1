#ifndef STAR_H
#define STAR_H
#include "CelestialObject.h"

class Star : public CelestialObject {

public:
    enum class starType { Red_Giant, White_Dwarf, Neutron_Star, Red_Dwarf, Brown_Dwarf, Main_sequence_Star };

    Star(const std::string &name, double mass, double temperature, starType type);

    void displayInfo() const override;

    std::string getType() const override;

    void setStarType(std::string type);

    starType getStarType();

private:
    double temperature;
    starType star_type;
};
#endif //STAR_H
