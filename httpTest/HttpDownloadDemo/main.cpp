#include "widget.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    //设置全局使用的是utf-8字符
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
    QFont font;
    //font.setFamily(("WenQuanYi Zen Hei"));
    //font.setPointSize(10);
    a.setFont(font);
    a.setFont(font);

    w.show();

    return a.exec();
}
