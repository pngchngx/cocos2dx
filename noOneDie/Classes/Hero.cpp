#include "Hero.h"


bool Hero::init()
{
	CCSprite::init();
	setAnchorPoint(ccp(0, 0));

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("hero.plist");

	CCAnimation * animation = CCAnimation::create();
	char name[100];
	for (int i = 0; i < 5; i++)
	{
		memset(name, 0, sizeof(name));
		sprintf(name, "Hero%d.png", i + 1);
		animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name));
	}
	animation->setDelayPerUnit(0.1f);
	animation->setLoops(-1);

	CCAnimate * animate = CCAnimate::create(animation);

	this->runAction(animate);
	return true;
}