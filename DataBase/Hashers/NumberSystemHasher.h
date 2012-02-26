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
    Q_OBJECT
    public:
        NumberSystemHasher(int tailPart, int degree);
        virtual ~NumberSystemHasher();

        int getDegree();
        int getTailPart();

    public slots:
        virtual quint64 getHash(quint64 number) const;

    signals:
        void giveHash(int hash, Record* record);

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

} /* namespace db */
#endif /* NUMBERSYSTEMHASHER_H_ */
