#-------------------------------------------------
#
# Project created by QtCreator 2014-09-13T15:03:57
#
#-------------------------------------------------

QT       += core gui
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Conway_s_Game_of_Life
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    world.cpp \
    logical.cpp

HEADERS  += mainwindow.h \
    world.h \
    logical.h
