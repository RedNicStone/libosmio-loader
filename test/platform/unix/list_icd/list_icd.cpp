//
// Created by RedNicStone on 17/08/23.
//

#include <iostream>

#include "icd_json.h"


int main() {
    std::vector<ICDJson> configurations;
    enumerate_icd_configurations(configurations);

    std::cout << "Found " << configurations.size() << " registered ICD(s).\n\n";

    for (const auto& config : configurations) {
        std::cout << "Entry at " << config.jsonPath << ":\n";
        std::cout << "\tAPI version: "
                << OS_API_VERSION_VARIANT(config.apiVersion) << '.'
                << OS_API_VERSION_MAJOR(config.apiVersion)   << '.'
                << OS_API_VERSION_MINOR(config.apiVersion)   << '.'
                << OS_API_VERSION_PATCH(config.apiVersion)   << '\n';
        std::cout << "\tICD location: " << config.icdPath << '\n';
        std::cout << '\n';
    }
}
