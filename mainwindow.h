#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStandardItemModel>
#include <QMainWindow>
#include <QtNetwork>

#include "light.h"
#include "lifxlan.h"

#include "scanwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QMenu *fileMenu;
    QAction *scanAction;

    void setupMenuBar();

    void openScanDialog();
};
#endif // MAINWINDOW_H
