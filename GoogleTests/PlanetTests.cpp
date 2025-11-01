#include "gtest/gtest.h"
#include "Planet.h"

TEST(PlanetTest, Initialization) {
    Planet* planet = new Planet("Earth", 1.0, 149.6, 29.8, 0.0, Planet::planetType::Terrestrial_Planet, true);

    EXPECT_EQ(planet->getName(), "Earth") << "Name should be correctly assigned";
    EXPECT_DOUBLE_EQ(planet->getMass(), 1.0) << "Mass should match constructor parameter";
    EXPECT_EQ(planet->getPlanetType(), Planet::planetType::Terrestrial_Planet)
        << "Planet type should match constructor parameter";
delete planet;
}

TEST(PlanetTest, GetType) {
    Planet* planet = new Planet("Jupiter", 318.0, 778.5, 13.1, 0.5, Planet::planetType::Gas_Giant, false);
    std::string type = planet->getType();

    EXPECT_EQ(type, "Planet") << "getType() should return correct planet type string";
    delete planet;
}

TEST(PlanetTest, InvalidMass) {
    EXPECT_THROW({
        Planet* p = new Planet("Invalid", -1.0, 100.0, 10.0, 0.0, Planet::planetType::Terrestrial_Planet, true);
    }, std::invalid_argument);
}

