#ifndef CONFIG_H
#define CONFIG_H

//#define DEBUG
//#define DEBUG2
//#define DEBUG3
//#define MEM_DEBUG
#define DEBUG4
#if defined(DEBUG) ||defined(DEBUG2) || defined (DEBUG3) || defined (MEM_DEBUG) ||defined(DEBUG4)
#include <qdebug.h>
#endif

namespace db {

    //const int recordsPerCircle = 500;
    //const int threadCount = 6;
    //int packNum = 1200000;
    //int packToRead = 1000000;
}        // namespace db
#endif
