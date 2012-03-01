/*
 * TProfiler.cpp
 *
 *  Created on: 29.02.2012
 *      Author: night
 */

#include "config.h"
#include "Profiler.h"
#include "Hashers/NumberSystemHasher.h"
#include "Hashers/CloserHasher.h"
#include "DataBase.h"
#include <stdexcept>
#include <qdebug.h>

namespace db {

TProfiler::TProfiler() {
	_nativeCount = 0;
	_recToRead = 0;
	_dbSize = 0;
}

void TProfiler::settings(QString settingFile) {
	QSettings settings(settingFile, QSettings::IniFormat);
	_source = settings.value("Source").toString();
	_dir1 = settings.value("Hasher1").toString();
	_dir2 = settings.value("Hasher2").toString();
	_stepsFile = settings.value("Steps").toString();
	_output = settings.value("Output", "out").toString();
	_dbSize = settings.value("DBsize").toInt();
	_recToRead = settings.value("RecordsToRead").toInt();
	_searchFile = settings.value("Search").toString();
#ifdef DEBUG3
	qDebug() << _source << _dir1 << _dir2 << _stepsFile;
#endif
	setSteps();
	_hashers[0] = NumberSystemHasher::build;
	_hashers[1] = CloserHasher::build;
}

void TProfiler::profile() {
	QDir rootDir;
	rootDir.mkdir(_dir1);
	rootDir.mkdir(_dir2);

	_hashDir[0] = QDir(rootDir.absolutePath() + "/" + _dir1);
	_hashDir[1] = QDir(rootDir.absolutePath() + "/" + _dir2);
	clean(_hashDir[0]);
	clean(_hashDir[1]);
	for (int i = 0; i < _steps.size(); ++i) {
		for (int numberOfPackages = _steps[i].begin;
				numberOfPackages < _steps[i].end;
				numberOfPackages += _steps[i].step) {
			qDebug() << numberOfPackages;
			for (int hasher = 0; hasher < 2; ++hasher) {
				DataBase base(
						_hashDir[hasher].absolutePath() + "/"
								+ QString::number(numberOfPackages) + ".db",
						_dbSize, numberOfPackages, _hashers[hasher]);
				base.buildDB(_source, _recToRead);
				searchInDB(&base);
				logResult(numberOfPackages, hasher);
				cleanResult();
			}
		}

	}
#ifdef DEBUG4
	qDebug() << _ro[0];
	qDebug() << _ro[1];
	qDebug();
	qDebug() << _over[0];
	qDebug() << _over[1];
#endif
	flushLogs();
}

void TProfiler::insertedInNative() {
	++_nativeCount;
}

void TProfiler::clean(QDir dir) {
	QDirIterator wolker(dir.absolutePath(), QDir::Files);
	while (wolker.hasNext()) {
		QFile::remove(wolker.next());
	}
}

void TProfiler::setSteps() {
	QFile file(_stepsFile);
	if (!file.open(QFile::ReadOnly)) {
		throw new std::runtime_error(
				(QString("Cann't open file ") + _stepsFile).toAscii().data());
	}
	QTextStream stream(&file);
	while (!stream.atEnd()) {
		Step step;
		QString line = stream.readLine();
#ifdef DEBUG3
		qDebug() << line;
#endif
		QStringList takeStep = line.split(" step ", QString::SkipEmptyParts,
				Qt::CaseInsensitive);
		bool state;
		step.step = takeStep[1].toInt(&state);
		if (state == false) {
			throw new std::runtime_error("Invalid step file.");
		}
		QStringList borders = takeStep[0].split("..", QString::SkipEmptyParts);
		step.begin = borders[0].toInt(&state);
		if (state == false) {
			throw new std::runtime_error("Invalid step file.");
		}
		step.end = borders[1].toInt(&state);
		if (state == false) {
			throw new std::runtime_error("Invalid step file.");
		}
#ifdef DEBUG3
		qDebug() << step.begin << step.end << step.step;
#endif
		_steps.push_back(step);
	}

}

TProfiler::~TProfiler() {
#ifdef MEM_DEBUG
	qDebug() << "Profiler destroyed";
#endif
}

void TProfiler::logResult(int number, int hasher) {
	Point point;
	point.y = static_cast<float>(_nativeCount) / static_cast<float>(_dbSize)
			* 100.;
	point.x = number;
	_ro[hasher].push_back(point);
	point.y = static_cast<float>(_recToRead - _nativeCount) / _recToRead * 100.;
	_over[hasher].push_back(point);
	point.y = _searchTime;
	_time[hasher].push_back(point);
}

void TProfiler::cleanResult() {
	_nativeCount = 0;
	_searchTime = 0;
}
/* namespace db */

void TProfiler::flushLogs() {
	for (int i = 0; i < 2; ++i) {
		QFile fileRo(_hashDir[i].absoluteFilePath(_output + ".ro"));
		if (!fileRo.open(QFile::WriteOnly)) {
			throw new std::runtime_error("Cann't write to output ro.");
		}
		QDataStream stream(&fileRo);
		stream << _ro[i];
		fileRo.close();

		QFile fileOver(_hashDir[i].absoluteFilePath(_output + ".over"));
		if (!fileOver.open(QFile::WriteOnly)) {
			throw new std::runtime_error("Cann't write to output over.");
		}
		stream.setDevice(&fileOver);
		stream << _over[i];
		fileOver.close();

		QFile fileTime(_hashDir[i].absoluteFilePath(_output + ".time"));
		if (!fileTime.open(QFile::WriteOnly)) {
			throw new std::runtime_error("Cann't write to output over.");
		}
		stream.setDevice(&fileTime);
		stream << _time[i];
		fileTime.close();
	}
}

void TProfiler::searchInDB(DataBase* db) {

	QFile searchFile(_searchFile);
	if (!searchFile.open(QFile::ReadOnly)) {
		qDebug() << "Skip searching.";
		return;
	}
	char* array = reinterpret_cast<char*>(searchFile.map(0, searchFile.size()));
	QTime timer;
	timer.start();
	int size = _recToRead*7*1.002;
	for (int i = 0; i < size; i+=7) {
		Record* r = db->searchByID(&array[i]);
		//qDebug() << r->ID << r->string;
		if (r != NULL) delete r;
	}
	_searchTime = timer.elapsed();
	qDebug() << "Time:" << _searchTime;
	searchFile.close();
}

}

#ifdef DEBUG4
QDebug& operator <<(QDebug& stream, db::TProfiler::Point point) {
	stream << point.x << ", " << point.y;
	return stream;
}
#endif

QDataStream& operator <<(QDataStream& stream, db::TProfiler::Point point) {
	stream << point.x << point.y;
	return stream;
}

QDataStream& operator >>(QDataStream& stream, db::TProfiler::Point point) {
	stream >> point.x >> point.y;
	return stream;
}

