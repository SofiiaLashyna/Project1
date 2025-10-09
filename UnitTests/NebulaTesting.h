#ifndef NEBULATESTING_H
#define NEBULATESTING_H
#include <cassert>
#include <iostream>
#include "Nebula.h"

class NebulaTesting {
public:
    static void runAll() {
        testInitialization();
        testGetType();
    }

private:
    static void testInitialization() {
        Nebula nebula("Orion", 5.0, Nebula::nebulaType::Emission);

        assert(nebula.getName() == "Orion" && "Name should be correctly assigned");
        assert(nebula.getMass() == 5.0 && "Mass should match constructor parameter");
        assert(nebula.nebula_type == Nebula::nebulaType::Emission &&
               "Nebula type should match constructor parameter");

        std::cout << "1/2 passed\n";
    }

    static void testGetType() {
        Nebula nebula("Crab", 2.5, Nebula::nebulaType::Supernova);
        std::string type = nebula.getType();

        assert((type == "Nebula") &&
               "getType() should return correct nebula type string");

        std::cout << "2/2 passed\n";
    }
};
#endif //NEBULATESTING_H
