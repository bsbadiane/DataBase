#include <QtGui/QApplication>
#include "mainwindow.h"
//
//#include "../DataBase/Hashers/CloserHasher.h"
//
//using namespace std;
//
//namespace db {
//    std::istream &operator>>(std::istream &is, db::Record &stan) {
//        is.read((char*)&stan, sizeof(db::Record));
//        return is;
//    }
//}
//
//QVector<db::Record> records;
//void twoHundredThousand() {
//    FILE *f = NULL;
//    //Record element;
//    f = fopen("../db_result/city.base", "rb");
//    for (int i = 0; i < 800000; i++) {
//        db::Record tempRecord;
//        fread(&tempRecord, sizeof(db::Record), 1, f);
//    }
//    for (int i = 0; i < 200000; i++) {
//        db::Record tempRecord;
//        fread(&tempRecord, sizeof(db::Record), 1, f);
//        records.push_back(tempRecord);
//    }
//
//    fclose(f);
//}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
//    typedef db::VSAM<db::Record, db::RecordString,
//            decltype(db::RecordString::string), &db::RecordString::string> StrVSAM;
//
//    typedef db::VSAM<db::Record, db::RecordNumber, int,
//            &db::RecordNumber::number> NumVSAM;
//
//    db::DataBase* base;
//    StrVSAM vsamString;
//    NumVSAM vsamInt;
//
//    try {
//        base = new db::DataBase("../db_result/base.db", 1200000, 10000,
//                                db::CloserHasher::build);
//    } catch (std::exception* e) {
//        qDebug() << e->what();
//        throw;
//    }
//
//    //Надо проинициализировать два VSAMa. Одни для чисел, второй для строк.
//    std::ifstream inputInt;
//    inputInt.open("../db_result/radix.out.int", ios::in | ios::binary);
//
//    istream_iterator<db::Record> firstInt(inputInt), lastInt;
//    vsamInt.clearAndInsertRecords(firstInt, lastInt);
//
//    std::ifstream inputString;
//    inputString.open("../db_result/radix.out.string", ios::in | ios::binary);
//
//    istream_iterator<db::Record> firstString(inputString), lastString;
//    vsamString.clearAndInsertRecords(firstString, lastString);
//
//    StrVSAM::ResultType res = vsamString.findByKeyField("Moscow");
//    NumVSAM::ResultType res2 = vsamInt.findByKeyField(50);
//
//    twoHundredThousand();
//
//    for (int i = 0; i < records.size(); ++i) {
//        vsamString.insertRecord(records[i]);
//        vsamInt.insertRecord(records[i]);
//    }
//
//    for (int i = 0; i < res.size(); ++i) {
//        qDebug() << res[i].ID << res[i].string;
//    }
//
//    for (int i = 0; i < res2.size(); ++i) {
//        qDebug() << res2[i].ID << res2[i].number;
//    }
}
