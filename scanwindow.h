#ifndef SCANWINDOW_H
#define SCANWINDOW_H

#include <QDialog>
#include <QStandardItemModel>
#include <QPushButton>

#include "lifxlan.h"
#include "lightmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ScanWindow; }
QT_END_NAMESPACE

class ScanWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ScanWindow(QWidget *parent = nullptr);
    ScanWindow(LifxLAN *lifxLAN, QWidget *parent = nullptr);
    ~ScanWindow();

private:
    Ui::ScanWindow *ui;

    QPushButton *scanButton;
    LightModel *scanModel;

    LifxLAN *lifxLAN;

private slots:
    void startScan();
    void accept();
};

#endif // SCANWINDOW_H
