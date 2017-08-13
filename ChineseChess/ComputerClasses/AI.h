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
	// 根据当前局面，产生一个Step
	static Step* getStep(SceneGame* game);

	// 计算分数
	static int getScore(SceneGame* game);

	// 产生可能的移动
	static vector<Step*> getAllPossibleMove(SceneGame* game);

	static void fakeMove(SceneGame* game, Step* step);
	static void unfakeMove(SceneGame* game, Step* step);

	// 多步智能算法实现
	static Step* getStep(SceneGame* game, int level);
	static int getMinScore(SceneGame* game, int level, int curMinScore);
	static int getMaxScore(SceneGame* game, int level, int curMaxScore);

	// alphaBeta算法
	static Step* getStepByAlphaBetaSearch(SceneGame* game, int level);
	static int alphaBetaSearch(SceneGame* game, int alpha, int beta, int level);
	// 给alphabeta算法计算时使用的中间变量
	static int _maxLevel;
	static Step* _step;
};

#endif // __AI_H__