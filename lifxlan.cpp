#include "lifxlan.h"

LifxLAN::LifxLAN(QObject *parent)
    : QObject{parent}
{
    socket = new QUdpSocket(this);
    socket->bind(56700);
}

LifxLAN::~LifxLAN()
{
    delete socket;
}

/*
 * Public functions
 */

/*!
 * \brief startScan begins a scan for LIFX devices on the network.
 * \return List of found devices.
 */
void LifxLAN::startScan()
{
    qDeleteAll(scanned.begin(), scanned.end());
    scanned.clear();

    QObject::connect(socket, &QUdpSocket::readyRead, this, &LifxLAN::messageReceived);

    QByteArray message = LifxPacket::getFrameHeader(true);
    message.append(LifxPacket::getFrameAddress());
    message.append(LifxPacket::getProtocolHeader(2));
    LifxPacket::fixHeaderSize(message);

    socket->writeDatagram(message, QHostAddress::Broadcast, 56700);
}

/*
 * Private functions
 */

/*
 * Private slots
 */

/*!
 * \brief LifxLAN::scanResponse Called whenever a device responds to a scan.
 */
void LifxLAN::messageReceived()
{
    if(socket->hasPendingDatagrams())
    {
        QNetworkDatagram datagram = socket->receiveDatagram();
        QHostAddress senderAddress = datagram.senderAddress();
        QByteArray data = datagram.data();

        if (scanned.contains(senderAddress))
        {
            scanned[senderAddress]->processPacket(data);
        }

        else
        {
            if (LifxPacket::getMessageType(data) != 3) return;

            QList<quint8> serial = LifxPacket::getSerial(data);

            Light *light = new Light(senderAddress, serial, socket);
            scanned[senderAddress] = light;

            emit scanFoundLight(light);
        }
    }
}
