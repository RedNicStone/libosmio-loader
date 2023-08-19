//
// Created by RedNicStone on 14/08/23.
//

#include <cstdint>
#include <istream>
#include <json/value.h>
#include <vector>
#include <filesystem>
#include <unistd.h>
#include <fstream>

#include <sys/types.h>
#include <sys/capability.h>
#include <dlfcn.h>

#include <json/json.h>

#include "icd.h"
#include "icd_json.h"
#include "libosmio/version.h"
#include "entrypoint.h"
#include "tables.h"


#ifndef OSMIO_CONFDIR
#define OSMIO_CONFDIR "osmio/icd.d"
#endif

#ifndef OSMIO_SYSCONFDIR
#define OSMIO_SYSCONFDIR "/etc"

#if !defined(OSMIO_EXTRASYSCONFDIR)
#define OSMIO_EXTRASYSCONFDIR "/etc"
#endif

#endif


std::vector<std::filesystem::path> osmio_global_load_path = {
    OSMIO_SYSCONFDIR,
#ifdef OSMIO_EXTRASYSCONFDIR
    OSMIO_EXTRASYSCONFDIR,
#endif
};
std::vector<std::pair<const char*, const char*>> osmio_global_load_path_evar = {
        // {ENV_VAR, FALLBACK}
        {"XDG_CONFIG_DIRS", "/etc/xdg" },
        {"XDG_DATA_DIRS", "/usr/local/share/:/usr/share/" },
};
std::vector<std::pair<const char*, const char*>> osmio_local_load_path_evar = {
        // {ENV_VAR, FALLBACK}
        {"XDG_CONFIG_HOME", "/.config" },
        {"XDG_DATA_HOME", "/.local/share" },
};


bool has_elevated_privileges() {
    // Check if the effective UID is not equal to the real UID
    if (geteuid() != getuid())
        return true;

    // Check if the effective GID is not equal to the real GID
    if (getegid() != getgid())
        return true;

    // Check for filesystem capabilities
    // todo: get this check working
    /*cap_t caps = cap_get_proc();
    if (caps) {
        if (cap_get_flag(caps, CAP_EFFECTIVE, CAP_NET_BIND_SERVICE, CAP_SET)) {
            cap_free(caps);
            return true;
        }
        cap_free(caps);
    }*/

    return false;
}

void locate_icd_configuration_directories(const std::filesystem::path& directory,
                                          std::vector<std::filesystem::path>& configPaths) {
    // Append our local config path to the absolute path
    const auto osmio_directory = directory / OSMIO_CONFDIR;

    // Make sure the directory exists
    if (!std::filesystem::exists(osmio_directory) || !std::filesystem::is_directory(osmio_directory))
        return;

    // List all regular files that are of json format
    for (const auto& entry : std::filesystem::directory_iterator(osmio_directory))
        if (entry.is_regular_file() && entry.path().extension() == ".json")
            configPaths.push_back(entry.path());
}

void locate_icd_configuration_files(std::vector<std::filesystem::path>& configPaths) {
    // Global load paths specified by SYSCONFIGDIR and EXTRASYSCONFDIR
#ifdef OSMIO_USE_SYSCONFDIR
    for (const auto& load_path : osmio_global_load_path)
        locate_icd_configuration_directories(load_path, configPaths);
#endif

#ifdef OSMIO_USE_ENVSYSCONFDIR
    // Global load paths given by environment variables
    for (const auto& evar_name : osmio_global_load_path_evar) {
        const char* paths;
#ifdef OSMIO_USE_SECURE_GETENV
        paths = secure_getenv(evar_name.first);
#else
        paths = getenv(evar_name);
#endif
        if (!paths)
            paths = evar_name.second;

        std::istringstream paths_stream(paths);
        std::string path;

        while (std::getline(paths_stream, path, ':'))
            locate_icd_configuration_directories(path, configPaths);
    }
#endif

#ifdef OSMIO_USE_ENVUSERCONFDIR
    // Don't proceed beyond if not a user, since we are searching for files in user-controlled paths
    if (has_elevated_privileges())
        return;

    // Local load paths given by environment variables
    for (const auto& evar_name : osmio_local_load_path_evar) {
        const char* paths;
#ifdef OSMIO_USE_SECURE_GETENV
        paths = secure_getenv(evar_name.first);
#else
        paths = getenv(evar_name);
#endif
        bool local = false;
        if (!paths) {
            local = true;
            paths = evar_name.second;
        }

        std::istringstream paths_stream(paths);
        std::string path;

        while (std::getline(paths_stream, path, ':')) {
            if (local) {
                const char* home = secure_getenv("HOME");
                if (!home)
                    continue;

                path.insert(0, home);
            }
            locate_icd_configuration_directories(path, configPaths);
        }
    }
#endif
}

