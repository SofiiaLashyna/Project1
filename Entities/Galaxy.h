#ifndef GALAXY_H
#define GALAXY_H

#include <utility>
#include <iostream>
#include <vector>
#include <fstream>
#include <qstring.h>

#include "CelestialObject.h"
#include "Nebula.h"
#include "StarSystem.h"


/**
 * @file Galaxy.h
 * @brief Defines the Galaxy template class, the main container for the simulation.
 *
 * @example
 * @code
 * // Example: creating a Galaxy with a GraphList backend
 * #include "Galaxy.h"
 * #include "GraphList.h"
 *
 * void main() {
 * // 1. Instantiate the Galaxy
 * Galaxy<GraphList<CelestialObject*>> milkyWay("Milky Way");
 *
 * // 2. Create objects (Galaxy takes ownership of pointers)
 * Star* sun = new Star("Sun", 1.0, 5778, Star::starType::Main_sequence_Star);
 * StarSystem* solarSystem = new StarSystem(1, "Solar System", sun);
 * Nebula* crabNebula = new Nebula("Crab Nebula", 10.0, Nebula::nebulaType::Supernova);
 *
 * // 3. Add objects
 * milkyWay.addObject(solarSystem); // ID 0
 * milkyWay.addObject(crabNebula);  // ID 1
 *
 * // 4. Connect them (distance = 100 light years)
 * milkyWay.connectObjects(0, 1, 100);
 *
 * // 5. Display info
 * milkyWay.showGalaxy();
 * }
 * @endcode
 */

/**
 * @class Galaxy
 * @brief Manages all celestial objects and their relationships (graph) in a galaxy.
 *
 * This class acts as the top-level container, owning all CelestialObject
 * pointers (StarSystem, Nebula, etc.) and managing the graph that connects them.
 *
 * @tparam GraphType The type of graph used to store object relationships
 * (e.g., GraphList<CelestialObject*> or GraphMatrix<CelestialObject*>).
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
     * @brief Adds specific object to the galaxy and the underlying graph.
     * @param obj A pointer to a CelestialObject to add.
     * @note The Galaxy class takes ownership of the `obj` pointer.
     *
     * @example
     * @code
     * Galaxy<GraphList<CelestialObject*>> g("Andromeda");
     * StarSystem* sys = new StarSystem(...);
     * g.addObject(sys); // 'sys' is now managed by 'g'
     * @endcode
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
     * @brief Connects two objects with an edge representing distance.
     * @param id1 An id of the first object to be connected.
     * @param id2 An id of the second object to be connected.
     * @param distance The weight of the edge (e.g., distance in light years).
     *
     * @example
     * @code
     * // Connect object at index 0 and index 5 with distance 42
     * galaxy.connectObjects(0, 5, 42);
     * @endcode
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

    /// @brief Display of all information about the galaxy and its objects to std::cout.
    void showGalaxy() {
        std::cout << "Galaxy: " << name << "\n";

        for (auto i: celestial_objects) {
            i->displayInfo();
        }
    }

    /**
     * @brief Generates a formatted QString with galaxy parameters for the UI.
     *
     * Iterates through all objects to count types (Nebulae, StarSystems, Planets)
     * and calculate the total mass.
     *
     * @note This method is NOT const because it calls system->calculateMass(),
     * which may update the internal state of a StarSystem.
     * @return A QString containing an HTML unordered list of parameters.
     *
     * @example
     * @code
     * QString info = galaxy.getGalaxyParameters();
     * // Result example:
     * // "<ul><li>Galaxy name: Milky Way</li><li>Star systems: 10</li>...</ul>"
     * myLabel->setText(info);
     * @endcode
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

};

#endif //GALAXY_H
