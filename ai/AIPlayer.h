#pragma once

#ifndef _AIPLAYER_H_
#define _AIPLAYER_H_

#include "Player.h"
#include <vector>

class AIPlayer : public Player
{
public:
	AIPlayer();
	int GetInnerCell(int o_pos, int i_pos);
	virtual bool getMove(int &board_out, int &position_out);
	int getRandWinnableBoard();
	std::vector<int> getRandomMove();
};

#endif