QT += core network
QT -= gui

CONFIG += c++11

TARGET = qtconsole
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp \
    server.cpp

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
    server.h
