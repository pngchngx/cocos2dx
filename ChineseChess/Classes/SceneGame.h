#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#include "Stone.h"
#include "cocos2d.h"
USING_NS_CC;
#include <vector>
using namespace std;

struct Step
{
	int moveid;
	int killid;
	int rowFrom;
	int colFrom;
	int rowTo;
	int colTo;
};

class SceneGame : public CCLayer
{
public:
	static CCScene* createScene();
	CREATE_FUNC(SceneGame);
	bool init();

	void CreatePlate();

	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);

	//////////////////////////////ʵ���߼�����
	void SelectStone(CCTouch* touch);
	void MoveStone(CCTouch* touch);

	void recordStep(int moveid, int killid, int rowFrom, int colFrom, int rowTo, int colTo);

	// �ƶ�����
	bool canMove(int moveid, int row, int col, int killid);
	bool canMoveChe(int moveid, int row, int col);
	bool canMovePao(int moveid, int row, int col, int killid);
	bool canMoveMa(int moveid, int row, int col);
	bool canMoveBing(int moveid, int row, int col);
	bool canMoveJiang(int moveid, int row, int col, int killid);
	bool canMoveShi(int moveid, int row, int col);
	bool canMoveXiang(int moveid, int row, int col);

	//void computerMove(float);

	//////////////////////////////��������
	// ͨ����Ļ���꣬��ȡ�������꣬����ֵ�����false��ʾptScreen��������
	bool Screen2Plate(const CCPoint& ptSceen, int& row, int& col);
	CCPoint Plate2Screen(int row, int col);

	// ͨ���������꣬��ȡ�����ϵ�����ID���������-1��ʾ��λ����û�����ӣ��������-1�����Ƕ�Ӧ����id
	int getStoneFromRowCol(int row, int col);

	// �ж�����������ɫ�Ƿ���ͬ
	bool isSameColor(int id1, int id2)
	{
		return _s[id1]->_red == _s[id2]->_red;
	}

	// ��ȡ���������֮������Ӹ���
	int getStoneCount(int row1, int col1, int row2, int col2);

	//���ӿ�����壨�������尴ť��
	void addCtrlPanel();
	void Regret(CCObject*);
	void moveNode(CCNode* node, CCPoint pt)
	{
		node->setPosition(node->getPosition() + pt);
	}

private:
	Stone* _s[32];
	// ��¼�Ѿ�ѡ������ӵ�ID,�����-1��ʾ֮ǰû�����屻ѡ��,�������-1��ô��ʾ����ID
	int _selectid;
	// ѡ�б�Ǿ���
	CCSprite* _selectSprite;
	// ���Ŀǰ��˭��
	bool _bRedTurn;
	// ��¼����켣��������
	vector<Step*> _steps;
};

#endif // __SCENE_GAME_H__