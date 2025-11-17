#ifndef GALAXY_H
#define GALAXY_H
#include <utility>
#include <iostream>
#include <vector>
#include "CelestialObject.h"
#include <fstream>
#include <qstring.h>

#include "Nebula.h"
#include "StarSystem.h"
#include "nlohmann/json.hpp"
#include "RandomUtilities.h"
using json = nlohmann::json;

/**
 * @file Galaxy.h
 * @brief Defines the Galaxy template class, the main container for the simulation.
 */

/**
 * @class Galaxy
 * @brief Manages all celestial objects and their relationships (graph) in a galaxy.
 *
* This class acts as the top-level container, owning all CelestialObject
 * pointers (StarSystem, Nebula, etc.) and managing the graph that connects them.
 * @tparam GraphType The type of graph used to store object relationships.
 */
template<typename GraphType>
class Galaxy {
private:
    std::string name; ///< The name of the galaxy.
    GraphType systemGraph; ///< The graph representing relationships between objects.
    std::vector<CelestialObject *> celestial_objects; ///< Owns all objects in the galaxy.

public:
    /**
         * @brief Constructor for the Galaxy class.
         * @param name The name of the galaxy.
         */
    Galaxy(std::string name = "Unnamed Galaxy") : name(std::move(name)) {
    }

    /**
         * @brief Virtual destructor.
         * Cleans up memory by deleting all owned CelestialObject pointers.
         */
    virtual ~Galaxy() {
        for (CelestialObject *obj: celestial_objects) {
            delete obj;
        }
        celestial_objects.clear();
    }

    /**
          * @brief Gets the number of objects included to the galaxy.
          * @return The total count of celestial objects.
        */
    int objectCount() {
        return celestial_objects.size();
    }

    /**
                 * @brief Adds specific object.
                 * @param obj A pointer to a CelestialObject to add.
               */
    void addObject(CelestialObject *obj) {
        celestial_objects.push_back(obj);
        systemGraph.addVertex(static_cast<int>(celestial_objects.size()) - 1, obj);
    }

    /**
                  * @brief Gets the internal vector of objects included to the galaxy
                  * @return A reference to the vector of CelestialObject pointers.
                */

    std::vector<CelestialObject *> &getObject() {
        return celestial_objects;
    }

    /**
                          * @brief Connects to objects with an edge which stands for a distance.
                          * @param id1 An id of the first object to be connected.
                          * @param id2 An id of the second object to be connected.
                          * @param distance The weight of the edge (e.g., distance).
                        */
    void connectObjects(int id1, int id2, int distance) {
        systemGraph.addEdge(id1, id2, distance);
    }

    /**
                  * @brief Returns the internal graph object.
                  * @return A reference to the graph.
                */
    GraphType &getGraph() {
        return systemGraph;
    }

    /**
                      * @brief Returns the name of a galaxy.
                      * @return The string name of galaxy.
                    */
    std::string getName() {
        return name;
    }

    /**
     * @brief Set the name of a galaxy.
     * @param newName The new name of the galaxy.
                    */
    void setName(std::string newName) {
        name = newName;
    }

