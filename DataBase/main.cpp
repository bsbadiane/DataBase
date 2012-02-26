#include "DataBase.h"
#include <QtCore>
#include <QCoreApplication>
#include <QDate>

#define DEBUG

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    try {
        db::DataBase base("/storage/base.dat", "/storage/nydb.db", 100);
        QTime timer;
        timer.start();
        base.buildDB();
        qDebug() << timer.elapsed();
    } catch (std::exception* e) {
        qDebug() << e->what();
        delete e;
    }

    return EXIT_SUCCESS;        //a.exec();
}
