#include "PlaneItem.h"



PlaneItem* PlaneItem::_splane = NULL;

PlaneItem* PlaneItem::getInstance()
{
	if (!_splane)
	{
		_splane = new PlaneItem;
		if (_splane && _splane->init())
		{
		}
	}
	return _splane;
}

bool PlaneItem::init()
{
	if (!CCSprite::init())
	{
		return false;
	}
	// ��֡������֡����������
	this->initWithSpriteFrameName("hero1.png");
	//һ����������
	CCBlink *blink = CCBlink::create(1, 3);
	//�������Ժ󣬿�ʼִ�ж���
	CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(PlaneItem::animatePlane));
	//CCsequence ���ܴ��ccrepeatforever�Ķ���
	CCSequence *sequence = CCSequence::create(blink, callFunc, NULL); 
	runAction(sequence);

	//��ը��������
	CCAnimation * animation = CCAnimation::create();
	animation->setDelayPerUnit(0.2f);
	animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero_blowup_n1.png"));
	animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero_blowup_n2.png"));
	animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero_blowup_n3.png"));
	CCAnimationCache::sharedAnimationCache()->addAnimation(animation, "PlaneBlowUp");

	return true;
}

void  PlaneItem::animatePlane()
{
	CCAnimation *Animation = CCAnimation::create();
	Animation->setDelayPerUnit(0.2f);
	Animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero1.png"));
	Animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hero2.png"));
	CCAnimate *animate = CCAnimate::create(Animation);
	this->runAction(CCRepeatForever::create(animate));
}

void PlaneItem::blowUp()
{
	CCAnimate * animate = CCAnimate::create(CCAnimationCache::sharedAnimationCache()->animationByName("PlaneBlowUp"));
	CCCallFunc * actionDone = CCCallFunc::create(this, callfunc_selector(PlaneItem::removePlane));
	CCSequence *sequence = CCSequence::create(animate, actionDone, NULL);
	this->runAction(sequence);
}

void PlaneItem::removePlane()
{
	removeFromParent();
}