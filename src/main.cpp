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
	void customSetup()
	{
		PauseLayer::customSetup();
		if (!player_1 || !player_2) return;
		bool value = player_1->m_isPlatformer || player_2->m_isPlatformer;
		auto menu = CCMenu::create();
		menu->setPosition(0, 0);
		addChild(menu);
		CCSize windowSize = CCDirector::sharedDirector()->getWinSize();
		auto sprite = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
		auto toggle = CCMenuItemToggler::create(sprite, CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"), this, menu_selector(MenuSelectorBypass::togglePlatformer));
		toggle->toggle(value);
		auto background = getChildByID("background");
		toggle->setPosition(sprite->getTextureRect().size.width, windowSize.height - background->getContentSize().height + (sprite->getTextureRect().size.height / 2));
		CCPoint pos = toggle->getPosition();
		menu->addChild(toggle);
		auto label = CCLabelBMFont::create("Platformer", "bigFont-hd.fnt");
		label->setScale(0.9f);
		label->setPosition(pos.x + (label->getContentSize().width / 1.5), pos.y);
		menu->addChild(label);
	}
};

class $modify(PlayLayer)
{
	void destructor()
	{
		PlayLayer::~PlayLayer();
		player_1 = nullptr;
		player_2 = nullptr;
	}
};