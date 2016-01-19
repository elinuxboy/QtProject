#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtNetwork>
#include <QMovie>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void startRequest(QUrl url); //请求链接
    void getUrlPic();

private slots:
    void replyFinished(QNetworkReply *);//请求完成处理
    void httpReadyRead(); //接收到数据时的处理

    void on_pBtnRefresh_clicked();

    void on_pBtnGet_clicked();

    void on_pBtnPost_clicked();

    void on_pBtnClear_clicked();

private:
    Ui::Widget *ui;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QUrl url; //存储网络地址
    QPixmap *currentPicture;
    QString	currentFileName;
};

#endif // WIDGET_H