bool parse_icd_configuration_file(ICDJson& icdJson) {
    std::ifstream file(icdJson.jsonPath);
    if (!file.is_open())
        return false;

    Json::CharReaderBuilder reader;
    Json::Value root;
    std::string errs;
    if (!Json::parseFromStream(reader, file, &root, &errs))
        return false;

    // We don't ever want to modify the json
    const Json::Value& const_root(root);

    const auto& file_format_version = const_root["file_format_version"];
    const auto& ICD = const_root["ICD"];

    if (ICD.isNull() || !ICD.isObject() ||
        file_format_version.isNull() || !file_format_version.isString())
        return false;

    if (file_format_version.asString() != "1.0.0")
        return false;

    const auto& library_path = ICD["library_path"];
    const auto& api_version = ICD["api_version"];
    const auto& library_arch = ICD["library_arch"];

    if (library_path.isNull() || !library_path.isString() ||
        api_version.isNull() || !api_version.isString())
        return false;

    if (!library_arch.isNull() && library_arch.isInt()) {
        uint32_t arch;
#if __SIZEOF_POINTER__ == 8
        // 64-bit system
        arch = 64;
#else
        //32-bit system
        arch = 32;
#endif
        if (arch != library_arch.asUInt())
            return false;
    }

    std::istringstream paths_stream(api_version.asString());

    std::vector<uint32_t> version_fragments(4);
    for (auto& fragment : version_fragments) {
        std::string version_blob;
        if (!std::getline(paths_stream, version_blob, '.'))
            break;

        try {
            fragment = std::stoul(version_blob);
        } catch (...) {
            return false;
        }
    }

    icdJson.icdPath = library_path.asString();
    icdJson.apiVersion = OS_MAKE_API_VERSION(version_fragments[0],
                                             version_fragments[1],
                                             version_fragments[2],
                                             version_fragments[3]);
    return true;
}

void enumerate_icd_configurations(std::vector<ICDJson> &icdJsonConfigurations) {
    std::vector<std::filesystem::path> configurationPaths;
    locate_icd_configuration_files(configurationPaths);

    for (const auto& jsonPath : configurationPaths) {
        ICDJson icdConfiguration{};
        icdConfiguration.jsonPath = jsonPath;

        if (!parse_icd_configuration_file(icdConfiguration))
            continue;

        if (!is_regular_file(icdConfiguration.icdPath) && !is_symlink(icdConfiguration.icdPath))
            continue;

        icdJsonConfigurations.push_back(icdConfiguration);
    }
}

bool load_icd(ICDJson &icdJson) {
    icdJson.pICD = dlopen(icdJson.icdPath.c_str(), RTLD_LAZY | RTLD_LOCAL);
    if (!icdJson.pICD)
        return false;

    auto pfn_icdGetGlobalDispatchTable = reinterpret_cast<PFN_os_icdGetGlobalDispatchTable>(
            dlsym(icdJson.pICD, "os_icdGetGlobalDispatchTable"));
    if (!pfn_icdGetGlobalDispatchTable) {
        dlclose(icdJson.pICD);
        icdJson.pICD = nullptr;
        return false;
    }

    Os_icdGlobalDispatchTable* icdGlobalDispatchTable;
    pfn_icdGetGlobalDispatchTable(&icdGlobalDispatchTable);
    icdJson.pICDGlobalDispatchTable = icdGlobalDispatchTable;

    return true;
}

void unload_icd(ICDJson& icdJson) {
    dlclose(icdJson.pICD);
    icdJson.pICD = nullptr;
    icdJson.pICDGlobalDispatchTable = nullptr;
}
