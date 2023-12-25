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

            buildTimestamp = 0;
            for (int i = 7; i >= 0; i--)
            {
                buildTimestamp |= (data[i] << (i * 8));
            }

            // 8 reserved bytes

            versionMinor = (data[17] << 8) + data[16];
            versionMajor = (data[19] << 8) + data[18];

            break;
        case MsgStateWifiInfo:
            signalStrength = (data[1] << 8) + data[0];
            break;
        case MsgStateWifiFirmware:

            wifiBuildTimestamp = 0;
            for (int i = 7; i >= 0; i--)
            {
                wifiBuildTimestamp |= (data[i] << (i * 8));
            }

            // 8 reserved bytes

            wifiVersionMinor = (data[17] << 8) + data[16];
            wifiVersionMajor = (data[19] << 8) + data[18];

            break;
        case MsgStatePower:
            power = (data[1] << 8) + data[0];
            break;
        case MsgStateLabel:
            label = QString(LifxPacket::trimPayload(data));
            break;
        case MsgStateVersion:

            vendor = 0;
            for (int i = 3; i >= 0; i--)
            {
                vendor |= (data[i] << (i * 8));
            }

            product = 0;
            for (int i = 3; i >= 0; i--)
            {
                product |= (data[i + 8] << (i * 8));
            }

            break;

        case MsgStateInfo:

            time = 0;
            for (int i = 7; i >= 0; i--)
            {
                time |= (data[i] << (i * 8));
            }

            uptime = 0;
            for (int i = 7; i >= 0; i--)
            {
                uptime |= (data[i + 8] << (i * 8));
            }

            downtime = 0;
            for (int i = 7; i >= 0; i--)
            {
                downtime |= (data[i + 16] << (i * 8));
            }

            break;

        case MsgStateLocation:
            locationID = QString(data.mid(0, 16));
            location = QString(LifxPacket::trimPayload(data.mid(16, 32)));
            break;
        case MsgStateGroup:
            groupID = QString(data.mid(0, 16));
            group = QString(LifxPacket::trimPayload(data.mid(16, 32)));
            break;
        case MsgLightState:
            hue = (data[1] << 8) + data[0];
            saturation = (data[3] << 8) + data[2];
            brightness = (data[5] << 8) + data[4];
            kelvin = (data[7] << 8) + data[6];

            // 2 reserved bytes

            power = (data[11] << 8) + data[10];

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

quint16 Light::getPower() const
{
    return power;
}

QString Light::getLabel() const
{
    return label;
}

quint16 Light::getHue() const
{
    return hue;
}

quint16 Light::getSaturation() const
{
    return saturation;
}

quint16 Light::getBrightness() const
{
    return brightness;
}

quint16 Light::getKelvin() const
{
    return kelvin;
}

quint64 Light::getBuildTimestamp() const
{
    return buildTimestamp;
}

quint16 Light::getVersionMajor() const
{
    return versionMajor;
}

quint16 Light::getVersionMinor() const
{
    return versionMinor;
}

qfloat16 Light::getSignalStrength() const
{
    return signalStrength;
}

quint64 Light::getWifiBuildTimestamp() const
{
    return wifiBuildTimestamp;
}

quint16 Light::getWifiVersionMajor() const
{
    return wifiVersionMajor;
}

quint16 Light::getWifiVersionMinor() const
{
    return wifiVersionMinor;
}

quint32 Light::getVendor() const
{
    return vendor;
}

quint32 Light::getProduct() const
{
    return product;
}

quint64 Light::getTime() const
{
    return time;
}

quint64 Light::getUptime() const
{
    return uptime;
}

quint64 Light::getDowntime() const
{
    return downtime;
}

QString Light::getGroupID() const
{
    return groupID;
}

QString Light::getLocation() const
{
    return location;
}

QString Light::getLocationID() const
{
    return locationID;
}

QString Light::getGroup() const
{
    return group;
}
