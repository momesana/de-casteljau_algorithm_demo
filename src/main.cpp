#include "mainwindow.h"
#include <QApplication>

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Salem Software Solutions");
    QCoreApplication::setOrganizationDomain("");
    QCoreApplication::setApplicationName("De Casteljau Demo");

    MainWindow w;
    w.show();

    return a.exec();
}
