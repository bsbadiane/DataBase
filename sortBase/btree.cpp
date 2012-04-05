#include "btree.h"

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
        stringtofloat(tempRecord.string);
        records.push_back(tempRecord);
    }
    fclose(f);
}

float btree::stringtofloat(char *string)
{
    int i = 0,j;
    float floatValue = 0;
    while (string[i]!=0)
    {
        j= (int) string[i];
        j = j -96;
        floatValue += pow(26,11-i)*j;
        i++;
    }
    //qDebug() << floatValue;
    return floatValue;
}

QString btree::start(bool tyep1)
{
    tyep = !tyep1;
    TNode* tree = NULL;
    for (int i = 0; i< records.size();i++)
    {
        makeTree(&tree,records.at(i));
    }

    walkTree(tree);

    QString returnMessage;
    returnMessage = "ololo";
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
            if((*pp)->value.number > x.number)
               makeTree(&((*pp)->pleft), x);
            else
               makeTree(&((*pp)->pright), x);
        } else {
            if(stringtofloat((*pp)-> value.string) > stringtofloat(x.string))
                makeTree(&((*pp)->pleft), x);
            else
                makeTree(&((*pp)->pright), x);
        }
}

void btree::walkTree(TNode* p) {
   if(p) {
      walkTree(p->pleft);
      if (tyep)
      {
          qDebug() << p->value.number << ' ';
      } else {
          qDebug() << p->value.string << ' ';
      }

      walkTree(p->pright);
   }}
