#include "passivehandler.h"
PassiveHandler::PassiveHandler(QObject *parent) : SocketHandler(parent),
    m_graberClass(new GraberClass(this)),
    m_inputSimulator(new InputSimulator(this))
{
    connect(m_graberClass, &GraberClass::imageParameters, this, &PassiveHandler::sendImageParameters);
    connect(m_graberClass, &GraberClass::imageTile, this, &PassiveHandler::sendImageTile);
    //connect(m_graberClass, &GraberClass::screenPositionChanged, m_inputSimulator, &InputSimulator::setScreenPosition);
    connect(this, &PassiveHandler::getDesktop, m_graberClass, &GraberClass::startSending);

    //    connect(m_graberClass, &GraberClass::imageParameters, socketHandler, &PassiveHandler::sendImageParameters);
    //    connect(m_graberClass, &GraberClass::imageTile, socketHandler, &PassiveHandler::sendImageTile);
    //    //connect(m_graberClass, &GraberClass::screenPositionChanged, m_inputSimulator, &InputSimulator::setScreenPosition);

    //    connect(socketHandler, &PassiveHandler::getDesktop, m_graberClass, &GraberClass::startSending);
    //    connect(webSocketHandler, &WebSocketHandler::changeDisplayNum, m_graberClass, &GraberClass::changeScreenNum);
    connect(this, &PassiveHandler::receivedTileNum, m_graberClass, &GraberClass::setReceivedTileNum);

    //    connect(webSocketHandler, &WebSocketHandler::setKeyPressed, m_inputSimulator, &InputSimulator::simulateKeyboard);

    connect(this, &PassiveHandler::setMouseMove, m_inputSimulator, &InputSimulator::simulateMouseMove);
    connect(this, &PassiveHandler::setMousePressed, m_inputSimulator, &InputSimulator::simulateMouseKeys);
    //    connect(webSocketHandler, &WebSocketHandler::setWheelChanged, m_inputSimulator, &InputSimulator::simulateWheelEvent);
    //    connect(webSocketHandler, &WebSocketHandler::setMouseDelta, m_inputSimulator, &InputSimulator::setMouseDelta);


    m_graberClass->start();
}
//resv_exc  接收的Exchange
void PassiveHandler::dealProto(int i,BigPack::Exchange resv_exc){
    switch(i){
    case BigPack::Exchange::REPLY_REGISTER:
    {
        if(resv_exc.replyinfo().success()){
            this->m_transferID = QString::fromStdString(resv_exc.replyinfo().registerid());
            emit showDeviceID(this->m_transferID);
        }else{
            qDebug()<<"ID 不注册成功";
        }
        break;
    }
    case BigPack::Exchange::REQUEST_AUTH:{
        QString password = QString::fromStdString(resv_exc.requestauth().authpass());

        BigPack::CCResponseAuth *responseauth = new BigPack::CCResponseAuth();
        if(password == m_tempPass){
            responseauth->set_success(true);
        }else{
            responseauth->set_info(QString("密码错误").toStdString());
            responseauth->set_success(false);
        }
        BigPack::Exchange write_ex ;
        write_ex.set_datatype(BigPack::Exchange::RESPONSE_AUTH);
        write_ex.set_resourceid(m_transferID.toStdString());
        write_ex.set_targetid(resv_exc.resourceid());
        write_ex.set_allocated_responseauth(responseauth);
        std::string s_info ;
        write_ex.SerializeToString(&s_info);
        m_sslsocket->write(intToBytes(write_ex.ByteSizeLong()),sizeof(qint32));
        m_sslsocket->write(s_info.c_str(),write_ex.ByteSizeLong());
        m_sslsocket->flush();
        break;
    }
    case BigPack::Exchange::GET_DESKTOP:{
        emit getDesktop();
        break;
    }
    case BigPack::Exchange::TILE_RECEIVED:{
        int tileNum = resv_exc.tilereceived().tilenum();
        //qDebug()<<tileNum;
        emit receivedTileNum(tileNum);
        break;
    }
    case BigPack::Exchange::MOUSE_MOVE:{
        int x = resv_exc.mousemove().pointx();
        int y = resv_exc.mousemove().pointy();
        emit setMouseMove(x,y);
        break;
    }
    case BigPack::Exchange::MOUSE_KEY:{
        int keycode = resv_exc.mousekey().keycode();
        int state = resv_exc.mousekey().state();
        emit setMousePressed(keycode,state);
    }
    }
}
void PassiveHandler::setTempPass(const QString &tempPass){
    this->m_tempPass = tempPass;
}
QString PassiveHandler::getTempPass() {
    return this->m_tempPass ;
}

void PassiveHandler::sendImageParameters(const QSize &imageSize, int rectWidth)
{
    if(!m_sslsocket)
        return ;

    BigPack::CCGiveImgParameters * imgPara = new BigPack::CCGiveImgParameters();
    imgPara->set_width(imageSize.width());
    imgPara->set_height( imageSize.height());
    imgPara->set_rectwidth(rectWidth);

    BigPack::Exchange write_ex ;
    write_ex.set_datatype(BigPack::Exchange::GIVE_IMG_PARA);
    write_ex.set_resourceid(m_transferID.toStdString());
    write_ex.set_allocated_imgparameters(imgPara);

    //    std::string s_info ;
    //    write_ex.SerializeToString(&s_info);
    //    m_sslsocket->write(intToBytes(write_ex.ByteSizeLong()),sizeof(qint32));
    //    m_sslsocket->write(s_info.c_str(),write_ex.ByteSizeLong());
    //    m_sslsocket->flush();
    serializeSend(write_ex);
}
void PassiveHandler::sendImageTile(quint16 posX, quint16 posY, const QByteArray &imageData, quint16 tileNum)
{
    //    if(!m_isAuthenticated)
    //        return;

    BigPack::CCGiveImg *img = new BigPack::CCGiveImg();
    img->set_posx(posX);
    img->set_posy(posY);
    img->set_tilenum(tileNum);
    img->set_imgdata(imageData.data(),imageData.size());

    BigPack::Exchange write_ex ;
    write_ex.set_datatype(BigPack::Exchange::GIVE_IMG);
    write_ex.set_resourceid(m_transferID.toStdString());
    write_ex.set_allocated_img(img);

    serializeSend(write_ex);
}