    /// @brief Display of all information about the galaxy and it's objects.
    void showGalaxy() {
        std::cout << "Galaxy: " << name << "\n";

        for (auto i: celestial_objects) {
            i->displayInfo();
        }
    }
    /**
     * @brief Generates a formatted QString with galaxy parameters for the UI.
     * @note This method is NOT const because it calls system->calculateMass(),
     * which may update the internal state of a StarSystem.
     * @return A QString containing an HTML list of parameters.
     */
    QString getGalaxyParameters() {
        int starSystems = 0;
        int planets = 0;
        int nebulae = 0;
        double galaxyMass = 0;
        for (auto i: celestial_objects) {
            if (i->getType() == "Nebula") {
                nebulae++;
                galaxyMass += i->getMass();
            }
            if (i->getType() == "StarSystem") {
                starSystems++;
                StarSystem *system = dynamic_cast<StarSystem *>(i);

                if (system) {
                    planets += system->getPlanets().size();
                    galaxyMass += system->calculateMass();
                }
            }
        }
        QString info;
        info += QString("<ul>");
        info += "<li> Galaxy name: " + QString::fromStdString(this->name) + "</li>";
        info += QString("<li>Star systems: %1</li>").arg(starSystems);
        info += QString("<li>Planets: %1</li>").arg(planets);
        info += QString("<li>Nebulae: %1</li>").arg(nebulae);
        info += QString("<li>Total mass: %1</li>").arg(galaxyMass, 0, 'g', 4);
        info += QString("</ul>");

        return info;
    }
    /**
             * @brief Factory method to generate a new random Planet.
             * @param rng Reference to the random number generator utility.
             * @param data JSON data containing generation parameters.
             * @return A pointer to a new Planet object (caller owns it).
             */
    Planet *generatePlanet(RandomGenerator &rng, const json &data) {
        const auto &planets = data["Planets"];
        const auto &pData = planets[rng.getInt(0, static_cast<int>(planets.size()) - 1)];
        std::string planetFile = pData["name"];
        std::string planetName = rng.getRandomNameFromFile(planetFile);
        std::string pTypeStr = pData["planetType"];
        double planetMass = rng.getDouble(pData["mass"][0], pData["mass"][1]);
        double distance = rng.getDouble(pData["orbitRadius"][0], pData["orbitRadius"][1]);
        double speed = rng.getDouble(pData["orbitSpeed"][0], pData["orbitSpeed"][1]);
        double inclination = rng.getDouble(pData["Inclination"][0], pData["Inclination"][1]);

        Planet::planetType pType;
        if (pTypeStr == "Gas_Giant") pType = Planet::planetType::Gas_Giant;
        else if (pTypeStr == "Terrestrial_Planet") pType = Planet::planetType::Terrestrial_Planet;
        else if (pTypeStr == "Dwarf") pType = Planet::planetType::Dwarf;
        Planet *planet = new Planet(planetName, planetMass, distance, speed, inclination, pType, false);
        return planet;
    }
    /**
             * @brief Factory method to generate a new random StarSystem.
             * @param id The ID to assign to the new system.
             * @param rng Reference to the random number generator utility.
             * @param data JSON data containing generation parameters.
             * @return A pointer to a new StarSystem object (caller owns it).
             */
    StarSystem *generateStarSystem(int id, RandomGenerator &rng, const json &data) {
        if (!data.contains("Stars") || !data["Stars"].is_array()) {
            std::cerr << "Stars key missing or not an array!" << std::endl;
            return 0;
        }

        const auto &stars = data["Stars"];
        const auto &sData = stars[rng.getInt(0, static_cast<int>(stars.size()) - 1)];

        std::string starFile = sData["name"];
        std::string starName = rng.getRandomNameFromFile(starFile);
        std::string typeStr = sData["starType"];

        double starMass = rng.getDouble(sData["mass"][0], sData["mass"][1]);
        double temperature = rng.getDouble(sData["temperature"][0], sData["temperature"][1]);

        Star::starType sType;
        if (typeStr == "White_Dwarf") sType = Star::starType::White_Dwarf;
        else if (typeStr == "Red_Giant") sType = Star::starType::Red_Giant;
        else if (typeStr == "Main_sequence_Star") sType = Star::starType::Main_sequence_Star;
        else if (typeStr == "Brown_Dwarf") sType = Star::starType::Brown_Dwarf;
        else if (typeStr == "Neutron_Star") sType = Star::starType::Neutron_Star;
        else sType = Star::starType::Red_Dwarf;

        Star *star = new Star(starName, starMass, temperature, sType);

        int planetCount = rng.getInt(0, 5);

        std::string name = starName + "'s system";
        auto *system = new StarSystem(id, name, star);
        for (int i = 0; i < planetCount; ++i) {
            Planet *planet = generatePlanet(rng, data);
            system->addPlanet(planet);
            system->lifeExists(*planet);
        }
        system->setMass(system->calculateMass());
        return system;
    }
    /**
             * @brief Factory method to generate a new random Nebula.
             * @param rng Reference to the random number generator utility.
             * @param data JSON data containing generation parameters.
             * @return A pointer to a new Nebula object (caller owns it).
             */
    Nebula *generateNebula(RandomGenerator &rng, const json &data) {
        const auto &nebulae = data["Nebulae"];
        const auto &nData = nebulae[rng.getInt(0, static_cast<int>(nebulae.size()) - 1)];
        std::string nebulaFile = nData["name"];
        std::string nebulaName = rng.getRandomNameFromFile(nebulaFile);
        std::string typeStr = nData["nebulaType"];
        double mass = rng.getDouble(nData["mass"][0], nData["mass"][1]);

        Nebula::nebulaType nType;
        if (typeStr == "Emission") nType = Nebula::nebulaType::Emission;
        else if (typeStr == "Reflection") nType = Nebula::nebulaType::Reflection;
        else if (typeStr == "Dark") nType = Nebula::nebulaType::Dark;
        else if (typeStr == "Supernova") nType = Nebula::nebulaType::Supernova;
        else nType = Nebula::nebulaType::Planetary;

        Nebula *nebula = new Nebula(nebulaName, mass, nType);
        return nebula;
    }
    /**
             * @brief Populates the galaxy with randomly generated objects.
             * This method calls the factory methods (generateStarSystem, etc.)
             * and adds the results to the galaxy, taking ownership.
             * @param data JSON data containing generation parameters.
             * @param rng Reference to the random number generator utility.
             */
    void generateGalaxy(const json &data, RandomGenerator &rng) {
        int systemCount = rng.getInt(3, 10);
        for (int i = 0; i < systemCount; ++i)
            this->addObject(this->generateStarSystem(i, rng, data));

        int nebulaCount = rng.getInt(1, 15);
        for (int i = 0; i < nebulaCount; ++i)
            this->addObject(this->generateNebula(rng, data));

        for (int i = 0; i < celestial_objects.size() - 1; ++i) {
            int j = rng.getInt(i + 1, celestial_objects.size() - 1);
            int dist = rng.getInt(10, 1000);
            systemGraph.addEdge(i, j, dist);
        }


        std::cout << "Galaxy successfully generated!" << std::endl;
    }
};
#endif //GALAXY_H
