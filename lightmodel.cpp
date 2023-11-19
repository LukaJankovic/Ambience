#include "lightmodel.h"

LightModel::LightModel(LifxLAN *lifxLAN, QObject *parent)
    : QAbstractListModel(parent)
    , lifxLAN(lifxLAN)
{

    connect(lifxLAN,
            &LifxLAN::savedLightsUpdated,
            this,
            &LightModel::lightListUpdated);

    connect(lifxLAN,
            &LifxLAN::lightUpdated,
            this,
            &LightModel::lightUpdated);
}

int LightModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    return lights.length();
}

QVariant LightModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    return lights.at(index.row())->getLabel();
}

/*
 * Public slots
 */

void LightModel::lightListUpdated(QList<Light *> lights)
{
    for (const auto &light : lifxLAN->saved)
    {
        lifxLAN->sendPacket(light, LifxPacket::getLabel(light->getSerial()));

        int row = lights.length();

        beginInsertRows(QModelIndex(), row, row);
        lights.append(light);
        endInsertRows();

        emit dataChanged(createIndex(0, 0), createIndex(rowCount(), 0));
    }
}

void LightModel::lightUpdated(Light *light)
{
    for (int i = 0; i < lifxLAN->saved.length(); i++)
    {
        if (lifxLAN->saved[i] == light) {
            lights[i] = light;
            QModelIndex index = createIndex(i, 0);
            emit dataChanged(index, index);
            return;
        }
    }
}
