#include "lightmodel.h"

LightModel::LightModel(LifxLAN *lifxLAN, QObject *parent)
    : QAbstractListModel(parent)
    , lifxLAN(lifxLAN)
{
}

/*!
 * \brief Amount of rows, matches internal lights list.
 * \param parent Parent node, should be invalid for list.
 * \return Amount of rows in list.
 */
int LightModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return lights.length();
}

/*!
 * \brief Contents of each row. For now only light label.
 * \param index Index of row to give data to.
 * \param role Role to be updated.
 * \return Data to be set.
 */
QVariant LightModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
        return QVariant::fromValue(lights.at(index.row())->getLabel());
    if (role == Qt::UserRole)
        return QVariant::fromValue(lights.at(index.row()));
    return QVariant();
}

/*!
 * \brief Get light pointer from index.
 * \param index Index of light to be retrieved.
 * \return Light *.
 */
Light *LightModel::lightAtIndex(QModelIndex &index)
{
    if (!index.isValid())
        return nullptr;

    return lights[index.row()];
}

/*
 * Public slots
 */

/*!
 * \brief Called whenever lifxLAN updates lights list. Recreates local list.
 * \param lights New list.
 */
void LightModel::lightListUpdated(QList<Light *> lights)
{
    beginRemoveRows(QModelIndex(), 0, this->lights.length() - 1);
    this->lights.clear();
    endRemoveRows();

    for (const auto &light : lights)
    {
        //lifxLAN->sendPacket(light, LifxPacket::getLabel(light->getSerial()));

        int row = lights.length();

        beginInsertRows(QModelIndex(), row, row);
        this->lights.append(light);
        endInsertRows();
    }
}

/*!
 * \brief Called whenever a light gets updated. Updates rows if light in list.
 * \param light The light that updated.
 */
void LightModel::lightUpdated(Light *light)
{
    for (int i = 0; i < lights.length(); i++)
    {
        if (lights[i] == light) {
            lights[i] = light;
            QModelIndex index = createIndex(i, 0);
            emit dataChanged(index, index);
            return;
        }
    }
}
