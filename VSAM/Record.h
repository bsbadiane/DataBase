/*
 * Record.h
 *
 *  Created on: 15.02.2012
 *      Author: night
 */

#ifndef RECORD_H_
#define RECORD_H_

#include <iterator>

namespace db {

    struct Record {
        const static int STRING_SIZE = 12;
        const static int ID_SIZE = 7;

        char ID[ID_SIZE];
        char string[STRING_SIZE];
        int number;
    };

    struct RecordString {
        const static int STRING_SIZE = 12;
        const static int ID_SIZE = 7;

        char ID[ID_SIZE];
        char string[STRING_SIZE];
    };

    struct RecordNumber {
        const static int ID_SIZE = 7;

        char ID[ID_SIZE];
        int number;
    };

} /* namespace db */
#endif /* RECORD_H_ */
