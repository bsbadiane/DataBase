/*
 * CloserHasher.cpp
 *
 *  Created on: 26.02.2012
 *      Author: night
 */

#include "CloserHasher.h"

namespace db {

    const int CloserHasher::_degreeTable[] = {1,
                                              BASE,
                                              BASE * BASE,
                                              BASE * BASE * BASE,
                                              BASE * BASE * BASE * BASE,
                                              BASE * BASE * BASE * BASE * BASE};

    CloserHasher::CloserHasher(int tailPart, int degree,
                               QSharedPointer<Writer> writer) {
        _writer = writer;
        _tailPart = tailPart;
        _degree = degree;
    }

    CloserHasher::~CloserHasher() {
    }

    void CloserHasher::getHash(unsigned number, Record* record) const {
        int* numbers = new int[_degree * 3](0);
        for (int i = _degree * 3 - 1; i >= 0; --i) {
            unsigned div = number / 10;
            unsigned a = number - div * 10;        //TODO more effective
            numbers[i] = a;

            if (div == 0) {
                break;
            }

            number = div;
        }

        //заворачиваем цифры
        int hightPart = _degree*3 - 1;
        int lowPart   = _degree - 1;
        int sum = 0;
        for (int i = 0; i < _degree; ++i) {
            numbers[_degree + i] += numbers[lowPart -i] + numbers[hightPart - i];
            numbers[_degree + i] %= 10;
            sum += numbers[_degree + i] * _degreeTable[i];

        }

        _writer->takeHash(sum, record);

    }

} /* namespace db */
