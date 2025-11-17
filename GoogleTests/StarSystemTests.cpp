#include "gtest/gtest.h"
#include "StarSystem.h"
#include "Star.h"
#include "Planet.h"
#include "TestFixtures.h"

TEST_F(StarSystemFixture, Initialization) {
    EXPECT_EQ(system->getId(), 1);
    EXPECT_EQ(system->getStar().getName(), star->getName());
    EXPECT_TRUE(system->getPlanets().empty());
}

TEST_F(StarSystemFixture, AddPlanet) {
    Planet* p1 = new Planet("Earth", 1.0, 1.0, 1, 0, Planet::planetType::Terrestrial_Planet, true);

    size_t before = system->getPlanets().size();
    system->addPlanet(p1);
    size_t after = system->getPlanets().size();

    EXPECT_EQ(after, before + 1);

    bool found = false;
    for (auto &p : system->getPlanets())
        if (p->getName() == "Earth") found = true;

    EXPECT_TRUE(found);

}

TEST_F(StarSystemFixture, RemovePlanet) {
    Planet* p1 = new Planet("Mars", 0.5, 1.2, 12, 0, Planet::planetType::Terrestrial_Planet, false);
    Planet* p2 = new Planet("Venus", 0.8, 0.9, 11, 0.2, Planet::planetType::Terrestrial_Planet, false);

    system->addPlanet(p1);
    system->addPlanet(p2);

    size_t before = system->getPlanets().size();
    system->removePlanet("Mars");
    size_t after = system->getPlanets().size();

    EXPECT_EQ(after, before - 1);

    bool stillThere = false;
    for (auto &p : system->getPlanets())
        if (p->getName() == "Mars") stillThere = true;
    EXPECT_FALSE(stillThere);

    system->removePlanet("Pluto");
    EXPECT_EQ(system->getPlanets().size(), after);

}

TEST_F(StarSystemFixture, GetType) {
    EXPECT_EQ(system->getType(), "StarSystem");
}
