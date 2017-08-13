#include "LayerBullet.h"
#include "PlaneItem.h"
#include "AppMacros.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

bool LayerBullet::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	_bulletArray = CCArray::create();
	_bulletArray->retain();

	_bulletBatchNode = CCSpriteBatchNode::create("ui/shoot.png");
	addChild(_bulletBatchNode);

	startShoot();

	return true;
}

void LayerBullet::startShoot()
{
	schedule(schedule_selector(LayerBullet::addBulletCallback), 0.2f); // fine tune
}
void LayerBullet::stopShoot()
{
	unschedule(schedule_selector(LayerBullet::addBulletCallback));
}

void LayerBullet::addBulletCallback(float dt)
{
	SimpleAudioEngine::getInstance()->playEffect("sound/bullet.wav");
	//在另外一个层里，获得飞机的单例
	CCSprite *hero = PlaneItem::getInstance();
	CCPoint birthPlace = hero->getPosition() + ccp(0, hero->getContentSize().height / 2 + 2);
	CCSprite * bullet = CCSprite::createWithSpriteFrameName("bullet1.png");
	bullet->setPosition(birthPlace);
	_bulletBatchNode->addChild(bullet);
	_bulletArray->addObject(bullet);

	float ditance = winSize.height - hero->getPositionY() - hero->getContentSize().height / 2;
	float velocity = 600 / 1; //adjust fine tune
	float movedt = ditance / velocity;

	CCMoveTo *to = CCMoveTo::create(movedt, ccp(birthPlace.x, winSize.height + bullet->getContentSize().height));
	CCCallFuncN * actionDone = CCCallFuncN::create(this, callfuncN_selector(LayerBullet::bulletMoveFinished));

	CCSequence * sequence = CCSequence::create(to, actionDone, NULL);

	bullet->runAction(sequence);
}

void LayerBullet::bulletMoveFinished(CCNode* node)
{
	CCSprite * bullet = (CCSprite*)node;
	_bulletBatchNode->removeChild(bullet, true);
	_bulletArray->removeObject(bullet);
}



void LayerBullet::removeBullet(CCSprite* bullet)
{
	_bulletBatchNode->removeChild(bullet, true);
	_bulletArray->removeObject(bullet);
}
