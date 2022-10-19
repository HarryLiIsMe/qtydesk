#ifndef PASSIVEHANDLER_H
#define PASSIVEHANDLER_H

#include "sockethandler.h"
#include "component/graberclass.h"
#include "component/inputsimulator.h"

#include <QObject>
#include <QSize>
class PassiveHandler : public SocketHandler
{
    Q_OBJECT
public:
    PassiveHandler(QObject *parent = Q_NULLPTR);
    GraberClass *m_graberClass;
    InputSimulator *m_inputSimulator;
signals:
   void showDeviceID(QString);
   void getDesktop();
   void receivedTileNum(int tileNum);

   void setMouseMove(quint16 posX, quint16 posY);
protected:
   void dealProto(int i,BigPack::Exchange resv_exc) override;

private:
   QString m_transferID ;
   QString m_tempPass;

public slots:
   void setTempPass(const QString &tempPass);
   QString getTempPass();

   void sendImageParameters(const QSize &imageSize, int rectWidth);
   void sendImageTile(quint16 posX, quint16 posY, const QByteArray &imageData, quint16 tileNum);
};

#endif // PASSIVEHANDLER_H
