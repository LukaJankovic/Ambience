#include "lifxpacket.h"

/*!
 * \brief Creates a LIFX-compatible frame header. Size is set to 0.
 * \param tagged Determines whether the target field should be used.
 * \return byte array representing the header.
 */
QByteArray LifxPacket::getFrameHeader(bool tagged)
{
    QByteArray header;

    quint16 size = 0;
    header.append(static_cast<char>(size & 0x00FF));
    header.append(static_cast<char>(size >> 8));

    quint16 protocol = 1024;

    protocol |= (1 << 12); // Addressable (must be 1)
    protocol |= (tagged << 13); // Tagged (determines usage of frame address target field)
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
 * \brief Generates a LIFX-compatible frame address.
 * \param target 6 quint8 values representing the MAC-address of the target device.
 * \return byte array representing the frame address.
 */
QByteArray LifxPacket::getFrameAddress(const QList<quint8> &target)
{
    if (target.length() != 6)
        throw std::invalid_argument("target length must be 6");

    QByteArray address;

    for (int i = 0; i < 6; i++)
    {
        address.append(target[i]);
    }

    for (int i = 0; i < 8; i++) address.append((char) 0);

    quint8 res_ack = 0;

    //res_ack |= (1 << 1);

    address.append(res_ack);
    address.append((char) 0);

    return address;
}

/*!
 * \brief Generates LIFX-compatible frame address targeting all devices.
 * \return byte array representing the frame address.
 */
QByteArray LifxPacket::getFrameAddress()
{
    QList<quint8> empty;

    for (int i = 0; i < 6; i++) empty.push_back(0);

    return getFrameAddress(empty);
}

/*!
 * \brief Generates a LIFX-compatible protocol header.
 * \param msg The message type to be sent.
 * \return byte array representing the protocol header.
 */
QByteArray LifxPacket::getProtocolHeader(quint16 msg)
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
 * \brief Updates the size field in the packet.
 * \param packet Byte array of the packet to be modified.
 */
void LifxPacket::fixHeaderSize(QByteArray &packet)
{
    qsizetype packetSize = packet.size();

    packet[0] = static_cast<char>(packetSize & 0x00FF);
    packet[1] = static_cast<char>(packetSize >> 8);
}

/*!
 * \brief Gets the serial number from a packet.
 * \param packet Packet to be decoded.
 * \return Returns the serial number in the form of a byte array.
 */
QList<quint8> LifxPacket::getSerial(const QByteArray &packet)
{
    QList<quint8> serial;

    for (int i = 8; i < 14; i++)
    {
        serial.append(packet.at(i));
    }

    return serial;
}

/*!
 * \brief Gets the message type from a packet.
 * \param packet Packet to be decoded.
 * \return Returns the message type number.
 */
quint8 LifxPacket::getMessageType(const QByteArray &packet)
{
    auto messageType = packet.at(32);
    return messageType;
}

/*!
 * \brief Gets the payload from the packet.
 * \param packet Packet to be decoded
 * \return Returns the payload in the form of a byte array.
 */
QByteArray LifxPacket::getPayload(const QByteArray &packet)
{
    QByteArray payload;

    for (int i = 36; i < packet.size(); i++)
    {
        payload.append(packet.at(i));
    }

    return payload;
}

/*!
 * \brief Removes null characters from payload.
 * \param payload Payload to be processed.
 * \return Payload excluding null characters.
 */
QByteArray LifxPacket::trimPayload(const QByteArray &payload)
{
    int nullIndex = payload.indexOf('\0');
    if (nullIndex == -1) return payload;
    return payload.left(nullIndex);
}

/*
 * Lifx LAN Packet payloads
 */

/*!
 * \brief Generates a complete getService packet to be sent.
 * \return Byte array containing full packet.
 */
QByteArray LifxPacket::getService()
{
    QByteArray message = LifxPacket::getFrameHeader(true);
    message.append(LifxPacket::getFrameAddress());
    message.append(LifxPacket::getProtocolHeader(MsgGetService));
    LifxPacket::fixHeaderSize(message);

    return message;
}

/*!
 * \brief Generates a complete getLabel packet to be sent.
 * \param Serial of the receiving light.
 * \return Byte array containing full packet.
 */
QByteArray LifxPacket::getLabel(const QList<quint8> &target)
{

    QByteArray message = LifxPacket::getFrameHeader(false);
    message.append(LifxPacket::getFrameAddress(target));
    message.append(LifxPacket::getProtocolHeader(MsgGetLabel));
    LifxPacket::fixHeaderSize(message);

    return message;
}
