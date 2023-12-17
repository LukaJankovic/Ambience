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

    QObject::connect(socket, &QUdpSocket::readyRead, this, &LifxLAN::messageReceived);

    manager = new QNetworkAccessManager(this);
    downloadProductList();
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

    socket->writeDatagram(LifxPacket::getService(), QHostAddress::Broadcast, 56700);
}

/*!
 * \brief Saves a scanned light to saved lights
 * \param light object to be saved. Must be scanned
 * \return true if successful, false otherwise
 */
bool LifxLAN::saveScannedLight(Light *light)
{
    if (!scanned.contains(light))
    {
        qWarning() << "Attempt to add unscanned light";
        return false;
    }

    if (saved.contains(light))
    {
        qWarning() << "Ignoring attempt to re-add saved light";
        return false;
    }

    scanned.removeAll(light);
    saved.append(light);

    saveSettings();
    emit savedLightsUpdated(saved);
    emit scannedUpdated(scanned);

    return true;
}

/*!
 * \brief Removes a light from saved.
 * \param index Light To be removed. Must be in saved hash.
 * \return true if successful, false otherwise
 */
bool LifxLAN::removeSavedLight(int index)
{
    if (index < 0 || index > saved.size())
    {
        qWarning() << "Attempt to remove unsaved light";
        return false;
    }

    saved.removeAt(index);
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
        sendPacket(l, LifxPacket::sendRequest(l->getSerial(), MsgGetLabel));
        saved.append(l);
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

/*!
 * \brief Sends a packet to a light
 * \param target light object to receive the message
 * \param packet Packet to be sent
 */
void LifxLAN::sendPacket(Light *target, QByteArray packet)
{
    socket->writeDatagram(packet, target->getAddress(), 56700);
}

void LifxLAN::sendRequest(Light *target, LifxMessageType messageID)
{
    sendPacket(target, LifxPacket::sendRequest(target->getSerial(), messageID));
}

/*
 * Private functions
 */

/*!
 * \brief Downloads product list from GitHub.
 */
void LifxLAN::downloadProductList()
{
    const QString URL = "https://raw.githubusercontent.com/LIFX/products/master/products.json";

    QNetworkRequest request{QUrl(URL)};
    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            reply->deleteLater();

            QJsonDocument document = QJsonDocument::fromJson(data);
            productList = document.array()[0].toObject()["products"].toArray();

            /*
            for (const auto &it : productList) {
                qInfo() << it.toObject()["name"].toString();
            }
            */
        } else {
            qInfo() << "Error when downloading product list:" << reply->errorString();
            reply->deleteLater();
        }
    });
}

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
        for (const auto& light : scanned)
        {
            if (light->getAddress() == senderAddress)
            {
                light->processPacket(data);
                emit lightUpdated(light);
                return;
            }
        }

        // Saved light responded
        for (const auto& light : saved)
        {
            if (light->getAddress() == senderAddress)
            {
                light->processPacket(data);
                emit lightUpdated(light);
                return;
            }
        }

        // Light responded to me but is neither scanned nor saved
        if (LifxPacket::getMessageType(data) != MsgStateService)
            return;

        Light *light = new Light(senderAddress,
                                 LifxPacket::getSerial(data));

        scanned.append(light);

        emit scannedUpdated(scanned);

        socket->writeDatagram(LifxPacket::sendRequest(light->getSerial(), MsgGetLabel),
                              light->getAddress(),
                              56700);
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
