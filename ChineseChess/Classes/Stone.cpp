#include "Stone.h"

int Stone::_d = 0;
int Stone::_offx = 0;
int Stone::_offy = 0;

bool Stone::init()
{
	if (!CCSprite::init())
		return false;
	_dead = false;
	return true;
}

void Stone::initStone(int id)
{
	struct
	{
		TYPE type;
		int row;
		int col;
	} proper[9] = {
		{ CHE, 0, 0 },/*proper[0]*/
		{ MA, 0, 1 },/*proper[1]*/
		{ XIANG, 0, 2 },/*proper[2]*/
		{ SHI, 0, 3 },/*proper[3]*/
		{ BING, 3, 2 },/*proper[4]*/
		{ BING, 3, 0 },/*proper[5]*/
		{ PAO, 2, 1 },/*proper[6]*/

		{ JIANG, 0, 4 },/*proper[7]*/
		{ BING, 3, 4 }/*proper[8]*/
	};

	_id = id; // 0~31
	_red = id < 16;
#if 0
	if (id < 16) _red = true;
	else _red = false;
#endif

	if (id <= 8)/*left down*/
	{
		this->_row = proper[id].row;
		this->_col = proper[id].col;
		this->_type = proper[id].type;
	}
	if (id > 8 && id < 16)/*right down*/
	{
		int index = id - 9;

		this->_row = proper[index].row;
		this->_col = 8 - proper[index].col;
		this->_type = proper[index].type;
	}
	if (id >= 16)
	{
		int index = id - 16;
		if (index <= 8)/*right up*/
		{
			this->_row = 9 - proper[index].row;
			this->_col = 8 - proper[index].col;
			this->_type = proper[index].type;
		}
		if (index > 8)/*left up*/
		{
			index -= 9;

			this->_row = 9 - proper[index].row;
			this->_col = proper[index].col;
			this->_type = proper[index].type;
		}
	}

	// ��������ʾ����Ļ��
	// ͨ������ȥ��ȡ����ͼƬ
	CCTexture2D* texture;
	switch (_type)
	{
	case CHE:
		if (_red)
			texture = CCTextureCache::sharedTextureCache()->addImage("rche.png");
		else
			texture = CCTextureCache::sharedTextureCache()->addImage("bche.png");
		break;
	case MA:
		if (_red)
			texture = CCTextureCache::sharedTextureCache()->addImage("rma.png");
		else
			texture = CCTextureCache::sharedTextureCache()->addImage("bma.png");
		break;
	case PAO:
		if (_red)
			texture = CCTextureCache::sharedTextureCache()->addImage("rpao.png");
		else
			texture = CCTextureCache::sharedTextureCache()->addImage("bpao.png");
		break;
	case BING:
		if (_red)
			texture = CCTextureCache::sharedTextureCache()->addImage("rbing.png");
		else
			texture = CCTextureCache::sharedTextureCache()->addImage("bzu.png");
		break;
	case JIANG:
		if (_red)
			texture = CCTextureCache::sharedTextureCache()->addImage("rshuai.png");
		else
			texture = CCTextureCache::sharedTextureCache()->addImage("bjiang.png");
		break;
	case SHI:
		if (_red)
			texture = CCTextureCache::sharedTextureCache()->addImage("rshi.png");
		else
			texture = CCTextureCache::sharedTextureCache()->addImage("bshi.png");
		break;
	case XIANG:
		if (_red)
			texture = CCTextureCache::sharedTextureCache()->addImage("rxiang.png");
		else
			texture = CCTextureCache::sharedTextureCache()->addImage("bxiang.png");
		break;
	}

	// ��ʾ����
	setTexture(texture);
	setTextureRect(CCRectMake(0, 0, texture->getContentSize().width, texture->getContentSize().height));

	// ����λ��
	setPosition(fromPlate());

	// ����
	setScale(.6);
}

CCPoint Stone::fromPlate()
{
	int x = _col * _d + _offx;
	int y = _row * _d + _offy;

	return ccp(x, y);
}

