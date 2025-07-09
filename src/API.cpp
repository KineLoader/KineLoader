#include "../include/KineLoader.hpp"

#include "PackManager.hpp"

using namespace geode;

namespace ext = geode::kine_loader;

template <class>
struct ToFilterImpl;

template <class... Args>
struct ToFilterImpl<geode::DispatchEvent<Args...>> {
    using type = geode::DispatchFilter<Args...>;
};

template <class T>
using ToFilter = typename ToFilterImpl<T>::type;

ext::Pack convertPack(std::shared_ptr<Pack> const& pack) {
    ext::Pack res;
    res.id = pack->getID();
    res.name = pack->getDisplayName();
    res.path = pack->getOriginPath();
    res.resourcesPath = pack->getResourcesPath();
    // if the pack has a mod.json
    if (auto opt = pack->getInfo()) {
        auto info = opt.value();
        res.version = info.m_version;
        res.authors = info.m_authors;
    }
    return res;
}

$execute {
    new EventListener(+[](std::vector<ext::Pack>* res) {
        *res = utils::ranges::map<std::vector<ext::Pack>>(PackManager::get()->getAvailablePacks(), convertPack);
        return ListenerResult::Stop;
    }, ToFilter<ext::impl::EventGetAvailablePacks>("oki3ed.kineloader/v1/get-available-packs"));

    new EventListener(+[](std::vector<ext::Pack>* res) {
        *res = utils::ranges::map<std::vector<ext::Pack>>(PackManager::get()->getAppliedPacks(), convertPack);
        return ListenerResult::Stop;
    }, ToFilter<ext::impl::EventGetAppliedPacks>("oki3ed.kineloader/v1/get-applied-packs"));
}