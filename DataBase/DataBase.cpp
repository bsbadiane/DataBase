/*
 * DataBase.cpp
 *
 *  Created on: 15.02.2012
 *      Author: night
 */

#include "config.h"
#include "reader.h"
#include "writer.h"
#include "Hashers/NumberSystemHasher.h"
#include "DataBase.h"
#include "Hashers/CloserHasher.h"
#include <stdexcept>

namespace db {

    DataBase::DataBase(QString db, int numberOfPackages, Hasher::Constructor hashBuilder,
                       QObject* parent) :
            QObject(parent) {

        QFile* file = new QFile(db);
        if (!file->open(QFile::ReadWrite)) {
            throw new std::runtime_error("Cann't open db.");
        }
        int checkNumber = 0;
        if (file->read((char*)&checkNumber, (qint64)sizeof(checkNumber))
                != sizeof(checkNumber)) {
            prepareDB(file, numberOfPackages);
            checkNumber = numberOfPackages;
        } else {
            _basePos = sizeof(int) * (numberOfPackages + 1);
        }
        if (checkNumber != numberOfPackages) {
            throw new std::invalid_argument("Invalid number of packages");
        }

        int* metaPackages = reinterpret_cast<int*>(file->map(
        		(qint64)sizeof(int), (qint64)sizeof(int) * checkNumber));
        if (metaPackages == NULL) {
            throw new std::runtime_error("Cann't map DB.");
        }
#ifdef DEBUG
        for (int i = 0; i < checkNumber; ++i) {
            qDebug() << metaPackages[i];
        }
#endif
        int hash = 1;
        int deg = 0;
        while (numberOfPackages > hash) {
            hash *= 10;
            ++deg;
        }
        _writer = QSharedPointer<Writer>(
                new Writer(file, checkNumber, hash, metaPackages, _basePos));
        _hasher = QSharedPointer<Hasher>(hashBuilder(hash, deg));
    }

    DataBase::~DataBase() {
#ifdef DEBUG
        qDebug() << "Destroying DB";
#endif
    }

    void DataBase::buildDB(QString src) {
        QScopedPointer<Reader> reader(new Reader(src, _writer, _hasher));
        reader->readRecords();
    }

    void DataBase::prepareDB(QFile* file, int numberOfPackages) {
        _basePos = sizeof(int) * (numberOfPackages + 1);
        file->resize((qint64)(_basePos + packNum * sizeof(Record)));
        int* metaTable = reinterpret_cast<int*>(
        		file->map(0, (qint64)(numberOfPackages*sizeof(Record))+1) );
        if (metaTable == NULL) {
        	throw new std::runtime_error(
        	                    "Can't write to DB in DataBase::prepareDB");
        }
        metaTable[0] = numberOfPackages;
        for (int i = 1; i <= numberOfPackages; ++i) {
			metaTable[i] = 0;
		}
        file->unmap((uchar*)metaTable);

        /*if (file->write((char*)&numberOfPackages, (qint64)sizeof(numberOfPackages))
                != sizeof(numberOfPackages)) {
            throw new std::runtime_error(
                    "Can't write to DB in DataBase::prepareDB");
        }
        int zero = 0;
        for (int i = 0; i < numberOfPackages; ++i) {
            if (file->write((char*)&zero, (qint64)sizeof(zero)) != sizeof(zero)) {
                throw new std::runtime_error(
                        "Can't write to DB in DataBase::prepareDB");
            }
        }*/

        file->flush();
    }

} /* namespace db */
