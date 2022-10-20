#include "mainwindow.h"

#include "common/log4j.h"

#include <QApplication>
#include <QMessageBox>
int main(int argc, char *argv[])
{
    //protobuf启动
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    //使用日志
    Log4j::userLog(Log4j::CONSOLE);

    QApplication app(argc, argv);

    app.setQuitOnLastWindowClosed(false);
    MainWindow mw ;
    mw.show();
    int returnCode = app.exec();
    //protobuf关闭
    google::protobuf::ShutdownProtobufLibrary();
    return returnCode;
}

