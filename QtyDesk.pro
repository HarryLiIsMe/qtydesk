#-------------------------------------------------
#
# Project created by QtCreator 2022-10-10T11:15:52
#
#-------------------------------------------------

QT += core gui widgets network
CONFIG += c++11
TARGET = QtyDesk
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES  -= UNICODE
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    activewindow.cpp \
    component/graberclass.cpp \
    component/inputsimulator.cpp \
        main.cpp \
        mainwindow.cpp \
    common/log4j.cpp \
    net/activehandler.cpp \
    net/passivehandler.cpp \
    net/sockethandler.cpp \
    machine/deviceinfo.cpp \
    proto/Exchange.pb.cc \
    ui/button/btnpasssetting.cpp \
    ui/button/btnshowpasswd.cpp \
    ui/dialog/dlgchangepasswd.cpp \
    ui/dialog/dlgconnectmessage.cpp \
    ui/dialog/movedialog.cpp

HEADERS += \
    activewindow.h \
    component/graberclass.h \
    component/inputsimulator.h \
        mainwindow.h \
    common/log4j.h \
    net/activehandler.h \
    net/passivehandler.h \
    net/sockethandler.h \
    machine/deviceinfo.h \
    proto/Exchange.pb.h \
    ui/button/btnpasssetting.h \
    ui/button/btnshowpasswd.h \
    ui/dialog/dlgchangepasswd.h \
    ui/dialog/dlgconnectmessage.h \
    ui/dialog/movedialog.h

FORMS += \
        activewindow.ui \
        mainwindow.ui \
        ui/dialog/dlgchangepasswd.ui \
        ui/dialog/dlgconnectmessage.ui

RESOURCES += \
    resources/skin.qrc

# protobuf version: 3.16.0
win32: LIBS += -LD:/alib/protobuf/lib -llibprotobuf.dll
INCLUDEPATH += D:/alib/protobuf/include
DEPENDPATH += D:/alib/protobuf/include

# libyuv version: only
#win32: LIBS += -LD:/alib/libyuv/lib -llibyuv.dll
#INCLUDEPATH += D:/alib/libyuv/include
#DEPENDPATH += D:/alib/libyuv/include
## libvpx version: libvpx-1.8.0.zip
#win32: LIBS += -LD:/alib/libvpx/lib -llibvpx
#INCLUDEPATH += D:/alib/libvpx/include
#DEPENDPATH += D:/alib/libvpx/include

#win32: LIBS += -LC:\Qt\Qt5.9.9\Tools\mingw530_32\i686-w64-mingw32\lib -luser32
#win32: LIBS += -LC:\Qt\Qt5.9.9\Tools\mingw530_32\i686-w64-mingw32\lib -llibgdi32
#win32: LIBS += -LC:\Qt\Qt5.9.9\Tools\mingw530_32\i686-w64-mingw32\lib -lole32
