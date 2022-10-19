#ifndef ACTIVEWINDOW_H
#define ACTIVEWINDOW_H

#include "net/activehandler.h"
#include "ui/dialog/dlgconnectmessage.h"
#include <QOpenGLWidget>

namespace Ui {
class ActiveWindow;
}

class ActiveWindow : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit ActiveWindow(QWidget *parent = nullptr);
    ~ActiveWindow();
    void paintEvent(QPaintEvent *) override;
    void closeEvent ( QCloseEvent * e ) override;

    int m_rectwidht ;
    int m_posX ;
    int m_posY ;
    int m_tileNum ;
    QPixmap m_pixmap ;
    QPixmap m_lastImg ;

    DlgConnectMessage *dlgConnectMessage;
signals:
    void closeSignal();
private:
    Ui::ActiveWindow *ui;
    ActiveHandler *m_activeHandler ;
    QString m_remoteID;
    QString m_remotePass ;



public slots:
//    void setRemoteID(QString remoteID) ;
//    void setRemotePass(QString remotePass) ;
    void startActiveHandler(QString remoteIP,int port,QString remoteID,QString remotePass,bool ssl);
    void finishedSockeHandler();
    void showInfo(const QString &info);
    void imgParameters(int width,int height,int rectwidth);
    void dealPaintImg(quint16 posX, quint16 posY, const QByteArray &imageData, quint16 tileNum);
};

#endif // ACTIVEWINDOW_H
