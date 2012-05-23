/*
 * CtrlRegion.h
 *
 *  Created on: 29.04.2012
 *      Author: night
 */

#ifndef CTRLREGION_H_
#define CTRLREGION_H_

#include "Interval.h"
#include <cassert>
#include <functional>

namespace db {

    template<class _InsertType, class _StoreType, class _IDType,
            _IDType _StoreType::* keyField>
    class CtrlRegion {
    public:
        typedef _InsertType InsertType;
        typedef _StoreType StoreType;
        typedef Interval<InsertType, StoreType, _IDType, keyField> Inter;
        typedef typename Inter::IDType IDType;
        typedef typename Inter::ResultType ResultType;
        typedef QVector<Inter*> SoliteType;
        typedef QVector<typename Inter::Statistic> Statistic;

        static const int DEF_CAPACITY = 24;
        std::less<IDType> less;
        std::greater<IDType> greater;
        std::equal_to<IDType> equal;

        CtrlRegion(int intervalCapacity, int next = -1);
        virtual ~CtrlRegion();

        int getNextRegion();
        void setNextRegion(int next);
        SoliteType soliteRegion();
        bool insertRecord(const InsertType& record);
        ResultType findByKeyField(IDType value);
        IDType getMaxElement();
        IDType getMinElement();
        ResultType getInterval(int interval);

        Statistic getStatistic() {
            Statistic res;
            for (int i = 0; i < DEF_CAPACITY; ++i) {
                res.push_back(_intervals[i]->getStatistic());
            }
            return res;
        }

        template<class InputIterator>
        InputIterator clearAndInsertRecords(InputIterator first,
                                            InputIterator last);
        template<class InputIterator>
        InputIterator clearAndCopyIntervals(InputIterator first,
                                            InputIterator last);

    private:
        Inter** _intervals;
        QVector<int> _emptyIntervals;
        int _next;
        int _intervalCapacity;
        IDType _maxStoredElement;
        IDType _minStoredElement;

        SoliteType findInsertationInterval(const InsertType& record);
        void updateMaxMin() {
            int next = 0;
            bool firstIteration = true;
            while (next != -1) {
                if (firstIteration) {
                    _maxStoredElement = _intervals[next]->getMaxElement();
                    _minStoredElement = _intervals[next]->getMinElement();
                    firstIteration = false;
                } else {
                    if (greater(_intervals[next]->getMaxElement(),
                                _maxStoredElement)) {
                        _maxStoredElement = _intervals[next]->getMaxElement();
                    }
                    if (less(_intervals[next]->getMinElement(),
                             _minStoredElement)) {
                        _minStoredElement = _intervals[next]->getMinElement();
                    }
                }

                next = _intervals[next]->getNextInterval();
            }
        }
    };

    template<class _InsertType, class _StoreType, class _IDType,
            _IDType _StoreType::* keyField>
    inline CtrlRegion<_InsertType, _StoreType, _IDType, keyField>::CtrlRegion(
            int intervalCapacity, int next) {
        _next = next;
        _intervals = new Inter*[DEF_CAPACITY];
        _intervalCapacity = intervalCapacity;
        int nextInterval = 1;
        int last = -1;
        for (int i = 0; i < DEF_CAPACITY; ++i) {
            if ((i + 1) % 6 == 0) {
                _intervals[i] = new Inter(intervalCapacity);
                _emptyIntervals.push_back(i);
            } else {
                while (((nextInterval + 1) % 6) == 0) {
                    ++nextInterval;
                }
                _intervals[i] = new Inter(intervalCapacity, nextInterval);
                ++nextInterval;
                last = i;
            }
        }
        if (last != -1) {
            _intervals[last]->setNextInterval(-1);
        }
    }

    template<class _InsertType, class _StoreType, class _IDType,
            _IDType _StoreType::* keyField>
    inline CtrlRegion<_InsertType, _StoreType, _IDType, keyField>::~CtrlRegion() {
        for (int i = 0; i < DEF_CAPACITY; ++i) {
            delete _intervals[i];
        }
        delete _intervals;
    }

