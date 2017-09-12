#include "TTT.h"
#include <iostream>
#include <string>
#define print(x) std::cout<<x<<std::endl


void checkwin(int start, int end,int step,int player, TTT a)
{
	for(int i=start;i<end;i+=step)
	{
		a.PlaceMarker(player, i);
	}
	a.printBoard();
	print("Return value form check for win: "<<a.checkForWin());
	a.FillBoard();
}

void testWins(TTT a, int player)
{
	print("Testing all wins");
	print("\nHorizontal wins");

	checkwin( 0, 3,1, player, a);
	checkwin( 3, 6,1, player, a);
	checkwin( 6, 9,1, player, a);
	print("\nVertical wins");
	checkwin(0,9,3, player, a);
	checkwin(1,9,3, player, a);
	checkwin(2,9,3, player, a);
	print("\nDiagnal wins");
	checkwin(0,9,4, player, a);
	checkwin(2,9,2, player, a);
	print("\nNo win");
	checkwin(3,9,4, player, a);

}

void fullboard(TTT a)
{
	// --JL-- Added a call to checkForWin() to set the owner of the board if needed
	a.checkForWin();
	bool f = a.isFull();
	int own = a.getOwner();
	a.printBoard();
	std::string full;
	if (f)
	{	full = "yes" ;}
	else
	{	full = "no" ;}
	print("Board full? "<<full);
	print("Owner of board =  Player: "<<own);
	
}

void testPositions(TTT a,int player)
{
	// should gracelfully ignore out of range plays
	print("Testing positions -1 through 10.");
	print("Positions -1 9 10 should be gracefully ignored. \n");
	// --JL-- All return values are 1 because it is return true, which is a 1/ It should return a 0 now when you try to place a marker in an invalid position
	for(int i= -1;i<=10;i++)
	{
		bool neg = a.PlaceMarker(player, i);
		print("Position: "<<i<<" Return value: "<<neg);
		a.printBoard();
		a.FillBoard();
	}
}

void testFullBoard(TTT a,int player)
{
	print("Testing if board is full.");
	a.PlaceMarker(player, 4);
	fullboard(a);
	a.FillBoard();
	for(int i= 0;i<=9;i++)
	{
		a.PlaceMarker(player, i);
	}
	fullboard(a);
	a.FillBoard();
	a.PlaceMarker(player, 4);
	a.PlaceMarker(player, 6);
	a.PlaceMarker(player, 8);
	fullboard(a);
}

int main()
{
	// --SC-- move printBoard() out of protected area in TTT.h
	// --SC-- player will be set in UTTT
	// --SC-- set player should auto set to player 1 with new game can we override it so if UTTT then UTTT sets it?
	int player = 1;

	// test basic functions
	print("Create board");
	TTT a = TTT();
	a.printBoard();

	print("Make a move");
	a.PlaceMarker(player, 4);
	a.printBoard();

	print("Clear board");
	a.FillBoard();
	a.printBoard();

	// try all positions
	testPositions(a,player);
	// check all wins
	testWins(a,player);
	// full board and owner
	testFullBoard(a,player);

	// can one player overwrite another?
	print("\nTest if one player can overwrite the other.");
	print("Shoud be three 1's and two 2's.");
	a.FillBoard();
	a.PlaceMarker(player, 4);
	a.PlaceMarker(player, 6);
	a.PlaceMarker(player, 8);

	player= 2;

	a.PlaceMarker(player, 4);
	a.PlaceMarker(player, 6);
	a.PlaceMarker(player, 8);
	a.PlaceMarker(player, 0);
	a.PlaceMarker(player, 7);
	a.printBoard();

	/*

	// --SC-- function is in header but not in cpp. Are we going to implement it?
	int getPlayer();
	
	*/
	return 0;
}


