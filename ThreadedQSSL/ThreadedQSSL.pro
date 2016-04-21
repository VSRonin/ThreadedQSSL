QT += core network
QT -= gui

CONFIG += c++11

TARGET = ThreadedQSSL
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    LanClient.cpp \
    LanServer.cpp \
    LanServerSocket.cpp \
    LanServerThread.cpp

HEADERS += \
    Globals.h \
    LanClient.h \
    LanServer.h \
    LanServerSocket.h \
    LanServerThread.h

