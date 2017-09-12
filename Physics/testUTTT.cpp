#include <iostream>
#include "UTTT.h"
#include <string>
#include <vector>


//#ifdef DEBUG	// comment out for debugging
#ifndef DEBUG // comment out for normal run
#define printl_DB(x) std::cerr<<x<<std::endl
#define print_DB(x) std::cerr<<x
#else
#define printl_DB(x)
#define print_DB(x)

#endif

#define printl(x) std::cout<<x<<std::endl
#define print(x) std::cout<<x

/** Helper functions**/
/** Prints message. Loop through tests to see if any failed. */
bool passed(std::vector<bool> &tests,std::vector<std::string> &testnames){
	for(unsigned int i=0;i<tests.size() ;i++){
		if (!tests[i]){
			printl("-- Failed test. "<<testnames[i]<<"\n");
			return false;
		}
	}
	printl("-- Passed all tests!"<<"\n");
	return true;
}

/** No message. Loop through tests to see if any failed.*/
bool passedNoMessage(std::vector<bool> &tests,std::vector<std::string> &testnames){
	for(unsigned int i=0;i<tests.size() ;i++){
		if (!tests[i]){
			return false;
		}
	}
	return true;
}

/** Add name to VNames prints testName*/
void addUnitName(std::string testName,std::vector<std::string> &VNames){
	VNames.push_back(testName);
	printl(testName);
}

/** Add name to VNames prints testName if debugging*/
void addTestName(std::string testName,std::vector<std::string> &VNames){
	VNames.push_back(testName);
	print_DB(testName);
}

/** Add to tests, and if debugging prints pass or fail messsage */
void passOrFail(bool condition,std::vector<bool> &tests, UTTT game){
	if(condition){
		printl_DB("-- Passed.");
		tests.push_back(true);
	}
	else{
		printl_DB("-- Failed.");
		game.printGame();
		tests.push_back(false);
	}
}

/** UTTT unit test functions. */
/** 1. Switching players turn. */
bool changePlayers(){
	std::vector<bool> tests;
	std::vector<std::string> names;

	addTestName("Game starts with player 1.  ",names);
	UTTT game = UTTT();
	passOrFail(game.getPlayer()==1,tests,game);

	addTestName("End of turn, turn changes to player 2.  ",names);
	game.takeTurn(1,0,4);
	passOrFail(game.getPlayer()==2,tests,game);
	
	addTestName("End of turn, turn changes to player 1.  ",names);

	game.takeTurn(2,4,0);
	passOrFail(game.getPlayer()==1,tests,game);

	return passed(tests,names);
}

/** 2. Playable board switches based on previous move. */
bool changeBoard(){
	std::vector<bool> tests;
	std::vector<bool> tests1;
	std::vector<std::string> names;


	addTestName( "Game starts with playing on any board.  ",names);

	int i;
	UTTT game = UTTT();
	for(i = 0 ; i<9 ; i++){
		UTTT game = UTTT();
		tests1.push_back(game.takeTurn(1,i,4));
	}
	bool playanywhere =  passedNoMessage(tests1,names);
	passOrFail(playanywhere,tests1,game);

	addTestName("Next board to be played on set.",names);
	//UTTT game1 = UTTT();

	game.takeTurn(1,4,5);
	passOrFail(game.getNextBoard()==5,tests,game);

	addTestName("Play on next board.",names);
	bool t3 = game.takeTurn(2,5,0);
	passOrFail(t3,tests,game);

	return passed(tests,names);
}

/** 3. Out of range posistions and mini boards are gracefully ignored when making a move.*/
bool outOfRangeMoves(){
	std::vector<bool> tests;
	std::vector<std::string> names;
	addTestName("Player 2 tries to play on top of player 1.   ",names);
	UTTT game0 = UTTT();
	bool m0 = game0.takeTurn(1,4,4);
	bool m1 = game0.takeTurn(2,4,4);
	passOrFail(m0 && !m1,tests,game0);
	
	addTestName("Play on out of range mini board.  ",names);

	UTTT game1 = UTTT();
	bool m2 = game1.takeTurn(1,9,0);
	bool m3 = game1.takeTurn(2,-1,0);
	passOrFail(!m2 && !m3, tests,game1);

	addTestName("Play on out of range positions.  ",names);

	UTTT game2 = UTTT();
	bool m4 = game2.takeTurn(1,0,-1);
	bool m5 = game2.takeTurn(2,8,9);
	passOrFail(!m4 && !m5 , tests,game2);

	return passed(tests,names);
}

