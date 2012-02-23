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

    class DataBase: public QObject {
    Q_OBJECT
    public:
        DataBase(QString src, QString db, int numberOfPackages, QObject* parent = 0);
        virtual ~DataBase();

        void buildDB();

        //int getBasePos();

    private:
        qint64 _basePos;
        QSharedPointer<Reader>  _reader;
        QSharedPointer<Writer>  _writer;
        QSharedPointer<NumberSystemHasher>    _hasher;

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
