#include "AIPlayer.h"
#include "debug.h"
#include <vector>
#include <cstdlib>

AIPlayer::AIPlayer()
{}

int AIPlayer::GetInnerCell(int o_pos, int i_pos)
{
	return board[o_pos][i_pos];
}

bool AIPlayer::getMove(int &board_out, int &position_out)
{
	if (isMyTurn())
	{
		std::vector<int> valid_moves;
		if (nextBoard == -1)
		{
			std::vector<int> move = getRandomMove();
			if (move[0] == -1 || move[1] == -1 || move.size() != 2)
			{
				return false;
			}
			else
			{
				board_out = move[0];
				position_out = move[1];
				MSG("Player " << this->player << "\'s move: " << "board[" << board_out << "][" << position_out << "]");
				MSG("opponent must play on board[" << position_out << "]");
				return true;
			}
		}

		for (int i = 0; i < 9; i++)
		{
			if (board[nextBoard][i] == NO_PLAYER)
				valid_moves.push_back(i);
		}
		if (valid_moves.size() > 0)
		{
			board_out = nextBoard;
			position_out = valid_moves[(std::rand() % valid_moves.size())];
			MSG("Player " << this->player << "\'s move: " << "board[" << board_out << "][" << position_out << "]");
			MSG("opponent must play on board[" << position_out << "]");
			return true;
		}

		else
		{
			std::vector<int> move = getRandomMove();
			if (move[0] == -1 || move.size() != 2)
				return false;
			else
			{
				board_out = move[0];
				position_out = move[1];
				MSG("Player " << this->player << "\'s move: " << "board[" << board_out << "][" << position_out << "]");
				MSG("opponent must play on board[" << position_out << "]");
				return true;
			}
		}
	}
	else
	{
		return false;
	}
}

std::vector<int> AIPlayer::getRandomMove()
{
	std::vector<int> move;
	std::vector<int> valid_moves;
	int rand_chosen_board = getRandWinnableBoard();
	if (rand_chosen_board != -1)
	{
		for (int i = 0; i < 9; i++)
		{
			//MSG("board[" << rand_chosen_board << "][" << i << "] == NO_PLAYER ==> " << (board[rand_chosen_board][i] == NO_PLAYER));
			if (board[rand_chosen_board][i] == NO_PLAYER)
			{
				valid_moves.push_back(i);
				//MSG("board[" << rand_chosen_board << "][" << i << "] pushed to valid_moves.");
			}
		}
		//MSG("valid_moves.size() > 0 == " << (valid_moves.size() > 0) << std::endl);
		if (valid_moves.size() > 0)
		{
			int position = valid_moves[(std::rand() % valid_moves.size())];
			move.push_back(rand_chosen_board);
			move.push_back(position);
			return move;
		}
	}
	move.clear();
	move.push_back(-1);
	move.push_back(-1);
	//MSG(__LINE__ << "  getMove returns false.");
	return move;
}

int AIPlayer::getRandWinnableBoard()
{
	std::vector<int> playable_boards;
	std::vector<int> winnable_boards;
	bool break_mark = false;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (board[i][j] == NO_PLAYER)
			{
				playable_boards.push_back(i);
				//MSG("board[" << i << "] pushed to playable_boards.");
				break_mark = true;
			}
			if (break_mark)
				break;
		}
		if (break_mark)
		{
			break_mark = false;
			continue;
		}
	}
	for (size_t k = 0; k < playable_boards.size(); k++)
	{
		if (boardWinners[playable_boards[k]] == NO_PLAYER)
			winnable_boards.push_back(playable_boards[k]);
	}

	if (winnable_boards.size() > 0)
	{
		return winnable_boards[rand() % winnable_boards.size()];
	}
	else if (playable_boards.size() > 0)
	{
		return playable_boards[rand() % playable_boards.size()];
	}
	else {
		return -1;
	}
}
