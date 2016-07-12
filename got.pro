#-------------------------------------------------
#
# Project created by QtCreator 2016-06-29T18:40:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = got
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    solarcalc.cpp \
    gotcalc.cpp \
    howto.cpp \
    logfile.cpp \
    optionmenu.cpp

HEADERS  += mainwindow.h \
    solarcalc.h \
    gotcalc.h \
    howto.h \
    logfile.h \
    optionmenu.h

FORMS    += mainwindow.ui \
    howto.ui \
    optionmenu.ui
