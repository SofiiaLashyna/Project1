#ifndef PLANETTESTING_H
#define PLANETTESTING_H
#include <cassert>
#include <iostream>
#include "Planet.h"

class PlanetTesting {
public:
    static void runAll() {
        testInitialization();
        testGetType();
    }

private:
    static void testInitialization() {
        Planet planet("Earth", 1.0, 149.6, 29.8, 0.0, Planet::planetType::Terrestrial_Planet);

        assert(planet.getName() == "Earth" && "Name should be correctly assigned");
        assert(planet.getMass() == 1.0 && "Mass should match constructor parameter");
        assert(planet.planet_type == Planet::planetType::Terrestrial_Planet &&
               "Planet type should match constructor parameter");


        std::cout << "1/2 passed\n";
    }

    static void testGetType() {
        Planet planet("Jupiter", 318.0, 778.5, 13.1, 0.5, Planet::planetType::Gas_Giant);
        std::string type = planet.getType();

        assert((type == "Planet") &&
               "getType() should return correct planet type string");

        std::cout << "2/2 passed\n";
    }
};
#endif //PLANETTESTING_H
