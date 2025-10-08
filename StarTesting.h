#ifndef STARTESTING_H
#define STARTESTING_H
#include <cassert>
#include <iostream>
#include "Star.h"

class StarTesting {
public:
    static void runAll() {
        testInitialization();
        testGetType();
    }

private:
    static void testInitialization() {
        Star star("Sun", 1.0, 5778.0, Star::starType::Main_sequence_Star);

        assert(star.getName() == "Sun" && "Name should be correctly assigned");
        assert(star.getMass() == 1.0 && "Mass should match constructor parameter");
        assert(star.star_type == Star::starType::Main_sequence_Star && "Star type should match constructor parameter");

        std::cout << "1/2 passed\n";
    }

    static void testGetType() {
        Star star("Betelgeuse", 20.0, 3500.0, Star::starType::Red_Giant);
        std::string type = star.getType();

        assert((type == "Star") &&
               "getType() should return correct star type string");

        std::cout << "2/2 passed\n";
    }
};
#endif //STARTESTING_H
