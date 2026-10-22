#ifndef CELESTIALOBJECT3DMODEL_H
#define CELESTIALOBJECT3DMODEL_H

#include <QAbstractListModel>
#include <QColor>
#include <vector>
#include "CelestialObject.h"

/**
 * @file CelestialObject3DModel.h
 * @brief Provides a data model to expose celestial objects to QML 3D scenes.
 */

/**
 * @enum CelestialObjectRoles
 * @brief Custom roles for accessing celestial object properties in QML.
 * * These roles map C++ data fields to names accessible within QML delegates
 * (e.g., using 'model.xPos' or 'model.objectColor').
 */
enum CelestialObjectRoles {
    XPosRole = Qt::UserRole + 1, ///< The X-coordinate in 3D space.
    YPosRole,                    ///< The Y-coordinate in 3D space.
    ZPosRole,                    ///< The Z-coordinate in 3D space.
    SizeFactorRole,              ///< Scaling factor for the object's visual model.
    ObjectColorRole,             ///< Base color of the celestial body.
    ObjectTypeRole               ///< Type identifier (Star, Planet, Nebula).
};

/**
 * @class CelestialObject3DModel
 * @brief A bridge between the C++ backend and the Qt Quick 3D frontend.
 *
 * This class implements a list model that stores references to CelestialObjects
 * and their current coordinates. It is optimized for the Model-View-Delegate
 * architecture, allowing the 3D engine to react to physics updates efficiently.
 */
class CelestialObject3DModel : public QAbstractListModel {
    Q_OBJECT

private:
    std::vector<double> currentX;           ///< Cached X-coordinates for rapid UI access.
    std::vector<double> currentY;           ///< Cached Y-coordinates for rapid UI access.
    std::vector<double> currentZ;           ///< Cached Z-coordinates for rapid UI access.
    std::vector<CelestialObject*> objects;  ///< Pointers to the source celestial data.

public:
    /**
     * @brief Constructs the model with a reference to the galaxy's objects.
     * @param parent The parent QObject.
     * @param objects Reference to the vector of celestial object pointers.
     */
    CelestialObject3DModel(QObject *parent, std::vector<CelestialObject*>& objects);

    /** @brief Default destructor. */
    ~CelestialObject3DModel() override = default;

    /**
     * @brief Returns the number of objects in the model.
     * Required by QAbstractListModel.
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    /**
     * @brief Maps custom roles to string names for use in QML.
     * (e.g., maps XPosRole to "xPos").
     */
    QHash<int, QByteArray> roleNames() const override;

    /**
     * @brief Retrieves data for a specific object and role.
     * Used by the QML view to get the position, color, or type of a body.
     * @param index The index of the item.
     * @param role The role requested by the view.
     * @return QVariant The data for the requested role.
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    /**
     * @brief Updates the cached 3D coordinates from the physics simulation.
     * * This method triggers the `dataChanged` signal for all items,
     * forcing the QML 3D scene to move the objects to their new positions.
     * * @param newX, newY, newZ Updated coordinate vectors.
     */
    void updatePositions(const std::vector<double>& newX,
                         const std::vector<double>& newY,
                         const std::vector<double>& newZ);

    /**
     * @brief Updates the entire list of objects (e.g., after galaxy regeneration).
     * * Resets the model to notify the view that the underlying data set has changed.
     * @param newObjects The new vector of celestial objects.
     */
    void updateObjects(std::vector<CelestialObject *> &newObjects);
};

#endif //CELESTIALOBJECT3DMODEL_H