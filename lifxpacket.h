#ifndef LIFXPACKET_H
#define LIFXPACKET_H

#include <QObject>
#include <QIODevice>

enum LifxMessageType
{
    // General
    MsgGetService = 2,

    // Get (send to light)
    MsgGetHostFirmware = 14,
    MsgGetWifiInfo = 16,
    MsgGetWifiFirmware = 18,
    MsgGetPower = 20,
    MsgGetLabel = 23,
    MsgGetVersion = 32,
    MsgGetInfo = 34,
    MsgGetLocation = 48,
    MsgGetGroup = 51,
    MsgEchoRequest = 58,

    MsgGetColor = 101,
    MsgGetLightPower = 116,
    MsgGetInfrared = 120,
    MsgGetHevCycle = 142,

    // Set (update light)
    MsgSetPower = 21,

    MsgSetColor = 102,

    // State (response)
    MsgStateService = 3,

    MsgStateHostFirmware = 15,
    MsgStateWifiInfo = 17,
    MsgStateWifiFirmware = 19,

    MsgStatePower = 22,
    MsgStateLabel = 25,
    MsgStateVersion = 33,
    MsgStateInfo = 35,
    MsgStateLocation = 50,
    MsgStateGroup = 53,

    MsgLightState = 107,
};

class LifxPacket : public QObject
{
Q_OBJECT
public:
    static QByteArray getFrameHeader(bool tagged);
    static QByteArray getFrameAddress();
    static QByteArray getFrameAddress(const QList<quint8> &target);
    static QByteArray getProtocolHeader(quint16 msg);

    static void fixHeaderSize(QByteArray &packet);

    static QList<quint8> getSerial(const QByteArray &packet);
    static quint8 getMessageType(const QByteArray &packet);
    static QByteArray getPayload(const QByteArray &packet);

    static QByteArray trimPayload(const QByteArray &payload);

    static QByteArray sendRequest(const QList<quint8> &target, unsigned messageID);

    static QByteArray getService();

    static QByteArray setPower(const QList<quint8> &target, quint16 level);
    static QByteArray setColor(const QList<quint8> &target,
                               quint16 hue,
                               quint16 saturation,
                               quint16 brightness,
                               quint16 kelvin);
};

#endif // LIFXPACKET_H
