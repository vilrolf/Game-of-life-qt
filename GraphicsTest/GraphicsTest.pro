TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += qt
CONFIG += c++11
QT += widgets
QT += core gui widgets
QT -= gui declarative


SOURCES += main.cpp \
    mainwindow.cpp \
    life.cpp \
    superloop.cpp \
    test.cpp \
    parser.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    mainwindow.h \
    life.h \
    superloop.h \
    test.h \
    parser.h

