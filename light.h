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
    Light(const QHostAddress &address, const QList<quint8> &serial);

private:
    QUdpSocket *udpSocket;

    QString label;
    QHostAddress address;
    QList<quint8> serial;

signals:

};

#endif // LIGHT_H
