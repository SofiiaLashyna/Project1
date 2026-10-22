#include "GalaxyFactory.h"
#include <iostream>

Planet* GalaxyFactory::createPlanet(const nlohmann::json& data, RandomGenerator& rng) {
    const auto &planets = data["Planets"];
    const auto &pData = planets[rng.getInt(0, static_cast<int>(planets.size()) - 1)];
    std::string planetName = rng.getRandomNameFromFile(pData["name"]);
    std::string pTypeStr = pData["planetType"];
    double planetMass = rng.getDouble(pData["mass"][0], pData["mass"][1]);
    double distance = rng.getDouble(pData["orbitRadius"][0], pData["orbitRadius"][1]);
    double speed = rng.getDouble(pData["orbitSpeed"][0], pData["orbitSpeed"][1]);
    double inclination = rng.getDouble(pData["Inclination"][0], pData["Inclination"][1]);

    Planet::planetType pType;
    if (pTypeStr == "Gas_Giant") pType = Planet::planetType::Gas_Giant;
    else if (pTypeStr == "Terrestrial_Planet") pType = Planet::planetType::Terrestrial_Planet;
    else pType = Planet::planetType::Dwarf;

    Planet *planet = new Planet(planetName, planetMass, distance, speed, inclination, pType, false);

    int r = 100, g = 100, b = 100;
    if (pType == Planet::planetType::Gas_Giant) {
        r = rng.getInt(100, 255); g = rng.getInt(100, 200); b = rng.getInt(50, 255);
    } else if (pType == Planet::planetType::Terrestrial_Planet) {
        r = rng.getInt(50, 150); g = rng.getInt(100, 255); b = rng.getInt(100, 255);
    }
    planet->setColor(r, g, b);

    bool hasRings = (pType == Planet::planetType::Gas_Giant) ? (rng.getInt(0, 10) > 3) : (rng.getInt(0, 10) > 8);
    if (hasRings) {
        planet->setRings(true, 1.2, rng.getDouble(1.5, 2.8), r + 20, g + 20, b + 20);
    }
    return planet;
}

StarSystem* GalaxyFactory::createStarSystem(int id, const nlohmann::json& data, RandomGenerator& rng) {
    const auto &stars = data["Stars"];
    const auto &sData = stars[rng.getInt(0, static_cast<int>(stars.size()) - 1)];
    std::string starName = rng.getRandomNameFromFile(sData["name"]);
    std::string typeStr = sData["starType"];
    double starMass = rng.getDouble(sData["mass"][0], sData["mass"][1]);
    double temp = rng.getDouble(sData["temperature"][0], sData["temperature"][1]);

    Star::starType sType = Star::starType::Main_sequence_Star; // Default
    if (typeStr == "White_Dwarf") sType = Star::starType::White_Dwarf;
    else if (typeStr == "Red_Giant") sType = Star::starType::Red_Giant;
    
    Star *star = new Star(starName, starMass, temp, sType);
    StarSystem *system = new StarSystem(id, starName + "'s system", star);
    
    int planetCount = rng.getInt(0, 5);
    for (int i = 0; i < planetCount; ++i) {
        Planet *planet = createPlanet(data, rng);
        system->addPlanet(planet);
        system->lifeExists(*planet);
    }
    system->setMass(system->calculateMass());
    return system;
}

Nebula* GalaxyFactory::createNebula(const nlohmann::json& data, RandomGenerator& rng) {
    const auto &nebulae = data["Nebulae"];
    const auto &nData = nebulae[rng.getInt(0, static_cast<int>(nebulae.size()) - 1)];
    std::string nebulaName = rng.getRandomNameFromFile(nData["name"]);
    std::string typeStr = nData["nebulaType"];
    double mass = rng.getDouble(nData["mass"][0], nData["mass"][1]);

    Nebula::nebulaType nType = Nebula::nebulaType::Planetary;
    if (typeStr == "Emission") nType = Nebula::nebulaType::Emission;
    else if (typeStr == "Supernova") nType = Nebula::nebulaType::Supernova;

    return new Nebula(nebulaName, mass, nType);
}

void GalaxyFactory::populateGalaxy(Galaxy<GraphList<CelestialObject*>>& galaxy, const nlohmann::json& data, RandomGenerator& rng) {
    int systemCount = rng.getInt(40, 60);
    for (int i = 0; i < systemCount; ++i)
        galaxy.addObject(createStarSystem(i, data, rng));

    int nebulaCount = rng.getInt(20, 65);
    for (int i = 0; i < nebulaCount; ++i)
        galaxy.addObject(createNebula(data, rng));

    for (int i = 0; i < galaxy.objectCount() - 1; ++i) {
        int j = rng.getInt(i + 1, galaxy.objectCount() - 1);
        galaxy.connectObjects(i, j, rng.getInt(100, 450));
    }
    std::cout << "Galaxy successfully generated via GalaxyFactory!" << std::endl;
}