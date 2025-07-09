#include "PacksIndexPopup.hpp"
#include <Geode/ui/General.hpp>
#include <Geode/ui/Notification.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/binding/ButtonSprite.hpp>

static CCSize LAYER_SIZE = { 230.f, 210.f };

bool PacksIndexPopup::init() {
    if (!Popup<>::initAnchored(440.f, 290.f, "GJ_square02.png"))
        return false;

    GameManager::get()->fadeInMenuMusic();

    this->setID("PacksIndexPopup");
    this->setKeypadEnabled(true);

    auto size = m_mainLayer->getContentSize();

    auto title = CCLabelBMFont::create("Download Packs", "goldFont.fnt");
    title->setPosition(size.width / 2, size.height - 20.f);
    title->setID("download-packs-text");
    title->setScale(.8f);
    m_mainLayer->addChild(title);

    auto reloadSpr = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
    reloadSpr->setScale(.7f);
    auto reloadBtn = CCMenuItemSpriteExtra::create(reloadSpr, this, menu_selector(PacksIndexPopup::onDownload));
    reloadBtn->setID("reload-button");
    reloadBtn->setPosition({30, 25});
    m_buttonMenu->addChild(reloadBtn);

    auto downloadSpr = ButtonSprite::create("Download", "goldFont.fnt", "GJ_button_01.png", .8f);
    downloadSpr->setScale(0.9f);
    auto downloadBtn = CCMenuItemSpriteExtra::create(downloadSpr, this, menu_selector(PacksIndexPopup::onDownload));
    downloadBtn->setID("download-button");
    downloadBtn->setPosition({size.width / 2, 25});
    m_buttonMenu->addChild(downloadBtn);

    float distanceFromCenter = 0.f;

    float heightOffset = -5.f;
    float scale = .88f;

    auto availableTitle = CCLabelBMFont::create("Available", "bigFont.fnt");
    availableTitle->setPosition(
        size / 2 + CCPoint { distanceFromCenter, LAYER_SIZE.height / 2 + heightOffset }
    );
    availableTitle->setScale(.65f);
    availableTitle->setID("available-text");
    m_mainLayer->addChild(availableTitle);

    auto availableListBG = CCScale9Sprite::create(
        "square02b_001.png", { 0, 0, 80, 80 }
    );
    availableListBG->setColor({ 0, 0, 0 });
    availableListBG->setOpacity(90);
    availableListBG->setScale(scale);
    availableListBG->setContentSize(LAYER_SIZE);
    availableListBG->setPosition(size / 2 + CCPoint { distanceFromCenter, heightOffset });
    availableListBG->setID("available-list-background");
    m_mainLayer->addChild(availableListBG);

    m_availableList = BoundedScrollLayer::create(LAYER_SIZE * scale);
    m_availableList->m_contentLayer->setLayout(
        SimpleColumnLayout::create()
            ->setMainAxisDirection(AxisDirection::TopToBottom)
            ->setMainAxisAlignment(MainAxisAlignment::Start)
            ->setMainAxisScaling(AxisScaling::Grow)
    );
    m_availableList->setPosition(
        size / 2 + CCPoint { distanceFromCenter, heightOffset } - LAYER_SIZE * scale / 2
    );
    m_availableList->setID("available-list");
    m_mainLayer->addChild(m_availableList);

    this->updateLists();

    return true;
}

bool PacksIndexPopup::setup() {
    return true;
}

void PacksIndexPopup::updateList(
    ScrollLayer* list,
    std::vector<std::shared_ptr<Pack>> const& packs,
    bool resetPos
) {
    list->m_contentLayer->removeAllChildren();
    for (auto& pack : packs) {
        auto node = PackNode::create(this, pack, LAYER_SIZE.width);
        node->setScale(.88f);
        list->m_contentLayer->addChild(node);
    }

    list->m_contentLayer->setContentHeight(list->getContentHeight());
    list->m_contentLayer->updateLayout();
    if (resetPos) {
        list->moveToTop();
    }
}

void PacksIndexPopup::updateLists(bool resetPos) {
    this->updateList(m_availableList, PackManager::get()->getAvailablePacks(), resetPos);
}

void PacksIndexPopup::onDownload(CCObject*) {
    Notification::create("Download clicked!", NotificationIcon::Info, 1.f)->show();
}

PacksIndexPopup* PacksIndexPopup::create() {
    auto ret = new PacksIndexPopup();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}