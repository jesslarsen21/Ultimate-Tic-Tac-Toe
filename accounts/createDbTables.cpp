#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <sqlite3.h>

using namespace std;

int main(int argc, char* argv[])
{
  //open database
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;

  rc = sqlite3_open("accounts.db", &db);
  
  if (rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
  }
  else{
    std::cout << "Opened database successfully." << std::endl;
  }

  //add tables
  //USERS (accountname, password, email, played, won)
  const char *sql = "CREATE TABLE USERS (username STRING PRIMARY KEY, password STRING, email STRING, played INTEGER, won INTEGER, tied INTEGER);";

  rc = sqlite3_exec(db,sql,0,0,&zErrMsg);
  if (rc != SQLITE_OK){
    sqlite3_free(zErrMsg);
    std::cout << "Issue adding user table" << std::endl;
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

  }

  //CURGAMES (curgameid, p1, p2)

  sql = "CREATE TABLE CURGAMES (p1 STRING, p2 STRING);" ; // needs unique identifier?
  rc = sqlite3_exec(db,sql,0,0,&zErrMsg);
  if (rc != SQLITE_OK){
    sqlite3_free(zErrMsg);
    std::cout << "Issue adding curgame table." << std::endl;
  }
  else{
    std::cout << "Curgame table added successfully." << std::endl;
  }

  // ALLGAMES (p1,p2,winner)
  sql = "CREATE TABLE ALLGAMES (gameid INTEGER PRIMARY KEY ASC, p1 STRING, p2 STRING, winner STRING);" ;

  rc = sqlite3_exec(db,sql,0,0,&zErrMsg);
  if (rc != SQLITE_OK){
    sqlite3_free(zErrMsg);
    std::cout << "Issue adding allgame table." << std::endl;
  }
  else{
    std::cout << "Allgame table added successfully." << std::endl;
  }


  





  sqlite3_close(db);
  std::cout << "Database closed." << std::endl;

}

