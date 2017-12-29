#-------------------------------------------------
#
# Project created by QtCreator 2017-12-10T14:52:54
#
#-------------------------------------------------

QT       += core gui multimedia
QT += multimediawidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Music
TEMPLATE = app


SOURCES += main.cpp\
        musicplayer.cpp \
    msgbox.cpp

HEADERS  += musicplayer.h \
    msgbox.h

RESOURCES += \
    icon.qrc
