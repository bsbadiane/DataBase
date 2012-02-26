/*
 * reader.h
 *
 *  Created on: 16.02.2012
 *      Author: night
 */

#ifndef READER_H_
#define READER_H_

#include "Record.h"
#include <qthread.h>
#include <qfile.h>
#include <QtCore>

namespace db {
    class Record;
    class DataBase;
    class Writer;
    class Hasher;
    class NumberSystemHasher;

    class Reader{//: public QObject {
    //Q_OBJECT
    public:
        Reader(QString fileName, QSharedPointer<Writer> writer,
               QSharedPointer<Hasher> hasher);
        virtual ~Reader();

        static const int _windowCapacity = 1000000;
        static const qint64 _windowSize = _windowCapacity * sizeof(Record);

        void readRecords();

    private:
        QFile* _file;
        qint64 _pos;
        qint64 _fileSize;
        Record* _recordArray;
        qint64 _frameBorder;
        QSharedPointer<Writer> _writer;
        QSharedPointer<Hasher> _hasher;

    };

} /* namespace db */
#endif /* READER_H_ */
