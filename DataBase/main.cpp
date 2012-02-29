#include "Profiler.h"
#include <QtCore>
#include <QCoreApplication>


#define DEBUG

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    try {
    db::Profiler::instance().settings("settings.conf");
    db::Profiler::instance().profile();
    } catch(std::exception* e) {
    	qDebug() << e->what();
    	delete e;
    }

    return EXIT_SUCCESS;        //a.exec();
}
