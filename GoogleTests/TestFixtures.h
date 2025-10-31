#ifndef TESTFIXTURES_H
#define TESTFIXTURES_H
#include "Algorithm.h"
#include "Galaxy.h"
#include "GraphList.h"
#include "GraphMatrix.h"
#include <string>
#include "gtest/gtest.h"

using GalaxyType = Galaxy<GraphMatrix<CelestialObject*>>;

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


using GalaxyType2 = Galaxy<GraphList<CelestialObject*>>;

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

class StarSystemFixture : public ::testing::Test {
protected:
    Star* star;
    StarSystem* system;

    void SetUp() override {
        star = new Star("Sun", 1.0, 5778.0, Star::starType::Main_sequence_Star);
        system = new StarSystem(1, "Solar", *star);
    }

    void TearDown() override {
        delete system;
        delete star;
    }
};

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

    void TearDown() override {}
};

class BFSMatrixFixture : public GraphMatrixFixture {
protected:
    Algorithms<GraphMatrix<std::string>, std::string>* alg;

    void SetUp() override {
        GraphMatrixFixture::SetUp();

        alg = new Algorithms<GraphMatrix<std::string>, std::string>(g);
    }

    void TearDown() override {
        delete alg;
        GraphMatrixFixture::TearDown();
    }
};

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

    void TearDown() override {}
};
class BFSListFixture : public GraphListFixture {
protected:
    Algorithms<GraphList<std::string>, std::string>* alg;

    void SetUp() override {
        GraphListFixture::SetUp();
        alg = new Algorithms<GraphList<std::string>, std::string>(g);
    }

    void TearDown() override {
        delete alg;
        GraphListFixture::TearDown();
    }
};


#endif //TESTFIXTURES_H
