/* This is demo code to verify that the game engine
 * has the correct API to play a complete game.
 *
 * It SHOULD use the AI to generate moves to apply to the
 * game engine.  As such, it will also become a test
 * of the basic AI engine API.
 *
 * This does not use networking code.
 */

#include "debug.h"
#include "UTTT.h"
#include "Player.h"
#include "HumanPlayer.h"
#include "AIPlayer.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

bool updatePlayer(UTTT *game, Player *p)
{
	int i, j;
	for (i = MIN_BOARD_POSITION; i <= MAX_BOARD_POSITION; i++)
	{
		for (j = MIN_BOARD_POSITION; j <= MAX_BOARD_POSITION; j++)
		{
			p->setValue(game->getPosition(i,j) , i, j);
		}
		p->setBoardWinner(i, game->getBoard(i).getOwner());
	}
	p->setTurn(game->getPlayer());
	p->setNextBoard(game->getNextBoard());
	return true;
}


// to play on the Command Line:
// - simply put hp as an arguement to play human vs human
// - or put ai as arguement to play ai vs ai
// - if no arguements are specified it will assume you want to play ai vs human

int main(int argc, char *argv[])
{
	Player* p1 = new AIPlayer();
	Player* p2 = new HumanPlayer();
	std::string argv1;

	if (argc == 2)
	{
		argv1 = argv[1];
		if (argv1 == "ai")
		{
			p1 = new AIPlayer();
			p2 = new AIPlayer();
		}
		else if (argv1 == "hp")
		{
			p1 = new HumanPlayer();
			p2 = new HumanPlayer();
		}
		else
		{
			std::cerr << "Enter \"hp\" to play human vs human\nEnter \"ai\" to play ai vs ai\nDon't enter arguement to play ai vs human." << std::endl;
		}
	}

	std::srand(std::time(0));
	UTTT game;
	int  player_turn;
	int  board_position;
	int  marker_position;
	bool p1_ok;
	bool p2_ok;
	bool ok;
	int  count;
	int  big_count = 0;
	p1->setPlayer(1);
	p2->setPlayer(2);
	updatePlayer(&game, p1);
	updatePlayer(&game, p2);
	std::cout << std::endl;
	/* FIXME:
	 * Declare 2 AI player objects here
	 */
	
	/* FIXME:
	 * Pass entire game state to each AI player here
	 */
	
	/* Continue as long as the game hasn't finished
	 * and less than 10 turns have failed.
	 */
	while(game.getWinner() == PLAYER0 && big_count < 10)
	{

		p1_ok = p1->getMove(board_position, marker_position);
		if(p1_ok)
		{
			ok = game.takeTurn(p1->getPlayer(), board_position, marker_position);
		}

		p2_ok = p2->getMove(board_position, marker_position);
		if (p2_ok)
		{
			ok = game.takeTurn(p2->getPlayer(), board_position, marker_position);
		}

		big_count = ok ? 0 : (big_count + 1);
		if(ok)
		{
			updatePlayer(&game, p1);
			updatePlayer(&game, p2);
			std::cout << std::endl << std::endl;
			p1->showBoard();
			if (argv1 == "ai")
			{
				int x;
				std::cin >> x;
			}
		}
			
		/* FIXME:
			* Send all game state changes to both AI player objects.
			*/
	}

	std::cout << std::endl << std::endl;
	p1->showBoard();
	return 0;
}
