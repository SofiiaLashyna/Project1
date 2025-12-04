#include "CelestialObject3DModel.h"
#include <QDebug>
#include "StarSystem.h"
#include "Nebula.h"

CelestialObject3DModel::CelestialObject3DModel(QObject *parent, std::vector<CelestialObject *> &celestialObjects) : QAbstractListModel(parent) {
    updateObjects(celestialObjects);
}

int CelestialObject3DModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return static_cast<int>(objects.size());
}

QVariant CelestialObject3DModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= rowCount())
        return QVariant();

    CelestialObject *obj = objects[index.row()];

    if (!obj) return QVariant();

    StarSystem *system = dynamic_cast<StarSystem*>(obj);
    Nebula *nebula = dynamic_cast<Nebula*>(obj);

    switch (role) {
        case XPosRole:
            // if (index.row() == 0)
            //     qDebug() << "Pos(0):" << currentX[0] << currentY[0] << currentZ[0];
            return currentX[index.row()];
        case YPosRole:
            return currentY[index.row()];
        case ZPosRole:
            return currentZ[index.row()];
        case SizeFactorRole: {
            double mass = obj->getMass();
            double baseScale;

            if (system) {
                baseScale = std::cbrt(mass > 0 ? mass : 1.0) * 1.0;
            } else if (nebula) {
                baseScale = std::cbrt(mass > 0 ? mass : 1.0) * 5.0;
            } else {
                baseScale = 1.0;
            }

            double finalScale = std::min(baseScale, 20.0)*0.1;

            // if (index.row() == 0)
            //     qDebug() << "Size(0):" << finalScale;

            return finalScale;
        }
        case ObjectTypeRole: {
            if (system) return 1;
            if (nebula) return 2;
            return 0;
        }

        case ObjectColorRole: {
            if (system) {
                Star::starType type = system->getStar().getStarType();
                switch(type) {
                    case Star::starType::Red_Giant: return QColor("red");
                    case Star::starType::Red_Dwarf: return QColor(255, 120, 80);
                    case Star::starType::White_Dwarf: return QColor("cyan");
                    case Star::starType::Neutron_Star: return QColor("blue");
                    case Star::starType::Brown_Dwarf: return QColor(139, 69, 19);
                    case Star::starType::Main_sequence_Star: return QColor("yellow");
                    default: return QColor("white");
                }
            } else if (nebula) {
                Nebula::nebulaType type = nebula->getNebulaType();
                switch(type) {
                    case Nebula::nebulaType::Emission: return QColor(255, 0, 127);
                    case Nebula::nebulaType::Reflection: return QColor(100, 149, 237);
                    case Nebula::nebulaType::Dark: return QColor(40, 40, 40);
                    case Nebula::nebulaType::Supernova: return QColor(255, 69, 0);
                    case Nebula::nebulaType::Planetary: return QColor(0, 255, 127);
                    default: return QColor("purple");
                }
            }
            return QColor("gray");
        }
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> CelestialObject3DModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[XPosRole] = "xPos";
    roles[YPosRole] = "yPos";
    roles[ZPosRole] = "zPos";
    roles[SizeFactorRole] = "sizeFactor";
    roles[ObjectColorRole] = "objectColor";
    roles[ObjectTypeRole] = "objectType";
    return roles;
}

void CelestialObject3DModel::updatePositions(const std::vector<double>& newX,
                                            const std::vector<double>& newY,
                                            const std::vector<double>& newZ)
{
    currentX = newX;
    currentY = newY;
    currentZ = newZ;

    if (!objects.empty()) {
        emit dataChanged(index(0), index(rowCount() - 1),
                         {XPosRole, YPosRole, ZPosRole});
    }
}

void CelestialObject3DModel::updateObjects(std::vector<CelestialObject*>& newObjects)
{
    beginResetModel();
    objects = newObjects;
    currentX.resize(objects.size(), 0.0);
    currentY.resize(objects.size(), 0.0);
    currentZ.resize(objects.size(), 0.0);
    endResetModel();
}