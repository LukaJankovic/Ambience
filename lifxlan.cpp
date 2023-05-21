#include "lifxlan.h"

LifxLAN::LifxLAN(QObject *parent)
    : QObject{parent}
{
    scanSocket = new QUdpSocket(this);
    scanSocket->bind(56700);
}

LifxLAN::~LifxLAN()
{
    delete scanSocket;
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
    devices.clear();

    QObject::connect(scanSocket, &QUdpSocket::readyRead, this, &LifxLAN::messageReceived);

    QByteArray message = LifxPacket::getFrameHeader(true);
    message.append(LifxPacket::getFrameAddress());
    message.append(LifxPacket::getProtocolHeader(2));
    LifxPacket::fixHeaderSize(message);

    scanSocket->writeDatagram(message, QHostAddress::Broadcast, 56700);
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
    if(scanSocket->hasPendingDatagrams())
    {
        QNetworkDatagram datagram = scanSocket->receiveDatagram();

        QByteArray data = datagram.data();

        if (LifxPacket::getMessageType(data) != 3) return;

        QHostAddress address = datagram.senderAddress();
        QList<quint8> serial = LifxPacket::getSerial(data);

        Light light(address, serial);

        //devices.append(datagram.senderAddress());
    }

    // TODO: Implement slot-signal
    // TODO: keep track of found devices
}
