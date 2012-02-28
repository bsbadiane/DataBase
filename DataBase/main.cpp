#include "DataBase.h"
#include "Hashers/NumberSystemHasher.h"
#include "Hashers/CloserHasher.h"
#include <QtCore>
#include <QCoreApplication>
#include <QDate>

#define DEBUG

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    QTime timer;
    timer.start();
    for (int i = 1000; i <= 20000; i+=1000) {
    try {
        db::DataBase base(QString::number(i) + "nydb.db", 20000, db::NumberSystemHasher::build);
        base.buildDB("base.dat");

    } catch (std::exception* e) {
        qDebug() << e->what();
        delete e;
    }
    }
    qDebug() << timer.elapsed();

    return EXIT_SUCCESS;        //a.exec();
}
