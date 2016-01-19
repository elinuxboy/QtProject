#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QTextCodec>
#include <QTimer>
#include <QCloseEvent>
//#include <QFtp>
#include <QtGui>

#include "logindlg.h"
#include "optiondlg.h"

#include "lib/qftp/qftp.h"
#include "lib/excelengine/excelengine.h"//操作excelengine所需头文件
#include "lib/json/json.h"//操作json所需头文件
#include "lib/qextserialport/qextserialport.h"//操作串口所需头文件


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);//声明和定义closeEvent函数事件

signals:
    void sendDataToLogDlg();//发数据到登陆对话框的信号

public slots:
    void slotReboot();

    void recvDataFrLoginDlg(QVariant dataVar);//从用户登录对话框接收数据处理槽函数

private slots:
    //void SetTime();//动态显示时间
    void ReadMyCom();//读取串口数据
    void WriteMyCom();//写串口数据

    void on_pBtnOption_clicked();
    void on_pBtnLoginWeb_clicked();
    void on_tabWidget_currentChanged(int index);

    void on_pBtnOpenFrLocal_clicked();
    void on_pBtnPostWebFrLocal_clicked();

    void ftpCommandFinished(int,bool);
    void ftpCommandStarted(int);
    void addToList(const QUrlInfo &urlInfo);//将服务器上的文件添加到 Tree Widget 中
    void processItem(QTreeWidgetItem*,int);//双击一个目录时显示其内容
    void on_pBtnFtpConnect_clicked();
    void on_pBtnPostWebFrFtp_clicked();
    void on_pBtnFtpCdToParent_clicked();
    void on_pBtnFtpDownload_clicked();
    void on_pBtnFtpUpload_clicked();

    void on_pBtnOpenFrDev_clicked();
    void on_pBtnClearSendFrDev_clicked();
    void on_pBtnClearRecvFrDev_clicked();
    void on_pBtnPostWebFrDev_clicked();
    void on_pBtnDevSend_clicked();
    void on_checkIsDebugSimulate_stateChanged(int arg1);
    void on_checkHexReceive_stateChanged(int arg1);
    void on_checkHexSend_stateChanged(int arg1);
    void on_checkIsAutoSend_stateChanged(int arg1);
    void on_cBoxAutoSendTime_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    void mainWinInit();//主界面初始化函数
    void tabFrLocalInit();//本地选项卡初始化
    void tabFrFtpInit();//FTP选项卡初始化
    void tabFrDevInit();//串口设备选项卡初始化

    loginDlg dlgLogin;//声明登陆对话框类
    optionDlg dlgOption;//声明选项对话框类

    uint gRowCount;//excel表格行数
    uint gColumnCount;//excel表格列数
    uint gLineNum;//文本文件总行数

    QFtp *ftp;//定义对象
    QFile *file;
    QHash<QString, bool> isDirectory;//用来存储一个路径是否为目录的信息
    QString currentPath;//用来存储现在的路径
    QString _ToSpecialEncoding(const QString &InputStr);
    QString _FromSpecialEncoding(const QString &InputStr);
    void ftpFiletoJson(QString fileName);

    QTimer *myTimer;
    QTimer *myReadTimer;//定时读取串口数据
    QTimer *mySendTimer;//定时发送串口数据
    QextSerialPort *myCom;//串口对象
    int SendCount;//发送数据计数
    int ReceiveCount;//接收数据计数
    bool IsShow;//是否显示数据
    bool IsDebug;//是否启用调试,接收到数据后模拟发送数据
    bool IsAutoClear;//是否自动清空
    bool IsHexSend;//是否16进制数据发送
    bool IsHexReceive;//是否16进制数据接收
    bool isMyComOpened = false;//标记串口是否打开
    QStringList SendDataList;//转发数据链表
    void InitComForm();//串口初始化
    void ChangeEnableToCom(bool b);//串口使能
};

#endif // MAINWINDOW_H
