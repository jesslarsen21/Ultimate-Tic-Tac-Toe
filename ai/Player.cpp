
#include "Player.h"
#include <iostream>

Player::Player()
{
	int i, j;

	turn = PLAYER_1;
	nextBoard = 0;

	for (i = 0; i < NUM_BOARDS; i++)
	{
		for (j = 0; j < NUM_POSITIONS; j++)
		{
			board[i][j] = NO_PLAYER;
		}
		boardWinners[i] = NO_PLAYER;
	}
}

Player::Player(int player_num)
:player(player_num)
{}

Player::~Player()
{
}

bool Player::setValue(int player_in, int board_in, int position_in)
{
	this->board[board_in][position_in] = player_in;
	return true;
}

bool Player::setBoardWinner(int board_in, int player_in)
{
	this->boardWinners[board_in] = player_in;
	return true;
}

bool Player::setNextBoard(int board_in)
{
	this->nextBoard = board_in;
	return true;
}

bool Player::setTurn(int player_in)
{
	turn = player_in;
	return true;
}

int Player::getTurn() const
{
	return this->turn;
}

bool Player::setPlayer(int player_in)
{
	this->player = player_in;
	return true;
}

int Player::getPlayer() const
{
	return this->player;
}

bool Player::isMyTurn()
{
	return (this->getPlayer() == this->getTurn());
}

bool Player::showBoard() const
{
	int row, col;
	int board_in, position_in;
	for (row = 0; row < 9; row++)
	{
		for (col = 0; col < 9; col++)
		{
			board_in = 3 * (row / 3) + col / 3;
			position_in = 3 * (row % 3) + col % 3;

			if (col % 3 == 0)
			{
				if (row % 3 == 0 && boardWinners[board_in] != NO_PLAYER)
				{
					std::cout << boardWinners[board_in] << " ";
				}
				else
				{
					std::cout << "  ";
				}
			}
			std::cout << board[board_in][position_in] << " ";
			if (((col + 1) % 3) == 0) { std::cout << "  "; }
		}
		std::cout << std::endl;
		if (((row + 1) % 3) == 0) { std::cout << std::endl; }
	}
	return true;
}

int Player::getNextBoard() const
{
	return this->nextBoard;
}

int Player::getBoardWinner(int i) const
{
	return this->boardWinners[i];
}

int Player::getBoardPositionMarker(int board_in, int position_in) const
{
	return this->board[board_in][position_in];
}

std::ostream &operator<<(std::ostream &os, const Player &p)
{
	os << "Player: " << p.getPlayer() << std::endl;
	os << "nextBoard: " << p.getNextBoard() << std::endl;
	int row, col;
	int board_in, position_in;
	for (row = 0; row < 9; row++)
	{
		for (col = 0; col < 9; col++)
		{
			board_in = 3 * (row / 3) + col / 3;
			position_in = 3 * (row % 3) + col % 3;

			if (col % 3 == 0)
			{
				if (row % 3 == 0 && p.getBoardWinner(board_in) != 0)
				{
					os << p.getBoardWinner(board_in) << " ";
				}
				else
				{
					os << "  ";
				}
			}
			os << p.getBoardPositionMarker(board_in, position_in) << " ";
			if (((col + 1) % 3) == 0) { std::cout << "  "; }
		}
		os << std::endl;
		if (((row + 1) % 3) == 0) { os << std::endl; }
	}
	return os;
}
