#-------------------------------------------------
#
# Project created by QtCreator 2013-03-10T09:24:31
#
#-------------------------------------------------

QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FractalDraw
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    line.cpp \
    polygon.cpp

HEADERS  += mainwindow.h \
    line.h \
    polygon.h

FORMS    += mainwindow.ui
