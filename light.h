#ifndef LIGHT_H
#define LIGHT_H

#include <QObject>
#include <QtNetwork/QtNetwork>

#include "lifxpacket.h"

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
    [[nodiscard]] QString getGroup() const;
    [[nodiscard]] QString getGroupID() const;
    [[nodiscard]] QString getLocation() const;
    [[nodiscard]] QString getLocationID() const;
    [[nodiscard]] quint16 getPower() const;
    [[nodiscard]] quint16 getHue() const;
    [[nodiscard]] quint16 getSaturation() const;
    [[nodiscard]] quint16 getBrightness() const;
    [[nodiscard]] quint16 getKelvin() const;
    [[nodiscard]] quint64 getBuildTimestamp() const;
    [[nodiscard]] quint16 getVersionMajor() const;
    [[nodiscard]] quint16 getVersionMinor() const;
    [[nodiscard]] qfloat16 getSignalStrength() const;
    [[nodiscard]] quint64 getWifiBuildTimestamp() const;
    [[nodiscard]] quint16 getWifiVersionMajor() const;
    [[nodiscard]] quint16 getWifiVersionMinor() const;
    [[nodiscard]] quint32 getVendor() const;
    [[nodiscard]] quint32 getProduct() const;
    [[nodiscard]] quint64 getTime() const;
    [[nodiscard]] quint64 getUptime() const;
    [[nodiscard]] quint64 getDowntime() const;

private:

    // Basic info
    QHostAddress address;
    QList<quint8> serial;

    QString label;

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

#endif // LIGHT_H
