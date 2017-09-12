/* This is demo code to verify that the game engine
 * has the correct API to play a complete game.
 *
 * It SHOULD use the AI to generate moves to apply to the
 * game engine.  As such, it will also become a test
 * of the basic AI engine API.
 *
 * This does not use networking code.
 */

#include "UTTT.h"
#include "HumanPlayer.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

bool updatePlayer(UTTT *game, HumanPlayer *hp)
{
	int i,j;
	for(i = MIN_BOARD_POSITION; i <= MAX_BOARD_POSITION; i++)
		{
			for(j = MIN_BOARD_POSITION; j <= MAX_BOARD_POSITION; j++)
				{
					int marker = game->getPosition(i,j);
					hp->setValue(marker, i, j);
				}
			hp->setBoardWinner(i, game->getOwner(i));
		}
	hp->setTurn(game->getPlayer());
	hp->setNextBoard(game->getNextBoard());
	return true;
}

int main()
{
	std::srand(std::time(0));
	UTTT game;
	int  player_turn;
	int  board_position;
	int  marker_position;
	bool ok;
	int  count;
	int  big_count = 0;
	/* FIXME:
	 * Declare 2 AI player objects here
	 */
	HumanPlayer hp;
	updatePlayer(&game, &hp);
	

	/* FIXME:
	 * Pass entire game state to each AI player here
	 */
	
	/* Continue as long as the game hasn't finished
	 * and less than 10 turns have failed.
	 */
	while(game.getWinner() == PLAYER0 && big_count < 10)
		{
			player_turn = game.getPlayer();
			if(player_turn == PLAYER2)
				{
					ok = hp.getMove(board_position, marker_position);
					if(ok)
						{
							ok = game.takeTurn(player_turn, board_position, marker_position);
						}
				}
			else
				{
					/* FIXME:
					 * Choose the correct AI player object, and as it for
					 * a move instead of all of the play choosing code here
					 */
					board_position = game.getNextBoard();
					if(board_position < MIN_BOARD_POSITION ||
						 board_position > MAX_BOARD_POSITION)
						{
							board_position =
								(std::rand() %
								 (MAX_BOARD_POSITION - MIN_BOARD_POSITION + 1)) +
								MIN_BOARD_POSITION;
						}
					ok = false;
					count = 0;
					while(!ok && count < 10)
						{
							marker_position =
								(std::rand() %
								 (MAX_BOARD_POSITION - MIN_BOARD_POSITION + 1)) +
								MIN_BOARD_POSITION;
							ok = game.takeTurn(player_turn, board_position, marker_position);
							count ++;
						}
				}
			big_count = ok ? 0 : (big_count + 1);
			if(ok)
				{
					std::cout << std::endl << std::endl;
					game.printGame();
					updatePlayer(&game, &hp);
				}
			
			/* FIXME:
			 * Send all game state changes to both AI player objects.
			 */
		}

	std::cout << std::endl << std::endl;
	game.printGame();
	
	return 0;
}
