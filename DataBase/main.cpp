#include "DataBase.h"
#include <QtCore>
#include <QCoreApplication>

#define DEBUG

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    try {
        db::DataBase base("/storage/base.dat", "/storage/nydb.db", 20);
        base.buildDB();
    } catch (std::exception* e) {
        qDebug() << e->what();
        delete e;
    }

    return EXIT_SUCCESS;        //a.exec();
}
