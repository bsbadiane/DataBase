TEMPLATE = app
TARGET = DataBase
QT = core
HEADERS += Record.h \
    VSAM.h \
    CtrlRegion.h \
    Interval.h
SOURCES += VSAM.cpp
FORMS += 
RESOURCES += 
QMAKE_CXXFLAGS += -std=c++0x