    template<class _InsertType, class _StoreType, class _IDType,
            _IDType _StoreType::* keyField>
    template<class InputIterator>
    inline InputIterator CtrlRegion<_InsertType, _StoreType, _IDType, keyField>::clearAndInsertRecords(
            InputIterator first, InputIterator last) {
        int next = 0;
        bool firstIteration = true;
        while (next != -1) {
            if (first == last) {
                break;
            }

            first = _intervals[next]->clearAndInsertRecords(first, last);

            if (firstIteration) {
                _maxStoredElement = _intervals[next]->getMaxElement();
                _minStoredElement = _intervals[next]->getMinElement();
                firstIteration = false;
            } else {
                if (greater(_intervals[next]->getMaxElement(),
                            _maxStoredElement)) {
                    _maxStoredElement = _intervals[next]->getMaxElement();
                }
                if (less(_intervals[next]->getMinElement(),
                         _minStoredElement)) {
                    _minStoredElement = _intervals[next]->getMinElement();
                }
            }

            next = _intervals[next]->getNextInterval();

        }

        return first;
    }

    template<class _InsertType, class _StoreType, class _IDType,
            _IDType _StoreType::* keyField>
    inline bool CtrlRegion<_InsertType, _StoreType, _IDType, keyField>::insertRecord(
            const InsertType& record) {
        bool isInserted = false;
        //найти интервал
        SoliteType intervals = findInsertationInterval(record);
        //вставить туда
        for (Inter* inter : intervals) {
            if (inter->insertRecord(record)) {
                isInserted = true;
                break;
            }
        }
        if (!isInserted) {
            Inter* interval = intervals[0];
            //если не получилось, то расщипить
            if (_emptyIntervals.empty()) {
                return false;
            } else {
                typename Inter::ResultType memPart = interval->soliteMemory();

                int next = interval->getNextInterval();
                int newIndex = _emptyIntervals.front();
                Inter* dest = _intervals[newIndex];
                interval->setNextInterval(newIndex);        //fixme bug hear
                dest->setNextInterval(next);
                _emptyIntervals.pop_front();

                dest->clearAndInsertRecords(memPart.begin(), memPart.end());

                updateMaxMin();
                //еще раз вставить
                return insertRecord(record);
            }

        } else {
//            StoreType element = record;
//            if (greater((IDType)(element.*keyField), _maxStoredElement)) {
//                _maxStoredElement = element.*keyField;        //TODO тут какая-то хрень
//            }
//
//            if (less((IDType)(element.*keyField), _minStoredElement)) {
//                _minStoredElement = element.*keyField;
//            }
            /*int next = 0;
             bool firstIteration = true;
             while (next != 0) {
             if (firstIteration) {
             _maxStoredElement = _intervals[next]->getMaxElement();
             _minStoredElement = _intervals[next]->getMinElement();
             } else {
             if (greater(_intervals[next]->getMaxElement(),
             _maxStoredElement)) {
             _maxStoredElement = _intervals[next]->getMaxElement();
             }
             if (less(_intervals[next]->getMinElement(),
             _minStoredElement)) {
             _minStoredElement = _intervals[next]->getMinElement();
             }
             }
             }*/
            updateMaxMin();

            return true;
        }
    }

    template<class _InsertType, class _StoreType, class _IDType,
            _IDType _StoreType::* keyField>
    inline int CtrlRegion<_InsertType, _StoreType, _IDType, keyField>::getNextRegion() {
        return _next;
    }

    template<class _InsertType, class _StoreType, class _IDType,
            _IDType _StoreType::* keyField>
    inline void CtrlRegion<_InsertType, _StoreType, _IDType, keyField>::setNextRegion(
            int next) {
        _next = next;
    }

    template<class _InsertType, class _StoreType, class _IDType,
            _IDType _StoreType::* keyField>
    typename CtrlRegion<_InsertType, _StoreType, _IDType, keyField>::SoliteType CtrlRegion<
            _InsertType, _StoreType, _IDType, keyField>::soliteRegion() {
        int border = DEF_CAPACITY / 2;
        int next = 0;

        SoliteType res;
        for (int i = 0; i < border - 1; ++i) {
            next = _intervals[next]->getNextInterval();
        }

        int last = next;
        next = _intervals[last]->getNextInterval();
        _intervals[last]->setNextInterval(-1);
        _maxStoredElement = _intervals[last]->getMaxElement();

        while (next != -1) {
            int current = next;
            next = _intervals[current]->getNextInterval();
            res.push_back(_intervals[current]);
            _intervals[current] = new Inter(_intervalCapacity);
            _emptyIntervals.push_back(current);
            //next = _intervals[next]->getNextInterval();
        }

        updateMaxMin();

        return res;
    }

