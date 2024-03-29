#ifndef LIFXLAN_H
#define LIFXLAN_H

#include <QObject>
#include <QtNetwork/QtNetwork>
#include <QtAlgorithms>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "lifxpacket.h"
#include "light.h"

class LifxLAN : public QObject
{
Q_OBJECT

    friend class LightModel;
public:
    explicit LifxLAN(QObject *parent = nullptr);
    ~LifxLAN();

    void startScan();

    bool saveScannedLight(Light *light);
    bool removeSavedLight(int index);

    void sendPacket(Light *target, QByteArray packet);
    void sendRequest(Light *target, LifxMessageType messageID);

    void loadSettings();
    void saveSettings();

    // Saved list access functions

private:
    QUdpSocket *socket;
    QList<Light *> scanned;
    QList<Light *> saved;

    QNetworkAccessManager *manager;

    QJsonArray productList;

    void downloadProductList();

signals:
    void scannedUpdated(QList<Light *>scanned);
    void savedLightsUpdated(QList<Light *> saved);
    void lightUpdated(Light *light);

private slots:
    void messageReceived();
};

QVariantList convertToVariantList(const QList<quint8>& list);
QList<quint8> convertFromVariantList(const QVariantList& variantList);

#endif // LIFXLAN_H
