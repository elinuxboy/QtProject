#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QMovie>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void startRequest(QUrl url); //请求链接
    void getUrlPic();//获取网页图片

private slots:
    void on_pBtnRefresh_clicked();

    void on_pBtnGet_clicked();

    void on_pBtnPost_clicked();

    void on_pBtnClear_clicked();

    void replyFinished(QNetworkReply *);//请求完成处理
    void httpReadyRead(); //接收到数据时的处理

private:
    Ui::MainWindow *ui;

    QNetworkAccessManager *manager;//QNetworkAccessManager 类用于发送网络请求和接受回复
    QNetworkReply *reply;//QNetworkReply 类进行 接收回复，并对数据进行处理。
    QUrl url; //存储网络地址
    QPixmap *currentPicture;
    QString	currentFileName;
};

#endif // MAINWINDOW_H
