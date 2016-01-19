#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "mydlg.h" //包含头文件

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
    myDlg my2; //对 my2 进行定义
};

#endif // WIDGET_H
