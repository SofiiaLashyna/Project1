#ifndef GALAXYFACTORY_H
#define GALAXYFACTORY_H

#include "nlohmann/json.hpp"
#include "RandomUtilities.h"
#include "Galaxy.h"
#include "GraphList.h"
#include "Planet.h"
#include "StarSystem.h"
#include "Nebula.h"

/**
 * @file GalaxyFactory.h
 * @brief Header file for the GalaxyFactory class, responsible for celestial object generation.
 */

/**
 * @class GalaxyFactory
 * @brief A factory class that handles the creation and population of celestial entities.
 *
 * The GalaxyFactory provides a centralized way to generate complex objects like
 * Star Systems, Planets, and Nebulae based on configuration data (JSON) and
 * randomness. It follows the Factory Method pattern to decouple the object
 * creation logic from the Galaxy's structural management.
 */
class GalaxyFactory {
public:

    /**
     * @brief Populates the galaxy with randomly generated objects.
     *
     * This method orchestrates the creation of the entire galactic map. It uses
     * the factory methods to create objects, adds them to the galaxy's graph
     * structure, and establishes connections (edges) with associated weights
     * (distances) between them.
     *
     * @param galaxy Reference to the Galaxy object (template with GraphList of CelestialObject pointers).
     * @param data JSON object containing generation parameters (e.g., density, ranges, counts).
     * @param rng Reference to the RandomGenerator utility for consistent or seeded randomness.
     *
     * @note The galaxy takes ownership of the created pointers.
     * * @example
     * @code
     * std::ifstream f("universe_config.json");
     * nlohmann::json data = nlohmann::json::parse(f);
     * RandomGenerator rng;
     * * Galaxy<GraphList<CelestialObject*>> myGalaxy;
     * GalaxyFactory::populateGalaxy(myGalaxy, data, rng);
     * @endcode
     */
    static void populateGalaxy(Galaxy<GraphList<CelestialObject*>>& galaxy, const nlohmann::json& data, RandomGenerator& rng);

    /**
     * @brief Factory method to generate a new random Planet.
     * * Creates a Planet instance with randomized attributes (e.g., atmosphere, size)
     * based on the constraints provided in the JSON data.
     * * @param data JSON data containing generation parameters for planets.
     * @param rng Reference to the random number generator utility.
     * @return Planet* A pointer to the newly created Planet.
     * @warning The caller (or the container) is responsible for deleting the allocated memory.
     */
    static Planet* createPlanet(const nlohmann::json& data, RandomGenerator& rng);

    /**
     * @brief Factory method to generate a new random StarSystem.
     * * Creates a StarSystem, which might contain its own sub-objects or specific
     * stellar classifications.
     * * @param id The unique identifier to be assigned to the new star system.
     * @param data JSON data containing generation parameters for star systems.
     * @param rng Reference to the random number generator utility.
     * @return StarSystem* A pointer to the newly created StarSystem.
     * @warning The caller is responsible for memory management.
     */
    static StarSystem* createStarSystem(int id, const nlohmann::json& data, RandomGenerator& rng);

    /**
     * @brief Factory method to generate a new random Nebula.
     * * Generates a Nebula object, typically representing a region of gas/dust
     * with specific environmental effects or resource densities.
     * * @param data JSON data containing generation parameters for nebulae.
     * @param rng Reference to the random number generator utility.
     * @return Nebula* A pointer to the newly created Nebula.
     * @warning The caller is responsible for memory management.
     */
    static Nebula* createNebula(const nlohmann::json& data, RandomGenerator& rng);
};

#endif // GALAXYFACTORY_H