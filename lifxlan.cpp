#include "lifxlan.h"

LifxLAN::LifxLAN(QObject *parent)
    : QObject{parent}
{
    socket = new QUdpSocket(this);
    socket->bind(56700);

    qRegisterMetaType<QList<quint8>>("QList<quint8>");
    QMetaType::registerConverter<QList<quint8>, QVariantList>(&convertToVariantList);
    QMetaType::registerConverter<QVariantList, QList<quint8>>(&convertFromVariantList);
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

QVariantList convertToVariantList(const QList<quint8>& list)
{
    QVariantList variantList;
    for (const quint8 item : list) {
        variantList.append(QVariant(item));
    }
    return variantList;
}

QList<quint8> convertFromVariantList(const QVariantList& variantList)
{
    QList<quint8> list;
    for (const QVariant& variant : variantList) {
        list.append(variant.toUInt());
    }
    return list;
}
