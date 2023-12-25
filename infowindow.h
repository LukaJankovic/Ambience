#ifndef INFOWINDOW_H
#define INFOWINDOW_H

#include <QDialog>

#include "lifxlan.h"
#include "light.h"

namespace Ui {
class InfoWindow;
}

class InfoWindow : public QDialog
{
    Q_OBJECT

public:
    explicit InfoWindow(QWidget *parent = nullptr);
    ~InfoWindow();

    void loadInfo();

    void setLight(Light *newLight);
    void setLifxLAN(LifxLAN *newLifxLAN);

public slots:
    void updateLabel(Light *light);

private:
    Ui::InfoWindow *ui;

    LifxLAN *lifxLAN;
    Light *light;
};

#endif // INFOWINDOW_H
