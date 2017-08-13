#ifndef __LayerFood_H__
#define __LayerFood_H__

#include "cocos2d.h"
USING_NS_CC;

class LayerFood : public CCLayer
{
public:
	CREATE_FUNC(LayerFood);
	bool init();

	//void addMultiBullets(float dt);
	//void multiBulletsMoveFinished(CCNode * pSender);
	//void removeMultiBullets(CCSprite *mb);

	void addBigBoom(float dt);
	void bigBoomMoveFinished(CCNode *pSender);
	void removeBigBoom(CCSprite *bb);

public:
	CCArray * multiBulletArray;
	CCArray * bigBoomArray;
};

#endif // __LayerFood_H__