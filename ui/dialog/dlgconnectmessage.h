#ifndef DLGCONNECTMESSAGE_H
#define DLGCONNECTMESSAGE_H

#include <QDialog>

namespace Ui {
class DlgConnectMessage;
}

class DlgConnectMessage : public QDialog
{
    Q_OBJECT

public:
    explicit DlgConnectMessage(QWidget *parent = nullptr);
    ~DlgConnectMessage();
    void setMessageInfo(const QString &msg);
private slots:
    void on_btn_ok_clicked();

private:
    Ui::DlgConnectMessage *ui;
};

#endif // DLGCONNECTMESSAGE_H
