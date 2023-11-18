#include "lightmodel.h"

LightModel::LightModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    connect(lifxLAN,
            &LifxLAN::savedLightsUpdated,
            this,
            &LightModel::lightsUpdated);
}

QModelIndex LightModel::index(int row, int column, const QModelIndex &parent) const
{
    // FIXME: Implement me!
}

QModelIndex LightModel::parent(const QModelIndex &index) const
{
    // FIXME: Implement me!
}

int LightModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    return lifxLAN->saved.length();
}

int LightModel::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    return 3;
}

QVariant LightModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        return QVariant();
    }
}

void LightModel::lightsUpdated(QList<Light *> lights)
{
    emit dataChanged(createIndex(0,0), createIndex(rowCount(), columnCount()));
}
