//
// Created by RedNicStone on 16/08/23.
//

#ifndef OSMIO_LOADER_TABLES_H
#define OSMIO_LOADER_TABLES_H

// -------------- //
// Function Table //
// -------------- //

#include <libosmio/functions.h>


#undef OS_DECLARE_FUNCTION_INSTANCE
#define OS_DECLARE_FUNCTION_INSTANCE(type, functionName, ...) \
    CONCATENATE(OS_FUNCTION_PTR_PREFIX, functionName) functionName;

#undef OS_DECLARE_FUNCTION_GLOBAL
#define OS_DECLARE_FUNCTION_GLOBAL(type, functionName, ...) \
    CONCATENATE(OS_FUNCTION_PTR_PREFIX, functionName) functionName;

struct Os_icdGlobalDispatchTable {
#undef LIBOSMIO_FUNCTIONS_H
#include <libosmio/functions.h>
};

#undef OS_DECLARE_FUNCTION_GLOBAL
#define OS_DECLARE_FUNCTION_GLOBAL(type, functionName, ...)

struct Os_icdInstanceDispatchTable {
#undef LIBOSMIO_FUNCTIONS_H
#include <libosmio/functions.h>
};

#endif //OSMIO_LOADER_TABLES_H
