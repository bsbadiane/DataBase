/*
 * DataBase.h
 *
 *  Created on: 15.02.2012
 *      Author: night
 */

#ifndef DATABASE_H_
#define DATABASE_H_

#include "config.h"
#include <qobject.h>
#include <qfile.h>
#include <QtCore>

namespace db {
    class Reader;
    class Writer;
    class NumberSystemHasher;
    class Hasher;
    class Record;

    class DataBase: public QObject {
    Q_OBJECT
    public:
        DataBase(QString db, int numberOfPackages, QObject* parent = 0);
        virtual ~DataBase();

        void buildDB(QString src);
        Record* searchByID(char ID[10]);

        //int getBasePos();

    private:
        qint64 _basePos;
        QSharedPointer<Writer>  _writer;
        QSharedPointer<Hasher>    _hasher;

        void prepareDB(QFile* file, int numberOfPackages);

    signals:
        void build();

    };

    /*inline
    int DataBase::getBasePos() {
        return _basePos;
    }*/

} /* namespace db */
#endif /* DATABASE_H_ */
