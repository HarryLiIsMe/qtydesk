#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "activewindow.h"

#include <QSettings>
#include <QThread>
#include <QCommonStyle>
#include <QMessageBox>
#include <QUuid>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_socketHandler(Q_NULLPTR),
    m_changeDialog(new DlgChangePasswd(this)),
    m_trayMenu(new QMenu),
    m_trayIcon(new QSystemTrayIcon(this)),
    m_isConnectedToServer(false),
    m_remoteHost(""),
    m_remotePort(0),
    m_ssl(false)
{
    ui->setupUi(this);

    loadUIConnect();
    //加载配置文件
    loadTrayMenu();
    loadSettings();


}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::aboutMe()
{
    QString text = tr("<h3>QtyDesk远程桌面</h3>\n\n"
                       "<p>远程桌面软件，开箱即用，无需任何配置。您完全掌控数据，不用担心安全问题。</p>");
    QString contacts = tr("<p>联系:</p><p>邮箱:  javacgo@163.com</p>"
                       "<p>Github: <a href=\"https://%1/\">%1</a></p>"
                       "<p>当前版本: <a href=\"http://%2/\">0.0.5 beta</a></p>").
            arg(QStringLiteral("github.com/mathwm"),
                QStringLiteral("qtydesk.com"));

    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setWindowTitle(tr("关于QtyDesk远程桌面"));
    msgBox->setText(text);
    msgBox->setInformativeText(contacts);

    msgBox->setIconPixmap(QPixmap(":/img/images/logo.ico"));
    msgBox->exec();
    delete msgBox;
}
void MainWindow::loadUIConnect(){

    connect(ui->bt_settings,&BtnSettings::aboutQtyDesk,this,&MainWindow::aboutMe) ;

    connect(ui->bt_eye,&BtnShowPasswd::showPasswd,this,&MainWindow::showPasswd) ;

    connect(ui->bt_changePasswd,&BtnPassSetting::reflashPasswd,this,&MainWindow::getRandomString) ;
    connect(ui->bt_changePasswd,&BtnPassSetting::setNewPasswd,this,&MainWindow::showChangePassDialog) ;
    //     connect(ui->bt_changePasswd,&BtnPassSetting::copyPasswd,this,&MainWindow::copyPasswd) ;

    //对话框信号 到 主界面处理
    connect(m_changeDialog,&DlgChangePasswd::setPasswdOk,this,&MainWindow::setTempPassword) ;
}
void MainWindow::loadTrayMenu()
{
    QCommonStyle style;
    m_trayMenu->addAction(QIcon(style.standardPixmap(QStyle::SP_ComputerIcon)),"打开主界面");
    //m_trayMenu->addAction(QIcon(style.standardPixmap(QStyle::SP_MessageBoxInformation)),"基本设置");
    m_trayMenu->addAction(QIcon(style.standardPixmap(QStyle::SP_DialogCancelButton)),"退出");
    //托盘加入菜单
    m_trayIcon->setContextMenu(m_trayMenu);
    m_trayIcon->setIcon(QIcon(":/img/images/favicon.ico"));
    m_trayIcon->setToolTip("QtyDesk");

    //菜单子项触发
    connect(m_trayMenu,SIGNAL(triggered(QAction*)),this,SLOT(actionTriggered(QAction*)));
    //图标触发
    connect(m_trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    //让托盘图标显示在系统托盘上
    m_trayIcon->show();
}
//点击图标触发
void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason ireason)
{
    switch (ireason)
    {
    case QSystemTrayIcon::Trigger:
        //单击
        //this->showNormal();
        break;
    case QSystemTrayIcon::DoubleClick:
        this->showNormal();
        break;
    case QSystemTrayIcon::MiddleClick:
        break;
    default:
        break;
    }
}
//菜单子项触发
void MainWindow::actionTriggered(QAction *action)
{
    if(action->text() == "打开主界面")
    {
        this->showNormal();
    }
    else if(action->text() == "退出"){
        emit closeSignal();
    }
}
//加载配置文件
void MainWindow::loadSettings()
{
    QSettings settings("config.ini",QSettings::IniFormat);
    settings.beginGroup("REMOTE_DESKTOP_SERVER");

    QString remoteHost = settings.value("remoteHost").toString();
    if(remoteHost.isEmpty())
    {
        remoteHost = "server.qtydesk.com";
        settings.setValue("remoteHost",remoteHost);
    }
    m_remoteHost = remoteHost ;

    int remotePort = settings.value("remotePort",0).toInt();
    if(remotePort == 0)
    {
        remotePort = 443;
        settings.setValue("remotePort",remotePort);
    }
    m_remotePort = remotePort ;

    QString tempPass = settings.value("tempPass").toString();

    if(tempPass.isEmpty())
    {
        //得到随机数
        tempPass = getRandomString();
        settings.setValue("tempPass",tempPass);
    }

    int ssl = settings.value("ssl",1).toInt();
    if (!QSslSocket::supportsSsl()) {
        QMessageBox::information(0, "Secure Socket Client",
                                 "This system does not support OpenSSL."
                                 " The program will proceed with an insecure connection");
    }
    if(1 == ssl){
        m_ssl = true ;
        settings.setValue("ssl",ssl);
    }else{
        QMessageBox::information(0, "Secure Socket Client",
                                 "The program will proceed with an insecure connection.");
    }
    settings.endGroup();
    settings.sync();

    //被控连接
    startPassiveHandler(remoteHost, remotePort,ssl,tempPass);
}

