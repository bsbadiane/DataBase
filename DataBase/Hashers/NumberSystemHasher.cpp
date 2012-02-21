/*
 * NumberSystemHasher.cpp
 *
 *  Created on: 15.02.2012
 *      Author: night
 */

#include "NumberSystemHasher.h"
#include "../DataBase.h"
#include <stdexcept>
#ifdef DEBUG
#include <qdebug.h>
#endif

namespace db {

    const int NumberSystemHasher::_degreeTable[] = {1,
                                                    BASE,
                                                    BASE * BASE,
                                                    BASE * BASE * BASE,
                                                    BASE * BASE * BASE * BASE,
                                                    BASE * BASE * BASE * BASE * BASE};

    NumberSystemHasher::NumberSystemHasher(int index, int tailPart, int degree) :
            QObject(0) {
        _index = index;
        _tailPart = tailPart;
        _degree = degree;
    }

    NumberSystemHasher::~NumberSystemHasher() {
#ifdef DEBUG
        qDebug() << "Hasher destroyed";
#endif
    }

    void NumberSystemHasher::getHash(unsigned number, Record* record, int index) {
        if (record == NULL) {
            throw new std::runtime_error(
                    "Null pointer in NumberSystemHasher::getHash");
        }
        if (index == _index) {
#ifdef DEBUG
            qDebug() << "Hasher emited";
#endif

            int sum = 0;
            for (int i = 0; i < 6; ++i) {
                unsigned div = number / 10;
                unsigned a = number - div*10;       //TODO more effective
                sum += _degreeTable[i] * a;

                if (div == 0) {
                    break;
                }
                number = div;
            }

            sum %= _tailPart;
            emit giveHash(sum, record);
        }
    }

} /* namespace db */
