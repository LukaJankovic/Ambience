#include "light.h"

Light::Light(QObject *parent)
    : QObject{parent} {}

Light::Light(const QHostAddress &address, const QList<quint8> &serial) : address(address),
    serial(serial)
{}

Light::~Light()
{
}

QVariantMap Light::toVariantMap() const
{
    QVariantMap map;
    map["address"] = address.toString();

    QString serialString;

    for (const auto& item : serial)
    {
        serialString.append(static_cast<char>(item));
    }

    map["serial"] = serialString;
    map["label"] = label;

    return map;
}

Light::Light(const QVariantMap &map)
{
    address = QHostAddress(map["address"].toString());
    for (const auto& ch : map["serial"].toString())
    {
        serial.append(ch.unicode());
    }

    label = map["label"].toString();
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
        label = QString(LifxPacket::trimPayload(data));
        emit labelUpdated(this, label);
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

QList<quint8> Light::getSerial() const
{
    return serial;
}

QString Light::getLabel() const
{
    return label;
}
