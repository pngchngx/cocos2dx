#ifndef __LayerGameMain_H__
#define __LayerGameMain_H__

#include "LayerBullet.h"
#include "LayerEnemy.h"
#include "LayerControl.h"
#include "LayerFood.h"
#include "cocos2d.h"
USING_NS_CC;

const int  SMALL_SCORE = 1000;
const int  MID_SCORE = 6000;
const int  BIG_SCORE = 30000;

const int MAX_BIGBOOM_COUNT = 100000;
const int TAG_BIGBOOM = 1000;
const int TAG_BIGBOOMCOUNT = 2000;

class LayerGameMain : public CCLayer
{
public:
	CREATE_FUNC(LayerGameMain);
	bool init();
	static CCScene * scene();

	//加载主场景背景
	void addBackGround();
	//背景动起来
	void movingBackGround(float dt);

	void addHero();
	void addBulletLayer();
	void addEnemyLayer();
	void addCtrlLayer();
	void addFoodLayer();

	void updateBigBoomCount(int bigBoomCount);

	void boomMenuCallBack(CCObject * obj);

	void update(float dt);

	virtual bool onTouchBegan(Touch* pTouch, Event* pEvent);
	virtual void onTouchMoved(Touch* pTouch, Event* pEvent);

	enum BACKGROUND
	{
		BACK1, BACK2
	};

	static int getScore()
	{
		return score;
	}

private:
	static int score;
	int bigBoomCount;

	LayerBullet* _bulletLayer;
	LayerEnemy* _enemyLayer;
	LayerControl* _ctrlLayer;
	LayerFood* _foodLayer;
};

#endif // __LayerGameMain_H__