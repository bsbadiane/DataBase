TEMPLATE = app
TARGET = DataBase
QT = core
HEADERS += Profiler.h \
    Singleton.h \
    Searcher.h \
    Hashers/CloserHasher.h \
    Hashers/Hasher.h \
    config.h \
    Package.h \
    writer.h \
    reader.h \
    Hashers/NumberSystemHasher.h \
    Record.h \
    DataBase.h
SOURCES += Profiler.cpp \
    Searcher.cpp \
    Hashers/CloserHasher.cpp \
    Package.cpp \
    writer.cpp \
    reader.cpp \
    Hashers/NumberSystemHasher.cpp \
    DataBase.cpp \
    main.cpp
FORMS += 
RESOURCES += 
