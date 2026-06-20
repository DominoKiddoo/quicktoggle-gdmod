#pragma once
#include <Geode/Geode.hpp>

class GranularPopup : public geode::Popup {
protected:
    bool init() {
        if (!Popup::init(240.f, 200.f))
            return false;

        this->setTitle("Granular Toggle");
        auto cbfStatus = Loader::get()->isModLoaded("syzzi.click_between_frames");
        this->setID("GranularPopup"_spr);



        // make checkboxes
        auto cbsStatus = GameManager::get()->getGameVariable("0177");
        auto cosStatus = GameManager::get()->getGameVariable("0176"); 
        bool cbfEnabled;

        if (cbfStatus) {
            cbfEnabled = Loader::get()->getLoadedMod("syzzi.click_between_frames")->getSettingValue<bool>("soft-toggle");
            cbfEnabled = !cbfEnabled;
        }

        auto cbsToggler = CCMenuItemToggler::create(CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"), CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"), this, nullptr);
        cbsToggler->setID("cbsToggler");
        m_buttonMenu->addChildAtPosition(cbsToggler, Anchor::Center, {-94, 45});

        auto cosToggler = CCMenuItemToggler::create(CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"), CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"), this, nullptr);
        cosToggler->setID("cosToggler");
        m_buttonMenu->addChildAtPosition(cosToggler, Anchor::Center, {-94, 8});


        auto cbfToggler = CCMenuItemToggler::create(CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"), CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"), this, nullptr);
        cbfToggler->setID("cbfToggler");
        if (cbfStatus) {
            m_buttonMenu->addChildAtPosition(cbfToggler, Anchor::Center, {-94, -28});
        }

        
        // make labels
        auto cbsLabel = CCLabelBMFont::create("Click Between Steps", "bigFont.fnt");
        cbsLabel->setScale(0.5f);
        cbsLabel->setID("cbs-label");
        m_mainLayer->addChildAtPosition(cbsLabel, Anchor::Center, {18, 46});

        auto cosLabel = CCLabelBMFont::create("Click On Steps", "bigFont.fnt");
        cosLabel->setScale(0.5f);
        cosLabel->setID("cos-label");
        m_mainLayer->addChildAtPosition(cosLabel, Anchor::Center, {-5, 8});

        auto cbfLabel = CCLabelBMFont::create("Click Between Frames", "bigFont.fnt");
        cbfLabel->setScale(0.4f);
        cbfLabel->setID("cbf-label");
        if (cbfStatus) {
            m_mainLayer->addChildAtPosition(cbfLabel, Anchor::Center, {6, -28});
        }

        // confirm button

        auto confirmButtonSpr = ButtonSprite::create("Confirm");
        auto confirmButton = CCMenuItemSpriteExtra::create(
            confirmButtonSpr, this, menu_selector(GranularPopup::onConfirm)
        );


        m_buttonMenu->addChildAtPosition(confirmButton, Anchor::Center, {0, -75});
        confirmButton->setID("confirm-button");


        cbsToggler->toggle(!cbsStatus);
        cosToggler->toggle(!cosStatus);

        if (cbfStatus) {
            cbfToggler->toggle(!cbfEnabled);

        }

        return true;
    };
    
    void onConfirm(CCObject* sender) {        
        auto bGLayer = GJBaseGameLayer::get();
        auto cbfStatus = Loader::get()->isModLoaded("syzzi.click_between_frames");


        auto cosToggler = static_cast<CCMenuItemToggler*>(m_buttonMenu->getChildByID("cosToggler"));
        auto cbsToggler = static_cast<CCMenuItemToggler*>(m_buttonMenu->getChildByID("cbsToggler"));

        GameManager::get()->setGameVariable("0176", !cosToggler->isToggled()); //cos
		GameManager::get()->setGameVariable("0177", !cbsToggler->isToggled()); // cbs 

        bGLayer->m_clickOnSteps = !cosToggler->isToggled();
		bGLayer->m_clickBetweenSteps = !cbsToggler->isToggled();

        if (cbfStatus) {
            auto cbfToggler = static_cast<CCMenuItemToggler*>(m_buttonMenu->getChildByID("cbfToggler"));
            Loader::get()->getLoadedMod("syzzi.click_between_frames")->setSettingValue<bool>("soft-toggle", cbfToggler->isToggled());

        }

        this->onClose(this);
    }

public:
    static GranularPopup* create() {
        auto ret = new GranularPopup();
        if (ret->init()) {
            ret->autorelease();
            return ret;
        }
        
        delete ret;
        return nullptr;
    }


};
