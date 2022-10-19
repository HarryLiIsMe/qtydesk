#ifndef MOVEDIALOG_H
#define MOVEDIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include <QPoint>
/**
 * @brief The MoveDialog class
 * 可以移动自定义标题对话框
 */
class MoveDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MoveDialog(QWidget *parent = nullptr);

signals:
private:
    QPoint last;
    QPoint c_press;
    //标题栏高
    int top_height ;
protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void pressedAdjust();
    void setTopHeight(int height);



};

#endif // MOVEDIALOG_H
