#ifndef LIFXLAN_H
#define LIFXLAN_H

#include <QObject>
#include <QtNetwork>

#include "light.h"

class LifxLAN : public QObject
{
    Q_OBJECT
public:
    explicit LifxLAN(QObject *parent = nullptr);
    ~LifxLAN();

    QList<Light> scan();

private:
    QUdpSocket *udpSocket;

    QByteArray getFrameHeader();
    QByteArray getFrameAddress();
    QByteArray getFrameAddress(const QList<quint8> &target);
    QByteArray getProtocolHeader(quint16 msg);

    void fixHeaderSize(QByteArray &packet);
};

#endif // LIFXLAN_H
