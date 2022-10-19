#include "movedialog.h"

#include <QDebug>
MoveDialog::MoveDialog(QWidget *parent) :  QDialog(parent),
    top_height(30)
{
    setWindowFlags(Qt::FramelessWindowHint |
                   Qt::Tool | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
}

void MoveDialog::mousePressEvent(QMouseEvent *e)
{
    last = e->globalPos();
    c_press = e->pos();

}
void MoveDialog::mouseMoveEvent(QMouseEvent *e)
{
    if(c_press.y()>top_height){
        return ;
    }
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    last = e->globalPos();
    move(x()+dx, y()+dy);
}
void MoveDialog::mouseReleaseEvent(QMouseEvent *e)
{
    if(c_press.y()>top_height){
        return ;
    }
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    move(x()+dx, y()+dy);
}

void MoveDialog::pressedAdjust()
{
    c_press.setX(100);
    c_press.setY(100);
}
void MoveDialog::setTopHeight(int height){
    this->top_height = height ;
}
