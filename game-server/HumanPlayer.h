#ifndef _HUMANPLAYER_H_
#define _HUMANPLAYER_H_

class HumanPlayer
{
public:
  HumanPlayer();
  bool setValue(int player_in, int board_in, int position_in);
  bool setBoardWinner(int board_in, int player_in);
  bool setTurn(int player_in);
  bool setNextBoard(int board_in);
  bool getMove(int &board_out, int &position_out);
	bool prompt();
	bool setPlayer(int player_in);
	int getPlayer() const;

  static const int NUM_BOARDS = 9;
  static const int NUM_POSITIONS = 9;
  static const int NO_PLAYER = 0;
  static const int PLAYER_1 = 1;
  static const int PLAYER_2 = 2;
  
  bool showBoard() const;
  
protected:
  
  int turn;
  int board[NUM_BOARDS][NUM_POSITIONS];
  int boardWinners[NUM_BOARDS];
  int nextBoard;
	int player;
};

#endif /* _HUMANPLAYER_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
