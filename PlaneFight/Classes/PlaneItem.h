#ifndef __PlaneItem_H__
#define __PlaneItem_H__

#include "cocos2d.h"
USING_NS_CC;


class PlaneItem : public CCSprite
{
public:
	static PlaneItem* getInstance();
	static PlaneItem* _splane;
	
	void animatePlane();
	void blowUp();
	void removePlane();
private:
	bool init();
};

#endif // __PlaneItem_H__