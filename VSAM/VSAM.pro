TEMPLATE = app
TARGET = VSAM
QT += core
HEADERS += VSAM.h \
    CtrlRegion.h \
    Interval.h \
    mainwindow.h
SOURCES += main.cpp \
    mainwindow.cpp
FORMS += \ 
    mainwindow.ui
RESOURCES += 
QMAKE_CXXFLAGS += -std=c++0x
