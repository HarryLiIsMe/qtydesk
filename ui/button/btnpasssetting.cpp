#include "btnpasssetting.h"

#include <QAction>
#include <QFile>
BtnPassSetting::BtnPassSetting(QWidget *parent) : QPushButton(parent)
{
    myMenu=new QMenu;
    QAction *clickRandomPass=new QAction("刷新随机密码",this);
    //clickRandomPass->setCheckable(true);
    //clickRandomPass->setChecked(true);
    QAction *clickSetPass=new QAction("设置密码",this);
   // QAction *copyPass=new QAction("复制ID密码",this);
    myMenu->addAction(clickRandomPass);
    myMenu->addAction(clickSetPass);
   // myMenu->addAction(copyPass);
    //读取Qss
    QFile file(":/qs/qsss/menu.qss");/*QSS文件所在的路径*/
    file.open(QIODevice::ReadOnly);
    if(file.isOpen())
    {
        //QString style = this->styleSheet();
        QString strFile = file.readAll();
        myMenu->setStyleSheet(strFile);
        file.close();/*记得关闭QSS文件*/
    }

    this->setMenu(myMenu);
    connect(myMenu,SIGNAL(triggered(QAction*)),this,SLOT(actionTriggered(QAction*)));
}
void BtnPassSetting::actionTriggered(QAction *action)
{
    if(action->text() == "刷新随机密码")
        emit reflashPasswd();
    else if(action->text() == "设置密码")
        emit setNewPasswd();
    else if(action->text() == "复制ID密码"){
        emit copyPasswd() ;
    }
}
