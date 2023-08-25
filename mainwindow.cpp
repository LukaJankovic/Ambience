#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    lightsModel = new QStandardItemModel(this);
    ui->lightsList->setModel(lightsModel);
    ui->lightsList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->lightsList,
            &QTreeWidget::customContextMenuRequested,
            this,
            &MainWindow::showLightContextMenu);

    setupMenuBar();

    QSettings settings;
    qInfo() << "Settings path: "<< settings.fileName();

    showLightsList();
}

MainWindow::~MainWindow()
{
    delete scanAction;
    delete fileMenu;
    delete ui;
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
    lightsModel->clear();
    showLightsList();
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
 * \brief Shows lights from user settings.
 */
void MainWindow::showLightsList()
{
    QSettings settings;
    QVariantList lights = settings.value("lights").toList();
    for (const auto& light : lights)
    {
        Light *l = new Light(light.toMap());

        QStandardItem *row = new QStandardItem(l->getLabel());
        row->setData(QVariant::fromValue(l));

        lightsModel->appendRow(row);
    }
}

/*!
 * \brief Removes light from treeView and from user settings.
 * \param index Index of light to be removed.
 */
void MainWindow::removeLight(QModelIndex index)
{
    lightsModel->removeRow(index.row());
    QSettings settings;
    QVariantList lights = settings.value("lights").toList();
    lights.removeAt(index.row());
    settings.setValue("lights", lights);
}

/*!
 * \brief Opens dialog to add new lights.
 */
void MainWindow::openScanDialog()
{
    ScanWindow *scanWindow = new ScanWindow();

    connect(scanWindow, &QDialog::finished, this, &MainWindow::scanDone);

    scanWindow->show();
}
