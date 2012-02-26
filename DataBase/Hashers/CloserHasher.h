/*
 * CloserHasher.h
 *
 *  Created on: 26.02.2012
 *      Author: night
 */

#ifndef CLOSERHASHER_H_
#define CLOSERHASHER_H_

#include "Hasher.h"

namespace db {
    class Writer;

    class CloserHasher: public db::Hasher {
        Q_OBJECT
    public:
        CloserHasher(int tailPart, int degree,
                     QSharedPointer<Writer> writer);
        virtual ~CloserHasher(){}

        virtual void getHash(unsigned number, Record* record) const;

    private:
        Writer* _writer;
        int     _tailPart;
        int     _degree;

        static const int BASE = 11;
        static const int _degreeTable[];
    };

} /* namespace db */
#endif /* CLOSERHASHER_H_ */
