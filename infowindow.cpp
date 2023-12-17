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
