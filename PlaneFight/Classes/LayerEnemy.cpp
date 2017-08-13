#include "LayerEnemy.h"
#include "AppMacros.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

bool LayerEnemy::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	smallArray = CCArray::create();
	smallArray->retain();

	//¶¯»­ÌØÐ§
	CCAnimation *smallAnimation = CCAnimation::create();
	smallAnimation->setDelayPerUnit(0.1f);
	smallAnimation->addSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("enemy1_down1.png"));
	smallAnimation->addSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("enemy1_down2.png"));
	smallAnimation->addSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("enemy1_down3.png"));
	smallAnimation->addSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("enemy1_down4.png"));
	/*char nameBuf[100];
	for (int i = 0; i < 4; i++)
	{
		memset(nameBuf, 0, sizeof(nameBuf));
		sprintf(nameBuf, "enemy1_down%d.png", i + 1);
		smallAnimation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(nameBuf));
	}*/
	CCAnimationCache::sharedAnimationCache()->addAnimation(smallAnimation, "SmallBlowUp");

	schedule(schedule_selector(LayerEnemy::addSmallEnemy), 0.5f);


	CCAnimation *midAnimation = CCAnimation::create();
	midAnimation->setDelayPerUnit(0.1f);
	midAnimation->addSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("enemy2_down1.png"));
	midAnimation->addSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("enemy2_down2.png"));
	midAnimation->addSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("enemy2_down3.png"));
	midAnimation->addSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("enemy2_down4.png"));


	CCAnimation *midHitAnimation = CCAnimation::create();
	midHitAnimation->setDelayPerUnit(0.02f);
	midHitAnimation->addSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("enemy2_hit.png"));
	midHitAnimation->addSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("enemy2.png"));


	CCAnimation *bigAnimation = CCAnimation::create();
	bigAnimation->setDelayPerUnit(0.1f);
	bigAnimation->addSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("enemy3_down1.png"));
	bigAnimation->addSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("enemy3_down2.png"));
	bigAnimation->addSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("enemy3_down3.png"));
	bigAnimation->addSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("enemy3_down4.png"));
	bigAnimation->addSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("enemy3_down5.png"));
	bigAnimation->addSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("enemy3_down6.png"));


	CCAnimation *bigHitAnimation = CCAnimation::create();
	bigHitAnimation->setDelayPerUnit(0.02f);
	bigHitAnimation->addSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("enemy3_hit.png"));
	bigHitAnimation->addSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("enemy3_n1.png"));
	bigHitAnimation->addSpriteFrame(CCSpriteFrameCache::getInstance()->spriteFrameByName("enemy3_n2.png"));


	CCAnimationCache::sharedAnimationCache()->addAnimation(midAnimation, "MidBlowUp");
	CCAnimationCache::sharedAnimationCache()->addAnimation(bigAnimation, "BigBlowUp");

	CCAnimationCache::sharedAnimationCache()->addAnimation(midHitAnimation, "MidHit");
	CCAnimationCache::sharedAnimationCache()->addAnimation(bigHitAnimation, "BigHit");

	midArray = CCArray::create();
	midArray->retain();
	bigArray = CCArray::create();
	bigArray->retain();

	schedule(schedule_selector(LayerEnemy::addMidEnemy), 3.0f);
	schedule(schedule_selector(LayerEnemy::addBigEnemy), 6.0f);


	return true;

}


