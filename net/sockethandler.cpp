#include "sockethandler.h"

#include <QSslCipher>
#include "machine/deviceinfo.h"
SocketHandler::SocketHandler(QObject *parent) : QObject(parent),
    m_sslsocket(Q_NULLPTR),
    m_timerReconnect(Q_NULLPTR),
    m_ssl(false)
{
}
void SocketHandler::createSocket()
{
    if(m_sslsocket)
        return ;
    m_sslsocket = new QSslSocket(this);
    connect(m_sslsocket, &QAbstractSocket::stateChanged, this, &SocketHandler::socketStateChanged);
    //加密握手
    connect(m_sslsocket, SIGNAL(encrypted()),this, SLOT(socketEncrypted()));
    //连接上时候
    connect(m_sslsocket, &QAbstractSocket::connected, this, &SocketHandler::handleConnected);
    //网络连接中出现的错误
    connect(m_sslsocket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(handleSocketError(QAbstractSocket::SocketError)));
    //加密途中出现的错误
    connect(m_sslsocket, SIGNAL(sslErrors(QList<QSslError>)),this, SLOT(sslErrors(QList<QSslError>)));
    //准备读
    connect(m_sslsocket, SIGNAL(readyRead()),this, SLOT(readMessage()));

    //定时器
    if(!m_timerReconnect)
    {
        m_timerReconnect = new QTimer(this);
        connect(m_timerReconnect, &QTimer::timeout, this, &SocketHandler::timerReconnectTick);
    }

    if(m_ssl){
        m_sslsocket->connectToHostEncrypted(m_remoteHost, m_remotePort);
    }else{
        m_sslsocket->connectToHost(m_remoteHost,m_remotePort);
    }
}
void SocketHandler::timerReconnectTick()
{
    if(m_sslsocket->state() == QAbstractSocket::ConnectedState)
    {
        m_timerReconnect->stop();
        return;
    }
    if(m_sslsocket->state() == QAbstractSocket::ConnectingState)
        m_sslsocket->abort();
    //重连
    if(m_ssl){
        m_sslsocket->connectToHostEncrypted(m_remoteHost, m_remotePort);
    }else{
        m_sslsocket->connectToHost(m_remoteHost,m_remotePort);
    }
}
void SocketHandler::removeSocket()
{
    if(m_timerReconnect)
    {
        if(m_timerReconnect->isActive())
            m_timerReconnect->stop();

        m_timerReconnect->deleteLater();
        m_timerReconnect = Q_NULLPTR;
    }

    //    if(m_timerWaitResponse)
    //    {
    //        if(m_timerWaitResponse->isActive())
    //            m_timerWaitResponse->stop();

    //        m_timerWaitResponse->deleteLater();
    //        m_timerWaitResponse = Q_NULLPTR;
    //    }
    if(m_sslsocket)
    {
        if(m_sslsocket->state() != QAbstractSocket::UnconnectedState)
            m_sslsocket->close();

        m_sslsocket->disconnect();
        m_sslsocket->deleteLater();
        m_sslsocket = Q_NULLPTR;
    }
    emit finished();
}
//读取槽函数
void SocketHandler::readMessage(){
    ///sender() 方法 可以获取socket
    if(m_sslsocket->bytesAvailable() <=0)
        return;
    QByteArray buffer =m_sslsocket->readAll();
    //读取数据放入缓冲区
    m_buffer.append(buffer);
    qint32 pro_len;
    int bufferSize = m_buffer.size();
    while(bufferSize)
    {
        //不够包头数据长度，等下次解析
        if(bufferSize < (int)(sizeof(qint32)*1))
            break;
        //不够数据长度,返回，如果足够，获取协议长度，翻译协议
        pro_len =bytesToInt(m_buffer) ;
        if(bufferSize < pro_len + (int)sizeof(qint32)*1)
            break;
        QByteArray exBuf = m_buffer.mid((int)sizeof(qint32)*1,pro_len);
        //----------------------------------------------------------------
        BigPack::Exchange resv_exc ;
        //反序列化
        resv_exc.ParseFromString(exBuf.toStdString());
        //----------------------------处理协议------------------------------
        dealProto(resv_exc.datatype(),resv_exc);
        //-----------------------------------------------------------------
        //缓存多余的数据
        buffer = m_buffer.right(bufferSize - pro_len - sizeof(qint32)*1) ;
        //更新长度
        bufferSize = buffer.size();
        //更新多余的数据
        m_buffer = buffer;
    }
}
void SocketHandler::serializeSend(BigPack::Exchange ex){
    if(!m_sslsocket->isOpen()){
       qFatal("设备没有打开!!!");
       return ;
    }
    std::string s_info ;
    ex.SerializeToString(&s_info);
    m_sslsocket->write(intToBytes(ex.ByteSizeLong()),sizeof(qint32));
    m_sslsocket->write(s_info.c_str(),ex.ByteSizeLong());
    m_sslsocket->flush();
}
void SocketHandler::reisterHost(){
    QString cpuID = DeviceInfo::getAllPlatformCpuID();
    QString diskID = DeviceInfo::getDiskdrive();
    QString macAddr =DeviceInfo::getHostMacAddress();
    QString pcName =DeviceInfo::getPcName();
    //qDebug()<<"注册设备信息";
    BigPack::Exchange ex ;
    ex.set_datatype(BigPack::Exchange::REGISTER);
    BigPack::CsRegisterHost *info = new BigPack::CsRegisterHost();
    info->set_uniqueid(DeviceInfo::getHashSum(cpuID,diskID).toStdString());
    info->set_cpuid(cpuID.toStdString());
    info->set_deviceid(diskID.toStdString());
    info->set_mac(macAddr.toStdString());

    info->set_type(m_type);
    ex.set_allocated_hostinfo(info);

    serializeSend(ex);
}
void SocketHandler::handleConnected(){
    if(!m_ssl){
       reisterHost();
    }
}
void SocketHandler::handleSocketError(QAbstractSocket::SocketError)
{  
   qDebug() << m_sslsocket->errorString();
}
void SocketHandler::socketStateChanged(QAbstractSocket::SocketState state)
{
    switch(state)
    {
    case QAbstractSocket::UnconnectedState:
    {
        qDebug()<<"SocketHandler::socketStateChanged: 没有连到服务器.";
        if(m_timerReconnect)
            if(!m_timerReconnect->isActive())
                m_timerReconnect->start(5000);
        break;
    }
    case QAbstractSocket::HostLookupState:
    {
        break;
    }
    case QAbstractSocket::ConnectingState:
    {
        if(m_timerReconnect)
            if(!m_timerReconnect->isActive())
                m_timerReconnect->start(5000);
        break;
    }
    case QAbstractSocket::ConnectedState:
    {
        qDebug()<<"SocketHandler::socketStateChanged: 连接到服务.";
        emit connectedStatus(true);
        break;
    }
    case QAbstractSocket::ClosingState:
    {
        qDebug()<<"SocketHandler::socketStateChanged: 从服务器断开.";
        emit connectedStatus(false);
        //m_isAuthenticated = false;

        if(m_timerReconnect)
            if(!m_timerReconnect->isActive())
                m_timerReconnect->start(5000);

        break;
    }
    default:
    {
        qDebug()<<"SocketHandler::其他状态.";
        break;
    }
    }
}
//ssl 相关，加密握手结束后
void SocketHandler::socketEncrypted()
{
    if (!m_sslsocket)
        return;                 // might have disconnected already

    QSslCipher ciph = m_sslsocket->sessionCipher();
    QString cipher = QString("%1,  %2 (%3/%4)").arg(ciph.authenticationMethod())
            .arg(ciph.name()).arg(ciph.usedBits()).arg(ciph.supportedBits());;
    //form->cipherLabel->setText(cipher);
    qDebug()<<cipher;

    if(m_ssl){
       reisterHost();
    }

}
//ssl 相关，
void SocketHandler::sslErrors(const QList<QSslError> &errors)
{
    foreach (const QSslError &error, errors)
        qDebug()<<(error.errorString());

    // did the socket state change?
    //    if (socket->state() != QAbstractSocket::ConnectedState)
    //        socketStateChanged(socket->state());
    m_sslsocket->ignoreSslErrors();
}


