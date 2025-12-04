#ifndef PLANETARYSYSTEMMODEL_H
#define PLANETARYSYSTEMMODEL_H

#include <QAbstractListModel>
#include <QColor>
#include <vector>
#include "StarSystem.h" // Твій клас StarSystem

class PlanetarySystemModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum PlanetRoles {
        OrbitRadiusRole = Qt::UserRole + 1,
        PlanetSizeRole,
        PlanetColorRole,
        RotationSpeedRole,
        TexturePathRole
    };

    explicit PlanetarySystemModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void updateSystem(StarSystem* system);
    void clear();

private:
    struct PlanetData {
        double orbitRadius;
        double size;
        QColor color;
        double speed;
        QString texturePath;
    };
    std::vector<PlanetData> m_planets;
};

#endif // PLANETARYSYSTEMMODEL_H