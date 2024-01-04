#ifndef LIGHT_H
#define LIGHT_H

#include <QObject>
#include <QtNetwork/QtNetwork>

#include "lifxpacket.h"

struct LightData
{
    QString group;
    QString groupID;

    QString location;
    QString locationID;

    quint16 power;

    quint16 hue;
    quint16 saturation;
    quint16 brightness;
    quint16 kelvin;

    // Host Firmware
    quint64 buildTimestamp;
    quint16 versionMajor;
    quint16 versionMinor;

    // WiFi Info
    qfloat16 signalStrength;
    quint64 wifiBuildTimestamp;
    quint16 wifiVersionMajor;
    quint16 wifiVersionMinor;

    // Version
    quint32 vendor;
    quint32 product;

    // Info
    quint64 time;
    quint64 uptime;
    quint64 downtime;
};

class Light : public QObject
{
Q_OBJECT

public:
    explicit Light(QObject *parent = nullptr);
    Light(const QHostAddress &address, const QList<quint8> &serial);
    explicit Light(const QVariantMap &map);
    ~Light() override;

    [[nodiscard]] QVariantMap toVariantMap() const;

    void processPacket(const QByteArray &packet);

    [[nodiscard]] QHostAddress getAddress() const;
    [[nodiscard]] QList<quint8> getSerial() const;
    [[nodiscard]] QString getLabel() const;
    [[nodiscard]] LightData getLightData() const;

private:

    // Basic info
    QHostAddress address;
    QList<quint8> serial;
    QString label;

    LightData lightData;


};

#endif // LIGHT_H
