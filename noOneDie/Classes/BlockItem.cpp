#include "BlockItem.h"
#include "AppMacros.h"


bool BlockItem::init()
{
	CCSprite::init();

	CCSize size = CCSize(rand() % 20 + 10, rand() % 30 + 10); // adjust
	setContentSize(size);
	setAnchorPoint(ccp(0, 0));
	setTextureRect(CCRectMake(0, 0, size.width, size.height));
	setColor(ccc3(0, 0, 0));

	scheduleUpdate();


	return true;
}

void BlockItem::update(float dt)
{
	setPositionX(getPositionX() - 6);
}
