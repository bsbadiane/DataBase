
#include <QtCore>
#include <QCoreApplication>
#include "DataBase.h"
#include "Hashers/CloserHasher.h"

#define DEBUG

int main(int argc, char *argv[]) {
	QCoreApplication a(argc, argv);

	try {
//		db::Profiler::instance().settings("settings.conf");
//		db::Profiler::instance().profile();
		db::DataBase base("../db_result/base.db", 1200000, 10000, db::CloserHasher::build);
		base.buildDB("../db_result/city.base", 1000000);
	} catch (std::exception* e) {
		qDebug() << e->what();
		delete e;
	}

	return EXIT_SUCCESS; //a.exec();
}
