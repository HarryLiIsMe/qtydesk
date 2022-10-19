#ifndef BTNPASSSETTING_H
#define BTNPASSSETTING_H

#include <QPushButton>
#include <QMenu>
class BtnPassSetting : public QPushButton
{
    Q_OBJECT
public:
    explicit BtnPassSetting(QWidget *parent = nullptr);

signals:
    void reflashPasswd();
    void setNewPasswd();
    void copyPasswd();
private:
     QMenu *myMenu;
private slots:
    void actionTriggered(QAction *action);
};

#endif // BTNPASSSETTING_H
