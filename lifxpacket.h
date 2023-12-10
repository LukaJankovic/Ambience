#ifndef LIFXPACKET_H
#define LIFXPACKET_H

#include <QObject>

enum LifxMessageType
{
    // General
    MsgGetService = 2,

    // Get (send to light)
    MsgGetPower = 20,
    MsgGetLabel = 23,

    // State (response)
    MsgStateService = 3,
    MsgStatePower = 22,
    MsgStateLabel = 25
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

    static QByteArray emptyRequest(const QList<quint8> &target, unsigned messageID);

    static QByteArray getService();
    static QByteArray getLabel(const QList<quint8> &target);
    static QByteArray getPower(const QList<quint8> &target);
};

#endif // LIFXPACKET_H
