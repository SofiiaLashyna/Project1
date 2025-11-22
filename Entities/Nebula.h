#ifndef NEBULA_H
#define NEBULA_H
#include "CelestialObject.h"
/**
 * @file Nebula.h
 * @brief Defines the Nebula class, derived from CelestialObject.
 */

/**
 * @class Nebula
 * @brief A concrete derived class for all celestial objects.
 *
 * Receives a common interface (name, mass, displayInfo)
 * and implements it's specific methods.
 */
class Nebula : public CelestialObject {
public:
    /// Enum representing all supported nebula types.
    enum class nebulaType { Planetary, Supernova, Emission, Reflection, Dark };
    /**
         * @brief Constructor call for the Nebula class.
         * @param name A name of the nebula.
         * @param mass Mass of the nebula.
         * @param type A type of the nebula.
         * @throws std::invalid_argument If mass is zero or negative.
         */
    Nebula(const std::string &name, double mass, nebulaType type);
    /**
          * @brief Display of all information about the object.
          * (Virtual method override).
        */
    void displayInfo() const override;
    /**
          * @brief Returns the type of an object.
          * (Virtual method override).
          * @return The string "Nebula".
        */
    std::string getType() const override;
    /**
          * @brief Returns the type of nebula.
          * @return The enum value (nebulaType) of the nebula.
        */
    nebulaType getNebulaType();
    /**
          * @brief Returns the type of nebula converted to type string.
          * @return A string representation of the nebulae type
        */
    std::string getNebulaTypeString() const;
    /**
          * @brief Set the type of nebula.
          * @param newtype A type that is assigned to the nebula.
        */
    void setNebulaType(std::string newtype);
    /// @brief Default destructor call.
    virtual ~Nebula() = default;
private:
    nebulaType nebula_type; ///< Specific type of the nebula (Planetary, Supernova, etc.)
};
#endif //NEBULA_H
