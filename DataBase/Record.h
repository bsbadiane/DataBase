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

        RecordString() {
            for (int i = 0; i < ID_SIZE; ++i) {
                ID[i] = 0;
            }
            for (int i = 0; i < STRING_SIZE; ++i) {
                string[i] = 0;
            }
        }

        RecordString(const Record& record) {
            for (int i = 0; i < ID_SIZE; ++i) {
                ID[i] = record.ID[i];
            }
            for (int i = 0; i < STRING_SIZE; ++i) {
                string[i] = record.string[i];
            }
        }

        RecordString(const RecordString& record) {
            for (int i = 0; i < ID_SIZE; ++i) {
                ID[i] = record.ID[i];
            }
            for (int i = 0; i < STRING_SIZE; ++i) {
                string[i] = record.string[i];
            }
        }

        RecordString& operator=(const Record& record) {
            for (int i = 0; i < ID_SIZE; ++i) {
                ID[i] = record.ID[i];
            }
            for (int i = 0; i < STRING_SIZE; ++i) {
                string[i] = record.string[i];
            }
            return *this;
        }

        RecordString& operator=(const RecordString& record) {
            for (int i = 0; i < ID_SIZE; ++i) {
                ID[i] = record.ID[i];
            }
            for (int i = 0; i < STRING_SIZE; ++i) {
                string[i] = record.string[i];
            }
            return *this;
        }

    };

    struct RecordNumber {
        const static int ID_SIZE = 7;

        char ID[ID_SIZE];
        int number;

        RecordNumber() {
            for (int i = 0; i < ID_SIZE; ++i) {
                ID[i] = 0;
            }
            number = 0;
        }

        RecordNumber(const Record& record) {
            for (int i = 0; i < ID_SIZE; ++i) {
                ID[i] = record.ID[i];
            }
            number = record.number;
        }

        RecordNumber(const RecordNumber& record) {
            for (int i = 0; i < ID_SIZE; ++i) {
                ID[i] = record.ID[i];
            }
            number = record.number;
        }

        RecordNumber& operator=(const Record& record) {
            for (int i = 0; i < ID_SIZE; ++i) {
                ID[i] = record.ID[i];
            }
            number = record.number;
            return *this;
        }

        RecordNumber& operator=(const RecordNumber& record) {
            for (int i = 0; i < ID_SIZE; ++i) {
                ID[i] = record.ID[i];
            }
            number = record.number;
            return *this;
        }
    };

} /* namespace db */
#endif /* RECORD_H_ */
