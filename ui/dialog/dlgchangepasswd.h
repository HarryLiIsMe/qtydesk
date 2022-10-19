#ifndef DLGCHANGEPASSWD_H
#define DLGCHANGEPASSWD_H

#include "movedialog.h"

namespace Ui {
class DlgChangePasswd;
}

class DlgChangePasswd : public MoveDialog
{
    Q_OBJECT

public:
    explicit DlgChangePasswd(QWidget *parent = nullptr);
    ~DlgChangePasswd();

signals:
    void setPasswdOk(const QString &passwd);
private slots:
    void on_btn_cancel_pressed();

    void on_btn_cancel_clicked();

    void on_btn_ok_clicked();

private:
    Ui::DlgChangePasswd *ui;
};

#endif // DLGCHANGEPASSWD_H
