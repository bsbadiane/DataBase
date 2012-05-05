/*
 * main.cpp
 *
 *  Created on: 03.05.2012
 *      Author: night
 */

#include <iostream>
#include <algorithm>
#include <string>
#include <limits>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include "../DataBase/Record.h"
#include "VSAM.h"
#include <QDebug>
using namespace std;

namespace db {
    std::istream &operator>>(std::istream &is, db::Record &stan) {
        is.read((char*)&stan, sizeof(db::Record));
        return is;
    }
}

typedef db::VSAM<db::Record, db::RecordNumber, int,
        &db::RecordNumber::number> StrVSAM;

int main() {
    std::ifstream input;
    input.open("../db_result/radix.out.int", ios::in | ios::binary);
    StrVSAM vsam;
    istream_iterator<db::Record> first(input), last;
    vsam.clearAndInsertRecords(first, last);
    StrVSAM::ResultType res = vsam.findByKeyField(1);
    qDebug() << res.size();
    db::Record r = {"123456", "Abramtsevo", 1};
    for (int i = 0; i < 200000; ++i) {
        vsam.insertRecord(r);
    }
    res = vsam.findByKeyField(1);
    qDebug() << res.size();
    return 0;
}

