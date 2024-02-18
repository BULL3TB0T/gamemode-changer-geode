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

CCSprite* sprite;
CCSprite* createSprite()
{
	sprite = CCSprite::createWithSpriteFrameName(player_1->m_isPlatformer || player_2->m_isPlatformer ? "GJ_checkOn_001.png" : "GJ_checkOff_001.png");
	return sprite;
}
void updateSprite()
{
	auto parent = sprite->getParent();
	auto pos = sprite->getPosition();
	parent->removeChild(sprite, true);
	auto sprite = createSprite();
	sprite->setPosition(pos);
	parent->addChild(sprite);
}

class MenuSelectorBypass
{
public:
	void togglePlatformer(CCObject*)
	{
		bool value = !(player_1->m_isPlatformer || player_2->m_isPlatformer);
		if (player_1) player_1->togglePlatformerMode(value);
		if (player_2) player_2->togglePlatformerMode(value);
		updateSprite();
	}
};

class $modify(PauseLayer)
{
	void customSetup()
	{
		PauseLayer::customSetup();
		if (!player_1 || !player_2) return;
		auto menu = getChildByID("left-button-menu");
		auto sprite = createSprite();
		auto btn = CCMenuItemSpriteExtra::create(sprite, this, menu_selector(MenuSelectorBypass::togglePlatformer));
		btn->setPosition(menu->getContentSize().width / 2, menu->getContentSize().height - (sprite->getTextureRect().size.height / 2));
		auto pos = btn->getPosition();
		menu->addChild(btn);
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