void SocketHandler::setRemoteHost(const QString &hostIP)
{
    m_remoteHost = hostIP;
}

void SocketHandler::setRemotePort(quint16 port)
{
    m_remotePort = port;
}
void SocketHandler::setSSL(bool ssl)
{
    m_ssl = ssl ;
}

void SocketHandler::setType(int type)
{
    m_type = type ;
}

//写入的时候，把 int 转化为网络序
QByteArray SocketHandler::intToBytes(int n) {
    QByteArray buf;
    buf.resize(4);
    buf[3] = static_cast<quint16> (n & 0xff);
    buf[2] = static_cast<quint16> (n >> 8 & 0xff);
    buf[1] = static_cast<quint16> (n >> 16 & 0xff);
    buf[0] = static_cast<quint16> (n >> 24 & 0xff);
    return buf;
}
//读取回来需要转换
int SocketHandler::bytesToInt(QByteArray b) {
    return (b[3] & 0xff) | (b[2] & 0xff) << 8 | (b[1] & 0xff) << 16| (b[0] & 0xff) << 24;
}


////压缩处理
//QByteArray NetWorkHandler::GzipCompress(QByteArray postBody)
//{
//    QByteArray outBuf;
//    z_stream c_stream;
//    //int err = 0;
//    //int windowBits = 15;
//    int GZIP_ENCODING = 16;
//    if (!postBody.isEmpty())
//    {
//        c_stream.zalloc = (alloc_func)0;
//        c_stream.zfree = (free_func)0;
//        c_stream.opaque = (voidpf)0;
//        c_stream.next_in = (Bytef *)postBody.data();
//        c_stream.avail_in = postBody.size();
//        if (deflateInit2(&c_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
//                         MAX_WBITS + GZIP_ENCODING, 8, Z_DEFAULT_STRATEGY) != Z_OK) return QByteArray();

