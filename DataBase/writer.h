/*
 * writer.h
 *
 *  Created on: 16.02.2012
 *      Author: night
 */

#ifndef WRITER_H_
#define WRITER_H_

#include "config.h"
#include "Record.h"
#include "Package.h"
#include <qthread.h>
#include <qfile.h>
#include <stdexcept>

namespace db {
    class DataBase;

    class Writer: public QObject {
    Q_OBJECT
    friend class Package;
    public:
        Writer(QFile* file, int dbSize, int numberOfPackages, int hashDegree, int basePos);
        virtual ~Writer();

        //int getBasePos() const;
        //void setTaskSize(int size) const;

    public slots:
        void insertPackage(int number, Record* record) const;
        Record* searchPackage(int number, char ID[10]) const;

    signals:
        void done();

    private:
        int _numberOfPackages;
        int _dbSize;
        //int _basePos;
        Record* _addr;
        int _capacity;
        QFile* _file;
        float _scale;
        QVector<Package*> _packages;
        int* _metaPackages;

        void insertToNextPackage(int currentNumber, Record* record);
        Record* searchInNextPackage(int currentNumber, char ID[10]);
        bool emptyRecordExist(int currentNumber);
        int getCurPos(int currentNumber);
        void incPos(int currentNumber);
        int getCapacity();
    };

    //////////////////////////////////

   /* inline
    int Writer::getBasePos() const {
        return _basePos;
    }*/

    inline
    int Writer::getCapacity() {
    	return _capacity;
    }

    inline
    void Writer::insertToNextPackage(int currentNumber, Record* record) {
        if (currentNumber + 1 >= _numberOfPackages) {
            currentNumber = -1;
        }
        Q_UNUSED(currentNumber);
        Q_UNUSED(record);
        //_packages[currentNumber + 1]->insertRecord(record, false);
    }

    /*inline Record *Writer::searchInNextPackage(int currentNumber, char ID[10]) {
        if (currentNumber + 1 >= _numberOfPackages) {
            currentNumber = -1;
        }
        return _packages[currentNumber + 1]->searchRecord(ID);
    }*/

    inline
    void Writer::insertPackage(int number, Record *record) const {
        if ((number *= _scale) >= _numberOfPackages) {
            number = _numberOfPackages - 1;
        }
        if(!_packages[number]->insertRecord(record)) {
        	++number;
        	if (number >= _numberOfPackages) {
        	     number = 0;
        	}
        	while(!_packages[number]->insertRecord(record, false)) {
        		++number;
        		if (number >= _numberOfPackages) {
        			number = 0;
        		}
        	}
        }
    }

    inline
    Record* Writer::searchPackage(int number, char ID[10]) const {
        if ((number *= _scale) >= _numberOfPackages) {
            number = _numberOfPackages - 1;
        }
        Record* record = NULL;
        while(!_packages[number]->searchRecord(ID, record)) {
        	++number;
        	if (number >= _numberOfPackages) {
        		number = 0;
        	}
        	record = NULL;
        }
        return record;
    }

    inline
    bool Writer::emptyRecordExist(int currentNumber){
    	return _metaPackages[currentNumber] < _capacity;
    }

    inline
    int Writer::getCurPos(int currentNumber) {
    	return _metaPackages[currentNumber];
    }

    inline
    void Writer::incPos(int currentNumber) {
    	++_metaPackages[currentNumber];
    	if (_metaPackages[currentNumber] == INT_MAX) {
    		throw new std::out_of_range("Package overflow.");
    	}
    }
} /* namespace db */
#endif /* WRITER_H_ */
