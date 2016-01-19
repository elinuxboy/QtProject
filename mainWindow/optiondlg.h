#ifndef OPTIONDLG_H
#define OPTIONDLG_H

#include <QDialog>
#include <QSettings>
#include <QMetaType>
#include <QVariant>
#include "lib/myqtapi.h"//自定义API头文件
#include "iniConfig.h"

static const int EXIT_CODE_REBOOT = -123456789;

namespace Ui {
class optionDlg;
}
#if 0
#pragma pack(push,1)//原来对齐方式设置压栈，并设新的对齐方式设置为一个字节对齐
struct OptionDataStruct
{
    QString webUrl;//服务器URL
    bool isUsedVerifiCode;//是否使用验证码登陆
};
#pragma pack(pop)//恢复对齐状态
Q_DECLARE_METATYPE(OptionDataStruct)
#endif
class optionDlg : public QDialog
{
    Q_OBJECT

public:
    explicit optionDlg(QWidget *parent = 0);
    ~optionDlg();

    QString webUrlFrFile;//从配置文件取出的服务器URL
    bool isUsedVerifiCode = true;//是否使用验证码登陆
    QString VerifiCodeUrlFrFile;//从配置文件取出的服务器URL
    bool isBtnApplyClicked = false;//应用按键是否被按下

    //void loadOptionFrFile(bool);//从配置文件加载选项设置，并设置相关参数。isDefault为true时加载默认配置文件

signals:
    void signalRebotToMainWin();//发送重启信号到主窗口的信号

private slots:
    void on_pBtnDefault_clicked();

    void on_pBtnSave_clicked();

    void on_cBoxIsUsedVerifiCode_stateChanged(int arg1);

    void on_pBtnCancel_clicked();

    void on_tabWidget_currentChanged(int index);

private:
    Ui::optionDlg *ui;

    int ShowMessageBoxQuesion(QString info);//显示询问框，确定和取消按钮
};

#endif // OPTIONDLG_H
