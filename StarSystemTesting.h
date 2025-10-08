#ifndef STARSYSTEMTESTING_H
#define STARSYSTEMTESTING_H

#include <cassert>
#include <iostream>
#include "StarSystem.h"
#include "Star.h"
#include "Planet.h"

class StarSystemTesting {
public:
    static void runAll() {
        testInitialization();
        testAddPlanet();
        testRemovePlanet();
        testGetType();
    }

private:
    static void testInitialization() {
        Star star("Sun", 1.0, 1000, Star::starType::Main_sequence_Star);
        StarSystem system(1, "Solar", star);

        assert(system.getId() == 1 && "ID should be correctly assigned");
        assert(&system.getStar() == &star && "getStar() should return the same star");
        assert(system.getPlanets().empty() && "Initially planets vector should be empty");

        std::cout << "1/4 passed\n";
    }

    static void testAddPlanet() {
        Star star("Sun", 1.0, 1.0, Star::starType::Main_sequence_Star);
        StarSystem system(2, "Alpha", star);

        Planet p1("Earth", 1.0, 1.0, 1, 0, Planet::planetType::Terrestrial_Planet);
        size_t before = system.getPlanets().size();
        system.addPlanet(p1);
        size_t after = system.getPlanets().size();

        assert(after == before + 1 && "Planet count should increase after addPlanet()");

        bool found = false;
        for (auto &p : system.getPlanets())
            if (p.getName() == "Earth") found = true;

        assert(found && "Added planet should exist in the system");
        std::cout << "2/4 passed\n";
    }

    static void testRemovePlanet() {
        Star star("Sun", 1.0, 1.0, Star::starType::Main_sequence_Star);
        StarSystem system(3, "Beta", star);

        Planet p1("Mars", 0.5, 1.2, 12, 0, Planet::planetType::Terrestrial_Planet);
        Planet p2("Venus", 0.8, 0.9, 11, 0.2, Planet::planetType::Terrestrial_Planet);
        system.addPlanet(p1);
        system.addPlanet(p2);

        size_t before = system.getPlanets().size();
        system.removePlanet("Mars");
        size_t after = system.getPlanets().size();

        assert(after == before - 1 && "Planet count should decrease after removePlanet()");
        bool stillThere = false;
        for (auto &p : system.getPlanets())
            if (p.getName() == "Mars") stillThere = true;

        assert(!stillThere && "Removed planet should not exist anymore");

        system.removePlanet("Pluto");
        std::cout << "3/4 passed\n";
    }

    static void testGetType() {
        Star star("Sun", 1.0, 1.0, Star::starType::Main_sequence_Star);
        StarSystem system(4, "Gamma", star);

        std::string type = system.getType();
        assert(type == "StarSystem" && "getType() should return 'StarSystem'");
        std::cout << "4/4 passed\n";
    }
};
#endif //STARSYSTEMTESTING_H
