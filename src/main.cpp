#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

PlayerObject* player_1;
PlayerObject* player_2;
class $modify(PlayerObject)
{
	void preCollision()
	{
		PlayerObject::preCollision();
		auto gameManager = GameManager::sharedState();
		auto gameLayer = gameManager->getGameLayer();
		auto playLayer = gameManager->getPlayLayer();
		if (gameLayer && playLayer)
		{
			if (!player_1) player_1 = gameLayer->m_player1;
			if (!player_2) player_2 = gameLayer->m_player2;
		}
	}
};

class MenuSelectorBypass
{
public:
	void togglePlatformer(CCObject* sender)
	{
		bool value = !(player_1->m_isPlatformer || player_2->m_isPlatformer);
		if (player_1) player_1->togglePlatformerMode(value);
		if (player_2) player_2->togglePlatformerMode(value);
		static_cast<CCMenuItemToggler*>(sender)->toggle(!value);
	}
};

class $modify(PauseLayer)
{
	void onSettings(CCObject* p0)
	{
		PauseLayer::onSettings(p0);
		CCObject* obj;
		CCARRAY_FOREACH(CCDirector::sharedDirector()->getRunningScene()->getChildren(), obj)
		{
			if (auto gameOptions = typeinfo_cast<GameOptionsLayer*>(obj))
			{
				CCSize windowSize = CCDirector::sharedDirector()->getWinSize();
				auto menu = gameOptions->m_buttonMenu;
				if (player_1 || player_2)
				{
					bool value = player_1->m_isPlatformer || player_2->m_isPlatformer;
					auto off = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
					off->setScale(0.8f);
					auto on = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
					on->setScale(0.8f);
					auto toggle = CCMenuItemToggler::create(off, on, gameOptions->m_mainLayer, menu_selector(MenuSelectorBypass::togglePlatformer));
					toggle->toggle(value);
					toggle->setPosition(0, -windowSize.height / 2.75f);
					CCPoint pos = toggle->getPosition();
					menu->addChild(toggle);
					auto label = CCLabelBMFont::create("Platformer", "bigFont-uhd.fnt");
					label->setScale(0.4f);
					label->setPosition(pos.x + (off->getTextureRect().size.width * 1.95f), pos.y);
					menu->addChild(label);
				}
				else
				{
					auto label = CCLabelBMFont::create("Waiting for player to spawn to set platformer.", "bigFont-uhd.fnt");
					label->setScale(0.4f);
					label->setPosition(0, -windowSize.height / 2.75f);
					menu->addChild(label);
				}
			}
		}
	}
};

class $modify(PlayLayer)
{
	bool init(GJGameLevel* p0, bool p1, bool p2)
	{
		if (!PlayLayer::init(p0, p1, p2)) return false;
		if (player_1) player_1 = nullptr;
		if (player_2) player_2 = nullptr;
		return true;
	}
};