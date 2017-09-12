#pragma once

#ifndef _UTTT_H_
#define _UTTT_H_

#include "TTT.h"

// Game controls player's turn
const int UTTT_BOARD_SIZE = 3;
const int UTTT_NUM_POSITIONS = UTTT_BOARD_SIZE*UTTT_BOARD_SIZE-1;
const int ANY_BOARD = -1;
const int TIE = 3;

// When a player makes a move, board and position inside of board
// 0 1 2
// 3 4 5
// 6 7 8
// new board = last board position
// if full player can go anywhere.

class UTTT
{
public:
	UTTT();
	/* Records valid move in UBoard. -- player on board at posistion.
		Checks for win. Ends players turn by switches whose turn it is. */
	bool takeTurn(int player, int board, int position);

	// Returns owner of board
	int getOwner(int board);
	// Returns winner (1, 2, or 0 if not won, 3 = game over, no one won.)
	int getWinner();
	// Returns whose turn it is.
	int getPlayer();
	// Returns board that is the legal board to play on.
	int getNextBoard() const;
	// Returns the board object at position.
	const TTT &getBoard(const int board) const;

	// Returns the marrker at given position in given board. Returns player 1, player 2, or 0 if neither.
	int getPosition(int board, int position);

	bool isBigBoardFull();
	//has ownerChanged
	bool hasOwnerChanged(int board);
	//has winnerChanged
	bool hasWinnerChanged();
	//has playerChanged
	bool hasPlayerChanged();
	//has nextBoardChanged
	bool hasNextBoardChanged();

	//has positionChanged
	bool hasPositionChanged(int board, int position);
	// resets all arays to false;
	void resetChangeArrays();

	// Print current state of game
	void printGame();


protected:
	// Board where move is taking place is legal.
	bool isValidMove(int board);
	//	Do three boards in a row have same owner?
	bool checkForBigWin();
	// Checks if owners match. Helper for finding a winner.
	bool checksForWin(int winCheck[UTTT_BOARD_SIZE]);
	// Sets mPlayer to other player.
	int changePlayer();
	// Set up UBoard.
	void FillBoard();
	// Returns coordinates of board within the game.
	static Position getCoordinates(int board);


	TTT UBoard[UTTT_BOARD_SIZE][UTTT_BOARD_SIZE]; // Full game board.
	int mPlayer; // Current player.
	int mWinner; // Winner of game.
	int mNextBoard; // Next board to be played on.
	bool big[9]; // bool array that stores wether or not something has changed in one of the small boards
	bool small[9][9]; // bool array that stores what postions on the small board have changed.
	bool miscChanges[4];
					// [0] = player change
					// [1] = winner change
					// [2] = nextBoard change
					// [3] = owner change

};

#endif
