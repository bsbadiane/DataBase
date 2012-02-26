/*
 * reader.cpp
 *
 *  Created on: 16.02.2012
 *      Author: night
 */

#include "config.h"
#include "reader.h"
#include "DataBase.h"
#include "writer.h"
#include "Hashers/Hasher.h"
#include "Hashers/NumberSystemHasher.h"
#include <qfile.h>
#include <stdexcept>
#ifdef DEBUG
#include <qdebug.h>
#endif

namespace db {

    Reader::Reader(QString fileName, QSharedPointer<Writer> writerCallBack,
                   QSharedPointer<Hasher> hasher) :
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
        while (true) {
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

            _recordArray = reinterpret_cast<Record*>(_file->map(
                    _pos, _frameBorder - _pos));
            if (_recordArray == NULL) {
                throw new std::runtime_error("Cann't map a file.");
            }

            for (int i = 0; i < cap; ++i) {
                quint64 number = getNumber(_recordArray[i].ID);
#ifdef DEBUG2
                qDebug() << number;
#endif
                //emit findHash(number, &_recordArray[i], hasherIndex);
                int hash = _hasher->getHash(number);
                _writer->insertPackage(hash, &_recordArray[i]);
            }
        }
    }

     quint64 Reader::getNumber(char string[10]) {
        quint64 sum = 0;
        /*for (int i = 0; i < 9; i += 2) {
            sum += string[i];
        }
        for (int i = 1; i < 9; i += 2) {
            quint64 val = ~static_cast<quint64>(string[i]) & ~0xf0000000;
            sum += val;
        }*/
        int shIft = 0;
        for (int i = 0; i < 9; ++i) {
            sum += static_cast<quint64>(string[i]) << 6*i;//FIXME
            shIft += _shiftBase;
        }
        return sum;
    }

/*void Reader::run() {
 readRecords();
 exec();
 }*/

} /* namespace db */
