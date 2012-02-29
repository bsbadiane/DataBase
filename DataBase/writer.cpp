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

    Writer::Writer(QFile* file, int numberOfPackages, int hashDegree, int basePos) :
            QObject(0), _packages(numberOfPackages) {
        if (file == NULL) {
            throw new std::runtime_error("NULL pointer in Writer::Writer");
        }

        _file = file;
        //_metaPackages = metaPackages;
        _metaPackages = reinterpret_cast<int*>(_file->map(
        		(qint64)sizeof(int), (qint64)sizeof(int) * numberOfPackages));
        if (_metaPackages == NULL) {
            throw new std::runtime_error("Cann't map DB.");
        }
        _numberOfPackages = numberOfPackages;
        //_basePos = basePos;
        _scale = static_cast<float>(numberOfPackages-1)
                / static_cast<float>(hashDegree-1);

        _file->setParent(this);
        if (!_file->isOpen()) {
            if (_file->open(QFile::ReadWrite)) {
                throw new std::runtime_error(
                        "Cann't open file in Writer::Writer");
            }
        }

        _capacity = packNum / numberOfPackages;
        int size = _capacity*sizeof(Record);

        _addr = reinterpret_cast<Record*>(_file->map(basePos,
        		size*numberOfPackages));

        if (_addr == NULL) {
        	throw new std::runtime_error("Cann't map data");
        }

        //qDebug() << _file->size() << ", " << numberOfPackages*size;
        //return;

        int pack = 0;
        for (int i = 0; i < _capacity*numberOfPackages; i+=_capacity) {
            _packages[pack] = new Package(&_addr[i], pack, this);
            ++pack;
        }
    }

    Writer::~Writer() {
        for (int i = 0; i < _packages.size(); ++i) {
            delete _packages[i];
        }
        _file->unmap((uchar*)_metaPackages);
        _file->unmap((uchar*)_addr);
#ifdef MEM_DEBUG
    	qDebug() << "Writer destroyed";
#endif
    }


} /* namespace db */
