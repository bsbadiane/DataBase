/*
 * reader.cpp
 *
 *  Created on: 16.02.2012
 *      Author: night
 */

#include "reader.h"
#include "config.h"
#include "DataBase.h"
#include "writer.h"
#include "Hashers/NumberSystemHasher.h"
#include <qfile.h>
#include <stdexcept>
#ifdef DEBUG
#include <qdebug.h>
#endif

namespace db {

    Reader::Reader(QString fileName, Writer* writerCallBack, NumberSystemHasher* hasher) :
            QObject(0), _writer(writerCallBack), _hasher(hasher) {
        //_writer = writerCallBack;
        _file = new QFile(fileName, this);
        _pos = 0;
        _fileSize = _file->size();
        _recordArray = NULL;
        _frameBorder = 0;
        if (!_file->open(QFile::ReadOnly)) {
            throw new std::runtime_error("Cann't open file in Reader::Reader");
        }
    }
    Reader::~Reader() {
#ifdef DEBUG
        qDebug() << "Reader destroyed";
#endif
    }

    void Reader::readRecords() {
      while(true) {
        _file->unmap((uchar*)_recordArray);

        if (_frameBorder < _fileSize) {
            _pos = _frameBorder;
        } else {
            break;
        }

        int cap;
        if (_fileSize >= (_pos + _windowSize)) {
            _frameBorder = _pos + _windowSize;
            cap = _windowCapacity;
        } else {
            _frameBorder = _fileSize;
            cap = (_fileSize - _pos) / sizeof(Record);
        }
        //_writer->setTaskSize(cap);//TODO delete

        _recordArray =
                reinterpret_cast<Record*>(_file->map(_pos, _frameBorder - _pos));
        if (_recordArray == NULL) {
            throw new std::runtime_error("Cann't map a file.");
        }

        for (int i = 0; i < cap; ++i) {
            unsigned number = getNumber(_recordArray[i].ID);
            //emit findHash(number, &_recordArray[i], hasherIndex);
            _hasher->getHash(number, &_recordArray[i]);
        }
      }
    }

    unsigned Reader::getNumber(char string[10]) {
        unsigned sum = 0;
        for (int i = 0; i < 9; i += 2) {
            sum += string[i];
        }
        for (int i = 1; i < 9; i += 2) {
            unsigned short val = ~static_cast<unsigned short>(string[i]);
            sum += val;
        }
        return sum;
    }

    /*void Reader::run() {
        readRecords();
        exec();
    }*/

} /* namespace db */
