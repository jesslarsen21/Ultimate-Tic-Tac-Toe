#pragma once

#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "UTTT.h"
#include <iostream>

/*
	This is a Player class which creates a player for the game of:
	Ultimate Tic-Tac-Toe

	Player(); -initialized with:
		- board  = array of size NUM_BOARDS, board[i] = array of size NUM_POSITIONS with
			all indexes set to 0 (NO_PLAYER value). ex- board[0][0] = 0 ... board[8][8] = 0
		- boardWinners = array of size NUM_BOARDS with all indexes set to 0 (NO_PLAYER value).
			ex- boardWinners[0] = 0 ... boardWinners[8] = 0
		- 
	
*/


class Player
{
public:
	Player();
	virtual ~Player();
	Player(int player_num);
	virtual bool setValue(int player_in, int board_in, int position_in);
	virtual bool getMove(int &board_out, int &position_out) = 0;
	virtual bool setBoardWinner(int board_in, int player_in);
	virtual bool setNextBoard(int board_in);
	virtual bool setTurn(int player_in);
	virtual int getTurn() const;
	virtual bool showBoard() const;
	virtual bool setPlayer(int player_in);
	virtual int getPlayer() const;
	virtual bool isMyTurn();
	virtual int getNextBoard() const;
	virtual int getBoardWinner(int i) const;
	virtual int getBoardPositionMarker(int board_in, int position_in) const;

	// static data for the board and players
	static const int NUM_BOARDS = 9;
	static const int NUM_POSITIONS = 9;
	static const int NO_PLAYER = 0;
	static const int PLAYER_1 = 1;
	static const int PLAYER_2 = 2;

protected:
	int turn;
	int board[NUM_BOARDS][NUM_POSITIONS];
	int boardWinners[NUM_BOARDS];
	int nextBoard;
	int player;
};

std::ostream &operator<<(std::ostream &os, const Player &p);

#endif
