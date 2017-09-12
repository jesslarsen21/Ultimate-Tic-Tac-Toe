
#include <cstdlib>
#include <sqlite3.h>
#include <iostream>
#include <utility>
#include <vector>
#include "user.h"
#include <string>

using namespace std;

enum RESULTS {USER_OK = 0x1, EMAIL_OK = 0x2, PASS_OK = 0x4, ALL_OK = 0x7};

class DB
{

public:
	DB(); //will use default accounts.db
	DB(const char * dbname);
	virtual ~DB();

	// addUser: will return a combination of the RESULTS enum above.
	int addUser(const string &username, const string &password, const string &email); 

	bool userExists(const string &username);
	bool emailExists(const string &email);

	bool userLogin(const string &username, const string &password);
	bool emailLogin(const string &email, const string &password);

	bool removeUser(const string &username, const string &password, const string &email);
	bool updatePassword(const string &username, const string &oldpassword, const string &newpassword);

	// log in will return TRUE for username with the correct password, regardless of currently logged in state
	bool login(const string &username, const string &password); 

	bool logout(const string &username);

	// getProfile: will return a user object with a count of the number of games won and games played, accessible through getPlayed(), getTied(), and getWon() fuctions.
	User getProfile(const string &username);

	// startGame:  both players exist and be logged in to be able to start a game. same two players may  not be in more than one game at a time. int returned will be the game id (will be greater than 0). any error will return -1.
	int startGame(const string &p1, const string &p2);
	int getGameID(const string &p1, const string &p2);
	//  endGame: if winner = 0, game tied. tie count updated for both players. 1 = player 1 won. 2 = player 2 won.
	bool endGame(const string &p1, const string &p2, const int winner, const int gameID); 

	bool isLoggedIn(const string &username);
	bool close();


  
	//private: commented while testing naming database calls.
	bool open(const char* filename);
	int query(const char* statement);
	bool execute(const char* statement);
	sqlite3 *database;
	std::vector< std::pair< std::string, std::string > > usernames; // storage for callback function
	std::vector <std::pair< std::string, std::pair<std::string, std::string > > > curgames;

// make more useful return codes
// make function to return list of online users
};
