#pragma once

#include <Geode/loader/Dispatch.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/utils/VersionInfo.hpp>

#include <filesystem>
#include <vector>
#include <string>

namespace geode::kine_loader {

inline bool isLoaded() {
    return geode::Loader::get()->isModLoaded("oki3ed.kineloader");
}

struct Pack {
    std::string id;
    std::string name;
    VersionInfo version;
    std::vector<std::string> authors;

    std::filesystem::path path;
    std::filesystem::path resourcesPath;
};

namespace impl {
    using EventGetAvailablePacks = geode::DispatchEvent<std::vector<Pack>*>;
    using EventGetAppliedPacks = geode::DispatchEvent<std::vector<Pack>*>;
}

inline std::vector<Pack> getAvailablePacks() {
    std::vector<Pack> result;
    impl::EventGetAvailablePacks("oki3ed.kineloader/v1/get-available-packs", &result).post();
    return result;
}

inline std::vector<Pack> getAppliedPacks() {
    std::vector<Pack> result;
    impl::EventGetAppliedPacks("oki3ed.kineloader/v1/get-applied-packs", &result).post();
    return result;
}

}