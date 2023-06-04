#include "scanwindow.h"
#include "ui_scanwindow.h"

enum CustomRoles {
    ObjectRole = Qt::UserRole + 1
};

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
    QStandardItem *row = new QStandardItem(label);
    row->setData(QVariant::fromValue(light), ObjectRole);

    scanModel->appendRow(row);
}

void ScanWindow::accept()
{
    QModelIndex currentIndex = ui->scanListView->currentIndex();
    Light *light = currentIndex.data(ObjectRole).value<Light *>();

    QSettings settings;
    QVariantList lights = settings.value("lights").toList();
    lights.append(light->toVariantMap());
    settings.setValue("lights", lights);
}
