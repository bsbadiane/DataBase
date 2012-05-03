/*
 * VSAM.h
 *
 *  Created on: 29.04.2012
 *      Author: night
 */

#ifndef VSAM_H_
#define VSAM_H_

#include "CtrlRegion.h"
#include <QObject>
#include "../DataBase/Record.h"

namespace db {

    class VSAM : public QObject {
        Q_OBJECT
    public:
        typedef CtrlRegion<Record, RecordString, decltype(RecordString::string),
                &RecordString::string> Region;
        typedef typename Region::InsertType InsertType;
        typedef typename Region::ResultType ResultType;
        typedef typename Region::IDType IDType;

        VSAM(int intervalCapacity = 300);
        virtual ~VSAM();

        bool insertRecord(const InsertType& record);
        ResultType findByKeyField(IDType value);

        template<class InputIterator>
        InputIterator clearAndInsertRecords(InputIterator first,
                                            InputIterator last);

    private:
        QVector<Region*> _regions;
        int _intervalCapacity;
    };

    inline db::VSAM::VSAM(int intervalCapacity) {
        _intervalCapacity = intervalCapacity;
    }

    inline db::VSAM::~VSAM() {
        for (auto region : _regions) {
            delete region;
        }
    }

    inline bool db::VSAM::insertRecord(const InsertType& record) {
    }

    inline db::VSAM::ResultType db::VSAM::findByKeyField(IDType value) {
    }

    template<class InputIterator>
    inline InputIterator db::VSAM::clearAndInsertRecords(InputIterator first,
                                                         InputIterator last) {
    }
}/* namespace db */
#endif /* VSAM_H_ */