/** 4. Owner is set when mini board is won.*/
bool miniWin(){
	std::vector<bool> tests;
	std::vector<std::string> names;
	UTTT game = UTTT();


	addTestName("Player 2 wins mini board. ",names);
	game.takeTurn(1,0,0);
	game.takeTurn(2,0,8);
	game.takeTurn(1,8,0);
	game.takeTurn(2,0,4);
	game.takeTurn(1,4,0);
	game.takeTurn(2,0,6);
	game.takeTurn(1,6,0);
	game.takeTurn(2,0,7);
	passOrFail(game.getOwner(0)==2 , tests,game);

	addTestName("2. No winner of mini board. ", names);
	passOrFail(game.getOwner(1) == 0 && game.getOwner(8)==0, tests,game);

	return passed(tests,names);
}

/** 5. Player can go anywhere when where they are supposed to play is full.*/
bool fullMiniBoard(){
	std::vector<bool> tests;
	std::vector<std::string> names;
	UTTT game = UTTT();
	
	game.takeTurn(1,4,4);
	game.takeTurn(2,4,0);

	game.takeTurn(1,0,4);
	game.takeTurn(2,4,1);

	game.takeTurn(1,1,4);
	game.takeTurn(2,4,2);

	game.takeTurn(1,2,4);
	game.takeTurn(2,4,3);

	game.takeTurn(1,3,4);
	game.takeTurn(2,4,5);

	game.takeTurn(1,5,4);
	game.takeTurn(2,4,6);

	game.takeTurn(1,6,4);
	game.takeTurn(2,4,8);

	game.takeTurn(1,8,4);
	game.takeTurn(2,4,7);
	
	addTestName("Last move on full board should not set nextboard to go anywhere. ", names);
	int nb = game.getNextBoard();
	//printl(i);
	bool t = game.takeTurn(1,0,0);
	passOrFail(nb==7 && !t, tests,game);
	
	addTestName("Place anywhere when a mini board is full.  ",names);
	bool t1 = game.takeTurn(1,7,4);
	int nb2 = game.getNextBoard();
	bool t3 = game.takeTurn(2,5,6);
	
	passOrFail(nb2==-1 && t1 && t3, tests,game);
	return passed(tests,names);
}


/** 6. Check that winner is set when somone wins.*/
bool gameOver(){
	std::vector<bool> tests;
	std::vector<std::string> names;
	UTTT game = UTTT();
	game.takeTurn(1,4,0);
	game.takeTurn(2,0,2);
	game.takeTurn(1,2,0);
	game.takeTurn(2,0,5);
	game.takeTurn(1,5,0);
	game.takeTurn(2,0,8);
	addTestName("Game not over. ", names);

	passOrFail(game.getWinner()==0, tests,game);

	game.takeTurn(1,8,3);
	game.takeTurn(2,3,1);
	game.takeTurn(1,1,3);
	game.takeTurn(2,3,4);
	game.takeTurn(1,4,3);
	game.takeTurn(2,3,7);

	game.takeTurn(1,7,6);
	game.takeTurn(2,6,3);
	game.takeTurn(1,3,6);
	game.takeTurn(2,6,0);
	game.takeTurn(1,0,6);
	game.takeTurn(2,6,6);

	addTestName("Player 2 owning 3 mini boards in a row. ", names);
	//Player 2 should own mini boards 0,3,and 6. "
	passOrFail(game.getOwner(0) && game.getOwner(3) && game.getOwner(6), tests,game);

	addTestName("Game over, Player 2 wins.  ", names);
	// Player 2 should be the recorded winner. 

	int g = game.getWinner();
	passOrFail(g==2, tests,game);

	return passed(tests,names);
}

/** 7. Make sure out of turn plays are ignored.*/
bool outOfTurn(){
	std::vector<bool> tests;
	std::vector<std::string> names;
	UTTT game = UTTT();

	addTestName("Player 2 plays out of turn.  ", names);


	bool m0 = game.takeTurn(2,0,0);
	bool m1 = game.takeTurn(1,0,8);
	passOrFail(! m0 && m1, tests,game);

	return passed(tests,names);
}

