#include "PackNode.hpp"
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/ui/LazySprite.hpp>
#include "PackManager.hpp"
#include "InfoPopup.hpp"
#include "SelectPackPopup.hpp"
#include "DragThingy.hpp"

bool PackNode::init(
    geode::Popup<>* layer,
    const std::shared_ptr<Pack>& pack,
    float width
) {
    if (!CCNode::init())
        return false;

    constexpr float HEIGHT = PackNode::HEIGHT / .88f;
    constexpr float SPACE_FOR_MENU = 50.f;
    constexpr float MOVE_OFFSET = 20.f;
    constexpr float SPACE_FOR_LOGO = HEIGHT;
    constexpr float PADDING = 5.f;

    float labelWidth = width - SPACE_FOR_MENU - SPACE_FOR_LOGO;
    float menuPosX = width - SPACE_FOR_MENU + MOVE_OFFSET;

    m_pack = pack;
    m_layer = layer;

    this->setID("PackNode");

    this->setContentSize({ width, HEIGHT });

    auto menu = CCMenu::create();
    menu->setPosition({0, 0});
    menu->setID("pack-button-menu");
    menu->setContentSize(this->getContentSize());

    auto logoSize = CCSize { HEIGHT - PADDING * 2, HEIGHT - PADDING * 2 };
    auto logo = geode::LazySprite::create(logoSize, true);
    logo->setLoadCallback([this, logo, logoSize](Result<> res) {
        if (res.isErr()) {
            logo->CCSprite::initWithFile("noLogo.png"_spr);
            bool transparent = Mod::get()->getSettingValue<bool>("transparency-nologo");
            logo->setOpacity(transparent ? 100 : 255);
        }
        limitNodeSize(logo, logoSize, 1.f, .1f);
    });
    logo->loadFromFile((pack->getResourcesPath() / "pack.png"));
    logo->setPosition({ SPACE_FOR_LOGO / 2 + PADDING, HEIGHT / 2 });
    this->addChild(logo);

    logo->setID("pack-logo");

    auto nameLabel = CCLabelBMFont::create(
        m_pack->getDisplayName().c_str(), "bigFont.fnt"
    );
    nameLabel->limitLabelWidth(125.f, 0.40f, 0.1f);
    nameLabel->setPositionX(0);
    nameLabel->setAnchorPoint({0, 0.5f});
    nameLabel->setID("pack-name-text");

    auto nameButton = CCMenuItemSpriteExtra::create(
        nameLabel, this, menu_selector(PackNode::onView)
    );
    nameButton->setPosition(
        PADDING + SPACE_FOR_LOGO + nameLabel->getScaledContentSize().width / 2 - menuPosX,
        HEIGHT / 2
    );

    nameButton->setID("pack-name-button");
    nameButton->setContentWidth(nameLabel->getScaledContentWidth());
    nameButton->setEnabled(false);
    menu->addChild(nameButton);

    auto applyArrowSpr = CCSprite::create("dragIcon.png"_spr);
    applyArrowSpr->setScale(.6f);

    DragThingy* dragHandle = DragThingy::create(
        [=, this] {
            m_draggingBg->setVisible(true);
            if (auto selectLayer = dynamic_cast<SelectPackPopup*>(m_layer)) {
                selectLayer->startDragging(this);
            }
        },
        [=, this] (const CCPoint& offset) {
            if (auto selectLayer = dynamic_cast<SelectPackPopup*>(m_layer)) {
                selectLayer->moveDrag(offset);
            }
        },
        [=, this] {
            m_draggingBg->setVisible(false);
            if (auto selectLayer = dynamic_cast<SelectPackPopup*>(m_layer)) {
                selectLayer->stopDrag();
            }
        }
    );

    if (!m_pack->getInfo().has_value()) {
        nameButton->setPosition({40 + nameButton->getContentWidth()/2, this->getContentHeight()/2});
    }
    else {
        PackInfo packInfo = m_pack->getInfo().value();
        CCLabelBMFont* extraInfoLabel = CCLabelBMFont::create(packInfo.m_version.toNonVString().c_str(), "chatFont.fnt");
        extraInfoLabel->setColor({165, 165, 165});
        extraInfoLabel->limitLabelWidth(125.f, 0.2f, 0.1f);
        extraInfoLabel->setScale(0.45f);
        extraInfoLabel->setAnchorPoint({0, 0.5f});
        extraInfoLabel->setOpacity(165);
        extraInfoLabel->setPosition({40, 8});
        extraInfoLabel->setZOrder(-1);
        extraInfoLabel->setID("extra-info-text");

        this->addChild(extraInfoLabel);

        CCLabelBMFont* authorLabel = CCLabelBMFont::create(packInfo.m_authors.at(0).c_str(), "goldFont.fnt");
        authorLabel->limitLabelWidth(125.f, 0.3f, 0.1f);
        authorLabel->setAnchorPoint({0, 0.5f});
        authorLabel->setPosition({40.2f, 16});
        authorLabel->setZOrder(-1);
        authorLabel->setScale(0.3f);
        authorLabel->setID("author-text");
        this->addChild(authorLabel);

        nameButton->setPosition({40 + nameButton->getContentWidth()/2, this->getContentHeight() - 9.5f});
    }

    applyArrowSpr->setAnchorPoint({0, 0});
    dragHandle->addChild(applyArrowSpr);
    dragHandle->setContentSize(applyArrowSpr->getScaledContentSize());
    dragHandle->setID("apply-pack-button");
    dragHandle->setPosition(width - MOVE_OFFSET, HEIGHT / 2.f);
    dragHandle->setTouchPriority(-130);

    this->addChild(dragHandle);

    m_draggingBg = CCScale9Sprite::create(
        "square02b_001.png"
    );
    m_draggingBg->setCapInsets({10, 10, 50, 50});
    m_draggingBg->setColor({ 0, 0, 0 });
    m_draggingBg->setOpacity(90);
    m_draggingBg->setContentSize(this->getContentSize());
    m_draggingBg->setPosition({ width / 2.f, HEIGHT / 2.f });
    m_draggingBg->setZOrder(-10);
    m_draggingBg->setVisible(false);

    this->addChild(m_draggingBg);
    this->addChild(menu);

    return true;
}

void PackNode::onView(CCObject*) {
    auto popup = InfoPopup::create(m_pack);
    if (popup) {
        popup->show();
    }
}

PackNode* PackNode::create(
    geode::Popup<>* layer,
    const std::shared_ptr<Pack>& pack,
    float width
) {
    auto ret = new PackNode;
    if (ret && ret->init(layer, pack, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}