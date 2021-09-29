QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cwidgetbutton.cpp \
    main.cpp \
    mainwindow.cpp \
    pagecontainer.cpp \
    pageposuiji.cpp \
    splitertest.cpp

HEADERS += \
    cwidgetbutton.h \
    mainwindow.h \
    pagecontainer.h \
    pageposuiji.h \
    splitertest.h

FORMS += \
    mainwindow.ui \
    pagecontainer.ui \
    pageposuiji.ui \
    splitertest.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
