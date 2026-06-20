#include <Geode/Geode.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>


using namespace geode::prelude;

#include <Geode/modify/PauseLayer.hpp>
#include "granularPopup.hpp" 
#include <Geode/ui/Notification.hpp>

class $modify(MyPauseLayer, PauseLayer) {


	void customSetup() {
		PauseLayer::customSetup();

		auto spr = CircleButtonSprite::createWithSprite("toggleButton.png"_spr);

		spr->setScale(0.7f);

		auto toggleBtn = CCMenuItemSpriteExtra::create(
			spr, this, menu_selector(MyPauseLayer::onToggle)

		);
		toggleBtn->setID("toggle-button"_spr);
		auto bottomMenu = this->getChildByID("bottom-button-menu");

		if (bottomMenu) {
			bottomMenu->addChild(toggleBtn);
			bottomMenu->updateLayout();
		}

	}
	void onToggle(CCObject*) {
		if (Mod::get()->getSettingValue<std::string>("toggle-type") == "Toggle") {
			auto hasCBF = Loader::get()->isModLoaded("syzzi.click_between_frames");

			bool cbfStatus;

			// for my reference cuz im stupid: gv_0176 = Click On Steps | gv_0177 = Click Between Steps
			auto cosStatus = GameManager::get()->getGameVariable("0176"); 
			auto cbsStatus = GameManager::get()->getGameVariable("0177");

			auto bGLayer = GJBaseGameLayer::get();

			if (hasCBF) {
				cbfStatus = Loader::get()->getLoadedMod("syzzi.click_between_frames")->getSettingValue<bool>("soft-toggle");
				cbfStatus = !cbfStatus; // cuz cbf is a toggle for if its disabled not enabled

				if (cosStatus == false && cbsStatus == false && cbfStatus == false) {  // all off (toggle on)
					if (Mod::get()->getSavedValue<std::string>("last-saved") == "") {
						log::info("nothing previously set. setting to default (all on :3)");
						GameManager::get()->setGameVariable("0176", true); 
						GameManager::get()->setGameVariable("0177", true);
						bGLayer->m_clickBetweenSteps = true;
						bGLayer->m_clickOnSteps = true;
						Loader::get()->getLoadedMod("syzzi.click_between_frames")->setSettingValue<bool>("soft-toggle", false);

					} else {
						// ORDER: cbs, cos, cbf
						auto lastSaved = Mod::get()->getSavedValue<std::string>("last-saved");
						log::info("{}", lastSaved);

						// cbs
						if (lastSaved.at(0) == 't') {
							GameManager::get()->setGameVariable("0177", true);
						} else {
							GameManager::get()->setGameVariable("0177", false);
						}

						// cos
						if (lastSaved.at(1) == 't') {
							GameManager::get()->setGameVariable("0176", true);
						} else {
							GameManager::get()->setGameVariable("0176", false);
						}

						// cbf
						if (lastSaved.at(2) == 't') {
							Loader::get()->getLoadedMod("syzzi.click_between_frames")->setSettingValue<bool>("soft-toggle", false);
						} else {
							Loader::get()->getLoadedMod("syzzi.click_between_frames")->setSettingValue<bool>("soft-toggle", true);
						}
						


					}
				} else { // in the on state, turn off (and save vals ofc)

					std::string saveString = std::string(cbsStatus ? "t" : "f") + (cosStatus ? "t" : "f") + (cbfStatus ? "t" : "f");

					Mod::get()->setSavedValue<std::string>("last-saved", saveString);
					GameManager::get()->setGameVariable("0177", false);
					GameManager::get()->setGameVariable("0176", false);
					Loader::get()->getLoadedMod("syzzi.click_between_frames")->setSettingValue<bool>("soft-toggle", true);
				}


				// had to do this cuz i messed up the values earlier and cant be bothered to figure out why
				bool currentCos = GameManager::get()->getGameVariable("0176");
				bool currentCbs = GameManager::get()->getGameVariable("0177");

				bool cbfSetting = Loader::get()->getLoadedMod("syzzi.click_between_frames")->getSettingValue<bool>("soft-toggle");
				cbfSetting = !cbfSetting;
				auto notifText = fmt::format("CBS: {} | COS: {} | CBF: {}", currentCbs ? "ON" : "OFF", currentCos ? "ON" : "OFF", cbfSetting ? "ON" : "OFF");
				
				geode::Notification* oldNotif = static_cast<geode::Notification*>(OverlayManager::get()->getChildByID("old-notif"_spr));
				if (oldNotif) {
					oldNotif->hide();
				}
				

				Loader::get()->queueInMainThread([this, notifText]() {
					auto notification = geode::Notification::create(notifText, geode::NotificationIcon::Success, 1.5f);
					notification->setID("old-notif"_spr);
					notification->show();
				});	

			} else { // if no cbf installed


				if (cosStatus == false && cbsStatus == false) {  // all off (toggle on)
					if (Mod::get()->getSavedValue<std::string>("last-saved") == "") {
						log::info("nothing previously set. setting to default (all on :3)");
						GameManager::get()->setGameVariable("0176", true); 
						GameManager::get()->setGameVariable("0177", true);
						bGLayer->m_clickBetweenSteps = true;
						bGLayer->m_clickOnSteps = true;

					} else {
						// ORDER: cbs, cos
						auto lastSaved = Mod::get()->getSavedValue<std::string>("last-saved");
						log::info("{}", lastSaved);

						// cbs
						if (lastSaved.at(0) == 't') {
							GameManager::get()->setGameVariable("0177", true);
						} else {
							GameManager::get()->setGameVariable("0177", false);
						}

						// cos
						if (lastSaved.at(1) == 't') {
							GameManager::get()->setGameVariable("0176", true);
						} else {
							GameManager::get()->setGameVariable("0176", false);
						}


					}
				} else { // in the on state, turn off (and save vals ofc)

					std::string saveString = std::string(cbsStatus ? "t" : "f") + (cosStatus ? "t" : "f");

					Mod::get()->setSavedValue<std::string>("last-saved", saveString);
					GameManager::get()->setGameVariable("0177", false);
					GameManager::get()->setGameVariable("0176", false);
				}

				bool currentCos = GameManager::get()->getGameVariable("0176");
				bool currentCbs = GameManager::get()->getGameVariable("0177");

				auto notifText = fmt::format("CBS: {} | COS: {}", currentCbs ? "ON" : "OFF", currentCos ? "ON" : "OFF");
				
				geode::Notification* oldNotif = static_cast<geode::Notification*>(OverlayManager::get()->getChildByID("old-notif"_spr));
				if (oldNotif) {
					oldNotif->hide();
				}
				

				Loader::get()->queueInMainThread([this, notifText]() {
					auto notification = geode::Notification::create(notifText, geode::NotificationIcon::Success, 1.5f);
					notification->setID("old-notif"_spr);
					notification->show();
				});	


			}
			
		} else {
			auto popup = GranularPopup::create();
			popup->show();
		}
	}
};