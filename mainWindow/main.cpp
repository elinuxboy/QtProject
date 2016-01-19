#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
#if 1
    QApplication a(argc, argv);
    int currentExitCode = 0;

    do{
        myQtAPI::SetUTF8Code();//设置全局使用的是utf-8字符

        myQtAPI::SetStyle();//设置全局为plastique样式

        MainWindow w;

        w.showMaximized();

        currentExitCode = a.exec();

    }while( currentExitCode == EXIT_CODE_REBOOT );

    return 0;
#else
    QApplication a(argc, argv);

    myQtAPI::SetUTF8Code();//设置全局使用的是utf-8字符

    myQtAPI::SetStyle();//设置全局为plastique样式

    MainWindow w;
    w.showMaximized();

    return a.exec();
#endif
}
