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
    g.addObject(star);
    EXPECT_EQ(g.getGraph().getVertices().size(), 1);
}

TEST_F(GalaxyListFixture, ConnectObjects) {
    Star* star2 = new Star("Alpha", 1.0, 5000, Star::starType::Main_sequence_Star);

    g.addObject(star);
    g.addObject(star2);
    g.connectObjects(0, 1, 100);
    EXPECT_TRUE(g.getGraph().edgeExists(0, 1));

    delete star2;
}
TEST_F(GalaxyListFixture, GetGraph) {
    g.addObject(star);

    auto &graph = g.getGraph();
    EXPECT_EQ(&graph, &g.getGraph()) << "getGraph should return the same object";

    auto verts = graph.getVertices();
    ASSERT_FALSE(verts.empty()) << "Vertices should not be empty";
    EXPECT_EQ(verts[0].getData()->getName(), star->getName()) << "Vertex should store object";
}

TEST_F(GalaxyListFixture, ShowGalaxyOutput) {
    g.addObject(star);
    g.addObject(planet);

    std::stringstream out;
    CoutGuard guard(out);
    g.showGalaxy();

    std::string output = out.str();
    EXPECT_NE(output.find("Sun"), std::string::npos);
    EXPECT_NE(output.find("Earth"), std::string::npos);
}