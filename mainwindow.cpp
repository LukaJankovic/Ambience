#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupMenuBar();
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

    scanAction = new QAction("Scan lights", this);

    connect(scanAction, &QAction::triggered, this, &MainWindow::openScanDialog);

    fileMenu->addAction(scanAction);
    menuBar()->addMenu(fileMenu);
}

/*!
 * \brief openScanDialog Opens dialog to add new lights.
 */
void MainWindow::openScanDialog()
{
    ScanWindow *scanWindow = new ScanWindow();
    scanWindow->show();
}
