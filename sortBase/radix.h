#ifndef RADIX_H
#define RADIX_H

#include <QVector>
#include <QDebug>
#include <math.h>


class radix
{
public:
    radix();
    QString start (bool tyep);

private:
    struct Record
    {
            char ID[7];
            char string[12];
            int number;
    };
    QVector <Record> records;
    QVector <Record> recordsOtput;
    int getChar(Record recordVariable,bool longMethod, int numInt);

};

#endif // RADIX_H
