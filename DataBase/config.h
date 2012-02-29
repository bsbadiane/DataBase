#ifndef CONFIG_H
#define CONFIG_H

//#define DEBUG
//#define DEBUG2
//#define DEBUG3
#define MEM_DEBUG
#if defined(DEBUG) ||defined(DEBUG2) || defined (DEBUG3) || defined (MEM_DEBUG)
#include <qdebug.h>
#endif

namespace db {

    //const int recordsPerCircle = 500;
    //const int threadCount = 6;
    const int packNum = 1200000;
}        // namespace db
#endif
