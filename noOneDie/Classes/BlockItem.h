#ifndef __BlockItem_H__
#define __BlockItem_H__

#include "cocos2d.h"
USING_NS_CC;
class BlockItem : public CCSprite
{
public:
	CREATE_FUNC(BlockItem);
	bool init();

	void update(float dt);
};

#endif // __BlockItem_H__