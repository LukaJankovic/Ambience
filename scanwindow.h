#ifndef SCANWINDOW_H
#define SCANWINDOW_H

#include <QDialog>
#include <QStandardItemModel>
#include <QPushButton>

#include "lifxlan.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ScanWindow; }
QT_END_NAMESPACE

class ScanWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ScanWindow(QWidget *parent = nullptr);
    ~ScanWindow();

private:
    Ui::ScanWindow *ui;

    QPushButton *scanButton;
    QStandardItemModel *scanModel;

    LifxLAN lifxLAN;


private slots:
    void startScan();
    void scanFoundLight(Light *light);
    void scanLightLabelUpdated(Light *light, QString label);
};

#endif // SCANWINDOW_H
