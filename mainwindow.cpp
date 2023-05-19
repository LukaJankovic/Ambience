#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    udpSocket = new QUdpSocket(this);
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete udpSocket;
}

void MainWindow::startScan() {
    QByteArray message = getLIFXFrameHeader();
    message.append(getLIFXFrameAddress());
    message.append(getLIFXProtocolHeader(2));

    qsizetype messageSize = message.size();

    message[0] = static_cast<char>(messageSize & 0x00FF);
    message[1] = static_cast<char>(messageSize >> 8);

    udpSocket->writeDatagram(message, QHostAddress::Broadcast, 56700);
}

QByteArray MainWindow::getLIFXFrameHeader() {

    QByteArray header;

    quint16 size = 0;
    header.append(static_cast<char>(size & 0x00FF));
    header.append(static_cast<char>(size >> 8));

    quint16 protocol = 1024;

    protocol |= (1 << 12);
    protocol |= (0 << 13);
    protocol |= (0 << 14);
    protocol |= (0 << 15);

    header.append(static_cast<char>(protocol & 0x00FF));
    header.append(static_cast<char>(protocol >> 8));

    header.append((char) 3);
    header.append((char) 0);
    header.append((char) 0);
    header.append((char) 0);

    return header;
}

QByteArray MainWindow::getLIFXFrameAddress(const std::vector<quint8> &target) {

    QByteArray address;

    for (int i = 0; i < 6; i++) {
        address.append(target[i]);
    }

    for (int i = 0; i < 8; i++) address.append((char) 0);

    quint8 res_ack = 0;

    res_ack |= (1 << 1);

    address.append(res_ack);
    address.append((char) 0);

    return address;
}

QByteArray MainWindow::getLIFXFrameAddress() {
    std::vector<quint8> empty;

    for (int i = 0; i < 6; i++) empty.push_back(0);

    return getLIFXFrameAddress(empty);
}

QByteArray MainWindow::getLIFXProtocolHeader(quint16 msg) {

    QByteArray header;

    for (int i = 0; i < 8; i++) header.append((char) 0);

    header.append(static_cast<char>(msg & 0x00FF));
    header.append(static_cast<char>(msg >> 8));

    header.append((char) 0);
    header.append((char) 0);

    return header;
}