/****************************Small***begin**************************/
void  LayerEnemy::addSmallEnemy(float dt)
{
	Enemy * smallPlane = Enemy::create();
	smallPlane->bindEnemySprite(CCSprite::createWithSpriteFrameName("enemy1.png"), SMALL_MAXLIFE);
	smallArray->addObject(smallPlane);
	addChild(smallPlane);

	float x = CCRANDOM_0_1()*(winSize.width - smallPlane->getBoundingBox().size.width) + smallPlane->getBoundingBox().size.width / 2;
	float y = winSize.height + smallPlane->boundingBox().size.height / 2;

	CCPoint smallBirth = ccp(x, y);
	smallPlane->setPosition(smallBirth);
	CCMoveTo *to = CCMoveTo::create(3, ccp(smallBirth.x,
		smallBirth.y - winSize.height - smallPlane->getBoundingBox().size.height));
	CCCallFuncN * actionDone = CCCallFuncN::create(this, callfuncN_selector(LayerEnemy::smallEnemyMovefinished));
	CCSequence * sequence = CCSequence::create(to, actionDone, NULL);
	smallPlane->runAction(sequence);
}

void  LayerEnemy::smallEnemyMovefinished(CCNode *node)
{
	CCSprite * smallEnemy = (CCSprite *)node;
	smallArray->removeObject(smallEnemy);
	smallEnemy->removeFromParentAndCleanup(true);

}

void  LayerEnemy::smallEnemyBlowUp(Enemy * smallEnemy)
{
	SimpleAudioEngine::getInstance()->playEffect("sound/enemy1_down.wav");
	CCAnimate * smallAnimate = CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName("SmallBlowUp"));
	CCCallFuncND  * actionDone = CCCallFuncND::create(this, callfuncND_selector(LayerEnemy::removeSmallEnemy), (void*)smallEnemy);
	CCSequence *sequence = CCSequence::create(smallAnimate, actionDone, NULL);
	smallEnemy->getSprite()->runAction(sequence);
}

void  LayerEnemy::removeSmallEnemy(CCNode * target, void * data)
{
	Enemy * smallEnemy = (Enemy*)data;
	if (smallEnemy)
	{
		smallArray->removeObject(smallEnemy);
		smallEnemy->removeFromParentAndCleanup(true);
	}
}

void  LayerEnemy::removeAllSmallEnemy()
{
	CCObject* et;
	CCARRAY_FOREACH(smallArray, et)
	{
		Enemy * enemy = (Enemy *)et;
		if (enemy->getLife() >= 1)
		{
			smallEnemyBlowUp(enemy);
		}
	}
}
/****************************Small***end**************************/


/****************************Mid***begin**************************/
void  LayerEnemy::addMidEnemy(float dt)
{
	Enemy * midPlane = Enemy::create();
	midPlane->bindEnemySprite(CCSprite::createWithSpriteFrameName("enemy2.png"), MID_MAXLIFE);
	midArray->addObject(midPlane);
	addChild(midPlane);

	float x = CCRANDOM_0_1()*(winSize.width - midPlane->getBoundingBox().size.width) + midPlane->getBoundingBox().size.width / 2;
	float y = winSize.height + midPlane->boundingBox().size.height / 2;

	CCPoint midBirth = ccp(x, y);
	midPlane->setPosition(midBirth);
	CCMoveTo *to = CCMoveTo::create(3, ccp(midBirth.x,
		midBirth.y - winSize.height - midPlane->getBoundingBox().size.height));
	CCCallFuncN * actionDone = CCCallFuncN::create(this, callfuncN_selector(LayerEnemy::midEnemyMovefinished));
	CCSequence * sequence = CCSequence::create(to, actionDone, NULL);
	midPlane->runAction(sequence);
}

void LayerEnemy::midHitAnimate(Enemy* midEnemy)
{
	CCAnimate * midAnimate = CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName("MidHit"));
	midEnemy->getSprite()->runAction(midAnimate);
}

void  LayerEnemy::midEnemyMovefinished(CCNode *node)
{
	CCSprite * midEnemy = (CCSprite *)node;
	midArray->removeObject(midEnemy);
	midEnemy->removeFromParentAndCleanup(true);

}

void  LayerEnemy::midEnemyBlowUp(Enemy * midEnemy)
{
	//SimpleAudioEngine::getInstance()->playEffect("sound/enemy2_down.wav");
	CCAnimate * midAnimate = CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName("MidBlowUp"));
	CCCallFuncND  * actionDone = CCCallFuncND::create(this, callfuncND_selector(LayerEnemy::removeMidEnemy), (void*)midEnemy);
	CCSequence *sequence = CCSequence::create(midAnimate, actionDone, NULL);
	midEnemy->getSprite()->runAction(sequence);
}

