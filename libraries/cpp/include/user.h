#ifndef _USER_H
#define _USER_H

using namespace std;

#include <string>

class User
{
public:
	User();	
	User(string username,int played, int won, int tied);
	int getPlayed();
	int getWon();
	int getTied();



private:	
	string mUsername;
	int mPlayed;
	int mWon;
	int mTied;

};






#endif
