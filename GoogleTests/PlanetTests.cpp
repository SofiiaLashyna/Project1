#include <gtest/gtest.h>
#include "Planet.h"

TEST(PlanetTest, Initialization) {
    Planet planet("Earth", 1.0, 149.6, 29.8, 0.0, Planet::planetType::Terrestrial_Planet);

    EXPECT_EQ(planet.getName(), "Earth") << "Name should be correctly assigned";
    EXPECT_DOUBLE_EQ(planet.getMass(), 1.0) << "Mass should match constructor parameter";
    EXPECT_EQ(planet.planet_type, Planet::planetType::Terrestrial_Planet)
        << "Planet type should match constructor parameter";
}

TEST(PlanetTest, GetType) {
    Planet planet("Jupiter", 318.0, 778.5, 13.1, 0.5, Planet::planetType::Gas_Giant);
    std::string type = planet.getType();

    EXPECT_EQ(type, "Planet") << "getType() should return correct planet type string";
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
