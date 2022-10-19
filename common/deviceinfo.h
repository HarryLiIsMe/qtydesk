#ifndef HOSTINFO_H
#define HOSTINFO_H

#include <QObject>

class DeviceInfo : public QObject
{
    Q_OBJECT
public:
    explicit DeviceInfo(QObject *parent = nullptr);
public:
    static DeviceInfo &GetInstance()
    {
        static DeviceInfo instance;
        return instance;
    };
    void setRemoteInfo(const QString &host,quint16 port);
    void setPassword(const QString &pass);
    void setRemoteDeviceID(QString deviceID);
    QString getTempPass() ;
    QString getRemoteHost() ;
    quint16 getRemotePort() ;
    QString getRemoteDeviceID();
    //@brief:获取主机名称
    QString getPcName();
    //*@brief:获取本机IP
    QString getIP();
    //@brief:获取本机MAC地址
    QString getHostMacAddress();
    //brief:获取硬盘
    QString getDiskdrive();

    QString getAllPlatformCpuID();
    void xorData(QByteArray &data);
    QByteArray getHashSum(const QString &pcName, const QString &mac);
private:
    QString m_remoteHost ;
    quint16 m_remotePort ;
    QString m_tempPass ;
    QString m_remoteDeviceID ;

    void getcpuid(unsigned int CPUInfo[], unsigned int InfoType);
};

#endif // HOSTINFO_H
