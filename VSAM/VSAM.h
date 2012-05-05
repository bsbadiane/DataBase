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

    template<class _InsertType, class _StoreType, class _IDType,
            _IDType _StoreType::* keyField>
    class VSAM {
    public:
        typedef CtrlRegion<_InsertType, _StoreType, _IDType, keyField> Region;
        typedef typename Region::InsertType InsertType;
        typedef typename Region::ResultType ResultType;
        typedef typename Region::IDType IDType;
        typedef QVector<typename Region::Statistic> Statistic;

        VSAM(int intervalCapacity = 300);
        virtual ~VSAM();

        std::less<IDType> less;
        std::greater<IDType> greater;
        std::equal_to<IDType> equal;

        bool insertRecord(InsertType& record);
        ResultType findByKeyField(IDType value);

        Statistic getStatistic() {
            Statistic res;
            for (Region* region : _regions) {
                res.push_back(region->getStatistic());
            }
            return res;
        }

        template<class InputIterator>
        void clearAndInsertRecords(InputIterator first, InputIterator last);

    private:
        QVector<Region*> _regions;
        int _intervalCapacity;

        QVector<Region*> findInsertRegions(InsertType& record);
    };

    template<class _InsertType, class _StoreType, class _IDType,
            _IDType _StoreType::* keyField>
    inline db::VSAM<_InsertType, _StoreType, _IDType, keyField>::VSAM(
            int intervalCapacity) {
        _intervalCapacity = intervalCapacity;
    }

    template<class _InsertType, class _StoreType, class _IDType,
            _IDType _StoreType::* keyField>
    inline db::VSAM<_InsertType, _StoreType, _IDType, keyField>::~VSAM() {
        for (auto region : _regions) {
            delete region;
        }
    }

    template<class _InsertType, class _StoreType, class _IDType,
            _IDType _StoreType::* keyField>
    inline bool db::VSAM<_InsertType, _StoreType, _IDType, keyField>::insertRecord(
            InsertType& record) {
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

            typename Region::SoliteType intervals = r->soliteRegion();
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

    template<class _InsertType, class _StoreType, class _IDType,
            _IDType _StoreType::* keyField>
    QVector<typename VSAM<_InsertType, _StoreType, _IDType, keyField>::Region*> VSAM<
            _InsertType, _StoreType, _IDType, keyField>::findInsertRegions(
            InsertType& record) {
        int next = 0;
        IDType maxElement;
        Region* currentRegion;
        QVector<Region*> res;
        while (next != -1) {
            typename Region::StoreType stRecord = record;
            currentRegion = _regions[next];
            maxElement = currentRegion->getMaxElement();

            if (less(stRecord.*keyField, maxElement)
                    || equal(stRecord.*keyField, maxElement)) {
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

    template<class _InsertType, class _StoreType, class _IDType,
            _IDType _StoreType::* keyField>
    inline typename db::VSAM<_InsertType, _StoreType, _IDType, keyField>::ResultType
    db::VSAM<_InsertType, _StoreType, _IDType, keyField>::findByKeyField(
            IDType value) {
        int next = 0;
        ResultType res;
        while (next != -1) {
            if (less(_regions[next]->getMaxElement(), value)) {
                next = _regions[next]->getNextRegion();
                continue;
            }
            if (greater(_regions[next]->getMinElement(), value)) {
                break;
            }
            res += _regions[next]->findByKeyField(value);

            next = _regions[next]->getNextRegion();
        }

        return res;
    }

    template<class _InsertType, class _StoreType, class _IDType,
            _IDType _StoreType::* keyField>
    template<class InputIterator>
    inline void db::VSAM<_InsertType, _StoreType, _IDType, keyField>::clearAndInsertRecords(
            InputIterator first, InputIterator last) {
        for (Region* r : _regions) {
            delete r;
        }
        _regions.clear();

        while (first != last) {
            _regions.push_back(new Region(_intervalCapacity));
            first = _regions.back()->clearAndInsertRecords(first, last);
        }

        for (int i = 0; i < _regions.size(); ++i) {
            _regions[i]->setNextRegion(i + 1);
        }
        _regions.back()->setNextRegion(-1);
    }
}/* namespace db */
#endif /* VSAM_H_ */
