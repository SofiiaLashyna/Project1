#include <iostream>
#include "Algorithm.h"
#include "Galaxy.h"
#include "CelestialObject.h"
#include "Planet.h"
#include "Star.h"
#include "Dijkstra_list.h"
#include "GraphWidget.h"
#include "Graph_list.h"
#include "Nebula.h"
#include "StarSystem.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[]) {

    Star sun("Sun", 1.0, 5778, Star::starType::Main_sequence_Star);
    Star betelgeuse("Betelgeuse", 11.6, 3500, Star::starType::Red_Giant);
    Star sirius_b("Sirius B", 1.02, 25000, Star::starType::White_Dwarf);

    Planet earth("Earth", 1.0, 1.0, 29.78, 0.0, Planet::planetType::Terrestrial_Planet);
    Planet jupiter("Jupiter", 317.8, 5.2, 13.07, 0.0, Planet::planetType::Gas_Giant);

    StarSystem solar(1, "Solar system", sun);
    StarSystem sir_b(2, " Sirius binary star system", sirius_b);

    Nebula cat_eye(" Cat's Eye Nebula", 0.1, Nebula::nebulaType::Planetary);

    solar.addPlanet(earth);
    solar.addPlanet(jupiter);

    Galaxy<Graph_list<CelestialObject*>> galaxy("MilkyWay");
    galaxy.addObject(&solar);
    galaxy.addObject(&sir_b);
    galaxy.addObject(&betelgeuse);
    galaxy.addObject(&cat_eye);

    galaxy.connectObjects(1, 2, 4);
    galaxy.connectObjects(2, 3, 6);
    galaxy.connectObjects(1, 3, 16);
    galaxy.connectObjects(3, 4, 30);


    std::cout << "~~~GALAXY~~~\n\n";
    galaxy.showGalaxy();
    std::cout << "\n\n~~~STAR SYSTEM DETAILS~~~\n\n";
    solar.displayInfo();
    std::cout << std::endl<<std::endl;
    sir_b.displayInfo();
    std::cout << std::endl;


    GraphAlgorithm<Graph_list<CelestialObject*>, CelestialObject *> *distance = new DijkstraAlgorithm< CelestialObject *>();
    distance->run(galaxy.getGraph(), 1, 3);


    QApplication app(argc, argv);
    std::vector<W_Vertex> vertices;
    int i = 0;
    for (auto obj : galaxy.getObject()) {
        vertices.push_back({i, 0, 0, QString::fromStdString(obj->getName())}); // додамо ім'я для виводу
        ++i;
    }

    std::vector<W_Edge> edges;
    for (auto &e : galaxy.getGraph().getEdges()) {
        edges.push_back({e.from, e.to, e.weight});
    }

    int n = vertices.size();
    int radius = 150;
    int cx = 200, cy = 200;
    for(int i = 0; i < n; ++i) {
        vertices[i].x = cx + radius * cos(2*M_PI*i/n);
        vertices[i].y = cy + radius * sin(2*M_PI*i/n);
    }

    GraphWidget graphWidget;
    graphWidget.resize(400, 400);
    graphWidget.setGraph(vertices, edges);
    graphWidget.show();

    return app.exec();
}
