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
        //Q_OBJECT
    public:
    	static Hasher* build(int tailPart, int degree);

        CloserHasher(int tailPart, int degree);
        virtual ~CloserHasher();

        virtual quint64 getHash(quint64 number) const;

    private:
        int     _tailPart;
        int     _degree;

        static const int BASE = 10;
        static const int _degreeTable[];
    };

    inline
    Hasher* CloserHasher::build(int tailPart, int degree) {
    	return new CloserHasher(tailPart, degree);
    }

} /* namespace db */
#endif /* CLOSERHASHER_H_ */
