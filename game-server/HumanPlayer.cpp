#include "HumanPlayer.h"
#include <iostream>

HumanPlayer::HumanPlayer()
{
	int i, j;
	
	turn = PLAYER_1;
	nextBoard = 0;
	
	for(i = 0; i < NUM_BOARDS; i++)
		{
			for(j = 0; j < NUM_POSITIONS; j++)
				{
					board[i][j] = NO_PLAYER;
				}
			boardWinners[i] = NO_PLAYER;
		}
}

bool HumanPlayer::setValue(int player_in, int board_in, int position_in)
{
	board[board_in][position_in] = player_in;
	return true;
}

bool HumanPlayer::setBoardWinner(int board_in, int player_in)
{
	boardWinners[board_in] = player_in;
	return true;
}

bool HumanPlayer::setTurn(int player_in)
{
	turn = player_in;
	return true;
}

bool HumanPlayer::setNextBoard(int board_in)
{
	nextBoard = board_in;
	return true;
}

bool HumanPlayer::showBoard() const
{
	int row, col;
	int board_in, position_in;
	std::cout << std::endl;
	for(row = 0; row < 9; row++)
		{
			for(col = 0; col < 9; col++)
				{
					board_in = 3*(row / 3) + col / 3;
					position_in = 3*(row % 3) + col % 3;

					if(col % 3 == 0)
						{
							if(row % 3 == 0 && boardWinners[board_in] != NO_PLAYER)
								{
									std::cout << boardWinners[board_in] << " ";
								}
							else
								{
									std::cout << "  ";
								}
						}
					std::cout << board[board_in][position_in] << " ";
					//std::cout << board_in << "," << position_in << " ";
					if(((col + 1) % 3) == 0) { std::cout << "  "; }
				}
			std::cout << std::endl;
			if(((row + 1) % 3) == 0) { std::cout << std::endl; }
		}
	return true;
}

bool HumanPlayer::getMove(int &board_out, int &position_out)
{
	showBoard();
	std::cout << "Move (b=" << nextBoard << " p)? ";
	std::cin >> board_out >> position_out;
	return true;
}

bool HumanPlayer::prompt()
{
	if(turn == player)
		{
			std::cout << "Move ";
		}
	else
		{
			std::cout << "Waiting for opponent ";
		}
	std::cout << "(player=" << turn << " b=" << nextBoard << " p)? " << std::flush;
	return true;
}

bool HumanPlayer::setPlayer(int player_in)
{
	switch(player_in)
		{
		case PLAYER_1:
		case PLAYER_2:
			player = player_in;
			return true;
			break;
		default:
			return false;
			break;
		}
	return false;
}

int HumanPlayer::getPlayer() const
{
	return player;
}
