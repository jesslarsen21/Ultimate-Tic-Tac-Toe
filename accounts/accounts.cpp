#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <iostream>
#include <sstream>
#include "accounts.h"
#include <cstring>

using namespace std;

int d = 0;
int z = 1;

//create tables function
bool createtables(const char * dbname)
{
  //open database
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;

  rc = sqlite3_open(dbname, &db);
  
  if (rc) 
		{
		std::cerr << "Can't open database: \n" << sqlite3_errmsg(db) << std::endl;
		return false;
		}
  else
		{
    std::cout << "Opened database successfully." << std::endl;
		}
  //add tables

  //USERS (accountname, password, email, played, won)
  const char *sql = "CREATE TABLE USERS (username STRING PRIMARY KEY, password STRING, email STRING, played INTEGER, won INTEGER, tied INTEGER);";
  rc = sqlite3_exec(db,sql,0,0,&zErrMsg);
  if (rc != SQLITE_OK){
    sqlite3_free(zErrMsg);
    std::cerr << "Issue adding user table" << std::endl;
		return false;
  }
  else{
    std::cout << "User table added successfully." << std::endl;
  }

  //ONLINE (username, ingame)
  sql = "CREATE TABLE ONLINE (username STRING PRIMARY KEY, ingame INT);";
  rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
  if (rc != SQLITE_OK)
  {
	  sqlite3_free(zErrMsg);
		std::cerr << "Issue adding online table." << endl;
		return false;
  }

  //CURGAMES (gameID, p1, p2)
  sql = "CREATE TABLE CURGAMES (gameID INTEGER PRIMARY KEY, p1 STRING, p2 STRING);" ;
  rc = sqlite3_exec(db,sql,0,0,&zErrMsg);
  if (rc != SQLITE_OK){
    sqlite3_free(zErrMsg);
    std::cerr << "Issue adding curgame table." << std::endl;
		return false;
  }
  else{
    std::cout << "Curgame table added successfully." << std::endl;
  }

  // ALLGAMES (p1,p2,winner)
  sql = "CREATE TABLE ALLGAMES (gameid INTEGER PRIMARY KEY ASC, p1 STRING, p2 STRING, winner STRING);" ;
  rc = sqlite3_exec(db,sql,0,0,&zErrMsg);
  if (rc != SQLITE_OK){
    sqlite3_free(zErrMsg);
    std::cerr << "Issue adding allgame table." << std::endl;
		return false;
  }
  else{
    std::cout << "Allgame table added successfully." << std::endl;
  }
  sqlite3_close(db);
  std::cout << "Database closed." << std::endl;
	return true;
}

DB::DB()
{
	open("accounts.db");
	
	//check to see if tables exist
	string sql = "SELECT * FROM USERS;";
	int rc = query(sql.c_str());
	if (rc == SQLITE_ROW || rc == SQLITE_DONE)
		{
			return;
		}
	
	// if we get here, the needed tables are not in the datbase
	close();

	//
	createtables("accounts.db");
	open("accounts.db");
}

DB::DB(const char * dbname)
{
	open(dbname);
	//check to see if tables exist
	string sql = "SELECT * FROM USERS;";
	int rc = query(sql.c_str());
	if (rc == SQLITE_ROW || rc == SQLITE_DONE)
		{
			return;
		}
	// if we get here, the needed tables are not in the datbase
	close();
	//create tables in database
	createtables(dbname);
	//open database
	open(dbname);
	
}

DB::~DB()
{
	sqlite3_close(database);  
  
}

bool DB::open(const char* filename)
{
	//open database connection
	int rc;
	rc = sqlite3_open(filename, &database);
	
	//check connection
	if (rc) 
		{
			return true;
		}
	else
		{
			return false;
		}
}

bool DB::userExists(const string &username)
{
  //variables for database call
  int ex;
  
  // check if username already exists
  string check = "SELECT * FROM USERS WHERE username = '";
  check += username;
  check += "';";

  ex = query(check.c_str());
  if (ex == SQLITE_ROW)
  {
	  return true;
  }
  return false;
}

bool DB::emailExists(const string &email)
{
  //variables for database call
  int ex;
  
  // check if username already exists
  string check = "SELECT * FROM USERS WHERE email = '";
  check += email;
  check += "';";

  ex = query(check.c_str());
  if (ex == SQLITE_ROW)
  {
	  return true;
  }
  return false;
}

