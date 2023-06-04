#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    lightsModel = new QStandardItemModel(this);
    ui->lightsList->setModel(lightsModel);

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

/*
 * Private functions
 */

/*!
 * \brief setupMenuBar Setus up menu bar options.
 */
void MainWindow::setupMenuBar()
{
    fileMenu = new QMenu("File", this);

    scanAction = new QAction("Add lights", this);

    connect(scanAction, &QAction::triggered, this, &MainWindow::openScanDialog);

    fileMenu->addAction(scanAction);
    menuBar()->addMenu(fileMenu);
}

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
 * \brief openScanDialog Opens dialog to add new lights.
 */
void MainWindow::openScanDialog()
{
    ScanWindow *scanWindow = new ScanWindow();
    scanWindow->show();
}
