#ifndef __LayerEnemy_H__
#define __LayerEnemy_H__

#include "Enemy.h"
#include "cocos2d.h"
USING_NS_CC;

//只加了小飞机吗？？？？？？？？？？？？？？？
//关于hit函数没有写呢！！！！！！！！！！！

const int SMALL_MAXLIFE = 1;
const int MID_MAXLIFE = 3;
const int BIG_MAXLIFE = 5;

class LayerEnemy : public CCLayer
{
public:
	CREATE_FUNC(LayerEnemy);
	bool init();

	void addSmallEnemy(float dt);
	void smallEnemyMovefinished(CCNode *node);
	void smallEnemyBlowUp(Enemy * smallEnemy);
	void removeSmallEnemy(CCNode * target, void * data);
	void removeAllSmallEnemy();

	void addMidEnemy(float dt);
	void midHitAnimate(Enemy* midEnemy); // by myself
	void midEnemyMovefinished(CCNode *node);
	void midEnemyBlowUp(Enemy * midEnemy);
	void removeMidEnemy(CCNode * target, void * data);
	void removeAllMidEnemy();

	void addBigEnemy(float dt);
	void bigHitAnimate(Enemy* bigEnemy); // by myself
	void bigEnemyMovefinished(CCNode *node);
	void bigEnemyBlowUp(Enemy * bigEnemy);
	void removeBigEnemy(CCNode * target, void * data);
	void removeAllBigEnemy();

	void removeAllEnemy();

public:
	CCArray * smallArray;
	CCArray * midArray;
	CCArray * bigArray;
};

#endif // __LayerEnemy_H__