#ifndef CONFIG_H
#define CONFIG_H

//#define DEBUG
#ifdef DEBUG
#include <qdebug.h>
#endif

namespace db {

    //const int recordsPerCircle = 500;
    const int threadCount = 6;
    const int packNum = 1200000;
}        // namespace db
#endif
