#include "activehandler.h"

ActiveHandler::ActiveHandler(QObject *parent) : SocketHandler(parent),
    m_remoteID(""),
    m_remotePass("")
{

}

void ActiveHandler::dealProto(int i, BigPack::Exchange resv_exc)
{
    switch(i){
    case BigPack::Exchange::REPLY_REGISTER:
    {
        if(resv_exc.replyinfo().success()){
            this->m_transferID = QString::fromStdString(resv_exc.replyinfo().registerid());
            queryHost(m_remoteID);
        }
        break;
    }
    case BigPack::Exchange::REPLY_QUERY:{
        if(resv_exc.replyquery().ifexithost()){
            sendRequestAuth(m_remoteID,m_remotePass);
        }else{
            QString info = QString::fromStdString(resv_exc.replyquery().querymessage());
            emit replyInfo(info);
        }
        break;
    }
    case BigPack::Exchange::RESPONSE_AUTH:{
        if(resv_exc.responseauth().success()){
            GetDesktop();
        }else{
            QString info = QString::fromStdString(resv_exc.responseauth().info());
            emit replyInfo(info);
        }
        break;
    }
    case BigPack::Exchange::GIVE_IMG_PARA:{
        int rectwidth = resv_exc.imgparameters().rectwidth();
        int width = resv_exc.imgparameters().width();
        int height = resv_exc.imgparameters().height();
        emit imgParameters(width,height,rectwidth);
        break;
    }
    case BigPack::Exchange::GIVE_IMG:{
        int posX = resv_exc.img().posx();
        int posY = resv_exc.img().posy();
        QByteArray in(resv_exc.img().imgdata().c_str(),resv_exc.img().imgdata().size());
        int tileNum = resv_exc.img().tilenum();
        sendReceivedTileNum(tileNum);
        emit paintImg(posX,posY,in,tileNum);
        break;
        }
    }
}

void ActiveHandler::setRemoteID(const QString &remoteID)
{
    m_remoteID = remoteID ;
}

void ActiveHandler::setRemotePass(const QString &remotePass)
{
    m_remotePass = remotePass ;
}

void ActiveHandler::queryHost(const QString &remoteID)
{
    BigPack::Exchange ex ;
    ex.set_resourceid(m_transferID.toStdString());
    ex.set_targetid(remoteID.toStdString());
    ex.set_datatype(BigPack::Exchange::QUERY_HOST);

    BigPack::CsQueryHost *query = new BigPack::CsQueryHost();
    ex.set_allocated_queryhost(query);

    serializeSend(ex);
}
void ActiveHandler::sendRequestAuth(const QString &remoteID,const QString &remotePass){
    BigPack::Exchange ex ;
    ex.set_resourceid(m_transferID.toStdString());
    ex.set_targetid(remoteID.toStdString());
    ex.set_datatype(BigPack::Exchange::REQUEST_AUTH);

    BigPack::CCRequestAuth *requestAuth = new BigPack::CCRequestAuth();
    requestAuth->set_authpass(remotePass.toStdString());

    ex.set_allocated_requestauth(requestAuth);

    serializeSend(ex);
}

void ActiveHandler::GetDesktop()
{
     BigPack::Exchange ex ;
     ex.set_resourceid(m_transferID.toStdString());
     ex.set_targetid(m_remoteID.toStdString());
     ex.set_datatype(BigPack::Exchange::GET_DESKTOP);

     BigPack::CCGetDesktop *getDesktop = new BigPack::CCGetDesktop();
     ex.set_allocated_getdesktop(getDesktop);
     serializeSend(ex);
}

void ActiveHandler::sendReceivedTileNum(int tileNum)
{
    BigPack::Exchange ex ;
    ex.set_resourceid(m_transferID.toStdString());
    ex.set_targetid(m_remoteID.toStdString());
    ex.set_datatype(BigPack::Exchange::TILE_RECEIVED);

    BigPack::APTileReceived *tileReceived = new  BigPack::APTileReceived();
    tileReceived->set_tilenum(tileNum);
    ex.set_allocated_tilereceived(tileReceived);
    serializeSend(ex);
}