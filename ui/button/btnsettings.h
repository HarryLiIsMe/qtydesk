#ifndef BTNSETTINGS_H
#define BTNSETTINGS_H

#include <QMenu>
#include <QPushButton>
class BtnSettings : public QPushButton
{
    Q_OBJECT
public:
    explicit BtnSettings(QWidget *parent = nullptr);
    ~BtnSettings();

signals:
    void aboutQtyDesk();
    void showParameterDlg();
private slots:
    void actionTriggered(QAction *action);
private:
     QMenu *myMenu;
};

#endif // BTNSETTINGS_H
