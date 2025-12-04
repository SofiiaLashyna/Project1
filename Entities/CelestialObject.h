#ifndef CELESTIALOBJECT_H
#define CELESTIALOBJECT_H
#include <string>
/**
 * @file CelestialObject.h
 * @brief Defines the CelestialObject abstract base class.
 */

/**
 * @class CelestialObject
 * @brief An abstract base class for all celestial objects.
 *
 * Provides a common interface (name, mass, displayInfo)
 * that all derived classes must implement.
 */
class CelestialObject {
private:
    std::string name; ///< The common name of the object.
    double mass; ///< The mass of the object.
public:
    /**
         * @brief Constructor call for the CelestialObject class.
         * @param name A name of the object.
         * @param mass Mass of the object.
         */
    CelestialObject(const std::string &name, double mass);
/// @brief Default destructor call.
    virtual ~CelestialObject() = default;
    /**
          * @brief Display of all information about the object.
          * (Pure virtual method)
        */
    virtual void displayInfo() const = 0;
    /**
         * @brief Returns the type of an object.
         * (Pure virtual method)
       */
    virtual std::string getType() const = 0;
    /**
          * @brief Returns the name of an object.
          * @return The constant reference to the object's name.
        */
    std::string getName() const;
    /**
          * @brief Set the name of an object.
          * @param newName A name that is assigned to the object
        */
    void setName(const std::string& newName);
    /**
          * @brief Returns the mass of an object.
          * @return The mass of the object.
        */
    double getMass() const;
    /**
        * @brief Set the mass of an object.
        * @param newMass A mass that is assigned to the object
        * @throw "Mass must be greater than zero" if the mass <=0
      */
    void setMass(const double newMass);
};


#endif //CELESTIALOBJECT_H
