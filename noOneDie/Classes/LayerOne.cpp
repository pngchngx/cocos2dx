#include "LayerOne.h"
#include "BlockItem.h"
#include "LayerOver.h"
#include "AppMacros.h"

LayerOne * LayerOne::create(int layerIdx)
{
	LayerOne *pRet = new LayerOne();
	if (pRet && pRet->init(layerIdx))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = NULL;
	}
	return pRet;
}

bool LayerOne::init(int layerIdx)
{
	CCLayerColor::initWithColor(ccc4(255, 255, 255, 255));

	/*setContentSize(CCSizeMake(winSize.width, winSize.height / 3 - 10));
	setAnchorPoint(ccp(0, 0));
	setPosition(ccp(0, (winSize.height / 3)*layerIdx + 5));
	addGround();
	addHero();*/

	setContentSize(CCSizeMake(winSize.width, winSize.height / 3 - 10));
	setAnchorPoint(ccp(0, 0));
	setPosition(ccp(0, (winSize.height / 3 - 10)*layerIdx + 20));

	addGround();
	addHero();

	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(LayerOne::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	
	scheduleUpdate();
	reSetFrameCount();

	_array = CCArray::create();
	_array->retain();


	return true;
}

void  LayerOne::addGround()
{
	CCSprite *bg = CCSprite::create();
	bg->setTextureRect(CCRectMake(0, 0, winSize.width, 4));
	bg->setColor(ccc3(0, 0, 0));
	bg->setAnchorPoint(ccp(0, 0));
	bg->setPosition(ccp(0, 0));
	addChild(bg);
}

void  LayerOne::addHero()
{
	_hero = Hero::create();
	_hero->setPosition(ccp(50, -3)); // adjust
	_hero->setScale(2.0f); // adjust
	addChild(_hero);
}

void  LayerOne::addBlockItems()
{
	BlockItem * b = BlockItem::create();
	b->setPosition(ccp(300, 4));
	addChild(b);
	_array->addObject(b);
}

bool LayerOne::onTouchBegan(Touch* touch, Event* event)
{
	if (_hero->getPositionY() > 6)
	{
		return false;
	}
	if (this->boundingBox().containsPoint(touch->getLocation()))
	{
		CCJumpBy * by = CCJumpBy::create(0.5, ccp(0, 0), 40, 1); //adjust
		_hero->runAction(by);
	}
	return false;
}

void LayerOne::update(float dt)
{
	curFramCount++;
	if (curFramCount >randFrameCount)
	{
		reSetFrameCount();
		addBlockItems();
	}

	CCObject * obj;
	CCARRAY_FOREACH_REVERSE(_array, obj)
	{
		BlockItem * b = (BlockItem*)obj;
		if (b->getPositionX() < 0)
		{
			b->removeFromParentAndCleanup(true);
		}
		CCRect rect = _hero->boundingBox();
		rect.size.width -= 30;
		if (rect.intersectsRect(b->boundingBox()))
		{
			CCScene * scene = LayerOver::scene();
			CCDirector::sharedDirector()->replaceScene(scene);
		}
	}

}

void LayerOne::reSetFrameCount()
{
	curFramCount = 0;
	randFrameCount = rand() % 40 + 60; // adjust
}