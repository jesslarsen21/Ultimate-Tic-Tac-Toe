#include "HumanPlayer.h"
#include "debug.h"
#include <iostream>

HumanPlayer::HumanPlayer()
{}

bool HumanPlayer::getMove(int &board_out, int &position_out)
{
	if (isMyTurn())
	{
		std::cout << "Move (b=" << nextBoard << " p)? ";
		std::cin >> board_out >> position_out;
		MSG("Player " << this->player << "\'s move: " << "board[" << board_out << "][" << position_out << "]");
		MSG("opponent must play on board[" << position_out << "]");
		return true;
	}
	else
	{
		return false;
	}
}
