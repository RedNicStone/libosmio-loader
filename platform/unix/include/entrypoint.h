//
// Created by RedNicStone on 16/08/23.
//

#ifndef OSMIO_LOADER_ENTRYPOINT_H
#define OSMIO_LOADER_ENTRYPOINT_H

#include "tables.h"


#ifndef OS_DECLARE_FUNCTION_ICD
#define OS_DECLARE_FUNCTION_ICD(returnType, name, ...)                                  \
    typedef returnType (OS_FUNCTION_PTR OS_FUNCTION_PTR_ADD_PREFIX(name))(__VA_ARGS__); \
    returnType name(__VA_ARGS__)
#endif

OS_DECLARE_FUNCTION_ICD(void, os_icdGetGlobalDispatchTable, Os_icdGlobalDispatchTable** ppDispatchTable);

#endif //OSMIO_LOADER_ENTRYPOINT_H
