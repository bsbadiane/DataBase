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
#include <type_traits>
#include <cstring>

namespace std {

    template<>
    struct less<char*> : public binary_function<char*, char*, bool> {
        bool operator()(char* __x, char* __y) const {
            return std::strcmp(__x, __y) < 0;
        }
    };

    template<>
    struct greater<char*> : public binary_function<char*, char*, bool> {
        bool operator()(char* __x, char* __y) const {
            return std::strcmp(__x, __y) > 0;
        }
    };

    template<>
    struct equal_to<char*> : public binary_function<char*, char*, bool> {
        bool operator()(char* __x, char* __y) const {
            return std::strcmp(__x, __y) == 0;
        }
    };

}        // namespace  std

namespace db {

    template<class _InsertType, class _StoreType, class _IDType,
            _IDType _StoreType::* keyField>
    class Interval {
    public:
        typedef _InsertType InsertType;
        typedef _StoreType StoreType;
        typedef typename std::decay<_IDType>::type IDType;
        typedef QVector<StoreType> ResultType;
        struct Statistic{
            int busy, total;
        };

        std::less<IDType> less;
        std::greater<IDType> greater;
        std::equal_to<IDType> equal;

        Interval(int capacity, int next = -1, float emptyPart = DEF_EMPTY_PART);
        virtual ~Interval();

        bool insertRecord(const InsertType& record);
        int getNextInterval();
        void setNextInterval(int next);
        IDType getMaxElement();
        IDType getMinElement();
        ResultType soliteMemory();
        ResultType findByKeyField(const IDType& value);
        void clear();
        ResultType get();

        Statistic getStatistic() {
            return {_last, _capacity};
        }

        template<class InputIterator>
        InputIterator clearAndInsertRecords(InputIterator first,
                                            InputIterator last);

    private:
        static constexpr float DEF_EMPTY_PART = 0.1;

        int _capacity;
        QVector<StoreType> _array;
        int _next;
        IDType _maxStoredElement;
        IDType _minStoredElement;
        float _emptyPart;
        int _last;

        void updateMinMax() {
            _maxStoredElement = (*std::max_element(
                    _array.begin(), _array.begin() + _last,
                    [this](StoreType& first, StoreType& second) {
                        return less(first.*keyField, second.*keyField);        //first.*keyField < second.*keyField;
                })).*keyField;

            _minStoredElement = (*std::max_element(
                    _array.begin(), _array.begin() + _last,
                    [this](StoreType& first, StoreType& second) {
                        return greater(first.*keyField, second.*keyField);        //first.*keyField > second.*keyField;
                })).*keyField;
        }
    };

    template<class _InsertType, class _StoreType, class _IDType,
            _IDType _StoreType::* keyField>
    inline Interval<_InsertType, _StoreType, _IDType, keyField>::Interval(
            int capacity, int next, float emptyPart) :
            _capacity(capacity), _array(capacity), _next(next), _emptyPart(
                    emptyPart), _last(-1) {
    }

    template<class _InsertType, class _StoreType, class _IDType,
            _IDType _StoreType::* keyField>
    inline Interval<_InsertType, _StoreType, _IDType, keyField>::~Interval() {
    }

    template<class _InsertType, class _StoreType, class _IDType,
            _IDType _StoreType::* keyField>
    template<class InputIterator>
    inline InputIterator Interval<_InsertType, _StoreType, _IDType, keyField>::clearAndInsertRecords(
            InputIterator first, InputIterator last) {
        int busy = _capacity * (1 - _emptyPart);
        if (busy > _capacity) {
            busy = _capacity;
        }

        _last = busy;
        for (int i = 0; i < busy; ++i) {
            _array[i] = *first;
            ++first;
            if (first == last) {
                _last = i + 1;
                break;
            }
        }

//        _maxStoredElement = (*std::max_element(
//                _array.begin(), _array.begin() + _last,
//                [this](StoreType& first, StoreType& second) {
//                    return less(first.*keyField, second.*keyField);        //first.*keyField < second.*keyField;
//            })).*keyField;
//
//        _minStoredElement = (*std::max_element(
//                _array.begin(), _array.begin() + _last,
//                [this](StoreType& first, StoreType& second) {
//                    return greater(first.*keyField, second.*keyField);        //first.*keyField > second.*keyField;
//            })).*keyField;
        updateMinMax();

        return first;
    }

