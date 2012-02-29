/*
 * Hasher.h
 *
 *  Created on: 24.02.2012
 *      Author: night
 */

#ifndef HASHER_H_
#define HASHER_H_
#include "../Record.h"
#include <QtCore>
//#include <qobject.h>

namespace db {
    class Record;

    class Hasher {        //: public QObject {
        //Q_OBJECT
    public:
    	typedef Hasher* (*Constructor)(int tailPart, int degree);

    	Hasher() {
    		qsrand(time(NULL));
    	}
        virtual ~Hasher() {
        }

        virtual quint64 getHash(quint64 number) const = 0;
        virtual quint64 getNumber(char string[Record::ID_SIZE]);

    protected:
        static const int _shiftBase = 6;
    };

    inline quint64 Hasher::getNumber(char string[Record::ID_SIZE]) {
        quint64 sum = 0;
        /*for (int i = 0; i < 9; i += 2) {
         sum += string[i];
         }
         for (int i = 1; i < 9; i += 2) {
         quint64 val = ~static_cast<quint64>(string[i]) & ~0xf0000000;
         sum += val;
         }*/
        int shift = 0;
        for (int i = 0; i < Record::ID_SIZE-1; ++i) {
            sum += static_cast<quint64>(string[i]) << shift;
            shift += _shiftBase;
        }
        return sum;
    }

} /* namespace db */
#endif /* HASHER_H_ */
