#include "loginsetupdlg.h"
#include "ui_loginsetupdlg.h"

loginSetupDlg::loginSetupDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginSetupDlg)
{
    ui->setupUi(this);
}

loginSetupDlg::~loginSetupDlg()
{
    delete ui;
}
