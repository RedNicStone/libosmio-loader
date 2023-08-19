//
// Created by RedNicStone on 16/08/23.
//

#ifndef OSMIO_LOADER_ICD_JSON_H
#define OSMIO_LOADER_ICD_JSON_H

#include <filesystem>
#include <vector>

#include "tables.h"
#include "icd.h"


struct ICDJson : public ICD {
    void* pICD;

    //uint32_t jsonVersion;
    std::filesystem::path jsonPath;
    std::filesystem::path icdPath;
};

void enumerate_icd_configurations(std::vector<ICDJson>& icdJsonConfigurations);

bool load_icd(ICDJson& icdJson);

#endif //OSMIO_LOADER_ICD_JSON_H
