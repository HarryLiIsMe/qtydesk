#include "activewindow.h"
#include "ui_activewindow.h"

#include <QThread>
#include <QMouseEvent>
#include <QPainter>
#include <QBuffer>
#include <QFile>
ActiveWindow::ActiveWindow(QWidget *parent) :
    QOpenGLWidget(parent),
    dlgConnectMessage(new DlgConnectMessage(this)),
    ui(new Ui::ActiveWindow),
    m_activeHandler(Q_NULLPTR),
    m_remoteID(""),
    m_remotePass("")
{
    setMouseTracking(true);
    ui->setupUi(this);
}

ActiveWindow::~ActiveWindow()
{
    delete ui;
}
void ActiveWindow::dealPaintImg(quint16 posX, quint16 posY, const QByteArray &imageData, quint16 tileNum)
{
    m_posX = posX ;
    m_posY = posY ;
    QByteArray raw;
    unsigned char header[16] = {137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82};
    raw.append((char *)header,16);
    raw.append(imageData);

    m_pixmap.loadFromData(raw,"PNG");

    QPainter p(&m_lastImg);
    p.setCompositionMode(QPainter::CompositionMode_SourceOver);
    p.drawPixmap(m_posX*m_rectwidht,m_posY*m_rectwidht,m_pixmap);
    p.end();

    update();
}


void ActiveWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,this->width(),this->height(),m_lastImg);
}

void ActiveWindow::closeEvent(QCloseEvent *e)
{
    qDebug()<<"closeEvent";
    emit closeSignal();
}

void ActiveWindow::mouseMoveEvent(QMouseEvent *event)
{
    //当前窗体坐标
    QPoint currentPoint = event->pos();
    //当前窗体大小
    QSize currentSize = this->size()  ;
    int really_x = currentPoint.x() * m_imageWidth / currentSize.width() ;
    int really_y = currentPoint.y() * m_imageHeight / currentSize.height();
    //    QPoint newPoint(really_x,really_y);
    emit mouseMoveSend(really_x,really_y);
}

void ActiveWindow::mousePressEvent(QMouseEvent *event)
{
    state = true ;
    if(event->button() == Qt::LeftButton)
    {
        code = 0 ;
    }
    else if(event->button() == Qt::MidButton){
        code = 1 ;
    }
    else if(event->button() == Qt::RightButton)
    {
        code = 2 ;
    }
    emit sendMouseKeys(code,state) ;
}

void ActiveWindow::mouseReleaseEvent(QMouseEvent *event)
{
    state = false ;
    if(event->button() == Qt::LeftButton)
    {
        code = 0 ;
    }
    else if(event->button() == Qt::MidButton){
        code = 1 ;
    }
    else if(event->button() == Qt::RightButton)
    {
        code = 2 ;
    }
    emit sendMouseKeys(code,state) ;
}

void ActiveWindow::wheelEvent(QWheelEvent *event)
{
    if(event->delta() > 0)// 当滚轮远离使用者时
    {
        emit sendWheelEvent(false);
    }
    else//当滚轮向使用者方向旋转时
    {
        emit sendWheelEvent(true);
    }
}

void ActiveWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        emit sendMouseKeys(0,true) ;
    }
}

void ActiveWindow::keyPressEvent(QKeyEvent *event)
{
    emit sendKeyboard(event->nativeVirtualKey(),false);
}

void ActiveWindow::keyReleaseEvent(QKeyEvent *event)
{
    emit sendKeyboard(event->nativeVirtualKey(),true);
}

void ActiveWindow::startActiveHandler(QString remoteIP,int port,QString remoteID,QString remotePass,bool ssl){
    QThread *thread = new QThread;
    m_activeHandler = new ActiveHandler;
    m_activeHandler->setSSL(ssl);

    m_activeHandler->setType(SocketHandler::ACTIVE);
    m_activeHandler->setRemoteHost(remoteIP);
    m_activeHandler->setRemotePort(port);
    m_activeHandler->setRemoteID(remoteID);
    m_activeHandler->setRemotePass(remotePass);

    connect(thread, &QThread::started, m_activeHandler, &SocketHandler::createSocket);
    connect(this, &ActiveWindow::closeSignal, m_activeHandler, &SocketHandler::removeSocket);
    connect(m_activeHandler, &SocketHandler::finished, this, &ActiveWindow::finishedSockeHandler);
    connect(m_activeHandler, &SocketHandler::finished, thread, &QThread::quit);
    connect(thread, &QThread::finished, m_activeHandler, &SocketHandler::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    connect(m_activeHandler, &ActiveHandler::replyInfo, this, &ActiveWindow::showInfo);
    connect(m_activeHandler, &ActiveHandler::imgParameters, this, &ActiveWindow::imgParameters);

    connect(m_activeHandler, &ActiveHandler::paintImg, this, &ActiveWindow::dealPaintImg);

    //控制事件
    connect(this,SIGNAL(mouseMoveSend(int,int)),m_activeHandler,SLOT(slotSendMouseMove(int,int)));
    connect(this,SIGNAL(sendMouseKeys(int,bool)),m_activeHandler,SLOT(slotSendMouseKeys(int,bool)));
    connect(this,SIGNAL(sendWheelEvent(bool)),m_activeHandler,SLOT(slotSendWheelEvent(bool)));
    connect(this,SIGNAL(sendKeyboard(int,bool)),m_activeHandler,SLOT(slotSendKeyboard(int,bool)));

    m_activeHandler->moveToThread(thread);
    thread->start();
}
void ActiveWindow::showInfo(const QString &info){
    dlgConnectMessage->setMessageInfo(info);
    dlgConnectMessage->exec();
    //QMessageBox::information(0, "a",info);
    emit closeSignal();
}

void ActiveWindow::imgParameters(int width, int height, int rectwidth)
{
    m_imageWidth = width ;
    m_imageHeight = height;
    m_rectwidht = rectwidth;
    m_lastImg = QPixmap(width,height);
}


void ActiveWindow::finishedSockeHandler()
{
    m_activeHandler = Q_NULLPTR;
    this->deleteLater();
}
