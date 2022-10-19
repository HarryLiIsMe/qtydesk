#ifndef BTNSHOWPASSWD_H
#define BTNSHOWPASSWD_H

#include <QPushButton>

class BtnShowPasswd : public QPushButton
{
    Q_OBJECT
public:
    explicit BtnShowPasswd(QWidget *parent = 0);

    ~BtnShowPasswd();
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);
signals:
    void showPasswd(bool show);
};

#endif // BTNSHOWPASSWD_H
