#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFtp>
#include <QtGui>

#include "myqtapi.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pBtnFtpConnect_clicked();

    void on_pBtnFtpCdToParent_clicked();

    void on_pBtnFtpDown_clicked();

    void on_pBtnFtpUp_clicked();

    void ftpCommandFinished(int,bool);
    void ftpCommandStarted(int);
    void updateDataTransferProgress(qint64,qint64);//更新进度条
    void addToList(const QUrlInfo &urlInfo);//将服务器上的文件添加到 Tree Widget 中
    void processItem(QTreeWidgetItem*,int);//双击一个目录时显示其内容

    void on_pBtnExit_clicked();

    void uploadFile();

private:
    Ui::MainWindow *ui;

    QFtp *ftp;//定义对象

    QFile *file;

    QHash<QString, bool> isDirectory;//用来存储一个路径是否为目录的信息
    QString currentPath;//用来存储现在的路径

    QString _ToSpecialEncoding(const QString &InputStr);
    QString _FromSpecialEncoding(const QString &InputStr);
};

#endif // MAINWINDOW_H
