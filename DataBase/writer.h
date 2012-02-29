/*
 * writer.h
 *
 *  Created on: 16.02.2012
 *      Author: night
 */

#ifndef WRITER_H_
#define WRITER_H_

#include "config.h"
#include "Record.h"
#include "Package.h"
#include <qthread.h>
#include <qfile.h>

namespace db {
    class DataBase;

    class Writer: public QObject {
    Q_OBJECT
    friend class Package;
    public:
        Writer(QFile* file, int numberOfPackages, int hashDegree, int basePos);
        virtual ~Writer();

        int getBasePos() const;
        //void setTaskSize(int size) const;

    public slots:
        void insertPackage(int number, Record* record) const;
        Record* searchPackage(int number, char ID[10]) const;

    signals:
        void done();

    private:
        int _numberOfPackages;
        int _basePos;
        QFile* _file;
        float _scale;
        QVector<Package*> _packages;
        int* _metaPackages;

        void insertToNextPackage(int currentNumber, Record* record);
        Record* searchInNextPackage(int currentNumber, char ID[10]);
    };

    //////////////////////////////////

    inline
    int Writer::getBasePos() const {
        return _basePos;
    }

    inline
    void Writer::insertToNextPackage(int currentNumber, Record* record) {
        if (currentNumber + 1 >= _numberOfPackages) {
            currentNumber = -1;
        }
        _packages[currentNumber + 1]->insertRecord(record, false);
    }

    inline Record *Writer::searchInNextPackage(int currentNumber, char ID[10]) {
        if (currentNumber + 1 >= _numberOfPackages) {
            currentNumber = -1;
        }
        return _packages[currentNumber + 1]->searchRecord(ID);
    }

    inline
    void Writer::insertPackage(int number, Record *record) const {
        if ((number *= _scale) >= _numberOfPackages) {
            number = _numberOfPackages - 1;
        }
        _packages[number]->insertRecord(record);
    }

    inline Record* Writer::searchPackage(int number, char ID[10]) const {
        if ((number *= _scale) >= _numberOfPackages) {
            number = _numberOfPackages - 1;
        }
        return _packages[number]->searchRecord(ID);
    }
} /* namespace db */
#endif /* WRITER_H_ */
