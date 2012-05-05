#include <QtCore/QCoreApplication>
#include <QFile>
#include <QDataStream>
#include <QTextStream>
#include <QDebug>
#include <QVector>
const int dlina = 7;
const int razmer = 12;
struct Record
{
        char ID[7];
        char string[razmer];
        int number;
};

QDataStream& operator <<(QDataStream& stream, Record& record);

int main()
{
    QFile::remove("../db_result/city.base");
    QFile sourcesBase;
    sourcesBase.setFileName("../db_result/townBase.txt"); // копия его лежит в каталоге с исходниками
    sourcesBase.open(QIODevice::ReadOnly);
    int randsize = 0; //17589;
    char listTowns[17000][12];
    QString qstringtemp;
    QTextStream textstream(&sourcesBase);
    //for (int i = 0; i< 17589 ; i++)
    while(!textstream.atEnd())
    {
        qstringtemp = textstream.readLine();
        if ((qstringtemp.size()>0)&&(qstringtemp.size()<11))
        {
            for (int j =0; j<12; j++)
            {
                char mas = qstringtemp[j].toAscii();
                listTowns[randsize][j] = mas;
            }
            listTowns[randsize][11] = 0;
            randsize++;
        }
    }

    sourcesBase.close();

    QFile binbase;
    binbase.setFileName("../db_result/city.base");
    binbase.open(QIODevice::WriteOnly);
    QDataStream stream(&binbase);
    char j[dlina+1];
    for (int i = 0; i<dlina+1; i++)
    {
            j[i]='0';
    }

    for (long i = 0; i< 1000200;i++)
    {
            Record element ;
            if (j[0]=='Z')
            {
                    j[0] = '0';
                    j[1]++;
                    element.ID[0]=j[0];
            } else {
                    j[0]++;
                    element.ID[0]=j[0];

            }
            for (int k = 1; k< 7; k++)
            {
                    if (j[k]=='Z')
                    {
                            j[k] = '0';
                            j[k+1]++;
                            element.ID[k]=j[k];
                    } else {
                            element.ID[k]=j[k];
                    }
            }
            element.ID[6] = 0;

            int indexN = rand() % randsize;
            for (int j = 0; j<razmer-1; j++)
            {
                element.string[j]= listTowns[indexN][j];
            }
            element.string[razmer-1] = 0;
            element.number = rand() % 100;

            stream << element;
            //qDebug() << element.ID << element.string << element.number;
    }

    binbase.close();
    return 0; //a.exec();
}


QDataStream& operator <<(QDataStream& stream, Record& record) {
    //stream << record.ID << record.number << record.string;
        stream.writeRawData((char*)&record, sizeof(record));
    return stream;
}
