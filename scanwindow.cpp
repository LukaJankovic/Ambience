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

    scanModel = new LightModel(lifxLAN, this);
    ui->scanListView->setModel(scanModel);

    connect(lifxLAN,
            &LifxLAN::scannedUpdated,
            scanModel,
            &LightModel::lightListUpdated);

    connect(lifxLAN,
            &LifxLAN::lightUpdated,
            scanModel,
            &LightModel::lightUpdated);

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
 * \brief Called when user presses accept.
 *
 * Adds selected light to settings.
 */
void ScanWindow::accept()
{
    QModelIndex currentIndex = ui->scanListView->currentIndex();
    Light *light = scanModel->lightAtIndex(currentIndex);

    lifxLAN->saveScannedLight(light);
}
