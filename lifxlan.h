#ifndef LIFXLAN_H
#define LIFXLAN_H

#include <QObject>
#include <QtNetwork>
#include <QtAlgorithms>

#include "lifxpacket.h"
#include "light.h"

class LifxLAN : public QObject
{
    Q_OBJECT
public:
    explicit LifxLAN(QObject *parent = nullptr);
    ~LifxLAN();

    void startScan();

private:
    QUdpSocket *socket;
    QHash<QHostAddress, Light*> scanned;

signals:
    void scanFoundLight(Light *light);

private slots:
    void messageReceived();
};

QVariantList convertToVariantList(const QList<quint8>& list);
QList<quint8> convertFromVariantList(const QVariantList& variantList);

#endif // LIFXLAN_H