/** 8. Updates are being set. */
bool updates(){

	std::vector<bool> tests;
	std::vector<std::string> names;
	UTTT game = UTTT();
	
	game.resetChangeArrays();
	addTestName("Reset update arrays. ",names);
	bool t0 = game.hasPlayerChanged();
	bool t0_1 = game.hasPositionChanged(2,0);
	int  t0_2 = game.getPosition(0, 0);
	bool t0_3 = game.hasNextBoardChanged();
	bool t0_4 = game.hasOwnerChanged(0);
	bool t0_5 = game.hasWinnerChanged();
	passOrFail(!t0 && !t0_1 && t0_2 == 0 && !t0_3 && !t0_4 && !t0_5 ,tests,game);

	addTestName("Has player changed. ",names);
	game.takeTurn(1,4,0);
	bool t1 = game.hasPlayerChanged();
	passOrFail(t1,tests,game);

	game.resetChangeArrays();
	addTestName("Has position changed ", names);
	bool t3_1 = game.hasPositionChanged(4,0);
	game.takeTurn(2,0,2);
	bool t3 = game.hasPositionChanged(0,2);
	int  t4 = game.getPosition(0,2);
	int  t5 = game.getPosition(0, 0);
	passOrFail((!t3_1 && t3 && t4==2 && t5==0),tests,game);
	
	game.resetChangeArrays();
	addTestName("Has next board changed. ",names);
	game.takeTurn(1,2,0);
	bool t7 = game.hasNextBoardChanged();
	passOrFail(t7,tests,game);

	game.resetChangeArrays();
	addTestName("Has owner changed. ",names);
	game.takeTurn(2,0,5);
	bool t8 = game.hasOwnerChanged(0);
	game.takeTurn(1,5,0);
	game.takeTurn(2,0,8);
	bool t9 = game.hasOwnerChanged(0);
	passOrFail((!t8 && t9),tests,game);

	game.resetChangeArrays();
	addTestName("Has winner changed. ", names);
	game.takeTurn(1,8,3);
	game.takeTurn(2,3,1);
	game.takeTurn(1,1,3);
	game.takeTurn(2,3,4);
	game.takeTurn(1,4,3);
	game.takeTurn(2,3,7);
	bool t10 = game.hasWinnerChanged();
	game.takeTurn(1,7,6);
	game.takeTurn(2,6,3);
	game.takeTurn(1,3,6);
	game.takeTurn(2,6,0);
	game.takeTurn(1,0,6);
	game.takeTurn(2,6,6);
	bool t11 = game.hasWinnerChanged();
	passOrFail((!t10 && t11),tests,game);

	return passed(tests,names);
}

int main(){
	std::vector<bool> tests;
	std::vector<std::string> UnitNames;

	addUnitName("Unit 1: Test changing player. ", UnitNames);
	bool t1 = changePlayers();
	tests.push_back(t1);

	addUnitName("Unit 2: Test changing board. ", UnitNames);
	bool t2 =  changeBoard();
	tests.push_back(t2);

	addUnitName("Unit 3:	Test out of range moves. ", UnitNames);
	bool t3 =  outOfRangeMoves();
	tests.push_back(t3);

	addUnitName("Unit 4: Test owner of mini board. ", UnitNames);
	bool t4 = miniWin();
	tests.push_back(t4);

	addUnitName("Unit 5: Test full mini board. ", UnitNames);
	bool t5 = fullMiniBoard();
	tests.push_back(t5);

	addUnitName("Unit 6: Test game over. ", UnitNames);
	bool t6 = gameOver();
	tests.push_back(t6);

	addUnitName("Unit 7: Test out of turn play. ", UnitNames);
	bool t7 = outOfTurn();
	tests.push_back(t7);

	addUnitName("Unit 8: Updates. ", UnitNames);
	bool t8 = updates();
	tests.push_back(t8);	
	
	printl("Unit tests -- ");

	bool finished = passed(tests,UnitNames);
	if (!finished)
	{
		return -1;
	}
	return 0;
}
