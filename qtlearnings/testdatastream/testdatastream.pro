QT += core
QT -= gui

CONFIG += c++11

TARGET = qtconsole
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp

DEFINES += QT_DEPRECATED_WARNINGS
