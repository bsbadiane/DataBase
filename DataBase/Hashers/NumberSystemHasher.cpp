/*
 * NumberSystemHasher.cpp
 *
 *  Created on: 15.02.2012
 *      Author: night
 */

#include "NumberSystemHasher.h"
#include "../DataBase.h"
#include "../writer.h"
#include <stdexcept>
#include <memory>
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

    NumberSystemHasher::NumberSystemHasher(int tailPart, int degree,
                                           QSharedPointer<Writer> writer) {
        _writer = writer;
        _tailPart = tailPart;
        _degree = degree;
    }

    NumberSystemHasher::~NumberSystemHasher() {
#ifdef DEBUG
        qDebug() << "Hasher destroyed";
#endif
    }

    void NumberSystemHasher::getHash(unsigned number, Record* record) const {
        if (record == NULL) {
            throw new std::runtime_error(
                    "Null pointer in NumberSystemHasher::getHash");
        }
#ifdef DEBUG
        qDebug() << "Hasher emited";
#endif

        int sum = 0;
        for (int i = 0; i < 6; ++i) {
            unsigned div = number / 10;
            unsigned a = number - div * 10;        //TODO more effective
            sum += _degreeTable[i] * a;

            if (div == 0) {
                break;
            }
            number = div;
        }

        sum %= _tailPart;
        //emit giveHash(sum, record);
        _writer->takeHash(sum, record);
    }

} /* namespace db */
