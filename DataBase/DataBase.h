/*
 * DataBase.h
 *
 *  Created on: 15.02.2012
 *      Author: night
 */

#ifndef DATABASE_H_
#define DATABASE_H_

#include "config.h"
#include "Hashers/Hasher.h"
#include "Searcher.h"
#include <qobject.h>
#include <qfile.h>
#include <QtCore>

namespace db {
    class Reader;
    class Writer;
    class Record;

    class DataBase: public QObject {
    Q_OBJECT
    public:
        DataBase(QString db, int dbSize, int numberOfPackages, Hasher::Constructor hashBuilder,
        		QObject* parent = 0);
        virtual ~DataBase();

        void buildDB(QString src, int packToRead);
        Record* searchByID(char ID[10]);

        //int getBasePos();

    private:
        qint64  _basePos;
        int		_dbSize;
        QSharedPointer<Writer>  _writer;
        QSharedPointer<Hasher>    _hasher;
        QSharedPointer<Searcher>	_searcher;

        void prepareDB(QFile* file, int numberOfPackages);

    signals:
        void build();

    };

    /*inline
    int DataBase::getBasePos() {
        return _basePos;
    }*/

    inline
    Record* DataBase::searchByID(char ID[10]) {
    	return _searcher->search(ID);
    }

} /* namespace db */
#endif /* DATABASE_H_ */
