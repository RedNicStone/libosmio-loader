//
// Created by RedNicStone on 17/08/23.
//

#include <libosmio.h>
#include <new>
#include <vector>

#include "icd_json.h"
#include "libosmio/enums.h"
#include "version.h"


#undef OS_DECLARE_FUNCTION_GLOBAL
#define OS_DECLARE_FUNCTION_GLOBAL(returnType, name, ...)                               \
    returnType name(__VA_ARGS__)


// OsVersion
OS_DECLARE_FUNCTION_GLOBAL(OsResult, osEnumerateInstanceVersion,
                           OsVersion* pVersion) {
    try {
        std::vector<ICDJson> icdJsonConfigurations;
        enumerate_icd_configurations(icdJsonConfigurations);

        OsVersion maximumVersion = OS_LOADER_PLATFORM_VERSION;
        for (const auto& configuration : icdJsonConfigurations)
            maximumVersion = std::max(maximumVersion, configuration.apiVersion);

        *pVersion = maximumVersion;

        return OS_SUCCESS;
    } catch (const std::bad_alloc&) {
        return OS_ERROR_OUT_OF_MEMORY;
    } catch (...) {
        return OS_ERROR_LOADER_INTERNAL;
    }
}

// OsInstance
OS_DECLARE_FUNCTION_GLOBAL(OsResult, osCreateInstance,
                           OsInstance* pInstance) {

}
OS_DECLARE_FUNCTION_GLOBAL(OsResult, osDestroyInstance,
                           OsInstance instance) {

}

// OsProtocol
OS_DECLARE_FUNCTION_GLOBAL(OsResult, osEnumerateProtocols,
                           OsInstance instance, uint32_t* pProtocolCount, OsProtocol** ppProtocols) {

}
