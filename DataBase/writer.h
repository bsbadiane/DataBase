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

namespace db {
    class DataBase;

    class Writer: public QObject {
    Q_OBJECT
    public:
        Writer(QFile* file, int numberOfPackages, int hashDegree,
               int* metaPackages, int basePos);
        virtual ~Writer();

        void searchNextPackage(int currentNumber, Record* record);
        int getBasePos() const;
        void setTaskSize(int size) const;

    public slots:
        void takeHash(int number, Record* record) const;

    signals:
        void done();

    private:
        int _numberOfPackages;
        int _basePos;
        QFile* _file;
        float _scale;
        QVector<Package*> _packages;
        int* _metaPackages;
    };

    inline
    int Writer::getBasePos() const {
        return _basePos;
    }

    inline
    void Writer::searchNextPackage(int currentNumber, Record* record) {
        if (currentNumber + 1 >= _numberOfPackages) {
            currentNumber = -1;
        }
        _packages[currentNumber + 1]->insertRecord(record, false);
    }

} /* namespace db */
#endif /* WRITER_H_ */