void MainWindow::startPassiveHandler(const QString &remoteHost, quint16 port,bool ssl,const QString &tempPass)
{
    QThread *thread = new QThread;
    m_socketHandler = new PassiveHandler;
    m_socketHandler->setSSL(ssl);

    m_socketHandler->setType(SocketHandler::SESSION1);
    m_socketHandler->setRemoteHost(remoteHost);
    m_socketHandler->setRemotePort(port);
    m_socketHandler->setTempPass(tempPass);

    //    m_socketHandler->setName(name);
    //    m_socketHandler->setLoginPass(login, pass);
    //    m_socketHandler->setProxyLoginPass(proxyLogin, proxyPass);
    //线程开始,网络处理类创建socket
    connect(thread, &QThread::started, m_socketHandler, &SocketHandler::createSocket);
    connect(this, &MainWindow::closeSignal, m_socketHandler, &SocketHandler::removeSocket);
    connect(m_socketHandler, &SocketHandler::finished, this, &MainWindow::finishedSockeHandler);
    connect(m_socketHandler, &SocketHandler::finished, thread, &QThread::quit);
    connect(thread, &QThread::finished, m_socketHandler, &SocketHandler::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    connect(m_socketHandler, &PassiveHandler::showDeviceID, this, &MainWindow::uiShowDeviceID);
    connect(m_socketHandler, &SocketHandler::connectedStatus,this, &MainWindow::showConnectedStatus);

    //处理图片 和 模拟输入
    //createConnectionToHandler(m_socketHandler);

    m_socketHandler->moveToThread(thread);
    thread->start();
}


void MainWindow::showConnectedStatus(bool showStatus){
    m_isConnectedToServer = showStatus;
    QString message("");

    if(m_isConnectedToServer){
        ui->bt_lamp->setStyleSheet("background-color:rgb(50,190,166);border-radius:10px;");
        message.append("就绪");
        //ui->bt_connectRemoteDevice->setEnabled(true);
    }else{
        ui->bt_lamp->setStyleSheet("background-color:rgb(185,54,54);border-radius:10px;");
        message.append("连接失败，启动重连。。。 ");
        //ui->bt_connectRemoteDevice->setEnabled(false);
    }
    ui->lb_connect_state->setText(message);
}
void MainWindow::uiShowDeviceID(QString showID){
    showID = showID.mid(0,3)+" "+ showID.mid(3,3) +" "+ showID.mid(6,3) ;
    ui->lb_showDeviceId->setText(showID);
    //    ui->bt_connectRemoteDevice->setEnabled(true);
    //    ui->lb_showDeviceId->setText(showID);
}

void MainWindow::finishedSockeHandler()
{
    m_socketHandler = Q_NULLPTR;

    //if(!m_webSocketTransfer)
    //这里还不能退出，暂时这样处理。
    QApplication::quit();
}

void MainWindow::showPasswd(bool show)
{
    if(show){
        if(m_socketHandler)
            ui->lb_passwd->setText(m_socketHandler->getTempPass());
    }else{
        ui->lb_passwd->setText("******");
    }
}
QString MainWindow::getRandomString()
{
    QString strUUID = QUuid::createUuid().toString().remove("{").remove("}").remove("-");
    QString randomPass = strUUID.right(6) ;

    setTempPassword(randomPass);

    return randomPass;
}



//开始按钮连接
void MainWindow::on_bt_connectRemoteDevice_clicked()
{
    QString remoteID = ui->ed_remoteID->text().remove(QRegExp("\\s"));
    QString remotePass = ui->ed_remotePass->text();
    //注意内存泄漏，不用时候记得删除
    ActiveWindow *activeWindow = new ActiveWindow;
    activeWindow->show();
    activeWindow->startActiveHandler(m_remoteHost,m_remotePort,remoteID,remotePass,m_ssl);
}

void MainWindow::on_ed_remoteID_textChanged(const QString &arg1)
{
    QString willRemoteId =ui->ed_remoteID->text();
    if(arg1.size() >= m_lastString.size()){
        if( arg1.length()%4 == 3 ){
            willRemoteId = willRemoteId+" ";
            ui->ed_remoteID->setText(willRemoteId);
        }
    }
    m_lastString = willRemoteId ;
}
//--------------------显示改变密码对话框------------------------------//
void MainWindow::showChangePassDialog(){
    m_changeDialog->exec();
}
//--------------------显示改变密码对话框------------------------------//

void MainWindow::setTempPassword(const QString &passwd)
{ 
    if(m_socketHandler){
        m_socketHandler->setTempPass(passwd);
    }
    QSettings settings("config.ini",QSettings::IniFormat);
    settings.beginGroup("REMOTE_DESKTOP_SERVER");

    settings.setValue("tempPass",passwd);

    settings.endGroup();
    settings.sync();
}
