#include <GraphList.h>
#include <gtest/gtest.h>
#include "Galaxy.h"
#include "CelestialObject.h"
#include "Star.h"
#include "Planet.h"

using GalaxyType = Galaxy<GraphList<CelestialObject*>>;

TEST(GalaxyTest, Initialization) {
    GalaxyType g("Milky Way");
    EXPECT_TRUE(g.getGraph().getVertices().empty()) << "Graph should be empty initially";
}

TEST(GalaxyTest, AddObject) {
    GalaxyType g;

    Star star("Sun", 1.0, 5778, Star::starType::Main_sequence_Star);
    g.addObject(&star);
    EXPECT_EQ(g.getGraph().getVertices().size(), 1) << "Graph should have 1 vertex after adding object";

    g.addObject(nullptr);
    EXPECT_EQ(g.getGraph().getVertices().size(), 2) << "Graph should handle nullptr";
}

TEST(GalaxyTest, ConnectObjects) {
    GalaxyType g;

    Star star1("Alpha", 1.0, 5000, Star::starType::Main_sequence_Star);
    Star star2("Beta", 1.0, 5000, Star::starType::Main_sequence_Star);
    g.addObject(&star1);
    g.addObject(&star2);

    g.connectObjects(1, 2, 100);

    EXPECT_TRUE(g.getGraph().edgeExists(1, 2)) << "Vertices should be connected";
    EXPECT_FALSE(g.getGraph().edgeExists(1, 3)) << "Non-existent vertex should not crash";
}

TEST(GalaxyTest, GetGraph) {
    GalaxyType g;

    Star star("Sun", 1.0, 5778, Star::starType::Main_sequence_Star);
    g.addObject(&star);

    auto &graph = g.getGraph();
    EXPECT_EQ(&graph, &g.getGraph()) << "getGraph should return the same object";

    auto verts = graph.getVertices();
    ASSERT_FALSE(verts.empty()) << "Vertices should not be empty";
    EXPECT_NE(verts[0].data, nullptr) << "Vertex should store object";
}

TEST(GalaxyTest, ShowGalaxyOutput) {
    GalaxyType g;

    Star star("Sun", 1.0, 5778, Star::starType::Main_sequence_Star);
    Planet planet("Earth", 1.0, 1.0, 30.0, 0.0, Planet::planetType::Terrestrial_Planet);

    g.addObject(&star);
    g.addObject(&planet);

    std::stringstream out;
    std::streambuf* oldCout = std::cout.rdbuf(out.rdbuf());

    g.showGalaxy();

    std::cout.rdbuf(oldCout);

    std::string output = out.str();
    EXPECT_NE(output.find("Sun"), std::string::npos) << "Output should contain star name";
    EXPECT_NE(output.find("Earth"), std::string::npos) << "Output should contain planet name";
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
