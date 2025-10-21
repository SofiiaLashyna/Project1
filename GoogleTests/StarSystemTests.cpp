#include <gtest/gtest.h>
#include "StarSystem.h"
#include "Star.h"
#include "Planet.h"

TEST(StarSystemTest, Initialization) {
    Star star("Sun", 1.0, 1000, Star::starType::Main_sequence_Star);
    StarSystem system(1, "Solar", star);

    EXPECT_EQ(system.getId(), 1) << "ID should be correctly assigned";
    EXPECT_EQ(&system.getStar(), &star) << "getStar() should return the same star";
    EXPECT_TRUE(system.getPlanets().empty()) << "Initially planets vector should be empty";
}

TEST(StarSystemTest, AddPlanet) {
    Star star("Sun", 1.0, 1.0, Star::starType::Main_sequence_Star);
    StarSystem system(2, "Alpha", star);

    Planet p1("Earth", 1.0, 1.0, 1, 0, Planet::planetType::Terrestrial_Planet);
    size_t before = system.getPlanets().size();
    system.addPlanet(p1);
    size_t after = system.getPlanets().size();

    EXPECT_EQ(after, before + 1) << "Planet count should increase after addPlanet()";

    bool found = false;
    for (auto &p : system.getPlanets())
        if (p.getName() == "Earth") found = true;

    EXPECT_TRUE(found) << "Added planet should exist in the system";
}

TEST(StarSystemTest, RemovePlanet) {
    Star star("Sun", 1.0, 1.0, Star::starType::Main_sequence_Star);
    StarSystem system(3, "Beta", star);

    Planet p1("Mars", 0.5, 1.2, 12, 0, Planet::planetType::Terrestrial_Planet);
    Planet p2("Venus", 0.8, 0.9, 11, 0.2, Planet::planetType::Terrestrial_Planet);
    system.addPlanet(p1);
    system.addPlanet(p2);

    size_t before = system.getPlanets().size();
    system.removePlanet("Mars");
    size_t after = system.getPlanets().size();

    EXPECT_EQ(after, before - 1) << "Planet count should decrease after removePlanet()";

    bool stillThere = false;
    for (auto &p : system.getPlanets())
        if (p.getName() == "Mars") stillThere = true;

    EXPECT_FALSE(stillThere) << "Removed planet should not exist anymore";

    system.removePlanet("Pluto");
}

TEST(StarSystemTest, GetType) {
    Star star("Sun", 1.0, 1.0, Star::starType::Main_sequence_Star);
    StarSystem system(4, "Gamma", star);

    std::string type = system.getType();
    EXPECT_EQ(type, "StarSystem") << "getType() should return 'StarSystem'";
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
