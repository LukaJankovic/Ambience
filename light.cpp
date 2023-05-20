#include "light.h"

Light::Light(QObject *parent)
    : QObject{parent}
{

}

Light::Light(const QHostAddress &address, const QList<quint8> &serial) : address(address),
    serial(serial)
{
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(56700);

    QByteArray message = LifxPacket::getFrameHeader(true);
    message.append(LifxPacket::getFrameAddress(serial));
    message.append(LifxPacket::getProtocolHeader(23));
    LifxPacket::fixHeaderSize(message);

    // TODO: Move socket to "static" network class...
    //udpSocket->writeDatagram(message, address, 56700);
}
