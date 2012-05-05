/*
 * VSAM.h
 *
 *  Created on: 29.04.2012
 *      Author: night
 */

#ifndef VSAM_H_
#define VSAM_H_

#include "CtrlRegion.h"
#include "../DataBase/Record.h"

#include <QObject>

namespace db {

    class VSAM {
    public:
        typedef CtrlRegion<Record, RecordString, decltype(RecordString::string),
                &RecordString::string> Region;
        typedef typename Region::InsertType InsertType;
        typedef typename Region::ResultType ResultType;
        typedef typename Region::IDType IDType;

        VSAM(int intervalCapacity = 300);
        virtual ~VSAM();

    public:
        std::less<IDType> less;
        std::greater<IDType> greater;
        std::equal_to<IDType> equal;

        bool insertRecord(InsertType& record);
        ResultType findByKeyField(IDType value);

        template<class InputIterator>
        void clearAndInsertRecords(InputIterator first, InputIterator last);

    private:
        QVector<Region*> _regions;
        int _intervalCapacity;

        QVector<Region*> findInsertRegions(InsertType& record);
    };

    inline db::VSAM::VSAM(int intervalCapacity) {
        _intervalCapacity = intervalCapacity;
    }

    inline db::VSAM::~VSAM() {
        for (auto region : _regions) {
            delete region;
        }
    }

    inline bool db::VSAM::insertRecord(InsertType& record) {
        bool inserted = false;
        QVector<Region*> insertRegions = findInsertRegions(record);

        for (Region* r : insertRegions) {
            if (r->insertRecord(record)) {
                inserted = true;
                break;
            }
        }

        if (!inserted) {
            Region* r = insertRegions[0];

            Region::SoliteType intervals = r->soliteRegion();
            _regions.push_back(new Region(_intervalCapacity));
            _regions.back()->clearAndCopyIntervals(intervals.begin(),
                                                   intervals.end());
            int tempNext = r->getNextRegion();
            r->setNextRegion(_regions.size() - 1);
            _regions.back()->setNextRegion(tempNext);

            return insertRecord(record);
        } else {
            return true;
        }
    }

    QVector<VSAM::Region*> VSAM::findInsertRegions(InsertType& record) {
        int next = 0;
        IDType maxElement;
        Region* currentRegion;
        QVector<Region*> res;
        while (next != -1) {
            Region::StoreType stRecord = record;
            currentRegion = _regions[next];
            maxElement = currentRegion->getMaxElement();

            if (less(stRecord.string, maxElement)
                    || equal(stRecord.string, maxElement)) {
                res.push_back(currentRegion);
                next = currentRegion->getNextRegion();
                while (next != -1
                        && equal(_regions[next]->getMaxElement(), maxElement)) {
                    res.push_back(_regions[next]);
                    next = _regions[next]->getNextRegion();
                }
                return res;

            } else {
                next = currentRegion->getNextRegion();
            }
        }
        res.push_back(currentRegion);
        return res;
    }

    inline db::VSAM::ResultType db::VSAM::findByKeyField(IDType value) {
        int next = 0;
        ResultType res;
        while (next != -1) {
            if (less(_regions[next]->getMaxElement(), value)) {
                continue;
            }
            if (greater(_regions[next]->getMaxElement(), value)) {
                break;
            }
            res += _regions[next]->findByKeyField(value);
        }

        return res;
    }

    template<class InputIterator>
    inline void db::VSAM::clearAndInsertRecords(InputIterator first,
                                                InputIterator last) {
        for (Region* r : _regions) {
            delete r;
        }
        _regions.clear();

        while (first != last) {
            _regions.push_back(new Region(_intervalCapacity));
            first = _regions.back()->clearAndInsertRecords(first, last);
        }

        for (int i = 0; i < _regions.size(); ++i) {
            _regions[i]->setNextRegion(i+1);
        }
        _regions.back()->setNextRegion(-1);
    }
}/* namespace db */
#endif /* VSAM_H_ */
