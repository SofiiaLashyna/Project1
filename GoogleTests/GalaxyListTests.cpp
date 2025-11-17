#include <RAIIGuard.h>

#include "GraphList.h"
#include "TestFixtures.h"
#include "gtest/gtest.h"
#include "Galaxy.h"
#include "CelestialObject.h"
#include "Star.h"
#include "Planet.h"

TEST_F(GalaxyListFixture, Initialization) {
    EXPECT_TRUE(g.getGraph().getVertices().empty());
}

TEST_F(GalaxyListFixture, AddObject) {
    Star* starPtr = new Star("Sun", 1.0, 5800, Star::starType::Main_sequence_Star);
    g.addObject(starPtr);

    EXPECT_EQ(g.getGraph().getVertices().size(), 1);
    EXPECT_EQ(g.getGraph().getVertices()[0].getData()->getName(), "Sun");
    EXPECT_DOUBLE_EQ(g.getGraph().getVertices()[0].getData()->getMass(), 1.0);
}

TEST_F(GalaxyListFixture, ConnectObjects) {
    Star* star1 = new Star("Sun", 1.0, 5800, Star::starType::Main_sequence_Star);
    Star* star2 = new Star("Alpha", 1.5, 5000, Star::starType::Main_sequence_Star);

    g.addObject(star1);
    g.addObject(star2);

    g.connectObjects(0, 1, 100);

    EXPECT_TRUE(g.getGraph().edgeExists(0, 1));
}

TEST_F(GalaxyListFixture, GetGraph) {
    Star* starPtr = new Star("Sun", 1.0, 5800, Star::starType::Main_sequence_Star);
    g.addObject(starPtr);

    auto &graph = g.getGraph();
    EXPECT_EQ(&graph, &g.getGraph()) << "getGraph should return the same object";

    auto verts = graph.getVertices();
    ASSERT_FALSE(verts.empty()) << "Vertices should not be empty";

    EXPECT_EQ(verts[0].getData()->getName(), "Sun");
    EXPECT_DOUBLE_EQ(verts[0].getData()->getMass(), 1.0);
}

TEST_F(GalaxyListFixture, ShowGalaxyOutput) {
    Star* starPtr = new Star("Sun", 1.0, 5800, Star::starType::Main_sequence_Star);
    Planet* planetPtr = new Planet("Earth", 1.0, 1.0, 30.0, 0.0, Planet::planetType::Terrestrial_Planet, true);

    g.addObject(starPtr);
    g.addObject(planetPtr);

    std::stringstream out;
    CoutGuard guard(out);
    g.showGalaxy();

    std::string output = out.str();
    EXPECT_NE(output.find("Sun"), std::string::npos);
    EXPECT_NE(output.find("Earth"), std::string::npos);
}
