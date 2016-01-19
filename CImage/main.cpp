#include<QtGui/QtGui>
#include"CImage.h"

int main(int argc,char*argv[])
{
    QApplication app(argc,argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    CImage dlg;
    return dlg.exec();
}