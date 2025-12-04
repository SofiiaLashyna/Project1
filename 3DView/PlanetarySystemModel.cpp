#include "PlanetarySystemModel.h"
#include <QDebug>
#include <cmath>
#include <limits>
#include <algorithm>
#include <RandomUtilities.h>

PlanetarySystemModel::PlanetarySystemModel(QObject *parent)
    : QAbstractListModel(parent) {}

int PlanetarySystemModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return static_cast<int>(m_planets.size());
}

QVariant PlanetarySystemModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_planets.size())
        return QVariant();

    const auto &planet = m_planets[index.row()];

    switch (role) {
        case OrbitRadiusRole: return planet.orbitRadius;
        case PlanetSizeRole:  return planet.size;
        case PlanetColorRole: return planet.color;
        case RotationSpeedRole: return planet.speed;
        case TexturePathRole: return planet.texturePath;
        default: return QVariant();
    }
}

QHash<int, QByteArray> PlanetarySystemModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[OrbitRadiusRole] = "orbitRadius";
    roles[PlanetSizeRole] = "planetSize";
    roles[PlanetColorRole] = "planetColor";
    roles[RotationSpeedRole] = "rotationSpeed";
    roles[TexturePathRole] = "texturePath";
    return roles;
}

void PlanetarySystemModel::updateSystem(StarSystem* system) {
    beginResetModel();
    m_planets.clear();

    if (system) {
        auto& planets = system->getPlanets();

        double minMass = std::numeric_limits<double>::max();
        double maxMass = std::numeric_limits<double>::lowest();

        for (const auto &p: planets) {
            minMass = std::min(minMass, p->getMass());
            maxMass = std::max(maxMass, p->getMass());
        }

        if (std::abs(maxMass - minMass) < 0.0001) maxMass = minMass + 1.0;

        double baseOrbitStart = 250.0;
        double orbitSpacing = 60.0;
        for (size_t i = 0; i < planets.size(); ++i) {
            auto& p = planets[i];
            PlanetData data;

            double massNorm = (p->getMass() - minMass) / (maxMass - minMass);
            data.size = 0.1 + (0.1 * massNorm);
            data.orbitRadius = baseOrbitStart + (i * orbitSpacing);
            data.color = p->getColor().isValid() ? p->getColor() : QColor("white");
            data.speed = 5.0 * std::pow(data.orbitRadius, 1.5);


            if (p->getTexturePath().empty()) {
                std::string newPath;
                RandomGenerator rng;
                int randomNum = rng.getInt(1,3);
                switch (p->getPlanetType()) {
                    case Planet::planetType::Gas_Giant:
                        if (randomNum % 3 == 0) {
                            newPath ="qrc:/3DView/textures/gas_g1.jpg";
                        }
                        else if (randomNum % 3 == 1) {
                            newPath ="qrc:/3DView/textures/gas_g2.jpg";
                        }
                        else{
                            newPath ="qrc:/3DView/textures/gas_g3.jpg";
                        }
                    break;

                    case Planet::planetType::Terrestrial_Planet:
                        if (p->isHabitable()) {
                            newPath = "qrc:/3DView/textures/habitable_terrestrial1.jpg";
                        } else {
                            if (randomNum % 2 == 0)
                                newPath = "qrc:/3DView/textures/terrestrial1.jpg";
                            else
                                newPath = "qrc:/3DView/textures/terrestrial2.jpg";
                        }
                    break;

                    case Planet::planetType::Dwarf:
                        default:
                            if (randomNum % 3 == 0)
                                newPath = "qrc:/3DView/textures/dwarf1.jpg";
                            else if (randomNum % 3 == 1)
                                newPath = "qrc:/3DView/textures/dwarf2.jpg";
                            else
                                newPath = "qrc:/3DView/textures/dwarf2.jpg";
                    break;
                }
                p->setTexturePath(newPath);
            }
            data.texturePath = QString::fromStdString(p->getTexturePath());

            m_planets.push_back(data);
        }
    }
    endResetModel();
}

void PlanetarySystemModel::clear() {
    beginResetModel();
    m_planets.clear();
    endResetModel();
}