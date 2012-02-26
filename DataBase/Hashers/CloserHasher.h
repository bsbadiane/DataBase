/*
 * CloserHasher.h
 *
 *  Created on: 26.02.2012
 *      Author: night
 */

#ifndef CLOSERHASHER_H_
#define CLOSERHASHER_H_

#include "Hasher.h"
#include <QtCore>

namespace db {
    class Writer;

    class CloserHasher: public db::Hasher {
        Q_OBJECT
    public:
        CloserHasher(int tailPart, int degree,
                     QSharedPointer<Writer> writer);
        virtual ~CloserHasher();

        virtual void getHash(quint64 number, Record* record) const;

    private:
        QSharedPointer<Writer> _writer;
        int     _tailPart;
        int     _degree;

        static const int BASE = 10;
        static const int _degreeTable[];
    };

} /* namespace db */
#endif /* CLOSERHASHER_H_ */
