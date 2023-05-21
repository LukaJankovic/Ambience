#ifndef LIFXLAN_H
#define LIFXLAN_H

#include <QObject>
#include <QtNetwork>
#include <QDebug>

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
    QUdpSocket *scanSocket;
    QHash<QHostAddress, Light> devices;


private slots:
    void messageReceived();
};

#endif // LIFXLAN_H
