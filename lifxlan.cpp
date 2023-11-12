#include "lifxlan.h"

LifxLAN::LifxLAN(QObject *parent)
    : QObject{parent}
{
    socket = new QUdpSocket(this);
    socket->bind(56700);

    qRegisterMetaType<QList<quint8>>("QList<quint8>");
    QMetaType::registerConverter<QList<quint8>, QVariantList>(&convertToVariantList);
    QMetaType::registerConverter<QVariantList, QList<quint8>>(&convertFromVariantList);

    QSettings settings;
    qDebug() << "Settings path: "<< settings.fileName();
}

LifxLAN::~LifxLAN()
{
    delete socket;
}

/*
 * Public functions
 */

/*!
 * \brief Begins a scan for LIFX devices on the network.
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

/*!
 * \brief Saves a scanned light to saved lights
 * \param light object to be saved. Must be scanned
 * \return true if successful, false otherwise
 */
bool LifxLAN::saveScannedLight(Light *light)
{
    if (!scanned.contains(light->getAddress()))
    {
        qWarning() << "Attempt to add unscanned light";
        return false;
    }

    if (saved.contains(light->getAddress()))
    {
        qWarning() << "Ignoring attempt to re-add saved light";
        return false;
    }

    saved[light->getAddress()] = light;

    saveSettings();
    emit savedLightsUpdated(saved);

    return true;
}

/*!
 * \brief Removes a light from saved.
 * \param light To be removed. Must be in saved hash.
 * \return true if successful, false otherwise
 */
bool LifxLAN::removeSavedLight(Light *light)
{
    if (!saved.contains(light->getAddress()))
    {
        qWarning() << "Attempt to remove unsaved light";
        return false;
    }

    saved.remove(light->getAddress());
    saveSettings();
    emit savedLightsUpdated(saved);

    return true;
}

/*!
 * \brief Load saved lights from QSettings.
 */
void LifxLAN::loadSettings()
{
    QSettings settings;
    QVariantList lights = settings.value("lights").toList();

    // Rebuild hash of IP->Lights. Maybe find a better way...
    for (const auto& light : lights)
    {
        Light *l = new Light(light.toMap());
        saved[l->getAddress()] = l;
    }

    emit savedLightsUpdated(saved);
}

/*!
 * \brief Store updated saved lights to QSettings.
 */
void LifxLAN::saveSettings()
{
    QSettings settings;
    QVariantList lights;
    for (const auto& light : saved)
    {
        lights.append(light->toVariantMap());
    }
    settings.setValue("lights", lights);
}


/*
 * Private functions
 */

/*
 * Private slots
 */

/*!
 * \brief Called whenever a device responds to a scan.
 */
void LifxLAN::messageReceived()
{
    if(socket->hasPendingDatagrams())
    {
        QNetworkDatagram datagram = socket->receiveDatagram();
        QHostAddress senderAddress = datagram.senderAddress();
        QByteArray data = datagram.data();

        // Scanned light responded (usually with label)
        if (scanned.contains(senderAddress))
        {
            scanned[senderAddress]->processPacket(data);
            return;
        }

        // Saved light responded
        if (saved.contains(senderAddress))
        {
            saved[senderAddress]->processPacket(data);
            return;
        }

        // Light responded to me but is neither scanned nor saved
        if (LifxPacket::getMessageType(data) != 3) return;

        QList<quint8> serial = LifxPacket::getSerial(data);

        Light *light = new Light(senderAddress, serial, socket);
        scanned[senderAddress] = light;

        emit scanFoundLight(light);
    }
}

/*
 * Helper functions.
 */

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
