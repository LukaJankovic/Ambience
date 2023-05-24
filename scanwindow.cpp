#include "scanwindow.h"
#include "ui_scanwindow.h"

ScanWindow::ScanWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScanWindow)
{
    ui->setupUi(this);

    QObject::connect(&lifxLAN, &LifxLAN::scanFoundLight, this, &ScanWindow::scanFoundLight);

    scanModel = new QStandardItemModel(this);
    ui->scanListView->setModel(scanModel);

    scanButton = new QPushButton("Rescan", this);
    ui->buttonBox->addButton(scanButton, QDialogButtonBox::ActionRole);

    QWidget::connect(scanButton, &QPushButton::clicked, this, &ScanWindow::startScan);

    startScan();
}

ScanWindow::~ScanWindow()
{
    delete scanModel;
    delete ui;
}

/*
 * Private functions
 */

void ScanWindow::startScan()
{
    scanModel->removeRows(0, scanModel->rowCount());
    lifxLAN.startScan();
}

/*
 * Private slots
 */

void ScanWindow::scanFoundLight(Light *light)
{
    QObject::connect(light, &Light::labelUpdated, this, &ScanWindow::scanLightLabelUpdated);
}

void ScanWindow::scanLightLabelUpdated(Light *light, QString label)
{
    scanModel->appendRow(new QStandardItem(label));
}
