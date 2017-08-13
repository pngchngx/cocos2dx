#include "LayerOver.h"
#include "AppMacros.h"
#include "LayerStart.h"
#include "LayerMain.h"


CCScene * LayerOver::scene()
{
	CCScene * scene = CCScene::create();
	LayerOver * layer = LayerOver::create();
	scene->addChild(layer);
	return scene;
}
bool LayerOver::init()
{
	CCLayerColor::initWithColor(ccc4(255, 255, 255, 255));

	CCLabelTTF * label = CCLabelTTF::create("Game Over", "Arial", 50);
	label->setColor(ccc3(0, 0, 0));
	label->setPosition(ccp(winSize.width / 2, winSize.height / 3 * 2));
	addChild(label);


	CCMenuItem * redoItem = CCMenuItemFont::create("Redo", this, menu_selector(LayerOver::redoCallback));
	redoItem->setColor(ccc3(0, 0, 0));
	CCMenuItem * backItem = CCMenuItemFont::create("Back", this, menu_selector(LayerOver::backCallback));
	backItem->setColor(ccc3(0, 0, 0));

	CCMenu * menu = CCMenu::create(redoItem, backItem, NULL);

	menu->setPositionY(menu->getPositionY() - 40);
	menu->alignItemsHorizontallyWithPadding(30);
	addChild(menu);

	return true;
}

void LayerOver::redoCallback(CCObject * obj)
{
	CCScene *scene = LayerMain::scene(LayerMain::_layerCount);
	CCDirector::sharedDirector()->replaceScene(scene);
}
void LayerOver::backCallback(CCObject * obj)
{
	CCScene * scene = LayerStart::createScene();
	CCDirector::sharedDirector()->replaceScene(scene);
}