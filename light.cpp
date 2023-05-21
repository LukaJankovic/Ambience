#include "light.h"

Light::Light(QObject *parent)
    : QObject{parent} {}

Light::~Light()
{
}

Light::Light(const QHostAddress &address, const QList<quint8> &serial, QUdpSocket *socket) : address(address),
    serial(serial) , socket(socket)
{
    QByteArray message = LifxPacket::getFrameHeader(false);
    message.append(LifxPacket::getFrameAddress(serial));
    message.append(LifxPacket::getProtocolHeader(23));
    LifxPacket::fixHeaderSize(message);

    socket->writeDatagram(message, address, 56700);
}

/*!
 * \brief Light::processPacket Called by lifxlan whenever the corresponding physical device sent data to this object.
 * \param packet Packet from the device to be processed.
 */
void Light::processPacket(const QByteArray &packet)
{
    QByteArray data = LifxPacket::getPayload(packet);

    switch (LifxPacket::getMessageType(packet))
    {
    case 25:
        label = QString(LifxPacket::trimPayload(data));
        emit labelUpdated(this, label);
        break;
    default:
        break;
    }
}

QString Light::getLabel() const
{
    return label;
}
