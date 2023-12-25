#include "infowindow.h"
#include "ui_infowindow.h"

InfoWindow::InfoWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::InfoWindow)
{
    ui->setupUi(this);
}

InfoWindow::~InfoWindow()
{
    delete ui;
}

/*
 * Public functions
 */

/*!
 * \brief Queries the light for info.
 */
void InfoWindow::loadInfo()
{
    QObject::connect(lifxLAN, &LifxLAN::lightUpdated, this, &InfoWindow::updateLabel);

    lifxLAN->sendRequest(light, MsgGetLocation);
    lifxLAN->sendRequest(light, MsgGetGroup);
    lifxLAN->sendRequest(light, MsgGetHostFirmware);
    lifxLAN->sendRequest(light, MsgGetWifiInfo);
    lifxLAN->sendRequest(light, MsgGetWifiFirmware);
    lifxLAN->sendRequest(light, MsgGetVersion);
    lifxLAN->sendRequest(light, MsgGetInfo);
}

void InfoWindow::setLight(Light *newLight)
{
    light = newLight;
}

void InfoWindow::setLifxLAN(LifxLAN *newLifxLAN)
{
    lifxLAN = newLifxLAN;
}

void InfoWindow::updateLabel(Light *light)
{
    QList<QPair<QString, QString>> infoMap;
    QString infoText;

    infoMap.append(qMakePair("Label", light->getLabel()));
    infoMap.append(qMakePair("Group", light->getGroup()));
    infoMap.append(qMakePair("Location", light->getLocation()));
    infoMap.append(qMakePair("Build Timestamp", QString::number(light->getBuildTimestamp())));
    infoMap.append(qMakePair("Version", QString("%1.%2").arg(
        QString::number(light->getVersionMajor()),
        QString::number(light->getVersionMinor()))));
    infoMap.append(qMakePair("Signal Strength", QString::number(light->getSignalStrength())));
    infoMap.append(qMakePair("Wifi Build Timestamp", QString::number(light->getWifiBuildTimestamp())));
    infoMap.append(qMakePair("Wifi Version", QString("%1.%2").arg(
        QString::number(light->getWifiVersionMajor()),
        QString::number(light->getWifiVersionMinor()))));
    infoMap.append(qMakePair("Vendor:Product", QString("%1.%2").arg(
                                                   light->getVendor(), light->getProduct())));
    infoMap.append(qMakePair("Time", QString::number(light->getTime())));
    infoMap.append(qMakePair("Uptime", QString::number(light->getUptime())));
    infoMap.append(qMakePair("Downtime", QString::number(light->getDowntime())));

    for (const auto &it : infoMap)
    {
        infoText.append(it.first + ": " + it.second + "\n");
    }

    ui->label->setText(infoText);
}
