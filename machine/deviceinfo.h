#ifndef HOSTINFO_H
#define HOSTINFO_H

#include <QObject>

class DeviceInfo : public QObject
{
    Q_OBJECT
public:
    explicit DeviceInfo(QObject *parent = nullptr);
public:
    static QByteArray getHashSum(const QString &cpuID, const QString &diskDriveID);
    //获取主机名称
    static QString getPcName();
    //获取本机IP
    static QString getIP();
    //获取本机MAC地址
    static QString getHostMacAddress();
    //获取硬盘
    static QString getDiskdrive();
    //获取CPU ID
    static QString getAllPlatformCpuID();

};

#endif // HOSTINFO_H
