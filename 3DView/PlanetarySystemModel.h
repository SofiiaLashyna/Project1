#ifndef PLANETARYSYSTEMMODEL_H
#define PLANETARYSYSTEMMODEL_H

#include <QAbstractListModel>
#include <QColor>
#include <vector>
#include "StarSystem.h"

/**
 * @file PlanetarySystemModel.h
 * @brief Data model for visualizing planets within a focused star system in 3D.
 */

/**
 * @class PlanetarySystemModel
 * @brief A list model that exposes individual planet data to the QML 3D engine.
 *
 * This class translates the planetary data contained within a `StarSystem` into
 * a format that QML can use to instantiate 3D spheres, apply textures, and
 * animate orbital movements.
 */
class PlanetarySystemModel : public QAbstractListModel {
    Q_OBJECT
public:
    /**
     * @enum PlanetRoles
     * @brief Roles used to access specific planetary attributes in QML.
     */
    enum PlanetRoles {
        OrbitRadiusRole = Qt::UserRole + 1, ///< Radius of the planet's orbit from the star.
        PlanetSizeRole,                    ///< Calculated size (scale) of the planet model.
        PlanetColorRole,                   ///< Base color of the planet (for fallback or tinting).
        RotationSpeedRole,                 ///< Orbital velocity for 3D animation.
        TexturePathRole                    ///< Local path to the planet's surface texture file.
    };

    /**
     * @brief Constructor for PlanetarySystemModel.
     * @param parent The parent QObject for memory management.
     */
    explicit PlanetarySystemModel(QObject *parent = nullptr);

    /**
     * @brief Returns the number of planets in the currently selected system.
     * @return Count of planets in `m_planets`.
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    /**
     * @brief Provides data for a planet at a specific index.
     * Maps the internal `PlanetData` struct to the requested `PlanetRoles`.
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    /**
     * @brief Maps the `PlanetRoles` to property names usable in QML (e.g., "orbitRadius").
     */
    QHash<int, QByteArray> roleNames() const override;

    /**
     * @brief Populates the model with planets from a given StarSystem.
     * * This method extracts data from the `StarSystem`'s internal list of planets,
     * converts them into the simplified `PlanetData` format, and triggers a UI refresh.
     * * @param system Pointer to the StarSystem containing the planets to be displayed.
     */
    void updateSystem(StarSystem* system);

    /**
     * @brief Clears all planetary data from the model.
     * Useful when zooming out from a star system back to the galaxy view.
     */
    void clear();

private:
    /**
     * @struct PlanetData
     * @brief Simplified storage for planet attributes optimized for the UI layer.
     */
    struct PlanetData {
        double orbitRadius;    ///< Distance from the system center.
        double size;           ///< Visual scale factor.
        QColor color;          ///< Planet's diffuse color.
        double speed;          ///< Speed for orbital rotation logic in QML.
        QString texturePath;   ///< Path to the 3D texture resource.
    };

    /** @brief Internal container of processed planetary data. */
    std::vector<PlanetData> m_planets;
};

#endif // PLANETARYSYSTEMMODEL_H