std::string escape(const string &check)
{
	std::string newString = check;
	unsigned int cur;
	unsigned int size = newString.size();
	for (cur = 0; cur<size; cur++)
		{
			if (newString[cur] == 39)
				{
					newString.insert(cur,"'");
					cur ++;
					size++;
				}
		}
	if (d) cout << newString << endl;
	return newString;
}

bool DB::removeUser(const string &username, const string &password, const string &email)
{
	//escape any single ' characters in input
	std::string u = escape(username);
	std::string p = escape(password);
	std::string e = escape(email);

  int ex;
  char *zErrMsg = 0;

  //check the user exists
  bool exists = userExists(username);
  if (!exists)
  {
		if (d) cerr << "user doesn't exist. can't remove." << endl;
	  return false;
  }

  string remove = "DELETE FROM users WHERE username='";
  remove += u;
  remove += "' AND password='";
  remove += p;
  remove += "' AND email='";
  remove += e;
  remove += "';";

	//make sure to remove user from all tables?? 

  ex = sqlite3_exec(database, remove.c_str(), 0, 0, &zErrMsg);
	int count = 0;
	while (ex == SQLITE_BUSY && count < 100 )
		{
			sqlite3_free(zErrMsg);
			ex = sqlite3_exec(database, remove.c_str(), 0, 0, &zErrMsg);
			count ++ ;
		}

  if (ex != SQLITE_OK)
    {
      sqlite3_free(zErrMsg);
			if (d) cerr << "unable to remove user from database." << endl;
      return false;
    }
	if (d) cout << "user: " << u << " removed successfully." << endl;
  return true;
  
}


int DB::addUser(const string &username, const string &password, const string &email)
{
	//escape any single ' characters in input
	std::string u = escape(username);
	std::string p = escape(password);
	std::string e = escape(email);

	int result = 0;
	//variables for database calls
	int ex;
	char *zErrMsg = 0;

	//verify password length
	if (p.size() < 6 || p.size() > 128 )
		{
			if (d) cerr << "password not correct size." << endl;
		}  
	else 
		{
			result += PASS_OK;
		}
	
	// check to see if user already exists
	bool exists = userExists(u);
	if (exists)
	{
		if (d) cerr << "user already exists" << endl;
	}
	else
		{
			result += USER_OK;
		}

	// check if email ok
	exists = emailExists(e);
	if (exists)
	{
		if (d) cerr << "email already in use" << endl;
	}
	else
		{
			unsigned int checkemail = e.find("@");
			if (checkemail != std::string::npos && checkemail < e.size()-5 && checkemail >= 1)
				{
			 		result += EMAIL_OK;
				}
			else 
				{
					if (d) cout << "@ sign not found or at invalid position" << endl;
				}
		}

	if (result == ALL_OK)
		{
			//prepare sql statement
			string sql = "INSERT INTO USERS (username, password, email, played, won, tied) ";
			sql += "VALUES ( '";
			sql += u;
			sql += "', '";
			sql += p;
			sql += "', '";
			sql += e;
			sql += "', 0, 0, 0 );";
	
			//execute sql statement

			ex = sqlite3_exec(database, sql.c_str(), 0, 0, &zErrMsg);
	
			int count = 0;
			while (ex == SQLITE_BUSY && count < 100 )
				{
					sqlite3_free(zErrMsg);
					ex = sqlite3_exec(database, sql.c_str(), 0, 0, &zErrMsg);
					count ++ ;
				}

			if (ex != SQLITE_OK)
				{
					sqlite3_free(zErrMsg);
					if (d) cout << "unable to add user into database" << endl;
					if (d) cout << "sql statement: " << sql << endl;
					return 0;
				}

			if (d) cout << "user: " << u << " added successfully." << endl;
	
			return result;
		}
	
	else 
		{
			return result;
		}


}


