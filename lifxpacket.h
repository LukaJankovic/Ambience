#ifndef LIFXPACKET_H
#define LIFXPACKET_H

#include <QObject>

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

};

#endif // LIFXPACKET_H
