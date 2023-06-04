#include "mainwindow.h"
#include "light.h"

#include <QApplication>
#include <QVariant>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("lukjan");
    QCoreApplication::setApplicationName("ambience");

    MainWindow w;
    w.show();
    return a.exec();
}
