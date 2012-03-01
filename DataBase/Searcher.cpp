/*
 * Searcher.cpp
 *
 *  Created on: 26.02.2012
 *      Author: night
 */

#include "writer.h"
#include "Hashers/Hasher.h"
#include "Searcher.h"

namespace db {

    Searcher::Searcher(QSharedPointer<Writer> writer,
                       QSharedPointer<Hasher> hasher) :
            _writer(writer), _hasher(hasher) {}

    Searcher::~Searcher() {
    }

} /* namespace db */
