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

    lightsModel = new LightModel(lifxLAN, this);
    ui->lightsList->setModel(lightsModel);
    ui->lightsList->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(lifxLAN,
            &LifxLAN::savedLightsUpdated,
            lightsModel,
            &LightModel::lightListUpdated);

    connect(lifxLAN,
            &LifxLAN::lightUpdated,
            lightsModel,
            &LightModel::lightUpdated);

    connect(lifxLAN,
            &LifxLAN::lightUpdated,
            this,
            &MainWindow::lightUpdated);

    connect(ui->lightsList,
            &QListView::customContextMenuRequested,
            this,
            &MainWindow::showLightContextMenu);

    connect(ui->lightsList->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &MainWindow::lightSelectionChanged);

    connect(ui->power,
            &QCheckBox::clicked,
            this,
            &MainWindow::powerStateChanged);

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

/*!
 * \brief Called when the selection in the lights list changes.
 *        Loads light info and triggers ui updates.
 */
void MainWindow::lightSelectionChanged()
{
    QItemSelection selection = ui->lightsList->selectionModel()->selection();
    if (selection.indexes().empty())
        return;

    QModelIndex index = selection.indexes()[0];
    currentLight = index.data(Qt::UserRole).value<Light *>();

    lifxLAN->sendPacket(currentLight, LifxPacket::getPower(currentLight->getSerial()));
}

/*!
 * \brief Called when a light sends update. If it's the currently selected light,
 *        Updates the appropriate UI.
 */
void MainWindow::lightUpdated(Light *light)
{
    if (light != currentLight)
        return;

    ui->power->setChecked(light->getPower() != 0);
}

/*!
 * \brief Called when the power switch was toggled.
 *        Sends to current light to update power.
 * \param State of the power switch.
 */
void MainWindow::powerStateChanged(bool clicked)
{
    if (!currentLight)
        return;

    lifxLAN->sendPacket(currentLight, LifxPacket::setPower(currentLight->getSerial(), clicked * 32768));
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
    scanWindow->show();
}
