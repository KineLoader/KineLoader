#pragma once

#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/ui/Popup.hpp>
#include "Pack.hpp"
#include <memory>

using namespace geode::prelude;

class PackNode : public CCNode {
protected:
    geode::Popup<>* m_layer;
    std::shared_ptr<Pack> m_pack;
    CCScale9Sprite* m_draggingBg;

    bool init(
        geode::Popup<>* layer,
        const std::shared_ptr<Pack>& pack,
        float width
    );

    void onView(CCObject*);

public:
    static PackNode* create(
        geode::Popup<>* layer,
        const std::shared_ptr<Pack>& pack,
        float width
    );

    std::shared_ptr<Pack> getPack() { return m_pack; }

    static constexpr float HEIGHT = 35.f * .88f;
};