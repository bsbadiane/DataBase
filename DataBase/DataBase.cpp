/*
 * DataBase.cpp
 *
 *  Created on: 15.02.2012
 *      Author: night
 */

#include "reader.h"
#include "writer.h"
#include "Hashers/NumberSystemHasher.h"
#include "DataBase.h"
#include "config.h"
#include <stdexcept>

namespace db {

    DataBase::DataBase(QString src, QString db, int numberOfPackages,
                       QObject* parent) :
            QObject(parent) {

        QFile* file = new QFile(db);
        if (!file->open(QFile::ReadWrite)) {
            throw new std::runtime_error("Cann't open db.");
        }
        int checkNumber = 0;
        if (file->read((char*)&checkNumber, sizeof(checkNumber))
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
                sizeof(int), sizeof(int) * checkNumber));
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
        _writer = new Writer(file, checkNumber, hash, metaPackages, _basePos);
        _reader = new Reader(src, _writer);

        for (int i = 0; i < threadCount; ++i) {
            _hashers[i] = new NumberSystemHasher(i, hash, deg);
            connect(_reader, SIGNAL(findHash(unsigned, Record*, int)),
                    _hashers[i], SLOT(getHash(unsigned, Record*, int)));
            connect(_hashers[i], SIGNAL(giveHash(int, Record*)), _writer,
                    SLOT(takeHash(int, Record*)));
            /*QThread* thread = new QThread(this);
             thread->start();
             hasher->moveToThread(thread);*/
            //hasher->start();
        }
        connect(_writer, SIGNAL(done()), _reader, SLOT(readRecords()));
        connect(this, SIGNAL(build()), _reader, SLOT(readRecords()));
    }

    DataBase::~DataBase() {
#ifdef DEBUG
        qDebug() << "Destroying DB";
#endif
        delete _reader;
        delete _writer;
        for (int i = 0; i < threadCount; ++i) {
            delete _hashers[i];
        }
    }

    void DataBase::buildDB() {
        QThread* reader = new QThread(this);
        QThread* writer = new QThread(this);
        for (int i = 0; i < threadCount; ++i) {
            QThread* hasher = new QThread(this);
            _hashers[i]->moveToThread(hasher);
            hasher->start();
        }
        reader->start();
        writer->start();
        _reader->moveToThread(reader);
        _writer->moveToThread(writer);
        emit build();
        reader->wait();
        writer->quit();
        writer->wait();
        for (int i = 0; i < threadCount; ++i) {
            _hashers[i]->thread()->quit();
        }for (int i = 0; i < threadCount; ++i) {
            _hashers[i]->thread()->wait();
        }
        //_writer->start();
        //_reader->start();
        //_reader->wait();
        //_writer->quit();
    }

    void DataBase::prepareDB(QFile* file, int numberOfPackages) {
        if (file->write((char*)&numberOfPackages, sizeof(numberOfPackages))
                != sizeof(numberOfPackages)) {
            throw new std::runtime_error(
                    "Can't write to DB in DataBase::prepareDB");
        }
        int zero = 0;
        for (int i = 0; i < numberOfPackages; ++i) {
            if (file->write((char*)&zero, sizeof(zero)) != sizeof(zero)) {
                throw new std::runtime_error(
                        "Can't write to DB in DataBase::prepareDB");
            }
        }
        _basePos = sizeof(int) * (numberOfPackages + 1);
        file->resize(_basePos + 1200000 * sizeof(Record));
        file->flush();
    }

} /* namespace db */
