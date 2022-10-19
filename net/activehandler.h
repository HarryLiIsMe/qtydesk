#ifndef ACTIVEHANDLER_H
#define ACTIVEHANDLER_H

#include "sockethandler.h"

class ActiveHandler : public SocketHandler
{
    Q_OBJECT
public:
    explicit ActiveHandler(QObject *parent = nullptr);
    QString m_remoteID;
    QString m_remotePass;
signals:
    void replyInfo(QString info);
    void imgParameters(int width,int height,int rectwidth);
    void paintImg(quint16 posX, quint16 posY, const QByteArray &imageData, quint16 tileNum);
protected:
   void dealProto(int i,BigPack::Exchange resv_exc) override;

private:
   QString m_transferID ;

public slots:
   void setRemoteID(const QString &remoteID);
   void setRemotePass(const QString &remotePass);

private slots:
   void queryHost(const QString &remoteID);
   void sendRequestAuth(const QString &remoteID,const QString &remotePass);
   void GetDesktop();
   void sendReceivedTileNum(int tileNum);
};

#endif // ACTIVEHANDLER_H
