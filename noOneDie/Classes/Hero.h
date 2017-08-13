#ifndef __Hero_H__
#define __Hero_H__
#include "cocos2d.h"

USING_NS_CC;

class Hero : public CCSprite
{
public:
	CREATE_FUNC(Hero);
	bool init();
};

#endif // __Hero_H__