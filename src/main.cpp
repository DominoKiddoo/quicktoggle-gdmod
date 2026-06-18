#include <Geode/Geode.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>

using namespace geode::prelude;

#include <Geode/modify/PauseLayer.hpp>
class $modify(MyPauseLayer, PauseLayer) {
	void customSetup() {
		PauseLayer::customSetup();

		auto spr = CircleButtonSprite::createWithSprite("toggleButton.png"_spr);

		spr->setScale(0.7f);

		auto toggleBtn = CCMenuItemSpriteExtra::create(
			spr, this, menu_selector(MyPauseLayer::onToggle)

		);

		auto bottomMenu = this->getChildByID("bottom-button-menu");

		if (bottomMenu) {
			bottomMenu->addChild(toggleBtn);
			bottomMenu->updateLayout();
		}

	}
	void onToggle(CCObject*) {
		
		auto hasCBF = Loader::get()->isModLoaded("syzzi.click_between_frames");

		bool cbfStatus;

		if (hasCBF) {
			cbfStatus = Loader::get()->getLoadedMod("syzzi.click_between_frames")->getSettingValue<bool>("soft-toggle");

			Loader::get()->getLoadedMod("syzzi.click_between_frames")->setSettingValue<bool>("soft-toggle", !cbfStatus);

		}
		
		// for my reference cuz im stupid: gv_0176 = Click On Steps | gv_0177 = Click Between Steps

		auto cosStatus = GameManager::get()->getGameVariable("0176"); 
		auto cbsStatus = GameManager::get()->getGameVariable("0177");

		auto bGLayer = GJBaseGameLayer::get();

		log::info("cbs: {} cos: {}", cbsStatus, cosStatus);


		cosStatus = !cosStatus;
		cbsStatus = !cbsStatus;
		// cbfStatus = !cbfStatus;

		bGLayer->m_clickBetweenSteps = cbsStatus;
		bGLayer->m_clickOnSteps = cosStatus;

		GameManager::get()->setGameVariable("0176", cosStatus); 
		GameManager::get()->setGameVariable("0177", cbsStatus); 
		
		std::string notifText;


		if (hasCBF) {
			notifText = fmt::format("Toggled | CBF = {} | CBS: {} | COS {}", cbfStatus, cbsStatus, cosStatus);
		} else {
			notifText = fmt::format("Toggled | CBS: {} | COS {}", cbsStatus, cosStatus);
		}

		geode::Notification::create(notifText, geode::NotificationIcon::Success, 3.0f)->show();

	}
};