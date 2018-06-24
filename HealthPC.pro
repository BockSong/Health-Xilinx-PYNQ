#-------------------------------------------------
#
# Project created by QtCreator 2018-06-04T22:43:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HealthPC
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serialsingleton.cpp

HEADERS  += mainwindow.h \
    serialsingleton.h

FORMS    += mainwindow.ui
