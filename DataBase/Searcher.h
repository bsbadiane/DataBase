/*
 * Searcher.h
 *
 *  Created on: 26.02.2012
 *      Author: night
 */

#ifndef SEARCHER_H_
#define SEARCHER_H_

#include "writer.h"
#include <QtCore>

namespace db {
    class Hasher;
    class Record;

    class Searcher {
    public:
        Searcher(QSharedPointer<Writer> writer, QSharedPointer<Hasher> hasher);
        virtual ~Searcher();

        Record* search(char ID[10]);

    private:
        QSharedPointer<Writer> _writer;
        QSharedPointer<Hasher> _hasher;
    };

    inline
    Record* Searcher::search(char ID[10]) {
        quint64 number = _hasher->getNumber(ID);
        int hash = _hasher->getHash(number);
        return _writer->searchPackage(hash, ID);
    }

} /* namespace db */
#endif /* SEARCHER_H_ */
