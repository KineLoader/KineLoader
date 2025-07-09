#pragma once

#include <Geode/ui/Popup.hpp>
#include "Pack.hpp"

using namespace geode::prelude;

class InfoPopup : public Popup<std::shared_ptr<Pack>> {
protected:
    std::shared_ptr<Pack> m_pack;

    bool setup(std::shared_ptr<Pack> pack) override;

    std::filesystem::path getPathInPack(const char* filename) const;

public:
    static InfoPopup* create(const std::shared_ptr<Pack>& pack);
};
