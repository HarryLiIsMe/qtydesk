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
protected:
    void paintEvent(QPaintEvent *) override;
    void closeEvent ( QCloseEvent * e ) override;

    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override ;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent * event) override;
    void keyReleaseEvent(QKeyEvent * event) override;
public:
    int m_rectwidht ;
    int m_posX ;
    int m_posY ;
    int m_tileNum ;
    QPixmap m_pixmap ;
    QPixmap m_lastImg ;

    DlgConnectMessage *dlgConnectMessage;
signals:
    void closeSignal();

    void mouseMoveSend(int x,int y);
    void sendMouseKeys(int code,bool state);
    void sendWheelEvent(bool deltaPos);
    void sendKeyboard(int code,bool state);
private:
    Ui::ActiveWindow *ui;
    ActiveHandler *m_activeHandler ;
    QString m_remoteID;
    QString m_remotePass ;

    int m_imageWidth ;
    int m_imageHeight;
    //鼠标左键，右键
    int code ;
    //鼠标按下松开
    bool state ;

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
