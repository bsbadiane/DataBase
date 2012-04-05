#ifndef BTREE_H
#define BTREE_H

#include <QVector>
#include <QDebug>
#include <math.h>

class btree
{
public:
    btree();

    QString start (bool tyep);
    float stringtofloat(char *string);
private:
    struct Record
    {
            char ID[7];
            char string[12];
            int number;
    };
    QVector <Record> records;
    QVector <Record> recordsOtput;
    bool tyep;

    struct TNode {
       Record value;
       TNode *pleft, *pright;
       //constructor
       TNode() {
          pleft = pright = 0;
       }
    };

    void makeTree(TNode** pp, Record x);
    void walkTree(TNode* p);
    void walkNotRec(TNode* tree);

};

#endif // BTREE_H
