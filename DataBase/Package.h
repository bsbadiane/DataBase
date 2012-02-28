/*
 * Package.h
 *
 *  Created on: 16.02.2012
 *      Author: night
 */

#ifndef PACKAGE_H_
#define PACKAGE_H_
#include "Record.h"
#include <QtCore>

namespace db {
    class Writer;

    class Package {
    public:
        Package(QFile* file, int number, int capacity, int* filled, Writer* parent);
        virtual ~Package();

        void insertRecord(Record* record, bool isNativePackage = true);
        Record* searchRecord(char ID[10]);

        //static void setPackagesSize(qint64 capacity);

    private:
        Record* _base;
        Writer* _parent;
        QFile*  _file;
        qint64  _begin;
        int     _number;
        int*    _filled;

        qint64 _size;
        int    _capacity;
    };

    /*inline void Package::setPackagesSize(qint64 capacity) {
        _size = capacity*sizeof(Record);
        _capacity = capacity;
    }*/

} /* namespace db */
#endif /* PACKAGE_H_ */
