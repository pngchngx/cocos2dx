#include "LayerFood.h"



bool LayerFood::init()
{
	bigBoomArray = CCArray::create();
	bigBoomArray->retain();

	schedule(schedule_selector(LayerFood::addBigBoom), 5.0f);
	return true;
}
void LayerFood::addBigBoom(float dt)
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite * bigBoom = CCSprite::createWithSpriteFrameName("ufo2.png");
	float minX = bigBoom->getContentSize().width / 2;
	float maxX = winSize.width - minX;
	float rangeX = maxX - minX;
	CCPoint  boomBirth = ccp((minX + rangeX*CCRANDOM_0_1()), winSize.height + bigBoom->getContentSize().height / 2);
	bigBoom->setPosition(boomBirth);
	addChild(bigBoom);
	bigBoomArray->addObject(bigBoom);

	CCMoveBy * by1 = CCMoveBy::create(0.5f, ccp(0, -150));
	CCMoveBy * by2 = CCMoveBy::create(0.3f, ccp(0, 100));
	CCMoveBy * by3 = CCMoveBy::create(1.0f, ccp(0, 0 - winSize.height - bigBoom->getContentSize().height / 2 - 5));
	CCCallFuncN * actionDone = CCCallFuncN::create(this, callfuncN_selector(LayerFood::bigBoomMoveFinished));

	CCSequence * sequence = CCSequence::create(by1, by2, by3, actionDone, NULL);

	bigBoom->runAction(sequence);
}
void LayerFood::bigBoomMoveFinished(CCNode *pSender)
{
	CCSprite * bigBoom = (CCSprite*)pSender;
	bigBoom->removeAllChildrenWithCleanup(true);
	bigBoomArray->removeObject(bigBoom);
}

void LayerFood::removeBigBoom(CCSprite *bb)
{
	bb->removeFromParentAndCleanup(true);
	bigBoomArray->removeObject(bb);
}
