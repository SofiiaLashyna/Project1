#ifndef STARSYSTEM_H
#define STARSYSTEM_H
#include <vector>
#include <algorithm>
#include <iostream>
#include "CelestialObject.h"
#include "Planet.h"
#include "Star.h"
/**
 * @file StarSystem.h
 * @brief Defines the StarSystem class, derived from CelestialObject.
 */

/**
 * @class StarSystem
 * @brief A concrete class representing a StarSystem.
 *
 * Implements the CelestialObject interface and adds
 * system-specific properties like id, vector of planets, type of central star.
 */
class StarSystem : public CelestialObject {
private:
    int id; ///< The unique ID of the star system.
    Star *centralStar; ///< Pointer to the central star (owned by this class).
    std::vector<Planet *> planets; ///< Vector of planet pointers (owned by this class).
    /**
         * @brief Sorts the internal planets vector by orbital radius.
         */

    void sortPlanetsByOrbit();

public:
    /**
     * @brief Constructs a StarSystem.
     * @param id System ID.
     * @param name Name of the system.
     * @param star Pointer to the central star (ownership transferred).
     */
    StarSystem(int id, std::string name, Star *star);
    /// @brief Custom destructor to manage memory and delete pointers.
    ~StarSystem();
    /**
          * @brief Adds specific planet.
          * @param planet A planet that is being added.
        */
    void addPlanet(Planet *planet);
    /**
          * @brief Removes the last planet.
          * Used when it is not important which planet we have had removed.
        */
    void removePlanet();
    /**
         * @brief Removes specific planet.
         * @param name The name of the planet to remove.
       */
    void removePlanet(const std::string &name);
    /**
                  * @brief Returns the central star of a system.
                  * @return A reference to the central Star object
                */
    Star &getStar();
    /**
                      * @brief Returns the vector of planets of a system.
                      * @return A reference to the vector of Planet pointers.
                    */
    std::vector<Planet *> &getPlanets();

    /**
              * @brief Displays all information about the system (overrides CelestialObject).
            */
    void displayInfo() const override;
    /**
              * @brief Returns the specific type of the object (overrides CelestialObject).
              * @return The string "StarSystem".
            */
    std::string getType() const override;
    /**
              * @brief Makes the calculations for the mass of a system relying on the component's masses.
              * @return A double representation of mass
            */
    double calculateMass();
    /**
              * @brief Returns the id parameter of the star system.
              * @return The int value of the id
            */
    int getId();
  /**
            * @brief Makes the calculations whether the planet has a chance of being habitable.
            * Uses star type and radius and changes planet parameter habitable.
            * @param planet A pointer to the planet to update.
          */
    void lifeExists(Planet &planet);
};
#endif //STARSYSTEM_H
