#include "LayerOver.h"
#include "LayerGameMain.h"
#include "AppMacros.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

bool LayerOver::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	SimpleAudioEngine::getInstance()->playEffect("sound/game_over.wav");

	CCSprite * over = CCSprite::createWithSpriteFrameName("gameover.png");
	over->setAnchorPoint(ccp(0, 0));
	over->setPosition(ccp(0, 0));
	addChild(over);

	CCString *strScore = CCString::createWithFormat("%d", LayerGameMain::getScore());
	CCLabelBMFont* finalScore = CCLabelBMFont::create();
	finalScore->setString(strScore->_string);
	finalScore->setFntFile("font/font.fnt");
	finalScore->setColor(ccc3(255, 0, 0)); // ccc3(143, 146, 147)
	finalScore->setAnchorPoint(ccp(0, 0));
	finalScore->setPosition(ccp(winSize.width/2, winSize.height/2));
	addChild(finalScore);

	CCMenuItem * redoItem = CCMenuItemFont::create("Redo", this, menu_selector(LayerOver::redoCallback));
	redoItem->setColor(ccc3(0, 0, 0));
	CCMenu * menu = CCMenu::create(redoItem, NULL);
	menu->setPosition(ccp(winSize.width-40, 20)); // fine tune
	addChild(menu);

	return true;
}

CCScene * LayerOver::scene()
{
	CCScene * scene = CCScene::create();
	LayerOver * layer = LayerOver::create();
	scene->addChild(layer);
	return scene;
}

void LayerOver::redoCallback(CCObject * obj)
{
	CCScene * scene = LayerGameMain::scene();
	CCDirector::sharedDirector()->replaceScene(scene);
}
