/*
 * Singleton.h
 *
 *  Created on: 29.02.2012
 *      Author: night
 */

#ifndef SINGLETON_H_
#define SINGLETON_H_
#include <cstdio>
#include <QtCore>


namespace db {

	template<class T>
	class Singleton {
	public:
		typedef T instanceType;

		static instanceType& instance() {
			if (_instance == NULL) {
				QMutexLocker lock(&_mutex);
				if (_instance == NULL) {
					create();
				}
				lock.unlock();
			}
			return *_instance;
		}

	private:
		static T* _instance;
		static QMutex _mutex;

		static void create() {
			static instanceType instance;
			_instance = &instance;
		}

	};

	template<class T>
		T* Singleton<T>::_instance = NULL;
	template<class T>
		QMutex Singleton<T>::_mutex;
}

#endif /* SINGLETON_H_ */
