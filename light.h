#ifndef LIGHT_H
#define LIGHT_H

#include <QObject>
#include <QtNetwork>

#include "lifxpacket.h"

class Light : public QObject
{
    Q_OBJECT

public:
    explicit Light(QObject *parent = nullptr);
    Light(const QHostAddress &address, const QList<quint8> &serial, QUdpSocket *socket);
    Light(const QVariantMap &map);
    ~Light();

    QVariantMap toVariantMap() const;

    void processPacket(const QByteArray &packet);

    QHostAddress getAddress() const;

private:

    // Basic info
    // QString label;
    QHostAddress address;
    QList<quint8> serial;

    QUdpSocket *socket;

signals:
    void labelUpdated(Light *light, QString label);
};

#endif // LIGHT_H