bool DB::updatePassword(const string &username, const string &oldpassword, const string &newpassword)
{

	//escape any single ' characters in input
	std::string u = escape(username);
	std::string op = escape(oldpassword);
	std::string np = escape(newpassword);


	//check user exists
	bool exists = userExists(u);
	if (!exists)
	{
		return false;
	}

 
	//variables for database calls
	int ex;
	char *zErrMsg = 0;
	//prep statement
	string sql = "UPDATE users SET password = '";
	sql += np;
	sql += "' WHERE username = '";
	sql += u;
	sql += "' and password = '";
	sql += op;
	sql += "';";

	ex = sqlite3_exec(database, sql.c_str(), 0, 0, &zErrMsg);

	int count = 0;
	while (ex == SQLITE_BUSY && count < 100 )
		{
			sqlite3_free(zErrMsg);
			ex = sqlite3_exec(database, sql.c_str(), 0, 0, &zErrMsg);
			count ++ ;
			
		}

	if (ex == SQLITE_OK)
	{
		return true;
	}
	else
		{
			sqlite3_free(zErrMsg);
			return false;
		}
}

User DB::getProfile(const string &username)
{

	//construct statement here to select played and won from users table
	if (userExists(username))
	{
		string statement = "SELECT played, won, tied FROM users WHERE username = '";
		statement += username;
		statement += "';" ;
		
		// prep statement
		sqlite3_stmt * ppStmt = 0;
		int rc = sqlite3_prepare_v2(database, statement.c_str(), -1, &ppStmt, 0);
		rc = sqlite3_step(ppStmt);
		if (rc != SQLITE_ROW)
		{
			sqlite3_finalize(ppStmt);
			User temp_user = User("",0,0,0);
			return temp_user;
		}
		// extract values
		int played, won, tied;
		played = sqlite3_column_int(ppStmt, 0);
		won = sqlite3_column_int(ppStmt, 1);
		tied = sqlite3_column_int(ppStmt, 2);
		User tmp_user = User(username, played, won, tied);

		// finalize statement
		sqlite3_finalize(ppStmt);
		return tmp_user;
	}
	User temp_user = User("",0,0, 0);
	return temp_user;

}

bool DB::userLogin(const string &username, const string &password)
{
	//escape done in login funcion

	if (d) cout << "userLogin used" << endl;

	string check;
	int ex;

	// check password
	check = "SELECT * from USERS where username = '";
	check += username;
	check += "' and password = '";
	check += password;
	check += "';";
	ex = query(check.c_str());
	
	if (ex != SQLITE_ROW)
	{
		return false;
	}	
	
	// check if already in online table
	check = "SELECT * from ONLINE where username = '";
	check += username;
	check += "';";
	
	ex = query(check.c_str());
	
	string update, update2;
	bool login;

	// if already online, return true
	if (ex == SQLITE_ROW)
	{
		return true;	
	}

	//not online. move into online table with ingame set to false
	else if (ex == SQLITE_DONE)
	{
		update2 = "INSERT INTO ONLINE (username, ingame) ";
		update2 += "VALUES ( '";
		update2 += username;
		update2 += "', 0);";

		
		login = execute(update2.c_str());
		return login;

	}
	
	return false;
	
}

int e_callback(void *data, int argc, char **argv, char **azColName)
{
	DB * d = reinterpret_cast<DB*>(data);
	std::pair< std::string, std::string> cur(argv[0], argv[1]);
	d->usernames.push_back(cur);
	return 0;
}

bool DB::emailLogin(const string &email, const string &password)
{
	//check for escape done in login function

	if (d) cout << "emailLogin used" << endl;
	
	string sql = "SELECT username, email from users where email ='";
	sql += email;
	sql += "';";

	char * zErrMsg = 0;
	int ex = sqlite3_exec(database, sql.c_str(), e_callback, this, &zErrMsg);
	
	if (ex != SQLITE_OK)
		{
			sqlite3_free(zErrMsg);
			return false;
		}
	std::vector< std::pair< std::string, std::string> > :: iterator it;
	for (it = usernames.begin(); it!=usernames.end(); it++) 
		{
			if (d) cout << "username: " << it->first << " email: " << it->second << endl;
			if (it->second == email)
				{
					bool loggedin =  userLogin(it->first, password);
					usernames.erase(it);
					return loggedin;
				}
		}
	return false;
		
}



bool DB::login(const string &username, const string &password)
{
	//escape any single ' characters in input
	std::string u = escape(username);
	std::string p = escape(password);



	if (userExists(u))
		{
			if (userLogin(u, p)) return true;
		}
	if (emailExists(u))
		{
			if (emailLogin(u, p)) return true;
		}

	return false;
}

