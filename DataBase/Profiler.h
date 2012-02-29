/*
 * TProfiler.h
 *
 *  Created on: 29.02.2012
 *      Author: night
 */

#ifndef TPROFILER_H_
#define TPROFILER_H_

#include "Singleton.h"
#include "Hashers/Hasher.h"

namespace db {

class TProfiler;

typedef Singleton<TProfiler> Profiler;

class TProfiler {
	friend class Singleton<TProfiler>;
public:
	struct Point {
		int x;
		float   y;
	};

	void settings(QString settingFile);
	void profile();

	void insertedInNative();

private:

	struct Step {
		int begin;
		int end;
		int step;
	};

	Hasher::Constructor _hashers[2];
	QString _source;
	QString _dir1;
	QString _dir2;
	QString _stepsFile;
	QString _output;
	QList<Step> _steps;
	QDir _hashDir[2];

	int _nativeCount;
	QList<Point> _ro[2];
	QList<Point> _over[2];

	TProfiler();
	virtual ~TProfiler();

	void setSteps();
	void clean(QDir dir);
	void logResult(int number, int hasher);
	void cleanResult();
	void flushLogs();
};

} /* namespace db */

#ifdef DEBUG3
QDebug& operator << (QDebug& stream, db::TProfiler::Point point);
#endif

QDataStream& operator << (QDataStream& stream, db::TProfiler::Point point);

QDataStream& operator >> (QDataStream& stream, db::TProfiler::Point point);

#endif /* TPROFILER_H_ */
