TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += qt
QT += widgets
QT += core gui widgets
QT -= gui declarative


SOURCES += main.cpp \
    mainwindow.cpp \
    life.cpp \
    superloop.cpp \
    parser.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    mainwindow.h \
    life.h \
    superloop.h \
    parser.h