bool DB::logout(const string &username)
{
	//escape any single ' characters in input
	std::string u = escape(username);
	
	// need to make work if sent email instead of username, just like log in?
	if (!isLoggedIn(u))
	{
		return false;
	}
	string update;
	// if already online, remove from table

	update = "DELETE FROM online where username ='";
	update += u;
	update += "';";
	bool logout = execute(update.c_str());
	return logout;

}

bool DB::isLoggedIn(const string &username)
{
	// check username is online
	string check = "SELECT * FROM online WHERE username = '";
	check += username;
	check += "';";

	int ex = query(check.c_str());

	if (ex != SQLITE_ROW)
	{
		string check2 = "SELECT * FROM online WHERE email = '";
		check += username;
		check += "';";

		ex = query(check.c_str());

		if (ex != SQLITE_ROW)
			{
				if (d) cerr << "No log in row returned." << endl;
				return false;
			}
		
	}
	
	return true;

}

int atoi(std::string num)
{
	std::stringstream number;
	int n;
	number << num;
	number >> n;
	return n;

} 

int g_callback(void *data, int argc, char **argv, char **azColName)
{
	DB * d = reinterpret_cast<DB*>(data);
	std::string game = argv[0];
	std::pair<std::string, std::string > cur(argv[1], argv[2]);
	std::pair< std::string, std::pair<std::string, std::string > >cur2 (game, cur);
	d->curgames.push_back(cur2);
	return 0;
}

int DB::getGameID(const string &p1, const string &p2)
{
	//need to get back out game id to return.

	string sql1 = "SELECT gameID, p1, p2 from curgames where p1 = '";
	sql1 += p1;
	sql1 += "' and p2 = '";
	sql1 += p2;
	sql1 += "';" ;


	char * zErrMsg = 0;
	int ex1 = sqlite3_exec(database, sql1.c_str(), g_callback, this, &zErrMsg);

	//return if could not get game id table.
	if (ex1 != SQLITE_OK)
		{
			if (d || z) cout << "error: " << zErrMsg << endl;
			sqlite3_free(zErrMsg);
			if (d || z) cout << "problem with getting game id" << endl;
			return -1;
		}

	std::vector<std::pair < std::string, std::pair<std::string, std::string> > >::iterator vit;
	for (vit = curgames.begin(); vit != curgames.end(); vit++)
		{
			if (vit->second.first == p1 && vit->second.second == p2)
				{
					if (d) cout << "game id: " <<  vit->first << endl << "game id as int: " << atoi(vit->first) << endl;;
				 
					int game = atoi(vit->first);
					return game;
				}
		}
	return -1;
}

int DB::startGame(const string &p1, const string &p2)
{
	//escape any single ' characters in input
	std::string p1a = p1;
	std::string p2a = p2;

	
	//check players exists and are logged in
	if (!userExists(p1a) || !userExists(p2a))
		{
			if (d) cout << "one or more user doesn't exist" << endl;
			return -1; // one of the two players doesn't exist
		}

	if (!isLoggedIn(p1a) || !isLoggedIn(p2a))
		{
			if (d) cout << "one or more users not logged in" << endl;
			return -1;
		}
	
	// make sure players are not currently in a game with each other 
	int p1game, p2game;
	string check1, check2;
	check1 = "SELECT * FROM curgames WHERE p1 = '";
	check1 += p1a;
	check1 += "' and p2 = '";
	check1 += p2a;
	check1 += "';" ;

	check2 = "SELECT * FROM curgames WHERE p1 = '";
	check2 += p2a;
	check2 += "' and p2 = '";
	check2 += p1a;
	check2 += "';" ;

	p1game = query(check1.c_str());
	p2game = query(check2.c_str());
	if (p1game == SQLITE_ROW || p2game == SQLITE_ROW)
		{
			if (d) cerr << "players already in game. can't support two games with the same players currently" << endl;
			return -1;
		}
	
	//insert players into current games table

	string sql = "INSERT INTO CURGAMES (p1, p2) VALUES ('";
	sql += p1a;
	sql += "', '";
	sql += p2a;
	sql += "');";

	bool ex = execute(sql.c_str());

	//return if could not add to current games table.
	if (!ex)
		{
			if (d || z) cout << "problem inserting into curgames." << endl;
			return -1;
		}

	//update ingame on online table
	sql = "UPDATE online SET ingame = 1 where username = '";
	sql += p1a;
	sql += "' or username = '";
	sql += p2a; 
	sql += "';"; 

	ex = execute(sql.c_str());
	if (!ex)
	{
		if (d) cerr << "Couldn't update online table." << endl;
		return -1;
	}
	return getGameID(p1,p2);
}

