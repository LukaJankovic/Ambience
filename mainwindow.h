#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStandardItemModel>
#include <QMainWindow>
#include <QTreeWidget>
#include <QtNetwork/QtNetwork>

#include "light.h"
#include "lifxlan.h"
#include "lightmodel.h"

#include "scanwindow.h"
#include "infowindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void showLightContextMenu(const QPoint &pos);

public slots:
    void lightSelectionChanged();
    void lightUpdated(Light *light);

    void powerStateChanged(bool clicked);
    void colorSliderChanged(int value);

private:
    Ui::MainWindow *ui;

    LightModel *lightsModel;

    QMenu *fileMenu;
    QAction *scanAction;

    LifxLAN *lifxLAN;
    Light *currentLight;

    void setupMenuBar();

    void removeLight(QModelIndex index);
    void showLightInfo(QModelIndex index);

    void openScanDialog();
};
#endif // MAINWINDOW_H
