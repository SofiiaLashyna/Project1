
#ifndef TESTFIXTURES_H
#define TESTFIXTURES_H

#include <BFSList.h>
#include <BFSMatrix.h>
#include <DFSList.h>
#include <DFSMatrix.h>
#include <DijkstraList.h>
#include <DijkstraMatrix.h>

#include "GraphAlgorithms.h"
#include "Galaxy.h"
#include "GraphList.h"
#include "GraphMatrix.h"
#include <string>
#include "gtest/gtest.h"
#include "nlohmann/json.hpp"
#include "RandomUtilities.h"

using json = nlohmann::json;

/**
 * @brief Fixture for testing Galaxy with GraphMatrix.
 *
 * Provides a Star and a Planet for basic setup.
 * Cleans up resources after each test.
 */
using GalaxyType = Galaxy<GraphMatrix<CelestialObject *> >;

class GalaxyMatrixFixture : public ::testing::Test {
protected:
    GalaxyType g;
    Star* star = nullptr;
    Planet* planet = nullptr;

    void SetUp() override {
        star = new Star("Sun", 1.0, 5778, Star::starType::Main_sequence_Star);
        planet = new Planet("Earth", 1.0, 1.0, 30.0, 0.0, Planet::planetType::Terrestrial_Planet, true);
    }

    void TearDown() override {
        delete planet;
        delete star;
    }
};

/**
 * @brief Fixture for testing Galaxy with GraphList.
 */
using GalaxyType2 = Galaxy<GraphList<CelestialObject *> >;

class GalaxyListFixture : public ::testing::Test {
protected:
    GalaxyType2 g;
    Star* star = nullptr;
    Planet* planet = nullptr;

    void SetUp() override {
        star = new Star("Sun", 1.0, 5778, Star::starType::Main_sequence_Star);
        planet = new Planet("Earth", 1.0, 1.0, 30.0, 0.0, Planet::planetType::Terrestrial_Planet, true);
    }

    void TearDown() override {
        delete star;
        delete planet;
    }
};

/**
 * @brief Fixture for testing StarSystem.
 */
class StarSystemFixture : public ::testing::Test {
protected:
    Star* star;
    StarSystem* system;

    void SetUp() override {
        star = new Star("Sun", 1.0, 5778.0, Star::starType::Main_sequence_Star);
        system = new StarSystem(1, "Solar", star);
    }

    void TearDown() override {
        delete system;
    }
};

/**
 * @brief Fixture for testing GraphMatrix with basic vertices and edges.
 */
class GraphMatrixFixture : public ::testing::Test {
protected:
    GraphMatrix<std::string> g;
    int NOT_FOUND = -1;

    void SetUp() override {
        g.addVertex(1, "A");
        g.addVertex(2, "B");
        g.addVertex(3, "C");
        g.addEdge(1, 2);
        g.addEdge(1, 3);
    }
};


/**
 * @brief Fixture for testing BFS algorithms on GraphMatrix.
 */
class BFSMatrixFixture : public GraphMatrixFixture {
protected:
    BFSMatrixAlgorithm<std::string> *bfsAlg;

    void SetUp() override {
        GraphMatrixFixture::SetUp();
        bfsAlg = new BFSMatrixAlgorithm<std::string>;
    }

    void TearDown() override {
        delete bfsAlg;
        GraphMatrixFixture::TearDown();
    }
};

/**
 * @brief Fixture for testing DFS algorithms on GraphMatrix.
 */
class DFSMatrixFixture : public GraphMatrixFixture {
protected:
    DFSMatrixAlgorithm<std::string> *dfsAlg;

    void SetUp() override {
        GraphMatrixFixture::SetUp();
        dfsAlg = new DFSMatrixAlgorithm<std::string>();
    }

    void TearDown() override {
        delete dfsAlg;
        GraphMatrixFixture::TearDown();
    }
};

/**
 * @brief Fixture for testing Dijkstra algorithms on GraphMatrix.
 */
class DijkstraMatrixFixture : public GraphMatrixFixture {
protected:
    DijkstraMatrixAlgorithm<std::string> *djkAlg;

    void SetUp() override {
        GraphMatrixFixture::SetUp();
        djkAlg = new DijkstraMatrixAlgorithm<std::string>();
    }

    void TearDown() override {
        delete djkAlg;
        GraphMatrixFixture::TearDown();
    }
};

/**
 * @brief Fixture for testing GraphList with basic vertices and edges.
 */
class GraphListFixture : public ::testing::Test {
protected:
    GraphList<std::string> g;
    int NOT_FOUND = -1;

    void SetUp() override {
        g.addVertex(1, "A");
        g.addVertex(2, "B");
        g.addVertex(3, "C");
        g.addEdge(1, 2);
        g.addEdge(1, 3);
    }
};

/**
 * @brief Fixture for testing BFS algorithms on GraphList.
 */
class BFSListFixture : public GraphListFixture {
protected:
    BFSListAlgorithm<std::string>* bfsAlg;

    void SetUp() override {
        GraphListFixture::SetUp();
        bfsAlg = new BFSListAlgorithm<std::string>;
    }

    void TearDown() override {
        delete bfsAlg;
        GraphListFixture::TearDown();
    }
};
/**
 * @brief Fixture for testing DFS algorithms on GraphList.
 */
class DFSListFixture : public GraphListFixture {
protected:
    DFSListAlgorithm<std::string>* dfsAlg;

    void SetUp() override {
        GraphListFixture::SetUp();
        dfsAlg = new DFSListAlgorithm<std::string>;
    }

    void TearDown() override {
        delete dfsAlg;
        GraphListFixture::TearDown();
    }
};
/**
 * @brief Fixture for testing Dijkstra algorithms on GraphList.
 */
class DijkstraListFixture : public GraphListFixture {
protected:
    DijkstraListAlgorithm<std::string>* djkAlg;

    void SetUp() override {
        GraphListFixture::SetUp();
        djkAlg = new DijkstraListAlgorithm<std::string>;
    }

    void TearDown() override {
        delete djkAlg;
        GraphListFixture::TearDown();
    }
};
/**
 * @brief Fixture for testing RandomGenerator.
 */
class RandomGeneratorTest : public ::testing::Test {
protected:
    RandomGenerator rng;
};

/**
 * @brief Fixture for testing Galaxy generation using JSON data.
 */
class GalaxyGenerationTestFixture : public GalaxyListFixture {
protected:
    RandomGenerator rng;

    nlohmann::json testData = nlohmann::json::parse(R"({
        "Stars": [
            {"starType": "Main_sequence_Star", "temperature": [1000, 5000], "mass": [1, 5], "name": "C:/Users/Prj/Project1/RandomGalaxy/names/mainSequenceStar.txt"}
        ],
        "Planets": [
            {"planetType": "Terrestrial_Planet",
             "orbitRadius": [1.5, 1.5],
             "orbitSpeed": [1, 1],
             "Inclination": [0, 0],
             "mass": [1, 1],
             "name": "C:/Users/Prj/Project1/RandomGalaxy/names/terrestrialPlanet.txt"}
        ],
        "Nebulae": [
             {"nebulaType": "Emission", "mass": [1, 100], "name": "C:/Users/Prj/Project1/RandomGalaxy/names/emissionNebula.txt"}
        ]
    })");
};

#endif // TESTFIXTURES_H
