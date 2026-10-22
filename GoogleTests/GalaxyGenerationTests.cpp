#include "gtest/gtest.h"
#include "Galaxy.h"
#include "GalaxyFactory.h"
#include "GraphList.h"
#include "RandomUtilities.h"
#include "StarSystem.h"
#include "Planet.h"
#include "Nebula.h"
#include "TestFixtures.h"

TEST_F(GalaxyGenerationTestFixture, GeneratePlanetCreatesValidObject) {
    Planet *planet = GalaxyFactory::createPlanet(testData, rng);

    ASSERT_NE(planet, nullptr);
    EXPECT_EQ(planet->getType(), "Planet");

    EXPECT_EQ(planet->getPlanetType(), Planet::planetType::Terrestrial_Planet);

    EXPECT_GE(planet->getMass(), 1.0);
    EXPECT_LE(planet->getMass(), 1.0);

    delete planet;
}

TEST_F(GalaxyGenerationTestFixture, GenerateNebulaCreatesValidObject) {
    Nebula *nebula = GalaxyFactory::createNebula(testData, rng);

    ASSERT_NE(nebula, nullptr);
    EXPECT_EQ(nebula->getType(), "Nebula");

    EXPECT_EQ(nebula->getNebulaTypeString(), "Emission");

    EXPECT_GE(nebula->getMass(), 1.0);
    EXPECT_LE(nebula->getMass(), 100.0);

    delete nebula;
}

TEST_F(GalaxyGenerationTestFixture, GenerateStarSystemCreatesValidSystem) {
    StarSystem *system = GalaxyFactory::createStarSystem(101, testData, rng);

    ASSERT_NE(system, nullptr);
    EXPECT_EQ(system->getId(), 101);
    EXPECT_EQ(system->getStar().getType(), "Star");

    size_t planetCount = system->getPlanets().size();
    EXPECT_GE(planetCount, 0);
    EXPECT_LE(planetCount, 5);

    if (planetCount > 0) {
        EXPECT_TRUE(system->getPlanets()[0]->isHabitable());
    }

    delete system;
}
