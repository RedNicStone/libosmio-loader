//
// Created by RedNicStone on 23/03/23.
//

#ifndef OSMIO_STRUCTURES_H
#define OSMIO_STRUCTURES_H

#include "../../headers/include/core.h"

struct OsPhysicalInterface_T {
    OsICD* icd;
};

struct OsInterface_T  {
    OsDispatchTable* dispatchTable;
};

#endif //OSMIO_STRUCTURES_H
