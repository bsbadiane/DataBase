/*
 * NumberSystemHasher.h
 *
 *  Created on: 15.02.2012
 *      Author: night
 */

#ifndef NUMBERSYSTEMHASHER_H_
#define NUMBERSYSTEMHASHER_H_

#include "Hasher.h"
#include <QtCore>

namespace db {
    class Record;
    class DataBase;
    class Writer;

    class NumberSystemHasher: public Hasher {
    //Q_OBJECT
    public:
    	static Hasher* build(int tailPart, int degree);

        NumberSystemHasher(int tailPart, int degree);
        virtual ~NumberSystemHasher();

        int getDegree();
        int getTailPart();

        virtual quint64 getHash(quint64 number) const;


    private:
        int _degree;
        int _tailPart;

        static const int BASE = 11;
        static const int _degreeTable[];
    };

    inline
    int NumberSystemHasher::getDegree() {
        return _degree;
    }

    inline
    int NumberSystemHasher::getTailPart() {
        return _tailPart;
    }

    inline
    Hasher* NumberSystemHasher::build(int tailPart, int degree) {
    	return new NumberSystemHasher(tailPart, degree);
    }

} /* namespace db */
#endif /* NUMBERSYSTEMHASHER_H_ */
