#include "SceneGame.h"

CCScene* SceneGame::createScene()
{
	CCScene* scene = CCScene::create();
	SceneGame* game = SceneGame::create();
	scene->addChild(game);
	return scene;
}

bool SceneGame::init()
{
	if (!CCLayer::init())
		return false;

	// 摆放棋盘
	CreatePlate();

	// 棋子属性:颜色/类型(che,ma,pao)/位置/大小/ID
	
	Stone::_d = CCDirector::sharedDirector()->getWinSize().height / 10;//方格间隔
	Stone::_offx = Stone::_d;//方格阵列起始x偏移
	Stone::_offy = Stone::_d / 2;//方格阵列起始y偏移
	//摆放棋子
	for (int i = 0; i < 32; i++)
	{
		_s[i] = Stone::create();
		_s[i]->initStone(i);
		addChild(_s[i]);
	}

	// 使能触摸
	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(SceneGame::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(SceneGame::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	_selectid = -1;

	_selectSprite = CCSprite::create("selected.png");
	_selectSprite->setVisible(false);//初始不可见
	addChild(_selectSprite);
	_selectSprite->setScale(.6);

	_bRedTurn = true;

	_steps.clear();

	// 增加控制面板(包括悔棋按钮)
	addCtrlPanel();

	return true;
}

void SceneGame::CreatePlate()
{
	CCSprite* plate = CCSprite::create("background.png");
	addChild(plate);
	plate->setPosition(ccp(0, 0));
	plate->setAnchorPoint(ccp(0, 0));
	plate->setScale(CCDirector::sharedDirector()->getWinSize().height / plate->getContentSize().height);
}

bool SceneGame::onTouchBegan(Touch* touch, Event* event)
{
	return true;
}

// 点击两次才能移动象棋,一次选择,一次移动
void SceneGame::onTouchEnded(Touch* touch, Event* event)
{
	// 移动象棋
	if (_selectid == -1)
	{
		// 企图选中棋子
		SelectStone(touch);
	}
	else
	{
		// 企图移动棋子
		MoveStone(touch);
	}
}

// 1。通过touch获得点击位置
// 2。通过位置获得row，col：把屏幕坐标转换成棋盘坐标
// 3。通过棋盘坐标获得棋子id
// 4。记录选中的棋子的ID
void SceneGame::SelectStone(CCTouch* touch)
{
	CCPoint ptClicked = touch->getLocation();
	int row, col;
	bool bClick = Screen2Plate(ptClicked, row, col);
	// 如果点击在棋盘外，该次点击无效
	if (!bClick)
	{
		return;
	}
	int clickid = getStoneFromRowCol(row, col);
	// 如果点击位置没有象棋，那么点击也无效
	if (clickid == -1)
	{
		return;
	}

	// 如果被点中的棋子，和_bRedTurn颜色不一致，不让选中
	if (_s[clickid]->_red != _bRedTurn)
		return;

	// 记录这次的点击
	_selectid = clickid;

	// 显示该棋子被选中的效果
	_selectSprite->setPosition(Plate2Screen(row, col));
	_selectSprite->setVisible(true);

}

int SceneGame::getStoneFromRowCol(int row, int col)
{
	for (int i = 0; i < 32; ++i)
	{
		if (_s[i]->_row == row && _s[i]->_col == col && !_s[i]->_dead)
			return i;
	}
	return -1;
}

bool SceneGame::Screen2Plate(const CCPoint& ptSceen, int& row, int& col)
{
	// 遍历所有象棋坐标点，计算棋盘格子中心坐标点到点击的店的距离，如果小于半径，那么就对了
	int distance = Stone::_d*Stone::_d / 4;
	for (row = 0; row <= 9; ++row)
		for (col = 0; col <= 8; ++col)
		{
			CCPoint ptCenter = Plate2Screen(row, col);
			// getDistance, getDistanceSq
			if (ptCenter.getDistanceSq(ptSceen) < distance)
			{
				return true;
			}
		}
	return false;
}

CCPoint SceneGame::Plate2Screen(int row, int col)
{
	int x = col * Stone::_d + Stone::_offx;
	int y = row * Stone::_d + Stone::_offy;

	return ccp(x, y);
}

void SceneGame::MoveStone(CCTouch* touch)
{
	CCPoint ptClicked = touch->getLocation();
	int row, col;
	bool bClick = Screen2Plate(ptClicked, row, col);
	// 如果点击在棋盘外，该次点击无效
	if (!bClick)
	{
		return;
	}

	// 检查目标位置是否有棋子，如果有，那么杀掉
	int clickid = getStoneFromRowCol(row, col);
	// 如果点击位置没有象棋，那么点击也无效
	if (clickid != -1)
	{
		// 如果后一次点击的棋子和前一次相同，那么换选择
		if (isSameColor(clickid, _selectid))
		{
			_selectid = clickid;
			// 显示该棋子被选中的效果
			CCPoint pt = Plate2Screen(row, col);
			_selectSprite->setPosition(pt);
			_selectSprite->setVisible(true);

			// 换选择之后，这次点击处理就结束了
			return;
		}
	}

	// 判断棋子是否可以移动
	if (!canMove(_selectid, row, col, clickid))
	{
		return;
	}

	// 可以移动
	// 记录移动信息
	recordStep(_selectid, clickid, _s[_selectid]->_row, _s[_selectid]->_col, row, col);

	// 走棋相关
	if (clickid != -1)
	{
		// 杀掉
		_s[clickid]->setVisible(false);
		_s[clickid]->_dead = true;
	}


	_s[_selectid]->_row = row;
	_s[_selectid]->_col = col;
	_s[_selectid]->setPosition(Plate2Screen(row, col));
	
	_selectid = -1;
	_selectSprite->setVisible(false);
	_bRedTurn = !_bRedTurn;

}

bool SceneGame::canMove(int moveid, int row, int col, int killid)
{
	Stone* s = _s[moveid];
	switch (s->_type)
	{
	case Stone::CHE:
		return canMoveChe(moveid, row, col);

	case Stone::MA:
		return canMoveMa(moveid, row, col);

	case Stone::PAO:
		return canMovePao(moveid, row, col, killid);

	case Stone::BING:
		return canMoveBing(moveid, row, col);

	case Stone::JIANG:
		return canMoveJiang(moveid, row, col, killid);

	case Stone::SHI:
		return canMoveShi(moveid, row, col);

	case Stone::XIANG:
		return canMoveXiang(moveid, row, col);
	}
	return false;
}

bool SceneGame::canMoveChe(int moveid, int row, int col)
{
	// 一条线,中间不能有棋子
	Stone* s = _s[moveid];

	return getStoneCount(s->_row, s->_col, row, col) == 0;
}

int SceneGame::getStoneCount(int row1, int col1, int row2, int col2)
{
	int ret = 0;
	if (row1 != row2 && col1 != col2) return -1;
	if (row1 == row2 && col1 == col2) return -1;

	// row1 == row2 或者col1 == col2

	if (row1 == row2)
	{
		int min, max;
		min = col1 < col2 ? col1 : col2;
		max = col1 > col2 ? col1 : col2;
		for (int col = min + 1; col < max; ++col)
		{
			int id = getStoneFromRowCol(row1, col);
			if (id != -1) ++ret;
		}
	}
	else
	{
		int min, max;
		min = row1 < row2 ? row1 : row2;
		max = row1 > row2 ? row1 : row2;
		for (int row = min + 1; row < max; ++row)
		{
			int id = getStoneFromRowCol(row, col1);
			if (id != -1) ++ret;
		}
	}

	return ret;
}

bool SceneGame::canMovePao(int moveid, int row, int col, int killid)
{
	Stone* s = _s[moveid];
	if (killid == -1)
		return getStoneCount(s->_row, s->_col, row, col) == 0;

	return getStoneCount(s->_row, s->_col, row, col) == 1;
}

bool SceneGame::canMoveMa(int moveid, int row, int col)
{
	Stone* s = _s[moveid];
	int dRow = abs(s->_row - row);
	int dCol = abs(s->_col - col);
	int d = dRow * 10 + dCol;
	// 马走日条件
	if (d == 12 || d == 21)
	{
		// 蹩脚位置
		int cRow, cCol;
		if (d == 12)
		{
			cCol = (col + s->_col) / 2;
			cRow = s->_row;
		}
		else
		{
			cCol = s->_col;
			cRow = (s->_row + row) / 2;
		}

		// 没有蹩脚的棋子
		if (getStoneFromRowCol(cRow, cCol) == -1)
			return true;
	}

	return false;
}

bool SceneGame::canMoveBing(int moveid, int row, int col)
{
	Stone* s = _s[moveid];
	int dRow = abs(s->_row - row);
	int dCol = abs(s->_col - col);
	int d = dRow * 10 + dCol;

	// 走太远不行,只能走一格
	if (d != 1 && d != 10)
		return false;

	if (s->_red)
	{
		// 不能后退
		if (row < s->_row) return false;

		// 过河前不能平移
		if (s->_row <= 4 && s->_row == row) return false;
	}
	else
	{
		if (row > s->_row) return false;
		if (s->_row >= 5 && s->_row == row) return false;
	}

	return true;
}

bool SceneGame::canMoveJiang(int moveid, int row, int col, int killid)
{
	Stone* s = _s[moveid];

	// 将照面
	if (killid != -1)
	{
		Stone* kill = _s[killid];
		if (kill->_type == Stone::JIANG)
		{
			return canMoveChe(moveid, row, col);
		}
	}

	int dRow = abs(s->_row - row);
	int dCol = abs(s->_col - col);
	int d = dRow * 10 + dCol;
	// 走太远不行,只能走一格
	if (d != 1 && d != 10)
		return false;

	// 不能出九宫
	if (col < 3 || col > 5) return false;
	if (s->_red)
	{
		if (row < 0 || row > 2) return false;
	}
	else
	{
		if (row < 7 || row > 9) return false;
	}

	return true;
}

bool SceneGame::canMoveShi(int moveid, int row, int col)
{
	Stone* s = _s[moveid];
	int dRow = abs(s->_row - row);
	int dCol = abs(s->_col - col);
	int d = dRow * 10 + dCol;
	if (d != 11) return false;

	// 不能出九宫
	if (col < 3 || col > 5) return false;
	if (s->_red)
	{
		if (row < 0 || row > 2) return false;
	}
	else
	{
		if (row < 7 || row > 9) return false;
	}

	return true;
}

bool SceneGame::canMoveXiang(int moveid, int row, int col)
{
	Stone* s = _s[moveid];
	int dRow = abs(s->_row - row);
	int dCol = abs(s->_col - col);
	int d = dRow * 10 + dCol;
	if (d != 22) return false;

	// 象眼
	int cRow, cCol;
	cRow = (row + s->_row) / 2;
	cCol = (col + s->_col) / 2;
	if (getStoneFromRowCol(cRow, cCol) != -1)
		return false;

	// 判断象不能过河
	if (s->_red)
	{
		if (row > 4) return false;
	}
	else
	{
		if (row < 5) return false;
	}

	return true;
}

// 记录移动信息
void SceneGame::recordStep(int moveid, int killid, int rowFrom, int colFrom, int rowTo, int colTo)
{
	Step* s = new Step();
	s->colFrom = colFrom;
	s->colTo = colTo;
	s->killid = killid;
	s->moveid = moveid;
	s->rowFrom = rowFrom;
	s->rowTo = rowTo;

	_steps.push_back(s);
}

// 增加控制面板(包括悔棋按钮)
void SceneGame::addCtrlPanel()
{
	CCMenu* menu = CCMenu::create();
	addChild(menu);

	CCMenuItemImage* regretItem = CCMenuItemImage::create("regret.jpg", "regret.jpg",
		this, menu_selector(SceneGame::Regret));
	menu->addChild(regretItem);

	moveNode(regretItem, ccp(160, 60));
}

void SceneGame::Regret(CCObject*)
{
	// 游戏还没有开始不能悔棋
	if (_steps.size() == 0)
		return;

	// 恢复最后一步
	Step* step = *_steps.rbegin();
	_steps.pop_back();

	// 具体恢复工作
	Stone* s = _s[step->moveid];
	s->_row = step->rowFrom;
	s->_col = step->colFrom;
	s->setPosition(s->fromPlate());

	Stone* kill;
	if (step->killid != -1)
	{
		kill = _s[step->killid];
		kill->_dead = false;
		kill->setVisible(true);
	}

	_bRedTurn = !_bRedTurn;
	delete step;

	// 避免在选中棋子的情况下，悔棋
	_selectid = -1;
	_selectSprite->setVisible(false);
}

