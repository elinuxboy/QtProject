#include "optiondlg.h"
#include "ui_optiondlg.h"

optionDlg::optionDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::optionDlg)
{
    ui->setupUi(this);

    myQtAPI::SetUTF8Code();//设置全局使用的是utf-8字符

    myQtAPI::FormOnlyCloseInCenter(this);//窗体居中，并且只有关闭按钮，不能调整大小

    ui->tabWidget->setCurrentIndex(0);

    ui->labelMsg->setText(ui->tabWidget->tabText(ui->tabWidget->currentIndex()));

    qDebug() << "ui->cBoxIsUsedVerifiCode->checkState()" << ui->cBoxIsUsedVerifiCode->checkState();
    OptionDataStruct tmpOptionData;
    tmpOptionData = iniConfig::loadOptionFrFile(false);//加载配置文件，true为加载默认配置文件
    if(tmpOptionData.VerifiCodeUrl.isEmpty())
    {
        tmpOptionData = iniConfig::loadOptionFrFile(true);
    }

    //qDebug() << serverUrl;
    ui->lineEditWebUrl->setText(tmpOptionData.webUrl);
    ui->cBoxIsUsedVerifiCode->setChecked(tmpOptionData.isUsedVerifiCode);
    ui->lineEditVerifiCodeUrl->setText(tmpOptionData.VerifiCodeUrl);
}

optionDlg::~optionDlg()
{
    delete ui;
}

void optionDlg::on_pBtnDefault_clicked()
{
    OptionDataStruct tmpOptionData;
    tmpOptionData = iniConfig::loadOptionFrFile(true);//加载配置文件，true为加载默认配置文件

    //qDebug() << serverUrl;
    ui->lineEditWebUrl->setText(tmpOptionData.webUrl);
    ui->cBoxIsUsedVerifiCode->setChecked(tmpOptionData.isUsedVerifiCode);
    ui->lineEditVerifiCodeUrl->setText(tmpOptionData.VerifiCodeUrl);

    myQtAPI::ShowMessageBoxInfo(tr("默认设置已恢复成功。"));
    ui->labelMsg->setText(tr("默认设置已恢复"));
}

void optionDlg::on_pBtnSave_clicked()
{
    OptionDataStruct tmpOptionData;
    tmpOptionData.webUrl = ui->lineEditWebUrl->text();
    tmpOptionData.isUsedVerifiCode = ui->cBoxIsUsedVerifiCode->checkState();
    tmpOptionData.VerifiCodeUrl = ui->lineEditVerifiCodeUrl->text();

    iniConfig::saveOptionToFile(tmpOptionData);//保存设置信息


    if(!myQtAPI::ShowMessageBoxQuesion(tr("设置已保存成功。\n重启后生效！\n是否重启？")))
    {
        this->close();
        emit signalRebotToMainWin();//发送重启信号给主窗口
    }

    this->close();
}

void optionDlg::on_cBoxIsUsedVerifiCode_stateChanged(int arg1)
{
    isUsedVerifiCode = {arg1 == 0 ? false : true};
}

void optionDlg::on_pBtnCancel_clicked()
{
    OptionDataStruct tmpOptionData;
    tmpOptionData = iniConfig::loadOptionFrFile(false);//加载配置文件，true为加载默认配置文件
    if(tmpOptionData.VerifiCodeUrl.isEmpty())
    {
        tmpOptionData = iniConfig::loadOptionFrFile(true);
    }

    //qDebug() << serverUrl;
    ui->lineEditWebUrl->setText(tmpOptionData.webUrl);
    ui->cBoxIsUsedVerifiCode->setChecked(tmpOptionData.isUsedVerifiCode);
    ui->lineEditVerifiCodeUrl->setText(tmpOptionData.VerifiCodeUrl);

    this->close();
}

void optionDlg::on_tabWidget_currentChanged(int index)
{
    ui->labelMsg->setText(ui->tabWidget->tabText(ui->tabWidget->currentIndex()));
}
