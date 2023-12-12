#ifndef LIGHTMODEL_H
#define LIGHTMODEL_H

#include <QAbstractListModel>

#include "lifxlan.h"
#include "light.h"

class LightModel : public QAbstractListModel
{
Q_OBJECT

public:
    explicit LightModel(LifxLAN *lifxLAN, QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Light *lightAtIndex(QModelIndex &index);

public slots:
    void lightUpdated(Light *light);
    void lightListUpdated(QList<Light *> lights);

private:
    LifxLAN *lifxLAN;
    QList<Light *> lights;
};

#endif // LIGHTMODEL_H
