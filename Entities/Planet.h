#ifndef PLANET_H
#define PLANET_H
#include <QColor>

#include "CelestialObject.h"

class Planet : public CelestialObject {
private:
    double orbitRadius;
    double orbitSpeed;
    double inclination;
    bool habitable;
    QColor color = QColor(0,0,0);
public:
    enum class planetType { Gas_Giant, Dwarf, Terrestrial_Planet };

    Planet(const std::string &name, double mass, double orbit, double speed, double angle, planetType type, bool life);

    void displayInfo() const override;

    double getOrbit() const;

    void setColor(QColor newColor);

    QColor getColor();

    bool isHabitable() const;

    void setLifeExistence(bool life);

    std::string getType() const override;

    planetType getPlanetType();

private:
    planetType planet_type;
};

#endif //PLANET_H
