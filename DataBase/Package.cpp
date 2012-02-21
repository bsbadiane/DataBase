/*
 * Package.cpp
 *
 *  Created on: 16.02.2012
 *      Author: night
 */

#include "Package.h"
#include "writer.h"
#include <stdexcept>
#include <cstddef>
#include <climits>

namespace db {

    Package::Package(QFile *file, int number, int capacity, int* filled,
                     Writer *parent) {
        if (file == NULL || parent == NULL || filled == NULL) {
            throw new std::runtime_error("Null pointer in Package::Package");
        }

        _file = file;
        if (!_file->isOpen()) {
            if (_file->open(QFile::ReadWrite)) {
                throw new std::runtime_error(
                        "Cann't open file in Package::Package");
            }
        }
        _number = number;
        _parent = parent;
        _filled = filled;
        _capacity = capacity;
        _size = capacity * sizeof(Record);
        _begin = number * _size;
    }

    Package::~Package() {
    }

    void Package::insertRecord(Record *record, bool isNativePackage) {
        Q_UNUSED(isNativePackage);
        if (record == NULL) {
            throw new std::runtime_error(
                    "Null pointer in Package::insertRecord");
        }

        bool emptyRecordExist = *_filled < _capacity;
        if (emptyRecordExist) {
            _file->seek(
                    _parent->getBasePos() + _begin
                            + (*_filled) * sizeof(Record));
            if (_file->write((char*)record, sizeof(Record)) != sizeof(Record)) {
                throw new std::out_of_range("Cann't write to DB.");
            }
            (*_filled)++;
            if ((*_filled) == INT_MAX) {
                throw new std::out_of_range("Package overflow.");
            }
        } else {
            _parent->searchNextPackage(_number, record);
        }

    }

} /* namespace db */
