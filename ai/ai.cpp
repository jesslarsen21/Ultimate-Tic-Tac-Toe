//#include "AIPlayer.h"
//#include "HumanPlayer.h"
//#include "UTTT.h"
//#include <ctime>
//#include <cstdlib>
//#include <iostream>
//
//
//
//bool updatePlayer(UTTT *game, HumanPlayer *hp)
//{
//	int i, j;
//	for (i = MIN_BOARD_POSITION; i <= MAX_BOARD_POSITION; i++)
//	{
//		const TTT &board = game->getBoard(i);
//		for (j = MIN_BOARD_POSITION; j <= MAX_BOARD_POSITION; j++)
//		{
//			int marker = board.getMarkerAtPosition(j);
//			hp->setValue(marker, i, j);
//		}
//		hp->setBoardWinner(i, board.getOwner());
//	}
//	hp->setTurn(game->getCurrPlayer());
//	hp->setNextBoard(game->getCurrentBoard());
//	return true;
//}
//
//
//int main0()
//{
//	std::srand(std::time(0));
//	UTTT game;
//	int  player_turn;
//	int  board_position;
//	int  marker_position;
//	bool ok;
//	int  big_count = 0;
//
//	AIPlayer cp;
//	HumanPlayer hp;
//	hp.showBoard();
//
//	while (game.isGameOver() == PLAYER0 && big_count < 10)
//	{
//		player_turn = game.getCurrPlayer();
//		if (player_turn == PLAYER2)
//		{
//			ok = hp.getMove(board_position, marker_position);
//			if (ok)
//			{
//				ok = game.takeTurn(player_turn, board_position, marker_position);
//			}
//		}
//		else
//		{
//			ok = cp.getMove(board_position, marker_position);
//			if (ok)
//			{
//				ok = game.takeTurn(player_turn, board_position, marker_position);
//			}
//		}
//	}
//	big_count = ok ? 0 : (big_count + 1);
//	if (ok)
//	{
//		std::cout << std::endl << std::endl;
//		game.printGame();
//		updatePlayer(&game, &hp);
//	}
//
//	std::cout << std::endl << std::endl;
//	game.printGame();
//
//	return 0;
//}
