#include "radix.h"
#include <QFile>
#include <QDataStream>
#include<QTime>

QDataStream& operator <<(QDataStream& stream, radix::Record& record);

//TODO Фамилии вместо билиберды

radix::radix()
{
    /*
     * Открытие файла базы данных и загрузка в память значений
     */
    FILE *f=NULL;
    //Record element;
    f = fopen("../db_result/city.base","rb");
    for (int i=0; i <800000;i++)
    {
        Record tempRecord;
        fread(&tempRecord,sizeof(Record),1,f);
        records.push_back(tempRecord);
    }
    fclose(f);
}

int radix::getChar(Record recordVariable,bool longMethod, int numInt) {
    //numInt разряд
    int retval;
    if (longMethod) {
        //ИНТ
        retval = recordVariable.number;
        for (int j =0;j<numInt;j++)
            retval = retval/10;
        retval = retval % 10;
    } else {
/*    	if ((int) recordVariable.string[10 - numInt] == 0) {
    		retval = 0;
    	} else {*/
        	retval = (int) recordVariable.string[10 - numInt]/* - (int)'a' + 1*/;
/*    	}*/
    }

    //i = longMethod ? recordVariable.number :
   // qDebug() << retval;
    return retval;
}


//карманная сортировка
QString radix::start(bool longMethod)
//length - макс длина числа(кол-во разрядов), longmethod - по строкам или по числам
{
    longMethod = !longMethod;
    QString returnMessage;
    returnMessage = "ololo";
    //int alphabetStart = 97; //97 - номер буквы "a" в ASCII
    int range = longMethod ? 10 : 256;  //97 - номер буквы "a" в ASCII
    int length = longMethod ? 6 : 11;
    int size;
    size = records.size();


    QTime timer;
    timer.start();
    //создание копии базы
    QVector <Record> radixRecords;
    qDebug() << records.size();
    for (int i = 0; i < size; ++i) {
        radixRecords.push_back(records.at(i));
    }

    //повторять для каждого разряда
    for (int i = 0; i < length; i++) {
        QVector <Record> radixRecordsTemp[range];

        //повторять для каждого элемента
        for (int k = 0; k < records.size(); k++) {
            //переносим в разный буффер
        	int index = getChar(radixRecords.at(k),longMethod,i);
        	radixRecordsTemp[index].push_back(radixRecords.at(k));
        }

        radixRecords.clear();
        //А теперь собирается всё в одно
        for (int j =0; j<range; j++) {
            //qDebug() << radixRecordsTemp[j].size();
            for (int k = 0; k < radixRecordsTemp[j].size(); k++) {
                //recordsOtput.push_back(radixRecordsTemp[j].at(k));
                radixRecords.push_back(radixRecordsTemp[j].at(k));
            }
        }
    }

    returnMessage = QString::number(timer.elapsed());

    //recordsOtput в файл
    QString filename = "../db_result/radix.out.";
    if (longMethod) {
        filename += "int";
    } else {
        filename += "string";
    }
    QFile::remove(filename);
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    QDataStream stream(&file);
    for (int i = 0; i < radixRecords.size(); ++i) {
        stream << radixRecords[i];
        //qDebug() << radixRecords[i].string << radixRecords[i].number;
    }

    file.close();


    return returnMessage;
}

QDataStream& operator <<(QDataStream& stream, radix::Record& record) {
    //stream << record.ID << record.number << record.string;
	stream.writeRawData((char*)&record, sizeof(record));
    return stream;
}
