//
// Created by RedNicStone on 17/08/23.
//

#include <iostream>

#include "icd_json.h"


int main() {
    std::vector<ICDJson> configurations;
    enumerate_icd_configurations(configurations);

    std::cout << "Found " << configurations.size() << " registered ICD(s).\n\n";

    for (auto& config : configurations) {
        std::cout << "Loading ICD at " << config.icdPath << "\n";

        auto result = load_icd(config);
        std::cout << "  - Importing symbols " << (result ? "succeeded" : "failed") << "\n\n";
    }
}
