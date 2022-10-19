#include "dlgconnectmessage.h"
#include "ui_dlgconnectmessage.h"

DlgConnectMessage::DlgConnectMessage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgConnectMessage)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint |
                   Qt::Tool | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
}

DlgConnectMessage::~DlgConnectMessage()
{
    delete ui;
}

void DlgConnectMessage::setMessageInfo(const QString &msg)
{
    ui->lb_info->setText(msg);
}

void DlgConnectMessage::on_btn_ok_clicked()
{
    this->close();
}
