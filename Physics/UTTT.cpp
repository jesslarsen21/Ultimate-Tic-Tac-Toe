#include "UTTT.h"
#include <iostream>
#include <cstdlib>
#include <utility>
#define print(x) std::cout<<x<<std::endl

UTTT::UTTT(){
	FillBoard();
	mPlayer = PLAYER_X;
	mWinner = 0;
	mNextBoard = ANY_BOARD;
	//set all changes to true.
	for (int i = 0; i < 9; i++)

	{
		big[i] = true;
		for (int k = 0; k < 9; k++)
		{
			small[i][k] = true;
		}
	}
	
		
	miscChanges[0] = true;
	miscChanges[1] = true;
	miscChanges[2] = true;
	miscChanges[3] = true;
}

// Is board legal?
bool UTTT::isValidMove(int board)
{
	// Must be in range.
	if (board > 8 || board < 0)
	{
		return false;
	}
	if (getOwner(board) != PLAYER_N || getBoard(board).isFull())
		{ // board is unplayable (won or full)
			return false;
		}
	// Full or newgame is legal.
	return mNextBoard == ANY_BOARD || mNextBoard == board;
}

int UTTT::getWinner()
{
	return mWinner;
}

//when player makes move, board and position inside of board
// 0 1 2	(0,0) (0,1) (0,2)
// 3 4 5	(1,0) (1,1) (1,2)
// 6 7 8	(2,0) (2,1) (2,2)

// new board = last board position
// is full method

// [0] = player change
// [1] = winner change
// [2] = nextBoard change
// [3] = owner change
bool UTTT::isBigBoardFull()
{
	for (int col=0; col<UTTT_BOARD_SIZE; col++)
	{
		for (int row=0; row<UTTT_BOARD_SIZE; row++)
		{
			bool full = UBoard[row][col].isFull();
			int owner = UBoard[row][col].getOwner();
			if (!full && owner == PLAYER_N) return false;
		}
	}
	
	return true;
}

bool UTTT::takeTurn(int player, int board, int position)
{
	if (isBigBoardFull() || mWinner > 0) return false;
	
	if (!isValidMove(board) || player != mPlayer) return false;

	Position coords = getCoordinates(board);
	int owner1 = UBoard[coords.row][coords.col].getOwner();
	bool successful = UBoard[coords.row][coords.col].PlaceMarker(mPlayer, position);
	if (!successful) return false;


	// set changed marker in small and big
	big[board] = true;
	small[board][position] = true;

	//check if owner has changed
	int owner2 = UBoard[coords.row][coords.col].getOwner();
	if (owner1 != owner2)
	{

		miscChanges[3] = true;
	}

	// check for win
	bool win = checkForBigWin();
	if(win)
	{
		// set winner
		mWinner = mPlayer;
		miscChanges[1] = true;
	}
	if (!isBigBoardFull())
	{
		// set up for next players turn
		changePlayer();

		//check if the next positions board is full, if it is set nextBoard to -1
		Position next_coords = getCoordinates(position);
		miscChanges[2] = true;
		if (UBoard[next_coords.row][next_coords.col].isFull() ||
				UBoard[next_coords.row][next_coords.col].getOwner() != PLAYER_N)
		{
			mNextBoard = ANY_BOARD;
		}
		else
		{
			mNextBoard = position;
		}
	} 
	else
	{
		mWinner = TIE;
		miscChanges[1] = true;
	}
	
	return true;	
}

// bool miscChanges[4]
// [0] = player change
int UTTT::changePlayer()
{
	if (mPlayer == PLAYER_N)
	{
		mPlayer = PLAYER_X;
	}
	else if (mPlayer == PLAYER_X)
	{
		mPlayer = PLAYER_O;
	}
	else if (mPlayer == PLAYER_O)
	{
		mPlayer = PLAYER_X;
	}
	miscChanges[0] = true;
	return mPlayer;
}

