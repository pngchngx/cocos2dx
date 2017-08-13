#ifndef __AI_H__
#define __AI_H__

#include "SceneGame.h"
//struct Step
//{
//	int moveid;
//	int killid;
//	int rowFrom;
//	int colFrom;
//	int rowTo;
//	int colTo;
//};

class AI
{
public:
	// ���ݵ�ǰ���棬����һ��Step
	static Step* getStep(SceneGame* game);

	// �������
	static int getScore(SceneGame* game);

	// �������ܵ��ƶ�
	static vector<Step*> getAllPossibleMove(SceneGame* game);

	static void fakeMove(SceneGame* game, Step* step);
	static void unfakeMove(SceneGame* game, Step* step);

	// �ಽ�����㷨ʵ��
	static Step* getStep(SceneGame* game, int level);
	static int getMinScore(SceneGame* game, int level, int curMinScore);
	static int getMaxScore(SceneGame* game, int level, int curMaxScore);

	// alphaBeta�㷨
	static Step* getStepByAlphaBetaSearch(SceneGame* game, int level);
	static int alphaBetaSearch(SceneGame* game, int alpha, int beta, int level);
	// ��alphabeta�㷨����ʱʹ�õ��м����
	static int _maxLevel;
	static Step* _step;
};

#endif // __AI_H__