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
    [[nodiscard]] quint16 getPower() const;
    [[nodiscard]] quint16 getHue() const;
    [[nodiscard]] quint16 getSaturation() const;
    [[nodiscard]] quint16 getBrightness() const;
    [[nodiscard]] quint16 getKelvin() const;

private:

    // Basic info
    QHostAddress address;
    QList<quint8> serial;

    QString label;
    quint16 power;

    quint16 hue;

private:
    quint16 saturation;
    quint16 brightness;
    quint16 kelvin;
};

#endif // LIGHT_H
