/*
 * CloserHasher.cpp
 *
 *  Created on: 26.02.2012
 *      Author: night
 */

#include "CloserHasher.h"
#include "../writer.h"
#include <stdexcept>

namespace db {

    const int CloserHasher::_degreeTable[] = {1,
                                              BASE,
                                              BASE * BASE,
                                              BASE * BASE * BASE,
                                              BASE * BASE * BASE * BASE,
                                              BASE * BASE * BASE * BASE * BASE};

    CloserHasher::CloserHasher(int tailPart, int degree) {
        _tailPart = tailPart;
        _degree = degree;
    }

    CloserHasher::~CloserHasher() {
    }

    quint64 CloserHasher::getHash(quint64 number) const {

        ushort* numbers = new ushort[_degree * 3];
        for (int i = 0; i < _degree * 3; ++i) {
            quint64 div = number / 10;
            quint64 a = number - div * 10;        //TODO more effective
            numbers[i] = a;

            if (div == 0) {
                //остаток заполняем нулями
                for (int j = i+1; j < _degree*3; ++j) {
                    numbers[j] = 0;
                }
                break;
            }

            number = div;
        }

        //заворачиваем цифры
        int hightPart = _degree*3 - 1;
        int lowPart   = _degree - 1;
        quint64 sum = 0;
        for (int i = 0; i < _degree; ++i) {
            numbers[_degree + i] += numbers[lowPart -i] + numbers[hightPart - i];
            numbers[_degree + i] %= 10;
            sum += numbers[_degree + i] * _degreeTable[i];

        }
        delete[] numbers;

        return sum;
        //_writer->insertPackage(sum, record);
    }

} /* namespace db */
