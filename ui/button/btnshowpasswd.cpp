#include "btnshowpasswd.h"

//鼠标移动显示
BtnShowPasswd::BtnShowPasswd(QWidget *parent) : QPushButton(parent)
{


}
BtnShowPasswd::~BtnShowPasswd()
{

}
void BtnShowPasswd::enterEvent(QEvent *e)
{
    emit showPasswd(true);

}
void BtnShowPasswd::leaveEvent(QEvent *e)
{
    emit showPasswd(false);
}
