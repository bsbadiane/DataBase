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
        Package(Record* addr, int number, Writer* parent);
        virtual ~Package();

        bool insertRecord(Record* record, bool isNativePackage = true);
        bool searchRecord(char ID[10], Record*& record);

        //static void setPackagesSize(qint64 capacity);

    private:
        Record* _base;
        Writer* _parent;
        //QFile*  _file;
        //qint64  _begin;
        int     _number;
        //int*    _filled;

        //qint64 _size;
        //int    _capacity;
    };

    /*inline void Package::setPackagesSize(qint64 capacity) {
        _size = capacity*sizeof(Record);
        _capacity = capacity;
    }*/

} /* namespace db */
#endif /* PACKAGE_H_ */
