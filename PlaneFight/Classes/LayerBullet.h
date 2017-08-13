#ifndef __LayerBullet_H__
#define __LayerBullet_H__

#include "cocos2d.h"
USING_NS_CC; 

class LayerBullet : public CCLayer
{
public:
	CREATE_FUNC(LayerBullet);
	bool init();

	void startShoot();
	void stopShoot();
	void addBulletCallback(float dt);
	void bulletMoveFinished(CCNode* node);
	void removeBullet(CCSprite* bullet);

	CCArray *_bulletArray;
	CCSpriteBatchNode * _bulletBatchNode;
};

#endif // __LayerBullet_H__
