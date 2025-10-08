#include <iostream>
#include "Algorithm.h"
#include "Galaxy.h"
#include "CelestialObject.h"
#include "Planet.h"
#include "Star.h"
#include "Dijkstra.h"
#include "Graph_list.h"
#include "Nebula.h"
#include "StarSystem.h"


int main() {

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
    galaxy.connectObjects(2, 4, 30);


    std::cout << "~~~GALAXY~~~\n\n";
    galaxy.showGalaxy();
    std::cout << "\n\n~~~STAR SYSTEM DETAILS~~~\n\n";
    solar.displayInfo();
    std::cout << std::endl<<std::endl;
    sir_b.displayInfo();
    std::cout << std::endl;


    GraphAlgorithm<Graph_list<CelestialObject*>, CelestialObject *> *distance = new DijkstraAlgorithm<Graph_list<CelestialObject*>, CelestialObject *>();
    distance->run(galaxy.getGraph(), 1, 3);
    return 0;
}
