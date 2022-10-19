#ifndef SOCKETHANDLER_H
#define SOCKETHANDLER_H

#include "proto/Exchange.pb.h"

#include <QObject>
#include <QtNetwork/QSslSocket>
#include <QTimer>

class SocketHandler : public QObject
{
    Q_OBJECT
public:
    explicit SocketHandler(QObject *parent = nullptr);
    enum HandlerType
    {   //Windows session问题，所以要分别在不同session连接两次
        SESSION1,
        SESSION0,
        ACTIVE
    };

    enum WaitResponseType
    {
        WaitTypeUnknown,
        WaitTypeRemoteAuth
    };
protected:
    QSslSocket *m_sslsocket;
    //处理协议数据
    virtual void dealProto(int i,BigPack::Exchange resv_exc) = 0;
    void serializeSend(BigPack::Exchange ex);
    //数据处理
    //大端小段
    int bytesToInt(QByteArray b) ;
    QByteArray intToBytes(int n) ;
private:
    //处理粘包socket
    QByteArray m_buffer ;

    //定时重连
    QTimer *m_timerReconnect;
    //远程设备
    int m_type;
    bool m_ssl ;
    QString m_remoteHost ;
    quint16 m_remotePort ;
    QString m_deviceID ;
signals:
    void connectedStatus(bool);
    void finished();
//    void registerHost();
public slots:
    void createSocket();
    void removeSocket();

    void setType(int type);
    void setRemoteHost(const QString &hostIP);
    void setRemotePort(quint16 port);
    void setSSL(bool ssl);

private slots:

    void readMessage();
    void handleSocketError(QAbstractSocket::SocketError);
    void socketStateChanged(QAbstractSocket::SocketState state);
    void socketEncrypted();
    void sslErrors(const QList<QSslError> &errors);
    void handleConnected();  
    void timerReconnectTick();
    void reisterHost();

};

#endif // SOCKETHANDLER_H
