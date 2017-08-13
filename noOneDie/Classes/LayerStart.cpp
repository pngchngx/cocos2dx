#include "LayerStart.h"
#include "LayerMain.h"


CCScene * LayerStart::createScene()
{
	CCScene * scene = CCScene::create();
	LayerStart * layer = LayerStart::create();
	scene->addChild(layer);
	return scene;
}

bool LayerStart::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	CCMenuItem * classItem = CCMenuItemFont::create("Class", this,
		menu_selector(LayerStart::classCallBack));
	CCMenuItem * advanceItem = CCMenuItemFont::create("Advance", this,
		menu_selector(LayerStart::advanceCallBack));
	CCMenuItem * hellItem = CCMenuItemFont::create("Hell", this,
		menu_selector(LayerStart::hellCallBack));
	CCMenuItem * quitItem = CCMenuItemFont::create("Quit", this,
		menu_selector(LayerStart::quitCallBack));

	CCMenu * menu = CCMenu::create(classItem, advanceItem, hellItem, quitItem, NULL);
	addChild(menu);
	menu->alignItemsVerticallyWithPadding(40);

	return true;
}

void LayerStart::classCallBack(CCObject * obj)
{
	CCScene * scene = LayerMain::scene(CLASS);
	CCDirector::sharedDirector()->replaceScene(scene);
}
void LayerStart::advanceCallBack(CCObject * obj)
{
	CCScene * scene = LayerMain::scene(ADVANCE);
	CCDirector::sharedDirector()->replaceScene(scene);
}
void LayerStart::hellCallBack(CCObject * obj)
{
	CCScene * scene = LayerMain::scene(HELL);
	CCDirector::sharedDirector()->replaceScene(scene);
}
void LayerStart::quitCallBack(CCObject * obj)
{
	CCDirector::sharedDirector()->end();
}
