/*
 * Searcher.h
 *
 *  Created on: 26.02.2012
 *      Author: night
 */

#ifndef SEARCHER_H_
#define SEARCHER_H_

#include <QtCore>

namespace db {
    class Writer;
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

} /* namespace db */
#endif /* SEARCHER_H_ */
