/*
 * Hasher.h
 *
 *  Created on: 24.02.2012
 *      Author: night
 */

#ifndef HASHER_H_
#define HASHER_H_
#include <qobject.h>

namespace db {
    class Record;

    class Hasher: public QObject {
        Q_OBJECT
    public:
        Hasher(QObject* parent= 0): QObject(parent){}
        virtual ~Hasher(){}

        virtual void getHash(unsigned number, Record* record) const = 0;
    };

} /* namespace db */
#endif /* HASHER_H_ */
