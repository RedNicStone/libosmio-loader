//
// Created by RedNicStone on 16/08/23.
//

#ifndef OSMIO_LOADER_ICD_H
#define OSMIO_LOADER_ICD_H

#include "libosmio.h"
#include "tables.h"


struct ICD {
    Os_icdGlobalDispatchTable* pICDGlobalDispatchTable;

    OsVersion apiVersion;
};


#endif //OSMIO_LOADER_ICD_H
