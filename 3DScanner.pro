
# Project created by QtCreator 2015-03-01T16:56:29
#
#-------------------------------------------------

QT       += core gui widgets multimedia

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TrueVision
TEMPLATE = app


SOURCES += main.cpp\
        mainWindow.cpp \
    scanner.cpp

HEADERS  += mainWindow.h \
    scanner.hpp

FORMS    += \
    mainWindow.ui


INCLUDEPATH += /usr/include/opencv4
LIBS += `pkg-config --cflags --libs opencv4`
