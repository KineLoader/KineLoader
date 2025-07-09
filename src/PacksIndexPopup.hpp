#pragma once

#include <Geode/ui/Popup.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include "PackNode.hpp"
#include "BoundedScrollLayer.hpp"
#include "PackManager.hpp"

class PacksIndexPopup : public geode::Popup<> {
protected:
    BoundedScrollLayer* m_availableList{nullptr};

    bool init() override;
    bool setup() override;

    void updateList(
        ScrollLayer* list,
        const std::vector<std::shared_ptr<Pack>>& packs,
        bool resetPos = true
    );
    void updateLists(bool resetPos = true);

public:
    static PacksIndexPopup* create();

    void onDownload(CCObject*);
};