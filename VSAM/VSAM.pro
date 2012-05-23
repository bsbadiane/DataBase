#-------------------------------------------------
#
# Project created by QtCreator 2012-05-23T03:00:21
#
#-------------------------------------------------



QT       += core gui

TARGET = VSAM
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    CtrlRegion.h \
    Interval.h \
    VSAM.h

FORMS    += mainwindow.ui
LIBS +=  /storage/edu/uni/DB/DataBase/DataBase/libDataBase.a
QMAKE_CXXFLAGS += -std=c++0x
