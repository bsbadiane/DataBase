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

    bool Package::searchRecord(char ID[10], Record*& record) {
    	record = NULL;


        bool last = _parent->emptyRecordExist(_number);
        int pos = _parent->getCurPos(_number);

        for (int i = 0; i < pos; ++i) {
            /*if (strncmp(_base[i].ID, ID, Record::ID_SIZE-1) == 0) {
                record = new Record(_base[i]);
                return true;
            }*/
        	bool eq = true;
        	for (int j = 0; j < Record::ID_SIZE-1; ++j) {
        		if (_base[i].ID[j] != ID[j]) {
        			eq = false;
        			break;
        		}
			}
        	if (eq) {
        		record = new Record(_base[i]);
        		return true;
        	}
        }

        return last;
    }

} /* namespace db */
