#ifndef LIGHTMODEL_H
#define LIGHTMODEL_H

#include <QAbstractItemModel>

#include "lifxlan.h"

class LightModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit LightModel(QObject *parent = nullptr);

    // Basic functionality:
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

public slots:
    void lightsUpdated(QList<Light *> lights);

private:
    LifxLAN *lifxLAN;

};

#endif // LIGHTMODEL_H
