#include "radix.h"

//TODO Фамилии вместо билиберды

radix::radix()
{
    /*
     * Открытие файла базы данных и загрузка в память значений
     */
    FILE *f=NULL;
    //Record element;
    f = fopen("../base.dat","rb");
    for (int i=0; i <800000;i++)
    {
        Record tempRecord;
        fread(&tempRecord,sizeof(Record),1,f);
        //qDebug() << tempRecord.number;
        records.push_back(tempRecord);
    }
    fclose(f);
}

//float radix::stringtofloat(char *string)
//{
//    int i = 0,j;
//    float floatValue = 0;
//    while (string[i]!=0)
//    {
//        j= (int) string[i];
//        j = j -96;
//        floatValue += pow(26,11-i)*j;
//        i++;
//    }
//    //qDebug() << floatValue;
//    return floatValue;
//}
//

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
        retval = (int) recordVariable.string[11 - numInt];
    }

    //i = longMethod ? recordVariable.number :
    return retval;
}


//карманная сортировка
QString radix::start(bool longMethod)
//length - макс длина числа(кол-во разрядов), longmethod - по строкам или по числам
{
    QString returnMessage;
    returnMessage = "ololo";
    //int alphabetStart = 97; //97 - номер буквы "a" в ASCII
    int range = longMethod ? 10 : 26;  //97 - номер буквы "a" в ASCII
    int length = longMethod ? 6 : 12;
    int size;
    size = 800000;

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

            //повторять для каждой буквы, для заноса в определённый карман
            //можно сделать switch case
            for (int j = 0; j<range; j++) {

                //если значение равно нужному
                if (j==getChar(radixRecords.at(k),longMethod,i)) {
                    //переносим в разный буффер
                    radixRecordsTemp[j].push_back(radixRecords.at(i));
                    break;
                }
            }
        }

        //qDebug() << "Sborka";
        //А теперь собирается всё в одно
        int num = 0;
        for (int j =0; j<range; j++) {
            qDebug() << radixRecordsTemp[j].size();
            for (int k = 0; k < radixRecordsTemp[j].size(); k++) {
                //if (radixRecordsTemp[j].at(k)!= NULL) {
                    radixRecords.remove(num);
                    //вставить на место
                    radixRecords.insert(num,radixRecordsTemp[j].at(k));
                    num++;
                //}

            }
            //qDebug() << num;
        }
    }
    return returnMessage;
}
