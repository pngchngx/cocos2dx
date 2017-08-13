#include "LayerGameMain.h"
#include "PlaneItem.h"
#include "LayerOver.h"
#include "AppMacros.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

int LayerGameMain::score = 0;

CCScene * LayerGameMain::scene()
{
	CCScene * scene = CCScene::create();
	LayerGameMain * layer = LayerGameMain::create();
	scene->addChild(layer);
	return scene;
}

bool LayerGameMain::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	score = 0; // once again 0, back from over layer
	bigBoomCount = 0;
	//setKeypadEnabled(true);
	SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/game_music.wav", true);
	addBackGround();
	addHero();
	addBulletLayer();
	addEnemyLayer();
	addCtrlLayer();
	addFoodLayer();

	scheduleUpdate();

	//setTouchEnabled(true); // not necessary
	//setTouchMode(kCCTouchesOneByOne); // not necessary
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(LayerGameMain::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(LayerGameMain::onTouchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	

	return true;
}

//加载主场景背景
void  LayerGameMain::addBackGround()
{
	CCSprite * bg1 = CCSprite::createWithSpriteFrameName("background.png");
	bg1->setTag(BACK1);
	bg1->setAnchorPoint(ccp(0, 0));
	bg1->setPosition(ccp(0, 0));
	addChild(bg1);

	CCSprite * bg2 = CCSprite::createWithSpriteFrameName("background.png");
	bg2->setTag(BACK2);
	bg2->setAnchorPoint(ccp(0, 0));
	bg2->setPosition(ccp(0, bg2->getContentSize().height - 2));
	addChild(bg2);

	schedule(schedule_selector(LayerGameMain::movingBackGround), 0.02f);
}

//背景动起来
void LayerGameMain::movingBackGround(float dt)
{
	CCSprite * bg1 = (CCSprite*)getChildByTag(BACK1);
	CCSprite * bg2 = (CCSprite*)getChildByTag(BACK2);

	bg1->setPositionY(bg1->getPositionY() - 2);
	bg2->setPositionY(bg1->getPositionY() + bg2->getContentSize().height - 2);

	if (bg2->getPositionY() < 0)
	{
		bg1->setPositionY(0);
	}
}

void LayerGameMain::addHero()
{
	PlaneItem * hero = PlaneItem::getInstance();
	hero->setPosition(ccp(winSize.width / 2, hero->getContentSize().height / 2));
	addChild(hero);
}

bool LayerGameMain::onTouchBegan(Touch* pTouch, Event* pEvent)
{
	CCRect planeRect = PlaneItem::getInstance()->boundingBox();
	planeRect.origin.x -= 30;
	planeRect.origin.y -= 40;
	planeRect.size.width += 60;
	planeRect.size.height += 80;
	if (planeRect.containsPoint(pTouch->getLocation())) {
		return true;
	}
	else {
		return false;
	}
}

void LayerGameMain::onTouchMoved(Touch* pTouch, Event* pEvent)
{
	PlaneItem *hero = PlaneItem::getInstance();
	CCPoint positon = hero->getPosition() + pTouch->getDelta();

	//CCLog("%g", hero->boundingBox().size.width/2);
	//CCLog("%g", winSize.width - hero->boundingBox().size.width/2);
	//CCLog("%g", hero->boundingBox().size.height/2);
	//CCLog("%g", winSize.height - hero->boundingBox().size.height/2);
	if (positon.x < hero->boundingBox().size.width / 2 ||
		positon.x > winSize.width - hero->boundingBox().size.width / 2 ||
		positon.y <hero->boundingBox().size.height / 2 ||
		positon.y > winSize.height - hero->boundingBox().size.height / 2)
	{
		return;
	}
	else
	{
		PlaneItem::getInstance()->setPosition(positon);
	}
}

void LayerGameMain::addBulletLayer()
{
	_bulletLayer = LayerBullet::create();
	addChild(_bulletLayer);
}

void LayerGameMain::addEnemyLayer()
{
	_enemyLayer = LayerEnemy::create();
	addChild(_enemyLayer);
}

void LayerGameMain::update(float dt)
{
	//bullet vs enemysmall 
	CCObject * bt = NULL;
	CCObject * et = NULL;
	CCArray  *bulletsToDel = CCArray::create();

	CCARRAY_FOREACH(_bulletLayer->_bulletArray, bt)
	{
		CCSprite * bullet = (CCSprite*)bt;
		CCArray  * smallEnemyToDel = CCArray::create();

		CCARRAY_FOREACH(_enemyLayer->smallArray, et)
		{
			Enemy * smallEnemy = (Enemy *)et;
			if (bullet->boundingBox().intersectsRect(smallEnemy->getBoundingBox()))
			{
				//CCLog("%d", smallEnemy->getLife());
				if (smallEnemy->getLife() == 1)
				{
					smallEnemy->loseLife();
					bulletsToDel->addObject(bullet);
					smallEnemyToDel->addObject(smallEnemy);
					score += SMALL_SCORE;
					_ctrlLayer->updateScore(score);
				}
			}
		}
		CCARRAY_FOREACH(smallEnemyToDel, et)
		{
			Enemy * smallEnemy = (Enemy*)et;
			_enemyLayer->smallEnemyBlowUp(smallEnemy);
		}
		smallEnemyToDel->removeAllObjects();
	}
	CCARRAY_FOREACH(bulletsToDel, bt)
	{
		CCSprite * bullet = (CCSprite*)bt;
		_bulletLayer->removeBullet(bullet);
	}
	bulletsToDel->removeAllObjects();

	//hero vs enemySmall
	CCRect planeRect = PlaneItem::getInstance()->boundingBox();
	planeRect.origin.x += 30;
	planeRect.origin.y += 20;
	planeRect.size.width -= 60;
	planeRect.size.height -= 40;

	CCARRAY_FOREACH(_enemyLayer->smallArray, et)
	{
		Enemy  * smallEnemy = (Enemy  *)et;
		if (planeRect.intersectsRect(smallEnemy->getBoundingBox()))
		{
			((CCLayer*)(PlaneItem::getInstance()->getParent()))->setTouchEnabled(false);
			_bulletLayer->stopShoot();
			unscheduleAllSelectors();
			PlaneItem::getInstance()->blowUp();
			PlaneItem::getInstance()->removePlane();
			_enemyLayer->smallEnemyBlowUp(smallEnemy);
			CCScene* over = LayerOver::scene();
			CCDirector::sharedDirector()->replaceScene(over);
		}
	}

	//food vs hero
	CCObject * ut;
	CCARRAY_FOREACH(_foodLayer->bigBoomArray, ut)
	{
		CCSprite * bigBoom = (CCSprite*)ut;
		if (bigBoom->boundingBox().intersectsRect(PlaneItem::getInstance()->boundingBox()))
		{
			_foodLayer->removeBigBoom(bigBoom);
			bigBoomCount++;
			updateBigBoomCount(bigBoomCount);
		}
	}
}

void LayerGameMain::addCtrlLayer()
{
	_ctrlLayer = LayerControl::create();
	addChild(_ctrlLayer);
}

void LayerGameMain::addFoodLayer()
{
	_foodLayer = LayerFood::create();
	addChild(_foodLayer);
}

void  LayerGameMain::updateBigBoomCount(int bigBoomCount)
{
	CCString strBoomCount;
	if (bigBoomCount < 0)
	{
		return;
	}
	else if (bigBoomCount == 0)
	{
		if (getChildByTag(TAG_BIGBOOM))
		{
			removeChildByTag(TAG_BIGBOOM, true);
		}
		if (getChildByTag(TAG_BIGBOOMCOUNT))
		{
			removeChildByTag(TAG_BIGBOOMCOUNT, true);
		}
	}
	else if (bigBoomCount == 1)
	{
		CCSprite * norBoom = CCSprite::createWithSpriteFrameName("bomb.png");
		if (!getChildByTag(TAG_BIGBOOM))
		{
			CCSprite * selBoom = CCSprite::createWithSpriteFrameName("bomb.png");
			CCMenuItemSprite * boomItem = CCMenuItemSprite::create(norBoom, selBoom, this, menu_selector(LayerGameMain::boomMenuCallBack));
			boomItem->setPosition(ccp(norBoom->getContentSize().width / 2, norBoom->getContentSize().height / 2));
			CCMenu * boomMenu = CCMenu::create(boomItem, NULL);
			boomMenu->setPosition(ccp(0, 0));
			addChild(boomMenu, 100, TAG_BIGBOOM);
		}
		if (!getChildByTag(TAG_BIGBOOMCOUNT))
		{
			strBoomCount.initWithFormat("X%d", bigBoomCount);
			CCLabelBMFont * labelBoomCount = CCLabelBMFont::create(strBoomCount.getCString(), "font/font.fnt");
			labelBoomCount->setAnchorPoint(ccp(0, 0.5));
			labelBoomCount->setPosition(ccp(norBoom->getContentSize().width + 15, norBoom->getContentSize().height / 2));
			addChild(labelBoomCount, 200, TAG_BIGBOOMCOUNT);
		}
	}
	else if (bigBoomCount > 1)
	{
		strBoomCount.initWithFormat("X%d", bigBoomCount);
		((CCLabelBMFont*)(getChildByTag(TAG_BIGBOOMCOUNT)))->setString(strBoomCount.getCString());
	}
}

void LayerGameMain::boomMenuCallBack(CCObject * obj)
{
	bigBoomCount--;
	CCString *strBoomCount = CCString::createWithFormat("X%d", bigBoomCount);
	((CCLabelBMFont*)(getChildByTag(TAG_BIGBOOMCOUNT)))->setString(strBoomCount->getCString());

	score += _enemyLayer->smallArray->count() * SMALL_SCORE;
	//score += _enemyLayer->midArray->count() * MID_SCORE;
	//score += _enemyLayer->bigArray->count() * BIG_SCORE;

	_ctrlLayer->updateScore(score);
	_enemyLayer->removeAllEnemy();
	if (bigBoomCount == 0)
	{
		removeChildByTag(TAG_BIGBOOM);
		removeChildByTag(TAG_BIGBOOMCOUNT);
	}
}

