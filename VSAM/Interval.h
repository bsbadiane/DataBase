/*
 * Interval.h
 *
 *  Created on: 29.04.2012
 *      Author: night
 */

#ifndef INTERVAL_H_
#define INTERVAL_H_

#include <vector>
#include <array>
#include <algorithm>
#include <iterator>
#include <QVector>

namespace db {

    template<class _InsertType, class _StoreType, class _ID>
    class Interval {
    public:
        typedef _InsertType InsertType;
        typedef _StoreType StoreType;
        typedef _ID ID;
        typedef std::istream_iterator<InsertType> TypeReader;
        typedef QVector<StoreType> ResultType;

        Interval(int capacity, int next, float emptyPart = DEF_EMPTY_PART);
        virtual ~Interval();

        TypeReader clearAndInsertRecords(TypeReader first, TypeReader last);
        bool insertRecord(const InsertType& record);
        int getNextInterval();
        void setNextInterval(int next);

        template<typename type>
        ResultType findByField(type StoreType::*field, type value);

        ResultType soliteMemory();

        private:
        static constexpr float DEF_EMPTY_PART = 0.1;

        int _capacity;
        QVector<StoreType> _array;
        int _next;
        StoreType _maxStoredElement;
        float _emptyPart;
        int _last;
    };

    template<class _InsertType, class _StoreType, class _ID>
    inline Interval<_InsertType, _StoreType, _ID>::Interval(int capacity,
                                                            int next,
                                                            float emptyPart) :
            _capacity(capacity), _array(capacity), _next(next), _emptyPart(
                    emptyPart), _last(-1) {
    }

    template<class _InsertType, class _StoreType, class _ID>
    inline Interval<_InsertType, _StoreType, _ID>::~Interval() {
    }

    template<class _InsertType, class _StoreType, class _ID>
    inline typename Interval<_InsertType, _StoreType, _ID>::TypeReader Interval<
            _InsertType, _StoreType, _ID>::clearAndInsertRecords(
            TypeReader first, TypeReader last) {
        int busy = _capacity * (1 - _emptyPart);
        if (busy > _capacity) {
            busy = _capacity;
        }

        _last = busy;
        for (int i = 0; i < busy; ++i) {
            _array[i] = *first;
            ++first;
            if (first == last) {
                break;
                _last = i + 1;
            }
        }

        _maxStoredElement = *std::max(_array.begin(), _array.end());

        return first;
    }

    template<class _InsertType, class _StoreType, class _ID>
    inline bool Interval<_InsertType, _StoreType, _ID>::insertRecord(
            const InsertType& record) {
        if (_last >= _capacity) {
            return false;
        } else {
            auto before = std::find_if(_array.begin(), _array.end(),
                                       [&record](const StoreType& r) {
                                           return r > record;
                                       });
            _array.insert(before, record);
            ++_last;

            return true;
        }
    }

    template<class _InsertType, class _StoreType, class _ID>
    inline int Interval<_InsertType, _StoreType, _ID>::getNextInterval() {
        return _next;
    }

    template<class _InsertType, class _StoreType, class _ID>
    inline void Interval<_InsertType, _StoreType, _ID>::setNextInterval(
            int next) {
        _next = next;
    }

    template<class _InsertType, class _StoreType, class _ID>
    template<typename type>
    typename Interval<_InsertType, _StoreType, _ID>::ResultType
    Interval<_InsertType, _StoreType, _ID>::findByField(type StoreType::*field, type value) {
        ResultType result;
        for (int i = 0; i < _last; ++i) {
            if (_array[i].*field == value) {
                result.push_back(_array[i]);
            }
        }

        return result;
    }


    template<class _InsertType, class _StoreType, class _ID>
    inline typename Interval<_InsertType, _StoreType, _ID>::ResultType
    Interval<_InsertType, _StoreType, _ID>::soliteMemory() {
        ResultType result;
        int border = _last / 2;
        for (int i = border; border < _last; ++border) {
            result.push_back(_array[i]);
        }
        _last = border;

        return result;
    }

}


/* namespace db */
#endif /* INTERVAL_H_ */