    template<class _InsertType, class _StoreType, class _IDType,
            _IDType _StoreType::* keyField>
    typename CtrlRegion<_InsertType, _StoreType, _IDType, keyField>::ResultType CtrlRegion<
            _InsertType, _StoreType, _IDType, keyField>::findByKeyField(
            IDType value) {
        int next = 0;
        ResultType res;
        while (next != -1) {
            if (less(_intervals[next]->getMaxElement(), value)) {
                next = _intervals[next]->getNextInterval();
                continue;
            }
            if (greater(_intervals[next]->getMinElement(), value)) {
                break;
            }
            res += _intervals[next]->findByKeyField(value);

            next = _intervals[next]->getNextInterval();
        }

        return res;
    }

    template<class _InsertType, class _StoreType, class _IDType,
            _IDType _StoreType::*keyField>
    inline typename CtrlRegion<_InsertType, _StoreType, _IDType, keyField>::IDType CtrlRegion<
            _InsertType, _StoreType, _IDType, keyField>::getMaxElement() {
        return _maxStoredElement;
    }

    template<class _InsertType, class _StoreType, class _IDType,
            _IDType _StoreType::*keyField>
    inline typename CtrlRegion<_InsertType, _StoreType, _IDType, keyField>::IDType CtrlRegion<
            _InsertType, _StoreType, _IDType, keyField>::getMinElement() {
        return _minStoredElement;
    }

    template<class _InsertType, class _StoreType, class _IDType,
            _IDType _StoreType::* keyField>
    typename CtrlRegion<_InsertType, _StoreType, _IDType, keyField>::SoliteType CtrlRegion<
            _InsertType, _StoreType, _IDType, keyField>::findInsertationInterval(
            const InsertType& record) {
        int next = 0;
        IDType maxElement;
        Inter* currentInterval;
        SoliteType res;
        while (next != -1) {
            StoreType stRecord = record;
            currentInterval = _intervals[next];
            maxElement = currentInterval->getMaxElement();

            if (less(stRecord.*keyField, maxElement)
                    || equal(stRecord.*keyField, maxElement)) {
                res.push_back(currentInterval);
                next = currentInterval->getNextInterval();
                while (next != -1
                        && equal(_intervals[next]->getMaxElement(), maxElement)) {
                    res.push_back(_intervals[next]);
                    next = _intervals[next]->getNextInterval();
                }
                return res;

            } else {
                next = currentInterval->getNextInterval();
            }
        }
        res.push_back(currentInterval);
        return res;
    }

    template<class _InsertType, class _StoreType, class _IDType,
            _IDType _StoreType::* keyField>
    template<class InputIterator>
    inline InputIterator db::CtrlRegion<_InsertType, _StoreType, _IDType,
            keyField>::clearAndCopyIntervals(InputIterator first,
                                             InputIterator last) {

        _emptyIntervals.clear();
        int index = 0;
        while (first != last && index < DEF_CAPACITY) {
            delete _intervals[index];
            _intervals[index] = *first;
            ++index;
            ++first;
        }

        for (int i = 0; i < index; ++i) {
            _intervals[i]->setNextInterval(i + 1);
        }
        _intervals[index - 1]->setNextInterval(-1);
        for (int i = index; i < DEF_CAPACITY; ++i) {
            delete _intervals[i];
            _intervals[i] = new Inter(_intervalCapacity);
            _emptyIntervals.push_back(i);
        }

        updateMaxMin();

        return first;
    }

    template<class _InsertType, class _StoreType, class _IDType,
            _IDType _StoreType::* keyField>
    typename db::CtrlRegion<_InsertType, _StoreType, _IDType, keyField>::ResultType
    db::CtrlRegion<_InsertType, _StoreType, _IDType, keyField>::getInterval(
            int interval) {
        if (interval >= DEF_CAPACITY) {
            return ResultType();
        } else {
            return _intervals[interval]->get();
        }
    }
}

/* namespace db */
#endif /* CTRLREGION_H_ */
