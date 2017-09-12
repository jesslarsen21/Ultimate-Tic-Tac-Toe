#include "TTT.h"
#include <iostream>
#include <cstdlib>


TTT::TTT()
{
	mOwner = PLAYER_N;
	FillBoard();
}

//returns bool is successfully placed marker
bool TTT::PlaceMarker(int player, int position)
{

	// Check if it is a valid space to play on.
	if(position < 0 || position > TTT_NUM_POSITIONS)
	{
		return false;
	}

	// Check if the space has empty.
	Position coords = getCoordinates(position);
	if(mBoard[coords.row][coords.col] != 0)
	{
		return false; // invalid move
	}

	// Update the board with the valid move.
	mBoard[coords.row][coords.col] = player;

	// If the board hasn't been won, check for a win.
	if(mOwner == PLAYER_N)
	{
		checkForWin();
	}

	// Turn was executed succesfully.
	return true;
}

int TTT::getMarkerAtPosition(int position) const
{
	if (position < 0 || position > TTT_NUM_POSITIONS)
	{
		return 0;
	}
	Position coords = getCoordinates(position);
	return mBoard[coords.row][coords.col];
}

Position TTT::getCoordinates(int position)
{
	Position coords;
	coords.row = position/TTT_BOARD_SIZE;
	coords.col = position%TTT_BOARD_SIZE;
	return coords;
}

void TTT::FillBoard()
{
	mOwner = PLAYER_N;
	for (int row=0; row<TTT_BOARD_SIZE; row++)
	{
		for (int col=0; col<TTT_BOARD_SIZE; col++)
		{
			this->mBoard[row][col] = PLAYER_N;
		}
	}
}

bool TTT::checkForWin()
{
	int temp[] = { 0, 0, 0 };

	// Check vertical win
	for (int col = 0; col < 3; col++)
	{
		temp[0] = mBoard[0][col];
		temp[1] = mBoard[1][col];
		temp[2] = mBoard[2][col];
		if (checksForWin(temp)) return true;
	}

	// Check horizontal win
	for (int row = 0; row < 3; row++)
	{
		temp[0] = mBoard[row][0];
		temp[1] = mBoard[row][1];
		temp[2] = mBoard[row][2];
		if (checksForWin(temp)) return true;
	}
	// Check left diagonal
	temp[0] = mBoard[0][0];
	temp[1] = mBoard[1][1];
	temp[2] = mBoard[2][2];
	if (checksForWin(temp)) return true;

	// Check right diagonal
	temp[0] = mBoard[0][2];
	temp[1] = mBoard[1][1];
	temp[2] = mBoard[2][0];
	return checksForWin(temp);
}

bool TTT::checksForWin(int winCheck[3])
{
	//checks to see if values are the same, and if they are, there is a win, and the mOwner of the game/square is set to who won.
	bool gamesOver = false;
	if (winCheck[0] == PLAYER_X && winCheck[1] == PLAYER_X && winCheck[2] == PLAYER_X)
	{
		mOwner = PLAYER_X;
		gamesOver = true;
	}
	else if (winCheck[0] == PLAYER_O && winCheck[1] == PLAYER_O && winCheck[2] == PLAYER_O)
	{
		mOwner = PLAYER_O;
		gamesOver = true;
	}
	return gamesOver;
}


int TTT::getOwner() const
{
	return mOwner;
}

void TTT::printBoard() const
{
	std::cout << std::endl;
	for (int row=0; row<TTT_BOARD_SIZE; row++)
	{
		for (int col=0; col<TTT_BOARD_SIZE; col++)
		{
			std::cout << mBoard[row][col] << " ";
		}
		std::cout << std::endl;
	}
}


bool TTT::isFull() const
{
	for (int row=0; row<3; row++)
	{
		for (int col=0; col<3; col++)
		{
			if (mBoard[row][col] == PLAYER_N)
			{
				return false;
			}
		}
	}
	return true;
}

//--SC-- helper for printgame
void TTT::printrow(int row)
{
	for(int i=0;i<3; i++)
	{
		std::cout << mBoard[row][i];
	}
	std::cout<<" ";
}
