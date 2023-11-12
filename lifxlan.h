#ifndef LIFXLAN_H
#define LIFXLAN_H

#include <QObject>
#include <QtNetwork>
#include <QtAlgorithms>

#include "lifxpacket.h"
#include "light.h"

class LifxLAN : public QObject
{
    Q_OBJECT
public:
    explicit LifxLAN(QObject *parent = nullptr);
    ~LifxLAN();

    void startScan();

    bool saveScannedLight(Light *light);
    bool removeSavedLight(Light *light);

    void loadSettings();
    void saveSettings();
private:
    QUdpSocket *socket;
    QHash<QHostAddress, Light*> scanned;
    QHash<QHostAddress, Light*> saved;

signals:
    void scanFoundLight(Light *light);
    void savedLightsUpdated(QHash<QHostAddress, Light*> saved);

private slots:
    void messageReceived();
};

QVariantList convertToVariantList(const QList<quint8>& list);
QList<quint8> convertFromVariantList(const QVariantList& variantList);

#endif // LIFXLAN_H
