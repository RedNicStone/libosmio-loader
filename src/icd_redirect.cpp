//
// Created by RedNicStone on 16/08/23.
//

// Link redirects here :D

#include <new>

#include "handles.h"
#include "libosmio/enums.h"


#undef OS_DECLARE_FUNCTION_INSTANCE
#define OS_DECLARE_FUNCTION_INSTANCE(returnType, name, ...)                             \
    returnType name(__VA_ARGS__)


template<typename R, typename H, typename ...Ts>
inline R redirect_icd_call(R (pfn_icd)(H, Ts...), H handle, Ts... args) {
    return pfn_icd(handle->handle, args...);
}

template<typename T>
inline OsResult wrap_handle(T** old_handle) {
    T* new_handle = new (std::nothrow) T;
    if (!new_handle)
        return OS_ERROR_OUT_OF_MEMORY;

    new_handle->handle = *old_handle;
    *old_handle = new_handle;
    return OS_SUCCESS;
}


// OsProtocol
OS_DECLARE_FUNCTION_INSTANCE(OsResult, osGetProtocolProperties,
                             OsProtocol protocol, OsProtocolProperties* pProtocolProperties) {
    auto pfn_icd = protocol->icd->pICDGlobalDispatchTable->osGetProtocolProperties;
    return redirect_icd_call(pfn_icd, protocol, pProtocolProperties);
}

// OsPhysicalInterface
OS_DECLARE_FUNCTION_INSTANCE(OsResult, osEnumeratePhysicalInterfaces,
                             OsProtocol protocol, uint32_t* pPhysicalInterfaceCount,
                             OsPhysicalInterface** ppPhysicalInterfaces)  {
    auto pfn_icd = protocol->icd->pICDGlobalDispatchTable->osEnumeratePhysicalInterfaces;
    return redirect_icd_call(pfn_icd, protocol, pPhysicalInterfaceCount, ppPhysicalInterfaces);
}
OS_DECLARE_FUNCTION_INSTANCE(OsResult, osGetPhysicalDeviceProperties,
                             OsPhysicalInterface physicalInterface,
                             OsPhysicalInterfaceProperties* pPhysicalInterfaceProperties)  {
    auto pfn_icd = physicalInterface->icd->pICDGlobalDispatchTable->osGetPhysicalDeviceProperties;
    return redirect_icd_call(pfn_icd, physicalInterface, pPhysicalInterfaceProperties);
}

// OsInterface
OS_DECLARE_FUNCTION_INSTANCE(OsResult, osCreateInterface,
                             OsPhysicalInterface physicalInterface, OsInterfaceCreateInfo* interfaceCreateInfo,
                             OsInterface* pInterface)  {
    auto pfn_icd = physicalInterface->icd->pICDGlobalDispatchTable->osCreateInterface;
    auto result = redirect_icd_call(pfn_icd, physicalInterface, interfaceCreateInfo, pInterface);
    if (result != OS_SUCCESS)
        return result;

    return wrap_handle(pInterface);
}
OS_DECLARE_FUNCTION_INSTANCE(void, osDestroyInterface,
                             OsInterface interface)  {
    auto pfn_icd = interface->icd->pICDGlobalDispatchTable->osDestroyInterface;
    return redirect_icd_call(pfn_icd, interface);
}

// OsDevice
OS_DECLARE_FUNCTION_INSTANCE(OsResult, osCreateDevice,
                             OsInterface interface, OsDeviceCreateInfo* pCreateInfo, OsDevice* pDevice)  {
    auto pfn_icd = interface->icd->pICDGlobalDispatchTable->osCreateDevice;
    auto result = redirect_icd_call(pfn_icd, interface, pCreateInfo, pDevice);
    if (result != OS_SUCCESS)
        return result;

    return wrap_handle(pDevice);
}
OS_DECLARE_FUNCTION_INSTANCE(void, osDestroyDevice,
                             OsDevice device)  {
    auto pfn_icd = device->icd->pICDGlobalDispatchTable->osDestroyDevice;
    return redirect_icd_call(pfn_icd, device);
}

// OsBuffer
OS_DECLARE_FUNCTION_INSTANCE(OsResult, osAllocateBuffer,
                             OsDevice device, OsBufferCreateInfo* pCreateInfo, OsBuffer* pBuffer)  {
    auto pfn_icd = device->icd->pICDGlobalDispatchTable->osAllocateBuffer;
    auto result = redirect_icd_call(pfn_icd, device, pCreateInfo, pBuffer);
    if (result != OS_SUCCESS)
        return result;

    return wrap_handle(pBuffer);
}
OS_DECLARE_FUNCTION_INSTANCE(void, osFreeBuffer,
                             OsBuffer buffer)  {
    auto pfn_icd = buffer->icd->pICDGlobalDispatchTable->osFreeBuffer;
    return redirect_icd_call(pfn_icd, buffer);
}

