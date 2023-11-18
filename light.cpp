#include "light.h"

Light::Light(QObject *parent)
    : QObject{parent} {}

Light::~Light()
{
}

QVariantMap Light::toVariantMap() const
{
    QVariantMap map;
    // map["label"] = label;
    map["address"] = address.toString();

    QString serialString;

    for (const auto& item : serial)
    {
        serialString.append(static_cast<char>(item));
    }

    map["serial"] = serialString;
    return map;
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

Light::Light(const QVariantMap &map)
{
    address = QHostAddress(map["address"].toString());
    for (const auto& ch : map["serial"].value<QList<quint8>>())
    {
        serial.append(ch);
    }
}

/*!
 * \brief called by lifxlan whenever the corresponding physical device sent data to this object.
 * \param packet Packet from the device to be processed.
 */
void Light::processPacket(const QByteArray &packet)
{
    QByteArray data = LifxPacket::getPayload(packet);

    switch (LifxPacket::getMessageType(packet))
    {
    case MsgStateLabel:
        emit labelUpdated(this, QString(LifxPacket::trimPayload(data)));
        break;
    default:
        break;
    }
}

/*
 * Getters
 */

QHostAddress Light::getAddress() const
{
    return address;
}