// Check for win.
bool UTTT::checkForBigWin(){
	int temp[] = { 0, 0, 0 };

	// Check vertical win
	for (int col = 0; col < UTTT_BOARD_SIZE; col++){
		temp[0] = UBoard[0][col].getOwner();
		temp[1] = UBoard[1][col].getOwner();
		temp[2] = UBoard[2][col].getOwner();
		if (checksForWin(temp)){
			return true;
		}
	}

	// Check horizontal win
	for (int row = 0; row < UTTT_BOARD_SIZE; row++){
		temp[0] = UBoard[row][0].getOwner();
		temp[1] = UBoard[row][1].getOwner();
		temp[2] = UBoard[row][2].getOwner();
		if (checksForWin(temp)){
			return true;
		}
	}

	// Check left diagonal
	temp[0] = UBoard[0][0].getOwner();
	temp[1] = UBoard[1][1].getOwner();
	temp[2] = UBoard[2][2].getOwner();
	if (checksForWin(temp)){
		return true;
	}

	// Check right diagonal
	temp[0] = UBoard[0][2].getOwner();
	temp[1] = UBoard[1][1].getOwner();
	temp[2] = UBoard[2][0].getOwner();
	return checksForWin(temp);
}

// Check if owners match.
bool UTTT::checksForWin(int winCheck[UTTT_BOARD_SIZE]){
	//checks to see if values are the same, and if they are, there is a win, and the mOwner of the game/square is set to who won.
	bool gamesOver = false;
	if (winCheck[0] == 1 && winCheck[1] == 1 && winCheck[2] == 1)
	{
		gamesOver = true;
	}
	else if (winCheck[0] == 2 && winCheck[1] == 2 && winCheck[2] == 2)
	{
		gamesOver = true;
	}
	return gamesOver;
}

// Fills UBoard with TTT instances
void UTTT::FillBoard(){
	for (int col=0; col<UTTT_BOARD_SIZE; col++)
	{
		for (int row=0; row<UTTT_BOARD_SIZE; row++)
		{
			UBoard[row][col] = TTT();
		}
	}
}

int UTTT::getPlayer(){
	return mPlayer;
}

int UTTT::getNextBoard() const
{
	return mNextBoard;
}


int UTTT::getOwner(int board)
{
	Position coords = TTT::getCoordinates(board);
	int owner = UBoard[coords.row][coords.col].getOwner();
	return owner;
}

/*Position TTT::getCoordinates(int position)
{
	Position coords;
	coords.row = position/TTT_BOARD_SIZE;
	coords.col = position%TTT_BOARD_SIZE;
	return coords;
}*/

int UTTT::getPosition(int board, int position)
{
	Position coordinates = TTT::getCoordinates(board);
	int row = coordinates.row;
	int col = coordinates.col;
	int marker = UBoard[row][col].getMarkerAtPosition(position);
	return marker;
}

// --SC-- Print out current state of the game.
void UTTT::printGame()
{
	for(int br=0;br<UTTT_BOARD_SIZE;br++)
	{
		for(int row=0;row<UTTT_BOARD_SIZE;row++)
		{
			for(int bc=0;bc<UTTT_BOARD_SIZE;bc++)
			{
				UBoard[br][bc].printrow(row);
			}
			std::cout<<std::endl;
		}
		std::cout<<std::endl;
	}
	std::cout<<std::endl;
}


// --JL-- Added to UTTT class since no longer inheriting	from TTT class
Position UTTT::getCoordinates(int board)
{
	Position coords;
	coords.row = board/UTTT_BOARD_SIZE;
	coords.col = board%UTTT_BOARD_SIZE;
	return coords;
}


void UTTT::resetChangeArrays()
{
	for (int i = 0; i < 9; i++)

	{
		big[i] = false;
		for (int k = 0; k < 9; k++)
		{
			small[i][k] = false;
		}
	}
	
	miscChanges[0] = false;
	miscChanges[1] = false;
	miscChanges[2] = false;
	miscChanges[3] = false;
}

// [0] = player change
// [1] = winner change
// [2] = nextBoard change
// [3] = owner change

//has ownerChanged
bool UTTT::hasOwnerChanged(int board)
{
	return miscChanges[3];
}

//has winnerChanged
bool UTTT::hasWinnerChanged()
{
	return miscChanges[1];
}

//has playerChanged
bool UTTT::hasPlayerChanged()
{
	return miscChanges[0];
}

//has nextBoardChanged
bool UTTT::hasNextBoardChanged()
{
	return miscChanges[2];
}

//has positionChanged:
bool UTTT::hasPositionChanged(int board, int position)
{
	return big[board] && small[board][position];
}

const TTT &UTTT::getBoard(const int board) const
{
	Position coords = getCoordinates(board);
	return UBoard[coords.row][coords.col];
}
