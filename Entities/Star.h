#ifndef STAR_H
#define STAR_H
#include "CelestialObject.h"
/**
 * @file Star.h
 * @brief Defines the Star class, derived from CelestialObject.
 */

/**
 * @class Star
 * @brief A concrete class representing a Star.
 *
 * Implements the CelestialObject interface and adds
 * star-specific properties like temperature, type.
 */
class Star : public CelestialObject {

public:
    /// Enum representing all supported star types.
    enum class starType { Red_Giant, White_Dwarf, Neutron_Star, Red_Dwarf, Brown_Dwarf, Main_sequence_Star };
    /**
             * @brief Constructor for the Star class.
             * @param name The name of the star.
             * @param mass The mass of the star.
             * @param temperature The temperature of the star (Kelvin).
             * @param type The type of the star (e.g., Red_Giant).
             * @throws std::invalid_argument If mass or temperature is zero or negative.
             */
    Star(const std::string &name, double mass, double temperature, starType type);
    /// @brief Displays all information about the star.
    void displayInfo() const override;
    /**
              * @brief Returns the specific type of the object.
              * @return The string "Star".
            */
    std::string getType() const override;
    /**
         * @brief Sets the star's enum type based on string input.
         * @param type String representation of the desired type.
         */
    void setStarType(std::string type);
    /**
                  * @brief Gets the specific enum type of this star.
                  * @return The enum value (starType) of the star.
                  */
    starType getStarType();
    /**
        * @brief Converts the enum star type to a readable string.
        * @return Text representation of the star type.
        */
    std::string getStarTypeString() const;
    /**
              * @brief Returns the temperature parameter of the star.
              * @return Temperature value in Kelvin.
            */
    double getTemperature();
    /// @brief Default virtual destructor.
    virtual ~Star() = default;
private:
    double temperature; ///< The surface temperature of the star (e.g., in Kelvin).
    starType star_type; ///< The specific ENUM type of the star (e.g., Red_Giant).
};
#endif //STAR_H