//        for (;;) {
//            char destBuf[4096] = { 0 };
//            c_stream.next_out = (Bytef *)destBuf;
//            c_stream.avail_out = 4096;
//            int err = deflate(&c_stream, Z_FINISH);
//            outBuf.append(destBuf, 4096 - c_stream.avail_out);
//            if (err == Z_STREAM_END || err != Z_OK)
//            {
//                break;
//            }
//        }
//        //auto total = c_stream.total_out;
//        deflateEnd(&c_stream);
//        //total = c_stream.total_out;
//    }
//    return outBuf;
//}
////解压缩
//QByteArray NetWorkHandler::GZipUnCompress(QByteArray src)
//{
//    QByteArray outBuffer;
//    z_stream strm;
//    strm.zalloc = NULL;
//    strm.zfree = NULL;
//    strm.opaque = NULL;

//    strm.avail_in = src.size();
//    strm.next_in = (Bytef *)src.data();

//    int err = -1;
//    err = inflateInit2(&strm, MAX_WBITS + 16);
//    if (err == Z_OK) {
//        while (true)
//        {
//            char buffer[4096] = { 0 };
//            strm.avail_out = 4096;
//            strm.next_out = (Bytef *)buffer;
//            int code = inflate(&strm, Z_FINISH);
//            outBuffer.append(buffer, 4096 - strm.avail_out);
//            if (Z_STREAM_END == code )
//            {
//                break;
//            }
//        }
//    }
//    inflateEnd(&strm);
//    return outBuffer;
//}