// OsCallback
OS_DECLARE_FUNCTION_INSTANCE(void, osEnumerateProtocolCallbacks,
                             OsProtocol protocol, uint32_t* pCallbackCount, OsCallback** ppCallbacks)  {
    auto pfn_icd = protocol->icd->pICDGlobalDispatchTable->osEnumerateProtocolCallbacks;
    return redirect_icd_call(pfn_icd, protocol, pCallbackCount, ppCallbacks);
}
OS_DECLARE_FUNCTION_INSTANCE(void, osEnumerateInterfaceCallbacks,
                             OsInterface interface, uint32_t* pCallbackCount, OsCallback** ppCallbacks)  {
    auto pfn_icd = interface->icd->pICDGlobalDispatchTable->osEnumerateInterfaceCallbacks;
    return redirect_icd_call(pfn_icd, interface, pCallbackCount, ppCallbacks);
}
OS_DECLARE_FUNCTION_INSTANCE(void, osEnumerateDeviceCallbacks,
                             OsDevice device, uint32_t* pCallbackCount, OsCallback** ppCallbacks)  {
    auto pfn_icd = device->icd->pICDGlobalDispatchTable->osEnumerateDeviceCallbacks;
    return redirect_icd_call(pfn_icd, device, pCallbackCount, ppCallbacks);
}
OS_DECLARE_FUNCTION_INSTANCE(void, osEnumerateTransactionCallbacks,
                             OsDevice device, uint32_t* pCallbackCount, OsCallback** ppCallbacks)  {
    auto pfn_icd = device->icd->pICDGlobalDispatchTable->osEnumerateTransactionCallbacks;
    return redirect_icd_call(pfn_icd, device, pCallbackCount, ppCallbacks);
}
OS_DECLARE_FUNCTION_INSTANCE(void, osGetCallbackProperties,
                             OsCallback callback, OsCallbackProperties* pCallbackProperties)  {
    auto pfn_icd = callback->icd->pICDGlobalDispatchTable->osGetCallbackProperties;
    return redirect_icd_call(pfn_icd, callback, pCallbackProperties);
}
OS_DECLARE_FUNCTION_INSTANCE(OsResult, osRegisterCallback,
                             OsCallback callback, OsCallbackRegistryInfo callbackRegistryInfo)  {
    auto pfn_icd = callback->icd->pICDGlobalDispatchTable->osRegisterCallback;
    return redirect_icd_call(pfn_icd, callback, callbackRegistryInfo);
}
OS_DECLARE_FUNCTION_INSTANCE(void, osRemoveCallback,
                             OsCallback callback, void (*pFunction) (void* userData))  {
    auto pfn_icd = callback->icd->pICDGlobalDispatchTable->osRemoveCallback;
    return redirect_icd_call(pfn_icd, callback, pFunction);
}
OS_DECLARE_FUNCTION_INSTANCE(void, osRemoveAllCallbacks,
                             OsCallback callback)  {
    auto pfn_icd = callback->icd->pICDGlobalDispatchTable->osRemoveAllCallbacks;
    return redirect_icd_call(pfn_icd, callback);
}

// OsTransaction
OS_DECLARE_FUNCTION_INSTANCE(void, osGetInterfaceTransactionProperties,
                             OsInterface interface, OsTransactionProperties* pTransactionProperties)  {
    auto pfn_icd = interface->icd->pICDGlobalDispatchTable->osGetInterfaceTransactionProperties;
    return redirect_icd_call(pfn_icd, interface, pTransactionProperties);
}
OS_DECLARE_FUNCTION_INSTANCE(void, osCreateTransaction,
                             OsInterface interface, OsTransactionCreateInfo* pTransactionCreateInfo,
                             OsTransaction* pTransaction)  {
    auto pfn_icd = interface->icd->pICDGlobalDispatchTable->osCreateTransaction;
    return redirect_icd_call(pfn_icd, interface, pTransactionCreateInfo, pTransaction);
}
OS_DECLARE_FUNCTION_INSTANCE(void, osBeginTransaction,
                             OsTransaction transaction)  {
    auto pfn_icd = transaction->icd->pICDGlobalDispatchTable->osBeginTransaction;
    return redirect_icd_call(pfn_icd, transaction);
}
OS_DECLARE_FUNCTION_INSTANCE(void, osWaitForTransaction,
                             OsTransaction transaction)  {
    auto pfn_icd = transaction->icd->pICDGlobalDispatchTable->osWaitForTransaction;
    return redirect_icd_call(pfn_icd, transaction);
}
OS_DECLARE_FUNCTION_INSTANCE(void, osDestroyTransaction,
                             OsTransaction transaction)  {
    auto pfn_icd = transaction->icd->pICDGlobalDispatchTable->osDestroyTransaction;
    return redirect_icd_call(pfn_icd, transaction);
}
