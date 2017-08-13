#include "SceneGame.h"
#include "Net.h"

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

	// �ڷ�����
	CreatePlate();

	// ��������:��ɫ/����(che,ma,pao)/λ��/��С/ID

	Stone::_d = CCDirector::sharedDirector()->getWinSize().height / 10;//������
	Stone::_offx = Stone::_d;//����������ʼxƫ��
	Stone::_offy = Stone::_d / 2;//����������ʼyƫ��
								 //�ڷ�����
	for (int i = 0; i < 32; i++)
	{
		_s[i] = Stone::create();
		_s[i]->initStone(i);
		addChild(_s[i]);
	}

	// ʹ�ܴ���
	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(SceneGame::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(SceneGame::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	_selectid = -1;

	_selectSprite = CCSprite::create("selected.png");
	_selectSprite->setVisible(false);//��ʼ���ɼ�
	addChild(_selectSprite);
	_selectSprite->setScale(.6);

	_bRedTurn = true;

	_steps.clear();

	// ���ӿ������(�������尴ť)
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

// ������β����ƶ�����,һ��ѡ��,һ���ƶ�
void SceneGame::onTouchEnded(Touch* touch, Event* event)
{
	// �ƶ�����
	if (_selectid == -1)
	{
		// ��ͼѡ������
		SelectStone(touch);
	}
	else
	{
		// ��ͼ�ƶ�����
		MoveStone(touch);
	}
}

// 1��ͨ��touch��õ��λ��
// 2��ͨ��λ�û��row��col������Ļ����ת������������
// 3��ͨ����������������id
// 4����¼ѡ�е����ӵ�ID
void SceneGame::SelectStone(CCTouch* touch)
{
	CCPoint ptClicked = touch->getLocation();
	int row, col;
	bool bClick = Screen2Plate(ptClicked, row, col);
	// �������������⣬�ôε����Ч
	if (!bClick)
	{
		return;
	}
	int clickid = getStoneFromRowCol(row, col);
	// ������λ��û�����壬��ô���Ҳ��Ч
	if (clickid == -1)
	{
		return;
	}

	// ��������е����ӣ���_bRedTurn��ɫ��һ�£�����ѡ��
	if (_s[clickid]->_red != _bRedTurn)
		return;

	// �ڷ�ֻ�ܵ���壬�췽ֻ�ܵ���壬���������
	if (_s[clickid]->_red != _bRedSide)
		return;

	// ��¼��εĵ��
	_selectid = clickid;

	// ��ʾ�����ӱ�ѡ�е�Ч��
	_selectSprite->setPosition(Plate2Screen(row, col));
	_selectSprite->setVisible(true);

	// ����ѡ�����ӵ���Ϣ���Է�
	char buf[2];
	buf[0] = 1;
	buf[1] = _selectid;
	Net::Send(buf, 2);
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
	// ����������������㣬�������̸�����������㵽����ĵ�ľ��룬���С�ڰ뾶����ô�Ͷ���
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
	// �������������⣬�ôε����Ч
	if (!bClick)
	{
		return;
	}

	// ���Ŀ��λ���Ƿ������ӣ�����У���ôɱ��
	int clickid = getStoneFromRowCol(row, col);
	// ������λ��û�����壬��ô���Ҳ��Ч
	if (clickid != -1)
	{
		// �����һ�ε�������Ӻ�ǰһ����ͬ����ô��ѡ��
		if (isSameColor(clickid, _selectid))
		{
			_selectid = clickid;
			// ��ʾ�����ӱ�ѡ�е�Ч��
			CCPoint pt = Plate2Screen(row, col);
			_selectSprite->setPosition(pt);
			_selectSprite->setVisible(true);

			// ��ѡ��֮����ε������ͽ�����
			return;
		}
	}

	// �ж������Ƿ�����ƶ�
	if (!canMove(_selectid, row, col, clickid))
	{
		return;
	}

	// �����ƶ�
	// ��¼�ƶ���Ϣ
	recordStep(_selectid, clickid, _s[_selectid]->_row, _s[_selectid]->_col, row, col);

	// �������
	if (clickid != -1)
	{
		// ɱ��
		_s[clickid]->setVisible(false);
		_s[clickid]->_dead = true;
	}


	_s[_selectid]->_row = row;
	_s[_selectid]->_col = col;
	_s[_selectid]->setPosition(Plate2Screen(row, col));

	_selectid = -1;
	_selectSprite->setVisible(false);
	_bRedTurn = !_bRedTurn;

	// �����ƶ����ӱ���
	Step* step = *_steps.rbegin();
	char buf[4];
	buf[0] = 2;
	buf[1] = step->moveid;
	buf[2] = step->rowTo;
	buf[3] = step->colTo;
	Net::Send(buf, 4);

	// ������Ϣ
	Net::RecvStart();
	schedule(schedule_selector(SceneGame::CheckRecv));
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
	// һ����,�м䲻��������
	Stone* s = _s[moveid];

	return getStoneCount(s->_row, s->_col, row, col) == 0;
}

int SceneGame::getStoneCount(int row1, int col1, int row2, int col2)
{
	int ret = 0;
	if (row1 != row2 && col1 != col2) return -1;
	if (row1 == row2 && col1 == col2) return -1;

	// row1 == row2 ����col1 == col2

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
	// ����������
	if (d == 12 || d == 21)
	{
		// ����λ��
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

		// û�����ŵ�����
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

	// ��̫Զ����,ֻ����һ��
	if (d != 1 && d != 10)
		return false;

#if 0
	if (s->_red)
#endif
	{
		// ���ܺ���
		if (row < s->_row) return false;

		// ����ǰ����ƽ��
		if (s->_row <= 4 && s->_row == row) return false;
	}
#if 0
	else
	{
		if (row > s->_row) return false;
		if (s->_row >= 5 && s->_row == row) return false;
	}
#endif

	return true;
}

bool SceneGame::canMoveJiang(int moveid, int row, int col, int killid)
{
	Stone* s = _s[moveid];

	// ������
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
	// ��̫Զ����,ֻ����һ��
	if (d != 1 && d != 10)
		return false;

	// ���ܳ��Ź�
	if (col < 3 || col > 5) return false;
#if 0
	if (s->_red)
#endif
	{
		if (row < 0 || row > 2) return false;
	}
#if 0
	else
	{
		if (row < 7 || row > 9) return false;
	}
#endif
	
	return true;
}

bool SceneGame::canMoveShi(int moveid, int row, int col)
{
	Stone* s = _s[moveid];
	int dRow = abs(s->_row - row);
	int dCol = abs(s->_col - col);
	int d = dRow * 10 + dCol;
	if (d != 11) return false;

	// ���ܳ��Ź�
	if (col < 3 || col > 5) return false;
#if 0
	if (s->_red)
#endif
	{
		if (row < 0 || row > 2) return false;
	}
#if 0
	else
	{
		if (row < 7 || row > 9) return false;
	}
#endif

	return true;
}

bool SceneGame::canMoveXiang(int moveid, int row, int col)
{
	Stone* s = _s[moveid];
	int dRow = abs(s->_row - row);
	int dCol = abs(s->_col - col);
	int d = dRow * 10 + dCol;
	if (d != 22) return false;

	// ����
	int cRow, cCol;
	cRow = (row + s->_row) / 2;
	cCol = (col + s->_col) / 2;
	if (getStoneFromRowCol(cRow, cCol) != -1)
		return false;

	// �ж����ܹ���
#if 0
	if (s->_red)
#endif
	{
		if (row > 4) return false;
	}
#if 0
	else
	{
		if (row < 5) return false;
	}
#endif

	return true;
}

// ��¼�ƶ���Ϣ
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

// ���ӿ������(�������尴ť)
void SceneGame::addCtrlPanel()
{
	CCMenu* menu = CCMenu::create();
	addChild(menu);

	CCMenuItemImage* regretItem = CCMenuItemImage::create("regret.jpg", "regret.jpg",
		this, menu_selector(SceneGame::Regret));
	menu->addChild(regretItem);
	moveNode(regretItem, ccp(160, 60));

	// �����������İ�ť
	CCMenuItem* startServerItem = CCMenuItemFont::create("StartServer", this, menu_selector(SceneGame::startServer));
	menu->addChild(startServerItem);
	moveNode(startServerItem, ccp(160, 0));

	// �����ͻ��˵İ�ť
	CCMenuItem* startClientItem = CCMenuItemFont::create("StartClient", this, menu_selector(SceneGame::startClient));
	menu->addChild(startClientItem);
	moveNode(startClientItem, ccp(160, -60));
}

void SceneGame::doRegret()
{
	// ��Ϸ��û�п�ʼ���ܻ���
	if (_steps.size() == 0)
		return;

	// �ָ����һ��
	Step* step = *_steps.rbegin();
	_steps.pop_back();

	// ����ָ�����
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

	// ������ѡ�����ӵ�����£�����
	_selectid = -1;
	_selectSprite->setVisible(false);
}

void SceneGame::doRegret2()
{
	for (int i = 0; i < 2; ++i)
	{
		doRegret();
	}
}

void SceneGame::Regret(CCObject*)
{
	// �ֵ�˭�ߣ�˭��Ȩ����壬��Ȼ���ܵ�
	if (_bRedSide != _bRedTurn)
		return;

	// ���廹û�����������û���
	if (_steps.size() < 2)
		return;

	doRegret2();

	// ������Ϣ
	char buf = 3;
	Net::Send(&buf, 1);
}

void SceneGame::startServer(CCObject*)
{
	_bRedSide = true;
	Net::Listen();
	schedule(schedule_selector(SceneGame::CheckListen));
}

void SceneGame::CheckListen(float)
{
	// ���ӳɹ���
	if (Net::isConnected())
	{
		unschedule(schedule_selector(SceneGame::CheckListen));
		// ��Ϸ��ʼ�ˡ�����һЩ��ʼ���Ĺ���
		//CCLOG("server Start Game ......");
	}
}

void SceneGame::startClient(CCObject*)
{
	_bRedSide = false;
	if (Net::Connect("127.0.0.1"))
	{
		// ��Ϸ��ʼ��
		//CCLOG("client Start Game");

		// �����ӵ�����
		for (int i = 0; i < 32; i++)
		{
			Stone* s = _s[i];
			s->_row = 9 - s->_row;
			s->_col = 8 - s->_col;
			s->setPosition(s->fromPlate());
		}

		// ��ʼ����
		Net::RecvStart();
		schedule(schedule_selector(SceneGame::CheckRecv));
	}
	else
	{
		//CCLOG("Connect failure....");
	}
}

void SceneGame::CheckRecv(float)
{
	if (Net::isRecvComplete())
	{
		unschedule(schedule_selector(SceneGame::CheckRecv));

		int len;
		char* data = Net::RecvData(len);

		// �������ݽ���Э�飬����Э�������幤��
		if (data[0] == 3)
		{
			doRegret2();
			// ��������
			Net::RecvStart();
			schedule(schedule_selector(SceneGame::CheckRecv));
		}
		else if (data[0] == 1)
		{
			// ѡ��
			_selectid = data[1];
			_selectSprite->setPosition(_s[_selectid]->fromPlate());
			_selectSprite->setVisible(true);

			// ��������
			Net::RecvStart();
			schedule(schedule_selector(SceneGame::CheckRecv));
		}
		else if (data[0] == 2)
		{
			// �ƶ�����
			Stone* s = _s[data[1]];
			int row = 9 - data[2];
			int col = 8 - data[3];
			int killid = getStoneFromRowCol(row, col);

			recordStep(_selectid, killid, _s[_selectid]->_row, _s[_selectid]->_col, row, col);

			// �ƶ�����
			s->_row = row;
			s->_col = col;
			s->setPosition(s->fromPlate());
			if (killid != -1)
			{
				Stone* ks = _s[killid];
				ks->_dead = true;
				ks->setVisible(false);
			}

			_selectid = -1;
			_selectSprite->setVisible(false);
			_bRedTurn = !_bRedTurn;
		}
	}
}

