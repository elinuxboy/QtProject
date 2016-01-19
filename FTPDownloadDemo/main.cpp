#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    myQtAPI::SetUTF8Code();

    MainWindow w;
    w.show();

    return a.exec();
}
