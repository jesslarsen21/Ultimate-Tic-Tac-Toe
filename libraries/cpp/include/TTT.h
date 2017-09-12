#ifndef _TTT_H_
#define _TTT_H_
#include <utility>

const int TTT_BOARD_SIZE = 3;
const int TTT_NUM_POSITIONS = TTT_BOARD_SIZE*TTT_BOARD_SIZE-1;
enum { PLAYER_N, PLAYER_X, PLAYER_O };

struct Position {
	int col;
	int row;
};

class TTT
{
	/* positions : 0 - 8 (in the obvious order)
	 * player is : 1 or 2; 0 == none
	 */
public:
	TTT();

	bool PlaceMarker(int player, int position); //returns true if successfully placed marker
	int getMarkerAtPosition(int position) const;
	bool checkForWin();
	int getOwner() const; // returns player who won this board
	bool isFull() const; // true if no empty positions
	void printBoard() const; // --SC-- moved up for testing
	void printrow(int row); // --SC-- called by UTTT -- prints row 0-2 in board 

	//--JL-- removed virtual	
	void FillBoard(); //fill the array with zeros
	static Position getCoordinates(int board);

protected:
	
	int mBoard[TTT_BOARD_SIZE][TTT_BOARD_SIZE];
	bool checksForWin(int winCheck[TTT_BOARD_SIZE]);
	int mOwner;
};

#endif
