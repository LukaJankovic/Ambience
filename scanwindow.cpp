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

/*!
 * \brief ScanWindow::startScan calls lifxLAN to send UDP scan.
 */
void ScanWindow::startScan()
{
    scanModel->removeRows(0, scanModel->rowCount());
    lifxLAN.startScan();
}

/*
 * Private slots
 */

/*!
 * \brief ScanWindow::scanFoundLight called to attach label update to found light.
 * \param light object representing physical LIFX light.
 */
void ScanWindow::scanFoundLight(Light *light)
{
    QObject::connect(light, &Light::labelUpdated, this, &ScanWindow::scanLightLabelUpdated);
}

/*!
 * \brief ScanWindow::scanLightLabelUpdated called when label for light is received.
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
 * \brief ScanWindow::accept called when user presses accept.
 *
 * Adds selected light to settings.
 */
void ScanWindow::accept()
{
    QModelIndex currentIndex = ui->scanListView->currentIndex();
    Light *light = currentIndex.data(ObjectRole).value<Light *>();

    QSettings settings;
    QVariantList lights = settings.value("lights").toList();
    lights.append(light->toVariantMap());
    settings.setValue("lights", lights);
}
