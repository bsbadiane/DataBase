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
        _begin = number * _size + _parent->getBasePos();
        _base = reinterpret_cast<Record*>(_file->map(_begin, _size));
    }

    Package::~Package() {
    	_file->unmap((uchar*)_base);
    }

    void Package::insertRecord(Record *record, bool isNativePackage) {
        Q_UNUSED(isNativePackage);
        if (record == NULL) {
            throw new std::runtime_error(
                    "Null pointer in Package::insertRecord");
        }

        bool emptyRecordExist = *_filled < _capacity;
        if (emptyRecordExist) {
            /*_file->seek((quint64)(_begin + (*_filled) * sizeof(Record)));
            if (_file->write((char*)record, (quint64)sizeof(Record)) != sizeof(Record)) {
                throw new std::out_of_range("Cann't write to DB.");
            }*/
        	_base[*_filled] = *record;
            (*_filled)++;
            if ((*_filled) == INT_MAX) {
                throw new std::out_of_range("Package overflow.");
            }
        } else {
            _parent->insertToNextPackage(_number, record);
        }

    }

    Record* Package::searchRecord(char ID[10]) {
        Record* records = reinterpret_cast<Record*>(_file->map(_begin, _size));
        if (records == NULL) {
            throw new std::runtime_error(
                    "Cann't map file in Package::insertRecord");
        }

        bool last = *_filled < _capacity;

        for (int i = 0; i < _capacity; ++i) {
            if (strncmp(records[i].ID, ID, Record::ID_SIZE-1) == 0) {
                return new Record(records[i]);
            }
        }

        _file->unmap((uchar*)records);
        return last ? NULL : _parent->searchInNextPackage(_number, ID);
    }

} /* namespace db */
