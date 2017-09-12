#include "user.h"

User::User()
{
	mPlayed = 0;
	mWon = 0;
	mTied = 0;
	mUsername = "";

}

User::User(string username, int played, int won, int tied)
{
	mUsername = username;
	mPlayed = played;
	mWon = won;
	mTied = tied;

}
	
int User::getPlayed()
{
	return mPlayed;
}

int User::getWon()
{
	return mWon;
}

int User::getTied()
{
	return mTied;
}
