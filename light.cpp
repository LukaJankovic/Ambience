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
        case MsgStateHostFirmware:

            lightData.buildTimestamp = 0;
            for (int i = 7; i >= 0; i--)
            {
                lightData.buildTimestamp |= (data[i] << (i * 8));
            }

            // 8 reserved bytes

            lightData.versionMinor = (data[17] << 8) + data[16];
            lightData.versionMajor = (data[19] << 8) + data[18];

            break;
        case MsgStateWifiInfo:
            lightData.signalStrength = (data[1] << 8) + data[0];
            break;
        case MsgStateWifiFirmware:

            lightData.wifiBuildTimestamp = 0;
            for (int i = 7; i >= 0; i--)
            {
                lightData.wifiBuildTimestamp |= (data[i] << (i * 8));
            }

            // 8 reserved bytes

            lightData.wifiVersionMinor = (data[17] << 8) + data[16];
            lightData.wifiVersionMajor = (data[19] << 8) + data[18];

            break;
        case MsgStatePower:
            lightData.power = (data[1] << 8) + data[0];
            lightData.power = (data[1] << 8) + data[0];
            break;
        case MsgStateLabel:
            label = QString(LifxPacket::trimPayload(data));
            break;
        case MsgStateVersion:

            lightData.vendor = 0;
            for (int i = 3; i >= 0; i--)
            {
                lightData.vendor |= (data[i] << (i * 8));
            }

            lightData.product = 0;
            for (int i = 3; i >= 0; i--)
            {
                lightData.product |= (data[i + 8] << (i * 8));
            }

            break;

        case MsgStateInfo:

            lightData.time = 0;
            for (int i = 7; i >= 0; i--)
            {
                lightData.time |= (data[i] << (i * 8));
            }

            lightData.uptime = 0;
            for (int i = 7; i >= 0; i--)
            {
                lightData.uptime |= (data[i + 8] << (i * 8));
            }

            lightData.downtime = 0;
            for (int i = 7; i >= 0; i--)
            {
                lightData.downtime |= (data[i + 16] << (i * 8));
            }

            break;

        case MsgStateLocation:
            lightData.locationID = QString(data.mid(0, 16));
            lightData.location = QString(LifxPacket::trimPayload(data.mid(16, 32)));
            break;
        case MsgStateGroup:
            lightData.groupID = QString(data.mid(0, 16));
            lightData.group = QString(LifxPacket::trimPayload(data.mid(16, 32)));
            break;
        case MsgLightState:
            lightData.hue = (data[1] << 8) + data[0];
            lightData.saturation = (data[3] << 8) + data[2];
            lightData.brightness = (data[5] << 8) + data[4];
            lightData.kelvin = (data[7] << 8) + data[6];

            // 2 reserved bytes

            lightData.power = (data[11] << 8) + data[10];

            // TODO label?

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

LightData Light::getLightData() const
{
    return lightData;
}
