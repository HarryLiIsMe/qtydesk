#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "net/passivehandler.h"
#include "ui/dialog/dlgchangepasswd.h"

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString m_styleString;
    //WebSocketTransfer *m_webSocketTransfer;
    PassiveHandler *m_socketHandler;
    DlgChangePasswd *m_changeDialog;


    QMenu *m_trayMenu;
    QSystemTrayIcon *m_trayIcon;
    bool m_isConnectedToServer;

    QString m_remoteHost ;
    int m_remotePort;
    bool m_ssl ;

    QString m_lastString;

signals:
    void closeSignal();

public slots:
      void aboutMe();
      void showPasswd(bool show);
      void showChangePassDialog();
      void setTempPassword(const QString &passwd);
private slots:
      void loadStyleSheet();
      QString getRandomString();
      void loadTrayMenu();
      //小图标事件
      void actionTriggered(QAction *action);
      void iconActivated(QSystemTrayIcon::ActivationReason ireason);
      //加载配置文件
      void loadUIConnect();
      void loadSettings();
      void startPassiveHandler(const QString &remoteHost, quint16 remotePort,bool ssl,const QString &tempPass);
//      void createConnectionToHandler( PassiveHandler *m_socketHandler);
      void finishedSockeHandler();
      void uiShowDeviceID(QString showID);
      void showConnectedStatus(bool showStatus);
      void on_bt_connectRemoteDevice_clicked();
      void on_ed_remoteID_textChanged(const QString &arg1);
};

#endif // MAINWINDOW_H