void  LayerEnemy::removeMidEnemy(CCNode * target, void * data)
{
	Enemy * midEnemy = (Enemy*)data;
	if (midEnemy)
	{
		midArray->removeObject(midEnemy);
		midEnemy->removeFromParentAndCleanup(true);
	}
}

void  LayerEnemy::removeAllMidEnemy()
{
	CCObject* et;
	CCARRAY_FOREACH(midArray, et)
	{
		Enemy * enemy = (Enemy *)et;
		if (enemy->getLife() >= 1)
		{
			midEnemyBlowUp(enemy);
		}
	}
}
/****************************Mid***end**************************/


/****************************Big***begin**************************/
void  LayerEnemy::addBigEnemy(float dt)
{
	Enemy * bigPlane = Enemy::create();
	bigPlane->bindEnemySprite(CCSprite::createWithSpriteFrameName("enemy3_n2.png"), BIG_MAXLIFE); // no enemy3.png
	bigArray->addObject(bigPlane);
	addChild(bigPlane);

	float x = CCRANDOM_0_1()*(winSize.width - bigPlane->getBoundingBox().size.width) + bigPlane->getBoundingBox().size.width / 2;
	float y = winSize.height + bigPlane->boundingBox().size.height / 2;

	CCPoint bigBirth = ccp(x, y);
	bigPlane->setPosition(bigBirth);
	CCMoveTo *to = CCMoveTo::create(3, ccp(bigBirth.x,
		bigBirth.y - winSize.height - bigPlane->getBoundingBox().size.height));
	CCCallFuncN * actionDone = CCCallFuncN::create(this, callfuncN_selector(LayerEnemy::bigEnemyMovefinished));
	CCSequence * sequence = CCSequence::create(to, actionDone, NULL);
	bigPlane->runAction(sequence);
}

void LayerEnemy::bigHitAnimate(Enemy* bigEnemy)
{
	CCAnimate * bigAnimate = CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName("BigHit"));
	bigEnemy->getSprite()->runAction(bigAnimate);
}

void  LayerEnemy::bigEnemyMovefinished(CCNode *node)
{
	CCSprite * bigEnemy = (CCSprite *)node;
	bigArray->removeObject(bigEnemy);
	bigEnemy->removeFromParentAndCleanup(true);

}

void  LayerEnemy::bigEnemyBlowUp(Enemy * bigEnemy)
{
	//SimpleAudioEngine::getInstance()->playEffect("sound/enemy3_down.wav");
	CCAnimate * bigAnimate = CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName("BigBlowUp"));
	CCCallFuncND  * actionDone = CCCallFuncND::create(this, callfuncND_selector(LayerEnemy::removeBigEnemy), (void*)bigEnemy);
	CCSequence *sequence = CCSequence::create(bigAnimate, actionDone, NULL);
	bigEnemy->getSprite()->runAction(sequence);
}

void  LayerEnemy::removeBigEnemy(CCNode * target, void * data)
{
	Enemy * bigEnemy = (Enemy*)data;
	if (bigEnemy)
	{
		bigArray->removeObject(bigEnemy);
		bigEnemy->removeFromParentAndCleanup(true);
	}
}

void  LayerEnemy::removeAllBigEnemy()
{
	CCObject* et;
	CCARRAY_FOREACH(bigArray, et)
	{
		Enemy * enemy = (Enemy *)et;
		if (enemy->getLife() >= 1)
		{
			bigEnemyBlowUp(enemy);
		}
	}
}
/****************************Big***end**************************/



void  LayerEnemy::removeAllEnemy()
{
	removeAllSmallEnemy();
	removeAllMidEnemy();
	removeAllBigEnemy();
}
