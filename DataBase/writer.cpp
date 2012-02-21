/*
 * writer.cpp
 *
 *  Created on: 16.02.2012
 *      Author: night
 */

#include "config.h"
#include "writer.h"
#include <QtCore>
#include "Package.h"
#include "DataBase.h"
#include <stdexcept>

namespace db {

    Writer::Writer(QFile* file, int numberOfPackages, int hashDegree,
                   int* metaPackages, int basePos) :
            QObject(0), _packages(numberOfPackages) {
        if (file == NULL || metaPackages == NULL) {
            throw new std::runtime_error("NULL pointer in Writer::Writer");
        }

        _file = file;
        _metaPackages = metaPackages;
        _numberOfPackages = numberOfPackages;
        _basePos = basePos;
        _scale = static_cast<float>(numberOfPackages)
                / static_cast<float>(hashDegree);

        _file->setParent(this);
        if (!_file->isOpen()) {
            if (_file->open(QFile::ReadWrite)) {
                throw new std::runtime_error(
                        "Cann't open file in Writer::Writer");
            }
        }

        for (int i = 0; i < _packages.size(); ++i) {
            _packages[i] = new Package(_file, i, packNum / numberOfPackages,
                                       &metaPackages[i], this);
        }
        _recordWriten = 0;
    }

    Writer::~Writer() {
        for (int i = 0; i < _packages.size(); ++i) {
            delete _packages[i];
        }
#ifdef DEBUG
        qDebug() << "Writer destroyed";
#endif
    }

    void Writer::takeHash(int number, Record* record) {
#ifdef DEBUG
        qDebug() << "Writer emited";
#endif
        if ((number *= _scale) >= _numberOfPackages) {
            number = _numberOfPackages - 1;
        }
        _packages[number]->insertRecord(record);
        ++_recordWriten;
        if (_recordWriten >= _totalToWrite) {
            _recordWriten = 0;
            emit done();
        }
    }

} /* namespace db */
