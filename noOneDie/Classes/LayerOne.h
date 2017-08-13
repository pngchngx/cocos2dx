#ifndef __LayerOne_H__
#define __LayerOne_H__

#include "cocos2d.h"
#include "Hero.h"
USING_NS_CC;


class LayerOne : public CCLayerColor
{
public:
	static LayerOne * create(int layerIdx);
	bool init(int layerIdx);

	void addGround();
	void addHero();
	void addBlockItems();

	virtual bool onTouchBegan(Touch* touch, Event* event);

	void update(float dt);

	void reSetFrameCount();

private:
	Hero *_hero;
	int randFrameCount;
	int curFramCount;
	CCArray * _array;
};

#endif // __LayerOne_H__
