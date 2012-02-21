/*
 * Record.h
 *
 *  Created on: 15.02.2012
 *      Author: night
 */

#ifndef RECORD_H_
#define RECORD_H_

namespace db {

    struct Record {
        const static int STRING_SIZE = 12;

        char ID[10];
        char string[STRING_SIZE];
        int number;
    };

} /* namespace db */
#endif /* RECORD_H_ */
