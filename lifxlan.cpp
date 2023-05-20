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
    QByteArray message = LifxPacket::getFrameHeader(false);
    message.append(LifxPacket::getFrameAddress());
    message.append(LifxPacket::getProtocolHeader(2));
    LifxPacket::fixHeaderSize(message);

    scanSocket->writeDatagram(message, QHostAddress::Broadcast, 56700);

    QObject::connect(scanSocket, &QUdpSocket::readyRead, this, &LifxLAN::scanResponse);
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
void LifxLAN::scanResponse()
{
    qInfo() << "response received!\n";

    while (scanSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = scanSocket->receiveDatagram();
        QByteArray data = datagram.data();

        QHostAddress address = datagram.senderAddress();
        QList<quint8> serial = LifxPacket::getSerial(data);

        Light light(address, serial);
    }

    // TODO: Implement slot-signal
    // TODO: keep track of found devices
}
