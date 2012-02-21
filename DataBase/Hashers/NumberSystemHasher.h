/*
 * NumberSystemHasher.h
 *
 *  Created on: 15.02.2012
 *      Author: night
 */

#ifndef NUMBERSYSTEMHASHER_H_
#define NUMBERSYSTEMHASHER_H_

#include <QtCore>

namespace db {
    class Record;
    class DataBase;

    class NumberSystemHasher: public QObject {
    Q_OBJECT
    public:
        NumberSystemHasher(int index, int tailPart, int degree);
        virtual ~NumberSystemHasher();

        int getDegree();
        int getTailPart();

    public slots:
        void getHash(unsigned number, Record* record, int index);

    signals:
        void giveHash(int hash, Record* record);

    private:
        int _index;

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
