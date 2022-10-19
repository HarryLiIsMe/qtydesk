#include "dlgchangepasswd.h"
#include "ui_dlgchangepasswd.h"

DlgChangePasswd::DlgChangePasswd(QWidget *parent) :
    MoveDialog(parent),
    ui(new Ui::DlgChangePasswd)
{
    ui->setupUi(this);
}

DlgChangePasswd::~DlgChangePasswd()
{
    delete ui;
}

void DlgChangePasswd::on_btn_cancel_pressed()
{
    pressedAdjust();
}

void DlgChangePasswd::on_btn_cancel_clicked()
{
    this->hide();
}

void DlgChangePasswd::on_btn_ok_clicked()
{
    ui->lb_info->setText("");
    QString strPass = ui->ed_passwd->text().trimmed();
    if(strPass.length()<6){
        ui->lb_info->setText("密码必须大于等于六位数");
        return ;
    }
    QString confirm = ui->ed_confirm->text().trimmed();
    if(confirm !=strPass){
        ui->lb_info->setText("两次输入不匹配");
        return ;
    }
    emit setPasswdOk(strPass);
    this->hide();
}
