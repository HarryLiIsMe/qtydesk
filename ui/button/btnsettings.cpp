#include "btnsettings.h"

#include <QFile>
BtnSettings::~BtnSettings()
{
    delete myMenu;
}
BtnSettings::BtnSettings(QWidget *parent) : QPushButton(parent)
{
    myMenu=new QMenu;
    QAction *paraAct=new QAction("帮助信息",this);
    //clickRandomPass->setCheckable(true);
    //clickRandomPass->setChecked(true);
    QAction *clickSetPass=new QAction("关于QtyDesk",this);
    myMenu->addAction(paraAct);
    myMenu->addAction(clickSetPass);
    /*QSS文件所在的路径*/
    QFile file(":/qs/qsss/menu.qss");
    file.open(QIODevice::ReadOnly);
    if(file.isOpen())
    {
        //QString style = this->styleSheet();
        QString strFile = file.readAll();
        myMenu->setStyleSheet(strFile);
        /*记得关闭QSS文件*/
        file.close();
    }

    this->setMenu(myMenu);
    connect(myMenu,SIGNAL(triggered(QAction*)),this,SLOT(actionTriggered(QAction*)));
}
void BtnSettings::actionTriggered(QAction *action)
{
    if(action->text() == "帮助信息")
        emit aboutQtyDesk();
    else if(action->text() == "关于QtyDesk"){
        emit aboutQtyDesk();
    }

}

