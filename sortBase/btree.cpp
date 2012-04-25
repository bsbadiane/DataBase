#include "btree.h"
#include <QTime>
#include <QFile>
#include <QDataStream>
#include <cstring>

QDataStream& operator <<(QDataStream& stream, btree::Record& record);
bool compareStrings(const char* str1, const char* str2);

btree::btree()
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
        //stringtofloat(tempRecord.string);
        records.push_back(tempRecord);
    }
    fclose(f);
}

QString btree::start(bool tyep1)
{
    QTime timer;
    timer.start();
    tyep = !tyep1;
    TNode* tree = NULL;
    for (int i = 0; i< records.size();i++)
    {
        qDebug() << i;
        makeTree(&tree,records.at(i));
    }

    walkTree(tree);

    //qDebug() << timer.elapsed();
    QString returnMessage;
    returnMessage = QString::number(timer.elapsed());

    QString filename = "btree.out.";
    if (tyep) {
        filename += "int";
    } else {
        filename += "string";
    }
    QFile::remove(filename);
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    QDataStream stream(&file);
    for (int i = 0; i < recordsOtput.size(); ++i) {
        stream << recordsOtput[i];
        //qDebug() << radixRecords[i].string << radixRecords[i].number;
    }

    file.close();

    return returnMessage;

}

void btree::makeTree(TNode** pp, Record x)
{
   if(!(*pp))
   {
      TNode* p = new TNode();
      p->value = x;
      *pp = p;
   }
   else if (tyep)
        {
            if((*pp)->value.number >= x.number)
               makeTree(&((*pp)->pleft), x);
            else
               makeTree(&((*pp)->pright), x);
        } else {
            if(compareStrings((*pp)-> value.string, x.string)/*stringtofloat((*pp)-> value.string) >= stringtofloat(x.string)*/)
                makeTree(&((*pp)->pleft), x);
            else
                makeTree(&((*pp)->pright), x);
        }
}

void btree::walkTree(TNode* p)
{
   if(p)
   {
      walkTree(p->pleft);
      recordsOtput.push_back(p->value);
      /*
      if (tyep)
      {
          //qDebug() << p->value.number << ' ';
      } else {
          //qDebug() << p->value.string << ' ';
      }
        */
      walkTree(p->pright);
   }
}

bool compareStrings(const char* str1, const char* str2) {
	int res = std::strncmp(str1, str2, 11);
	return res >= 0;
}

QDataStream& operator <<(QDataStream& stream, btree::Record& record) {
    //stream << record.ID << record.string << record.number;
	stream.writeRawData((char*)&record, sizeof(record));
    return stream;
}