std::string itoa(int num)
{
	std::stringstream number;
	std::string n;
	number << num;
	number >> n;
	return n;

}

bool DB::endGame(const string &p1, const string &p2, const int winner, const int game)
{
	//escape any single ' characters in input
	std::string p1a = escape(p1);
	std::string p2a = escape(p2);
	std::string gamenum1 = itoa(game);
	std::string gamenum = escape(gamenum1);

	// check that the game is in current games table. 
	string check = "SELECT * FROM curgames WHERE gameID ='";
	check += gamenum;
	check += "';";

	int validgame;

	validgame = query(check.c_str());

	if (validgame != SQLITE_ROW)
	{
		if (d) cout << "SQL statement: " << check << endl;
		if (d) cout << "can't find game in curgames" << endl;
		return false;
	}

	//remove both possible games from current game table
	string sql3 = "DELETE FROM curgames WHERE gameID = '";
	sql3 += gamenum;
	sql3 += "';";

	bool ex = execute(sql3.c_str());
	
	if (!ex)
	{
		if (d) cerr << "Game not removed from curgame table." << endl;
		return false;
	}
	
	// take out of curgames vector
	std::string gamestring1 = itoa(game);
	std::string gamestring = escape(gamestring1);

	std::vector<std::pair < std::string, std::pair<std::string, std::string> > >::iterator vit;
	for (vit = curgames.begin(); vit != curgames.end(); vit++)
		{
			if (vit->first == gamestring)
				{
					curgames.erase(vit);
					if (d) cout << "game removed from curgames vector" << endl;
					break;
				}
		}

	

	// increment games played for both players
	string sql5;
	sql5 = "UPDATE users SET played = played + 1 WHERE username ='";
	sql5 += p1a;
	sql5 += "' or username = '";
	sql5 += p2a;
	sql5 += "';";

	ex = execute(sql5.c_str());

	if (!ex)
	{
		if (d) cerr << "Couldn't update played count. Figure out why." << endl;
		// do anything if can't update count??
		return false;
	}
	
	bool updated = false;
	// increment games won for winner
	if (winner == 0)
	{
		string sql7;
		sql7 = "UPDATE users SET tied = tied + 1 WHERE username ='";
		sql7 += p1a;
		sql7 += "' or username = '";
		sql7 += p2a;
		sql7 += "';";
		updated = execute(sql7.c_str());
	
	}
	
	else if (winner == 1 || winner == 2)
	{
		string sql6;
		sql6 = "UPDATE users SET won = won + 1 WHERE username = '";
	
		if (winner == 1)
			{
				sql6 += p1a;
			}
		else if (winner == 2)
			{
				sql6 += p2a;
			}
		
		sql6 += "';";

		updated = execute(sql6.c_str());
	}
	
	return updated;
}
  
bool DB::close()
{
	int rc = sqlite3_close(database);
	if (rc == SQLITE_OK)
	{
		return true;
	}
	return false;

}

int DB::query(const char* statement)
{

	sqlite3_stmt * ppStmt = 0;
	

	int rc = sqlite3_prepare_v2(database, statement, -1, &ppStmt, 0); // -1 will read to terminating 0 implied by double quotes in declaration, 0 means nothing left over

	rc = sqlite3_step(ppStmt);


	sqlite3_finalize(ppStmt);

	return rc;

}

bool DB::execute(const char* statement)
{
	char * zErrMsg = 0;
	int ex = sqlite3_exec(database, statement , 0, 0, &zErrMsg);

	int count = 0;
	while (ex == SQLITE_BUSY && count < 100 )
		{
			sqlite3_free(zErrMsg);
			ex = sqlite3_exec(database, statement, 0, 0, &zErrMsg);
			count ++;
		}
	
	if (ex != SQLITE_OK)
	{
		if (d) cout << "execute statement: " << statement << endl;
		sqlite3_free(zErrMsg);
		return false;
	}
	return true;

}