    template<class _InsertType, class _StoreType, class _IDType,
            _IDType _StoreType::* keyField>
    inline bool Interval<_InsertType, _StoreType, _IDType, keyField>::insertRecord(
            const InsertType& record) {
        if (_last >= _capacity) {
            return false;
        } else {
            StoreType stRecord = record;
            auto before = std::find_if(
                    _array.begin(), _array.begin() + _last,
                    [this, &stRecord](StoreType& r) -> bool {
                        return greater(r.*keyField, stRecord.*keyField);        //r.*keyField > ((StoreType)record).*keyField;
                });
            _array.insert(before, record);
            ++_last;

//            _maxStoredElement = (*std::max_element(
//                    _array.begin(), _array.begin() + _last,
//                    [this](StoreType& first, StoreType& second) {
//                        return less(first.*keyField, second.*keyField);        //first.*keyField < second.*keyField;
//                })).*keyField;
//
//            _minStoredElement = (*std::max_element(
//                    _array.begin(), _array.begin() + _last,
//                    [this](StoreType& first, StoreType& second) {
//                        return greater(first.*keyField, second.*keyField);        //first.*keyField > second.*keyField;
//                })).*keyField;
            updateMinMax();

            return true;
        }
    }

    template<class _InsertType, class _StoreType, class _IDType,
            _IDType _StoreType::* keyField>
    inline int Interval<_InsertType, _StoreType, _IDType, keyField>::getNextInterval() {
        return _next;
    }

    template<class _InsertType, class _StoreType, class _IDType,
            _IDType _StoreType::* keyField>
    inline void Interval<_InsertType, _StoreType, _IDType, keyField>::setNextInterval(
            int next) {
        _next = next;
    }

    template<class _InsertType, class _StoreType, class _IDType,
            _IDType _StoreType::* keyField>
    typename Interval<_InsertType, _StoreType, _IDType, keyField>::ResultType Interval<
            _InsertType, _StoreType, _IDType, keyField>::findByKeyField(
            const IDType& value) {
        ResultType result;
        for (int i = 0; i < _last; ++i) {
            if (equal(_array[i].*keyField,
                      value)/*_array[i].*keyField == value*/) {
                result.push_back(_array[i]);
            }
        }

        return result;
    }

    template<class _InsertType, class _StoreType, class _IDType,
            _IDType _StoreType::* keyField>
    inline typename Interval<_InsertType, _StoreType, _IDType, keyField>::ResultType Interval<
            _InsertType, _StoreType, _IDType, keyField>::soliteMemory() {
        ResultType result;
        int border = _last / 2;
        for (int i = border; i < _last; ++i) {
            result.push_back(_array[i]);
        }
        _last = border;

//        _maxStoredElement = (*std::max_element(
//                _array.begin(), _array.begin() + _last,
//                [this](StoreType& first, StoreType& second) {
//                    return less(first.*keyField, second.*keyField);        //first.*keyField < second.*keyField;
//            })).*keyField;
//
//        _minStoredElement = (*std::max_element(
//                _array.begin(), _array.begin() + _last,
//                [this](StoreType& first, StoreType& second) {
//                    return greater(first.*keyField, second.*keyField);        //first.*keyField > second.*keyField;
//            })).*keyField;
        updateMinMax();

        return result;
    }

    template<class _InsertType, class _StoreType, class _IDType,
            _IDType _StoreType::* keyField>
    typename Interval<_InsertType, _StoreType, _IDType, keyField>::IDType Interval<
            _InsertType, _StoreType, _IDType, keyField>::getMaxElement() {
        return _maxStoredElement;
    }

    template<class _InsertType, class _StoreType, class _IDType,
            _IDType _StoreType::* keyField>
    typename Interval<_InsertType, _StoreType, _IDType, keyField>::IDType Interval<
            _InsertType, _StoreType, _IDType, keyField>::getMinElement() {
        return _minStoredElement;
    }

    template<class _InsertType, class _StoreType, class _IDType,
            _IDType _StoreType::*keyField>
    inline void Interval<_InsertType, _StoreType, _IDType, keyField>::clear() {
        _last = 0;
        _maxStoredElement = IDType();
    }

    template<class _InsertType, class _StoreType, class _IDType,
                _IDType _StoreType::*keyField>
    typename Interval<_InsertType, _StoreType, _IDType, keyField>::ResultType
    Interval<_InsertType, _StoreType, _IDType, keyField>::get() {
        return _array;
    }
}

/* namespace db */
#endif /* INTERVAL_H_ */
