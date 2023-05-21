#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(&lifxLAN, &LifxLAN::scanFoundLight, this, &MainWindow::scanFoundLight);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startScan()
{
    lifxLAN.startScan();
}

void MainWindow::scanFoundLight(Light *light)
{
    QObject::connect(light, &Light::labelUpdated, this, &MainWindow::scanLightLabelUpdated);
}

void MainWindow::scanLightLabelUpdated(Light *light, QString label)
{
    qInfo() << label;
}
