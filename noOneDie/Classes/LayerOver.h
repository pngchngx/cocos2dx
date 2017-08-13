#ifndef __LayerOver_H__
#define __LayerOver_H__

#include "cocos2d.h"
USING_NS_CC;
class LayerOver :public CCLayerColor
{
public:
	static CCScene * scene();
	CREATE_FUNC(LayerOver);
	bool init();
	void redoCallback(CCObject * obj);
	void backCallback(CCObject * obj);
};

#endif // __LayerOver_H__