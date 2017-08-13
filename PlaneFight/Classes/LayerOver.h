#ifndef __LayerOver_H__
#define __LayerOver_H__

#include "cocos2d.h"
USING_NS_CC;


class LayerOver : public CCLayer
{
public:
	CREATE_FUNC(LayerOver);
	bool init();
	static CCScene * scene();

	void redoCallback(CCObject * obj);
};

#endif // __LayerOver_H__