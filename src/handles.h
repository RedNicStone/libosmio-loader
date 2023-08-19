//
// Created by RedNicStone on 16/08/23.
//

#ifndef OSMIO_LOADER_HANDLES_H
#define OSMIO_LOADER_HANDLES_H

#include <libosmio.h>

#include "icd.h"


struct OsInstance_T {
    OsInstance handle;
};

struct OsProtocol_T {
    OsProtocol handle;
    ICD* icd;
};

struct OsPhysicalInterface_T {
    OsPhysicalInterface handle;
    ICD* icd;
};

struct OsInterface_T {
    OsInterface handle;
    ICD* icd;
};

struct OsDevice_T {
    OsDevice handle;
    ICD* icd;
};

struct OsBuffer_T {
    OsBuffer handle;
    ICD* icd;
};

struct OsCallback_T {
    OsCallback handle;
    ICD* icd;
};

struct OsTransaction_T {
    OsTransaction handle;
    ICD* icd;
};

#endif //OSMIO_LOADER_HANDLES_H
