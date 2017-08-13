#ifndef __LayerStart_H__
#define __LayerStart_H__

#include "cocos2d.h"
USING_NS_CC;

class LayerStart :public CCLayer
{
public:
	static CCScene * createScene();
	CREATE_FUNC(LayerStart);
	bool init();

	enum DEGREE
	{
		CLASS = 1, ADVANCE, HELL
	};

	void classCallBack(CCObject * obj);
	void advanceCallBack(CCObject * obj);
	void hellCallBack(CCObject * obj);
	void quitCallBack(CCObject * obj);
};


#endif // __LayerStart_H__
