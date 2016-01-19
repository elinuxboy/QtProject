#ifndef LOGINSETUPDLG_H
#define LOGINSETUPDLG_H

#include <QDialog>

namespace Ui {
class loginSetupDlg;
}

class loginSetupDlg : public QDialog
{
    Q_OBJECT

public:
    explicit loginSetupDlg(QWidget *parent = 0);
    ~loginSetupDlg();

private:
    Ui::loginSetupDlg *ui;
};

#endif // LOGINSETUPDLG_H
