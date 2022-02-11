QT       += core gui xml sql
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4) {
        QT += widgets
}else {
        QMAKE_CXXFLAGS += -std=c++0x
        #INCLUDEPATH += /opt/poky/1.7/sysroots/cortexa9hf-vfp-neon-poky-linux-gnueabi/usr/include/c++/4.9.1
        INCLUDEPATH += /home/test/armc++4.9.1
        DEFINES += USE_JSON_SRC
}

include ($$PWD/Util/Util.pri)
include ($$PWD/UiCommon/UiCommon.pri)

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cbakeuprestorewid.cpp \
    cdeviceadrtblwid.cpp \
    cdeviceconfig.cpp \
    cdeviceiconwidget.cpp \
    cdevicemodpreview.cpp \
    cdevicepreview.cpp \
    cdevpointedit.cpp \
    cdevposctl2.cpp \
    cdevposmgr.cpp \
    chelpwid.cpp \
    ckeycheckbox.cpp \
    ckeycombobox.cpp \
    clineselector.cpp \
    cmodparampreview.cpp \
    cmodparamquery.cpp \
    cparamtablemngrwid.cpp \
    csingledevadrtblwid.cpp \
    cuploadquerywid.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    cbakeuprestorewid.h \
    cdeviceadrtblwid.h \
    cdeviceconfig.h \
    cdeviceiconwidget.h \
    cdevicemodpreview.h \
    cdevicepreview.h \
    cdevpointedit.h \
    cdevposctl2.h \
    cdevposmgr.h \
    chelpwid.h \
    ckeycheckbox.h \
    ckeycombobox.h \
    clineselector.h \
    cmodparampreview.h \
    cmodparamquery.h \
    cparamtablemngrwid.h \
    csingledevadrtblwid.h \
    cuploadquerywid.h \
    mainwindow.h

FORMS += \
    cbakeuprestorewid.ui \
    cdeviceadrtblwid.ui \
    cdeviceconfig.ui \
    cdeviceiconwidget.ui \
    cdevicemodpreview.ui \
    cdevicepreview.ui \
    cdevposctl2.ui \
    chelpwid.ui \
    cmodparampreview.ui \
    cmodparamquery.ui \
    cparamtablemngrwid.ui \
    csingledevadrtblwid.ui \
    cuploadquerywid.ui \
    mainwindow.ui

linux-g++{
          CONFIG(debug, debug|release) {
               QBUILD = $$PWD/../bin/debug/x86
          }
          CONFIG(release, debug|release) {
                QBUILD =$$PWD/../bin/release/x86
               DEFINES += QT_NO_DEBUG_OUTPUT
           }
           #库使用
           DEFINES += I386
           DEFINES+=SUPPORT_SYSCOMM
           #应用程序使用
           DEFINES += PC_PLATFORM
           #DEFINES+=FTPCONNECTION
}

linux-gnueabi-oe-g++{
           CONFIG(debug, debug|release) {
               QBUILD =  $$PWD/../bin/debug/arm
           }
           CONFIG(release, debug|release) {
                QBUILD = $$PWD/../bin/release/arm
                DEFINES += QT_NO_DEBUG_OUTPUT
            }
           #库使用
           DEFINES += ARM_PLATFORM
          # DEFINES += TASHIZHUJI
           DEFINES += ARM
           DEFINES+=SUPPORT_SYSCOMM
           #应用程序使用
           #DEFINES+=FTPCONNECTION
           message(linux-arm-g++)
}
#DESTDIR = $$PWD/../bin
DESTDIR = $$QBUILD
message(The project $$QBUILD)
message(qt version $$QT_VERSION)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

