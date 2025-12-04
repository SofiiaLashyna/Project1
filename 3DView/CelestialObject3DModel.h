#ifndef CELESTIALOBJECT3DMODEL_H
#define CELESTIALOBJECT3DMODEL_H
#include <QAbstractListModel>
#include <QColor>
#include <vector>
#include "CelestialObject.h"

enum CelestialObjectRoles {
    XPosRole = Qt::UserRole + 1,
    YPosRole,
    ZPosRole,
    SizeFactorRole,
    ObjectColorRole,
    ObjectTypeRole
};

class CelestialObject3DModel : public QAbstractListModel {
    Q_OBJECT
private:
    std::vector<double> currentX;
    std::vector<double> currentY;
    std::vector<double> currentZ;
    std::vector<CelestialObject*> objects;
public:
    CelestialObject3DModel(QObject *parent, std::vector<CelestialObject*>& objects);
    ~CelestialObject3DModel() override = default;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void updatePositions(const std::vector<double>& newX,
                         const std::vector<double>& newY,
                         const std::vector<double>& newZ);

    void updateObjects(std::vector<CelestialObject *> &newObjects);
};
#endif //CELESTIALOBJECT3DMODEL_H