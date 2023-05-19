#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>

#include <iostream>

const qsizetype LIFX_LAN_HEADER_SIZE = 8;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QUdpSocket *udpSocket;
    Ui::MainWindow *ui;

    QByteArray getLIFXFrameHeader();
    QByteArray getLIFXFrameAddress();
    QByteArray getLIFXFrameAddress(const std::vector<quint8> &target);
    QByteArray getLIFXProtocolHeader(quint16 msg);

private slots:
    void startScan();
};
#endif // MAINWINDOW_H
