#include "lifxlan.h"

LifxLAN::LifxLAN(QObject *parent)
    : QObject{parent}
{
    udpSocket = new QUdpSocket(this);
}

LifxLAN::~LifxLAN()
{
    delete udpSocket;
}

/*
 * Public functions
 */

/*!
 * \brief scan Scans LIFX devices on the network.
 * \return List of found devices.
 */
QList<Light> LifxLAN::scan()
{
    QByteArray message = getFrameHeader();
    message.append(getFrameAddress());
    message.append(getProtocolHeader(2));

    qsizetype messageSize = message.size();

    message[0] = static_cast<char>(messageSize & 0x00FF);
    message[1] = static_cast<char>(messageSize >> 8);

    udpSocket->writeDatagram(message, QHostAddress::Broadcast, 56700);
}

/*
 * Private functions
 */

/*!
 * \brief LifxLAN::getFrameHeader Generates a LIFX-compatible frame header. Size is set to 0.
 * \return byte array representing the header.
 */
QByteArray LifxLAN::getFrameHeader()
{
    QByteArray header;

    quint16 size = 0;
    header.append(static_cast<char>(size & 0x00FF));
    header.append(static_cast<char>(size >> 8));

    quint16 protocol = 1024;

    protocol |= (1 << 12); // Addressable (must be 1)
    protocol |= (0 << 13); // Tagged (determines usage of frame address target field)
    protocol |= (0 << 14); // Message origin, 2 bits (must be 0)
    protocol |= (0 << 15);

    header.append(static_cast<char>(protocol & 0x00FF));
    header.append(static_cast<char>(protocol >> 8));

    header.append((char) 3);
    header.append((char) 0);
    header.append((char) 0);
    header.append((char) 0);

    return header;
}

/*!
 * \brief LifxLAN::getFrameAddress Generates a LIFX-compatible frame address.
 * \param target 6 quint8 values representing the MAC-address of the target device.
 * \return byte array representing the frame address.
 */
QByteArray LifxLAN::getFrameAddress(const QList<quint8> &target)
{

    QByteArray address;

    for (int i = 0; i < 6; i++) {
        address.append(target[i]);
    }

    for (int i = 0; i < 8; i++) address.append((char) 0);

    quint8 res_ack = 0;

    res_ack |= (1 << 1);

    address.append(res_ack);
    address.append((char) 0);

    return address;
}

/*!
 * \brief LifxLAN::getFrameAddress Generates LIFX-compatible frame address targeting all devices.
 * \return byte array representing the frame address.
 */
QByteArray LifxLAN::getFrameAddress()
{
    QList<quint8> empty;

    for (int i = 0; i < 6; i++) empty.push_back(0);

    return getFrameAddress(empty);
}

/*!
 * \brief LifxLAN::getProtocolHeader Generates a LIFX-compatible protocol header.
 * \param msg The message type to be sent.
 * \return byte array representing the protocol header.
 */
QByteArray LifxLAN::getProtocolHeader(quint16 msg)
{
    QByteArray header;

    for (int i = 0; i < 8; i++) header.append((char) 0);

    header.append(static_cast<char>(msg & 0x00FF));
    header.append(static_cast<char>(msg >> 8));

    header.append((char) 0);
    header.append((char) 0);

    return header;
}

/*!
 * \brief LifxLAN::fixHeaderSize Updates the size field in the packet.
 * \param packet Byte array of the packet to be modified.
 */
void LifxLAN::fixHeaderSize(QByteArray &packet)
{
    qsizetype packetSize = packet.size();

    packet[0] = static_cast<char>(packetSize & 0x00FF);
    packet[1] = static_cast<char>(packetSize >> 8);
}
