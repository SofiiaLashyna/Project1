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
            if (index.row() == 0)
                qDebug() << "Pos(0):" << currentX[0] << currentY[0] << currentZ[0];
            return currentX[index.row()];
        case YPosRole:
            return currentY[index.row()];
        case ZPosRole:
            return currentZ[index.row()];
        case SizeFactorRole: {
            double mass = obj->getMass();
            double baseScale;

            if (system) {
                baseScale = (std::log10(mass > 0 ? mass : 1.0) + 1.0) * 15.0;
            } else if (nebula) {
                baseScale = 80.0;
            } else {
                baseScale = 1.0;
            }

            double finalScale = std::min(baseScale, 200.0);

            if (finalScale < 5.0) finalScale = 5.0;

            if (index.row() == 0)
                qDebug() << "Size(0):" << finalScale;

            return finalScale;
        }
        case ObjectColorRole: {
        if (system) {
            Star::starType type = system->getStar().getStarType();
            if (type == Star::starType::Red_Giant) return QColor("red");
            if (type == Star::starType::White_Dwarf) return QColor("lightblue");
            if (type == Star::starType::Main_sequence_Star) return QColor("yellow");
            return QColor("white");
        } else if (nebula) {
            Nebula::nebulaType type = nebula->getNebulaType();
            if (type == Nebula::nebulaType::Emission) return QColor(255, 100, 150);
            if (type == Nebula::nebulaType::Dark) return QColor(80, 60, 100);
            return QColor("purple");
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
