include ($$PWD/Util/Util.pri)
include ($$PWD/UiCommon/UiCommon.pri)

QT       += core gui xml sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cdeviceconfig.cpp \
    cdeviceiconwidget.cpp \
    cdevicemodpreview.cpp \
    cdevicepreview.cpp \
    cdevpointedit.cpp \
    cdevposctl2.cpp \
    cdevposmgr.cpp \
    ckeycheckbox.cpp \
    ckeycombobox.cpp \
    clineselector.cpp \
    cmodparampreview.cpp \
    cmodparamquery.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    cdeviceconfig.h \
    cdeviceiconwidget.h \
    cdevicemodpreview.h \
    cdevicepreview.h \
    cdevpointedit.h \
    cdevposctl2.h \
    cdevposmgr.h \
    ckeycheckbox.h \
    ckeycombobox.h \
    clineselector.h \
    cmodparampreview.h \
    cmodparamquery.h \
    mainwindow.h

FORMS += \
    cdeviceconfig.ui \
    cdeviceiconwidget.ui \
    cdevicemodpreview.ui \
    cdevicepreview.ui \
    cdevposctl2.ui \
    cmodparampreview.ui \
    cmodparamquery.ui \
    mainwindow.ui

DESTDIR = $$PWD/../bin

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

