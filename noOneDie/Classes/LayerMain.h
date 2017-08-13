#ifndef  __LayerMain_H__
#define  __LayerMain_H__

#include "cocos2d.h"
USING_NS_CC;

class LayerMain :public CCLayerColor
{
public:
	static CCScene * scene(int layerCount);
	static LayerMain * create(int layerCount);
	bool init(int layerCount);

	static int _layerCount;

};



#endif // __LayerMain_H__


