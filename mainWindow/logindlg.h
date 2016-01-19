#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QDialog>
#include <QtNetwork>
#include <QMovie>
#include <QMouseEvent>
#include <QVariant>

#include "lib/myqtapi.h"//自定义API头文件
#include "optiondlg.h"

namespace Ui {
class loginDlg;
}

#pragma pack(push,1)//原来对齐方式设置压栈，并设新的对齐方式设置为一个字节对齐
struct UserInfoStruct
{
    QString curUserName;//当前登陆的用户名
};
#pragma pack(pop)//恢复对齐状态
Q_DECLARE_METATYPE(UserInfoStruct)

class loginDlg : public QDialog
{
    Q_OBJECT

public:
    explicit loginDlg(QWidget *parent = 0);
    ~loginDlg();

    void startPicRequest(QUrl url); //图片验证码请求链接
    void startHttpRequest(QUrl url); //http请求链接

    void getUrlPic();//获取图片

signals:
    void signalVerifiCodeClicked();//验证码图片单击信号

    void sendDataToMainWindow(QVariant dataVar);//发送数据到主窗口的信号

protected:
    void mousePressEvent(QMouseEvent *e);//验证码图片显示区鼠标处理事件

public slots:
    void recvDataFrMainWin();//接收来自主窗口数据的槽函数

private slots:
    void replyFinished(QNetworkReply *);//请求完成处理
    void picReadyRead(); //接收到图片数据时的处理
    void httpReadyRead(); //接收到http数据时的处理

    void slotVerifiCodeClicked();//单击验证码图片处理槽函数

    void on_pBtnLogin_clicked();

private:
    Ui::loginDlg *ui;

    QNetworkAccessManager *manager;//QNetworkAccessManager 类用于发送网络请求和接受回复
    QNetworkReply *reply;//QNetworkReply 类进行 接收回复，并对数据进行处理。
    QUrl url; //存储网络地址
    QString urlFrOption; //存储网络地址
    QString tempUrl;
    bool isUsedVerifiCode = true;
    QString tmpVerifiCodeUrl;
    QPixmap currentPicture;
    QString	currentFileName;

    void isUsedVerifiCodeFun();//是否使用验证码处理函数
    void initLoginDlg();
};

#endif // LOGINDLG_H
