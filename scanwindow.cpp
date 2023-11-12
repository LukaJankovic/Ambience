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
}

ScanWindow::ScanWindow(LifxLAN *lifxLAN, QWidget *parent) :
    lifxLAN(lifxLAN),
    QDialog(parent),
    ui(new Ui::ScanWindow)
{
    ui->setupUi(this);

    QObject::connect(lifxLAN, &LifxLAN::scanFoundLight, this, &ScanWindow::scanFoundLight);

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

/*!
 * \brief ScanWindow::startScan calls lifxLAN to send UDP scan.
 */
void ScanWindow::startScan()
{
    scanModel->removeRows(0, scanModel->rowCount());
    lifxLAN->startScan();
}

/*
 * Private slots
 */

/*!
 * \brief Called when light found to attach label update function.
 * \param light object representing physical LIFX light.
 */
void ScanWindow::scanFoundLight(Light *light)
{
    QObject::connect(light, &Light::labelUpdated, this, &ScanWindow::scanLightLabelUpdated);
}

/*!
 * \brief Called when label for light is received.
 *
 * Creates a row for the light and adds it to the list model.
 *
 * \param light object representing physical LIFX light.
 * \param label string containing received light label.
 */
void ScanWindow::scanLightLabelUpdated(Light *light, QString label)
{
    QStandardItem *row = new QStandardItem(label);
    row->setData(QVariant::fromValue(light), ObjectRole);

    scanModel->appendRow(row);
}

/*!
 * \brief Called when user presses accept.
 *
 * Adds selected light to settings.
 */
void ScanWindow::accept()
{
    QModelIndex currentIndex = ui->scanListView->currentIndex();
    Light *light = currentIndex.data(ObjectRole).value<Light *>();

    lifxLAN->saveScannedLight(light);
}
