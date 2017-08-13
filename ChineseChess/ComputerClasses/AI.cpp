#include "AI.h"
#include <vector>
using namespace std;

//enum TYPE{ CHE, MA, PAO, BING, JIANG, SHI, XIANG };

static int g_score[7] =
{
	100,
	45,
	45,
	20,
	1500,
	10,
	10
};

// 计算局面分
int AI::getScore(SceneGame* game)
{
	int blackScore = 0;
	int redScore = 0;

	for (int i = 16; i < 32; ++i)
	{
		if (!game->_s[i]->_dead)
			blackScore += g_score[game->_s[i]->_type];
	}

	for (int i = 0; i < 16; ++i)
	{
		if (!game->_s[i]->_dead)
			redScore += g_score[game->_s[i]->_type];
	}

	return blackScore - redScore;
}

Step* AI::getStep(SceneGame* game)
{
	int highScore = -3000;
	Step* ret = NULL;

	// 产生所有可能的移动，遍历
	vector<Step*> allMove = getAllPossibleMove(game);
	vector<Step*>::iterator it;
	for (it = allMove.begin(); it != allMove.end(); ++it)
	{
		Step* step = *it;
		fakeMove(game, step);
		int score = getScore(game);
		unfakeMove(game, step);
		if (score > highScore)
		{
			highScore = score;
			ret = step;
		}
	}

	for (it = allMove.begin(); it != allMove.end(); ++it)
	{
		Step* step = *it;
		if (step != ret) delete step;
	}

	return ret;
}

vector<Step*> AI::getAllPossibleMove(SceneGame* game)
{
	vector<Step*> ret;
	int min, max;
	// 根据当前红方还是黑方走,决定可能移动的步骤信息
	if (game->_bRedTurn)
	{
		min = 0, max = 16;
	}
	else
	{
		min = 16, max = 32;
	}
	for (int i = min; i < max; ++i)
	{
		if (!game->_s[i]->_dead)
		{
			for (int row = 0; row <= 9; ++row)
				for (int col = 0; col <= 8; ++col)
				{
					int killid = game->getStoneFromRowCol(row, col);
					if (game->canMove(i, row, col, killid))
					{
						Step* step = new Step;
						step->moveid = i;
						step->killid = killid;
						step->rowFrom = game->_s[i]->_row;
						step->colFrom = game->_s[i]->_col;
						step->rowTo = row;
						step->colTo = col;
						ret.push_back(step);
					}
				}
		}
	}
	return ret;
}

void AI::fakeMove(SceneGame* game, Step* step)
{
	Stone* moveStone = game->_s[step->moveid];
	moveStone->_row = step->rowTo;
	moveStone->_col = step->colTo;

	if (step->killid != -1)
	{
		Stone* killStone = game->_s[step->killid];
		killStone->_dead = true;
	}

	game->_bRedTurn = !game->_bRedTurn;
}

void AI::unfakeMove(SceneGame* game, Step* step)
{
	Stone* moveStone = game->_s[step->moveid];
	moveStone->_row = step->rowFrom;
	moveStone->_col = step->colFrom;

	if (step->killid != -1)
	{
		Stone* killStone = game->_s[step->killid];
		killStone->_dead = false;
	}
	game->_bRedTurn = !game->_bRedTurn;
}



//////////////////////////////////////////////////////////////////////

Step* AI::getStep(SceneGame* game, int level)
{
	int highScore = -3000;
	Step* ret = NULL;

	// 产生所有可能的移动，遍历
	vector<Step*> allMove = getAllPossibleMove(game);
	vector<Step*>::iterator it;
	for (it = allMove.begin(); it != allMove.end(); ++it)
	{
		Step* step = *it;
		fakeMove(game, step);
		//int score = getScore(game);
		int score = getMinScore(game, level - 1, highScore);
		unfakeMove(game, step);
		if (score > highScore)
		{
			highScore = score;
			ret = step;
		}
	}

	for (it = allMove.begin(); it != allMove.end(); ++it)
	{
		Step* step = *it;
		if (step != ret) delete step;
	}

	return ret;
}

int AI::getMinScore(SceneGame* game, int level, int curMinScore)
{
	if (level == 0)
		return getScore(game);

	int minScore = 3000;
	// 产生所有可能的移动，遍历
	vector<Step*> allMove = getAllPossibleMove(game);
	vector<Step*>::iterator it;
	for (it = allMove.begin(); it != allMove.end(); ++it)
	{
		Step* step = *it;
		fakeMove(game, step);
		int score = getMaxScore(game, level - 1, minScore);
		unfakeMove(game, step);

		// 减枝算法
		if (score <= curMinScore)
		{
			minScore = score;
			break;
		}

		if (score < minScore)
		{
			minScore = score;
		}
	}

	for (it = allMove.begin(); it != allMove.end(); ++it)
	{
		Step* step = *it;
		delete step;
	}

	return minScore;
}

int AI::getMaxScore(SceneGame* game, int level, int curMaxScore)
{
	if (level == 0)
		return getScore(game);

	int maxScore = -3000;
	// 产生所有可能的移动，遍历
	vector<Step*> allMove = getAllPossibleMove(game);
	vector<Step*>::iterator it;
	for (it = allMove.begin(); it != allMove.end(); ++it)
	{
		Step* step = *it;
		fakeMove(game, step);
		int score = getMinScore(game, level - 1, maxScore);
		unfakeMove(game, step);
		if (score >= curMaxScore)
		{
			maxScore = score;
			break;
		}
		if (score > maxScore)
		{
			maxScore = score;
		}
	}

	for (it = allMove.begin(); it != allMove.end(); ++it)
	{
		Step* step = *it;
		delete step;
	}

	return maxScore;
}


//////////////////////////////////////////////////////////alphaBetaSearch
Step* AI::_step = NULL;
int AI::_maxLevel = 0;

// alpha beta search
Step* AI::getStepByAlphaBetaSearch(SceneGame* game, int level)
{
	// 初始化中间变量
	_maxLevel = level;
	_step = NULL;

	// 搜索
	alphaBetaSearch(game, -3000, 3000, level);

	// 返回搜索结果
	return _step;
}

int AI::alphaBetaSearch(SceneGame* game, int alpha, int beta, int level)
{
	if (level == 0)
		return getScore(game);

	Step* recordStep = NULL;
	vector<Step*> allMove = getAllPossibleMove(game);
	vector<Step*>::iterator it;
	for (it = allMove.begin(); it != allMove.end(); ++it)
	{
		Step* step = *it;
		fakeMove(game, step);
		int score = -alphaBetaSearch(game, -beta, -alpha, level - 1);
		unfakeMove(game, step);
		// 减枝
		if (score >= beta)
		{
			alpha = score;
			break;
		}

		// 获得最小值
		if (score > alpha)
		{
			alpha = score;
			// 记录最有利的移动步骤
			if (level == _maxLevel)
			{
				//	if (_step) delete _step;
				//	_step = new Step(*step);
				//	*_step = *step;
				recordStep = step;
			}
		}
	}

	// 清除内存
	for (it = allMove.begin(); it != allMove.end(); ++it)
	{
		Step* step = *it;
		if (recordStep != step) delete step;
	}

	if (recordStep)
	{
		if (_step) delete _step;
		_step = recordStep;
	}

	return alpha;
}
