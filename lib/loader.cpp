//
// Created by RedNicStone on 23/03/23.
//

#include <libosmio.h>
#include "structures.h"

#include <vector>

std::vector<OsICD*> registeredICDs;

void osRegisterICD(OsICD* icd) {
    if (icd == nullptr)
        return;

    registeredICDs.push_back(icd);
}

OsResult osCreateInterface(OsInterfaceCreateInfo physicalInterface, OsInterface* pInterface) {
    OsICD* icd = physicalInterface.physicalInterface->icd;
    if (icd == nullptr)
        return OS_ICD_NOT_FOUND;

    icd->functionRegister();

    OsResult result = icd->dispatchTable.osCreateInterface(physicalInterface, pInterface);
    if (result != OS_SUCCESS) {
        icd->functionDeregister();
        return result;
    }

    (*pInterface)->dispatchTable = &icd->dispatchTable;
    return OS_SUCCESS;
}
