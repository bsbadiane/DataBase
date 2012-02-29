/*
 * Package.cpp
 *
 *  Created on: 16.02.2012
 *      Author: night
 */

#include "Package.h"
#include "writer.h"
#include "Profiler.h"
#include <stdexcept>
#include <cstddef>
#include <climits>

namespace db {

    Package::Package(Record* addr, int number, Writer *parent) {
        if (addr == NULL || parent == NULL) {
            throw new std::runtime_error("Null pointer in Package::Package");
        }

        _number = number;
        _parent = parent;
        _base = addr;
    }

    Package::~Package() {
    	//_file->unmap((uchar*)_base);
#ifdef MEM_DEBUG
    	qDebug() << "Package destroyed";
#endif
    }

    bool Package::insertRecord(Record *record, bool isNativePackage) {
        if (record == NULL) {
            throw new std::runtime_error(
                    "Null pointer in Package::insertRecord");
        }

        //bool emptyRecordExist = *_filled < _capacity;
        if (_parent->emptyRecordExist(_number)) {
            /*_file->seek((quint64)(_begin + (*_filled) * sizeof(Record)));
            if (_file->write((char*)record, (quint64)sizeof(Record)) != sizeof(Record)) {
                throw new std::out_of_range("Cann't write to DB.");
            }*/
        	_base[_parent->getCurPos(_number)/**_filled*/] = *record;
        	if(isNativePackage) Profiler::instance().insertedInNative();
        	_parent->incPos(_number);
        	return true;
            /*(*_filled)++;
            if ((*_filled) == INT_MAX) {
                throw new std::out_of_range("Package overflow.");
            }*/
        } else {
            return false;
        }

    }

    Record* Package::searchRecord(char ID[10]) {
    	Q_UNUSED(ID);
    	return NULL;
        /*Record* records = reinterpret_cast<Record*>(_file->map(_begin, _size));
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
        return last ? NULL : _parent->searchInNextPackage(_number, ID);*/
    }

} /* namespace db */
