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

    struct Record
    {
            char ID[7];
            char string[12];
            int number;
    };

    private:
    QVector <Record> records;
    QVector <Record> recordsOtput;
    int getChar(Record recordVariable,bool longMethod, int numInt);

};

QDataStream& operator <<(QDataStream& stream, radix::Record& record);

#endif // RADIX_H
