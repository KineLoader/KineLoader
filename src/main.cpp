#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include "SelectPackPopup.hpp"
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/IDManager.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>

using namespace geode::prelude;

class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init())
            return false;

        NodeIDs::provideFor(this);

        auto menu = this->getChildByID("right-side-menu");

        auto button = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("gj_folderBtn_001.png"),
            this, menu_selector(MyMenuLayer::onPacks)
        );
        button->setID("packs-button"_spr);
        menu->addChild(button);
        menu->updateLayout();

        return true;
    }

    void onPacks(CCObject*) {
        SelectPackPopup::create()->show();
    }
};