#include "LayerMain.h"
#include "LayerOne.h"

int LayerMain::_layerCount = 0;

CCScene * LayerMain::scene(int layerCount)
{
	CCScene * scene = CCScene::create();
	LayerMain * layer = LayerMain::create(layerCount);
	scene->addChild(layer);
	return scene;
}
LayerMain * LayerMain::create(int layerCount)
{
	LayerMain * pRet = new LayerMain();
	if (pRet && pRet->init(layerCount))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = NULL;
	}
	return pRet;
}
bool LayerMain::init(int layerCount)
{
	CCLayerColor::initWithColor(ccc4(255, 255, 255, 255));
	
	_layerCount = layerCount;
	for (int i = 0; i < layerCount; i++)
	{
		LayerOne * one = LayerOne::create(i);
		addChild(one);
	}

	return true;
}