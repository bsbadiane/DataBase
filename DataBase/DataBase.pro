TEMPLATE = app
TARGET = DataBase
QT += core
HEADERS += Hashers/Hasher.h \
    config.h \
    Package.h \
    writer.h \
    reader.h \
    Hashers/NumberSystemHasher.h \
    Record.h \
    DataBase.h
SOURCES += Package.cpp \
    writer.cpp \
    reader.cpp \
    Hashers/NumberSystemHasher.cpp \
    DataBase.cpp \
    main.cpp
FORMS += 
RESOURCES += 
