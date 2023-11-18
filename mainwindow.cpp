#include "mainwindow.h"
#include "ui_mainwindow.h"

static QStringList headerLabels = {"Name",
                                   "Status",
                                   "Brightness"};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    lifxLAN = new LifxLAN();

    ui->setupUi(this);

    connect(lifxLAN,
            &LifxLAN::savedLightsUpdated,
            this,
            &MainWindow::updateLightsList);

    lightsModel = new QStandardItemModel(this);
    ui->lightsList->setModel(lightsModel);
    ui->lightsList->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->lightsList,
            &QTreeWidget::customContextMenuRequested,
            this,
            &MainWindow::showLightContextMenu);

    lightsModel->setColumnCount(headerLabels.count());
    lightsModel->setHorizontalHeaderLabels(headerLabels);

    setupMenuBar();
    lifxLAN->loadSettings();
}

MainWindow::~MainWindow()
{
    delete scanAction;
    delete fileMenu;
    delete ui;

    delete lifxLAN;
}

/*!
 * \brief Sets up and shows context menu for lights.
 * \param point Where to show the context menu.
 */
void MainWindow::showLightContextMenu(const QPoint &point)
{
    QModelIndex index = ui->lightsList->indexAt(point);
    if (index.isValid())
    {
        QMenu *lightMenu = new QMenu();

        QAction *removeLightAction = new QAction("Remove light", this);

        lightMenu->addAction(removeLightAction);

        QPoint globalPos = ui->lightsList->viewport()->mapToGlobal(point);
        QAction *selectedAction = lightMenu->exec(globalPos);

        if (selectedAction == removeLightAction) {
            removeLight(index);
        }
    }
}

/*
 * Public slots
 */

void MainWindow::scanDone(int r)
{
}

/*
 * Private functions
 */

/*!
 * \brief Setus up menu bar options.
 */
void MainWindow::setupMenuBar()
{
    fileMenu = new QMenu("File", this);

    scanAction = new QAction("Add lights", this);

    connect(scanAction, &QAction::triggered, this, &MainWindow::openScanDialog);

    fileMenu->addAction(scanAction);
    menuBar()->addMenu(fileMenu);
}

/*!
 * \brief Shows lights from LfixLAN saved.
 */
void MainWindow::updateLightsList(QList<Light *> lights)
{
    lightsModel->clear();
    for (const auto& light : lights)
    {
        QStandardItem *label = new QStandardItem(light->getLabel());

        QStandardItem *brightness = new QStandardItem(QString::number(light->getBrightness()));

        QStandardItem *power = new QStandardItem(QString::number(light->getPower()));

        lightsModel->appendRow({label, power, brightness});
    }
}

/*!
 * \brief Removes light from treeView and from user settings.
 * \param index Index of light to be removed.
 */
void MainWindow::removeLight(QModelIndex index)
{
    lifxLAN->removeSavedLight(index.row());
}

/*!
 * \brief Opens dialog to add new lights.
 */
void MainWindow::openScanDialog()
{
    ScanWindow *scanWindow = new ScanWindow(lifxLAN);

    connect(scanWindow, &QDialog::finished, this, &MainWindow::scanDone);

    scanWindow->show();
}
