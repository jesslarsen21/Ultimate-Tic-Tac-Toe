#include <iostream>
#include <string>
#include <vector>
#include "accounts.h"
#include "user.h"

//#if _DEBUG	// comment out for debugging
#ifndef DEBUG	// comment out for normal run
#define printl_DB(x) std::cerr<<x<<std::endl
#define print_DB(x) std::cerr<<x
#else
#define printl_DB(x) 
#define print_DB(x) 
#endif

#define printl(x) std::cout<<x<<std::endl
#define print(x) std::cout<<x

/** Tests helper functions */
/** Loop through tests to see if any failed, and prints message. */
bool passed(std::vector<bool> &tests,std::vector<std::string> &testnames){
	for(unsigned int i=0;i<tests.size() ;i++){
		if (!tests[i]){
			printl("-- Failed test : "<<testnames[i]<<"\n");
			return false;
		}
	}
	printl("-- Passed all tests!"<<"\n");
	return true;
}
/** Loop through tests to see if any failed. No message.  */
bool passedNoMessage(std::vector<bool> &tests,std::vector<std::string> &testnames){
	for(unsigned int i=0;i<tests.size() ;i++){
		if (!tests[i]){
			return false;
		}
	}
	return true;
}
/** Add testName, and prints testName. */
void addUnitName(std::string testName,std::vector<std::string> &VNames){
	VNames.push_back(testName);
	printl(testName);
}
/** Add testName prints testName if debugging.*/
void addTestName(std::string testName,std::vector<std::string> &VNames){
	VNames.push_back(testName);
	print_DB(testName);
}
/** Add to tests, and if debugging prints pass or fail messsage.*/
void passOrFail(bool condition, std::vector<bool> &tests){
	if(condition){
		printl_DB("-- Passed.");
		tests.push_back(true);
	}
	else{
		printl_DB("-- Failed.");
		tests.push_back(false);
	}
}

/** Test adding users to database. */
void addUsers(std::vector<bool> &tests,std::vector<std::string> &names,DB &access)
{
	addTestName("  Add a user.",names);
	int t = access.addUser("orange","superSecret","my@mail.com");
	int t1 = access.addUser("orange","superSecret","my@mail.com"); // user already exists.
	int t1_1 = access.addUser("orange","Secret","jello@mak.com"); // duplicate user.
	int t1_2 = access.addUser("jello","superduperSecret","my@mail.com"); // duplicate email.
	access.removeUser("orange","superSecret","mymail.com");
	passOrFail(t==7 && t1==4 && t1_1==6 && t1_2==5, tests);

	addTestName("  Password length enforced, between 6 and 128 characters",names);
	int t2 = access.addUser("green","small","mai@mail0.com0"); // 5
	int t3 = access.addUser("green1","larger","mail@mail1.com0"); // 6
	int t4 = access.addUser("green2", "big5649843biggest45654large467654largest564894huge47564hugest4567864hugemungous4567of123all46the789passwords456in12the2443world", "mail@mail1.com"); //127
	int t5 = access.addUser("green3", "big5649843biggest45654large467654largest564894huge47564hugest4567864hugemungous4567of123all456the789passwords456in12the2443world", "mail@mail2.com"); //128
	int t6 = access.addUser("green4", "big5649843biggest45654large467654largest564894huge47564hugest4567864hugemungous4567of123all456the789passwords456in12the24543world", "mail@mail3.com"); //129

	access.removeUser("green1","larger","mail@mail.com"); // 6
access.removeUser("green2", "big5649843biggest45654large467654largest564894huge47564hugest4567864hugemungous4567of123all46the789passwords456in12the2443world", "mail@mail1.com");//127
access.removeUser("green3", "big5649843biggest45654large467654largest564894huge47564hugest4567864hugemungous4567of123all456the789passwords456in12the2443world", "mail@mail2.com");//128
	passOrFail(t2==3 && t3==7 && t4==7 && t5==7 && t6 ==3, tests);

}
/** Test if users exists in database. */
void usersExists(std::vector<bool> &tests,std::vector<std::string> &names,DB &access)
{
	addTestName("  Check if user exists.",names);
	access.addUser("green","joyfull","mail@mail.com");
	bool t = access.userExists("green"); // exists
	bool t1 = access.userExists("hello"); // doesn't exists
	access.removeUser("green","joyfull","mail@mail.com");
	passOrFail(t && !t1, tests);
}
/** Test removing users from database. */
void removeUsers(std::vector<bool> &tests,std::vector<std::string> &names,DB &access)
{
	addTestName("  Remove a user.",names);
	access.addUser("blue","mypass","mymail@somewhere.com");
	bool t = access.removeUser("blue","mypass","mymail@somewhere.com");// exists
	bool t1 = access.removeUser("hello","secret", "myemail@dmail.dixie.edu"); //doesn't exists
	access.removeUser("blue","mypass","mymail@somewhere.com");
	passOrFail( t && !t1, tests);
}
/** Test updating user's password in database. */
void updatePassword(std::vector<bool> &tests,std::vector<std::string> &names,DB &access)
{
	addTestName("  Update password.",names);
	access.addUser("purple","buffulo","buff@me.com");
	bool t = access.updatePassword("purple","buffulo","buffer");  // exists
	bool t1 = access.updatePassword("yellow","someP", "someP");   // doesn't exists
	access.removeUser("purple","buffer","buff@me.com");
	passOrFail(t &&! t1,tests);
}
/** Test logging in users */
void login(std::vector<bool> &tests,std::vector<std::string> &names,DB &access)
{
	addTestName("  Login.",names);
	access.addUser("pink","pillow","my@me.com");
	// T
	bool t = access.login("pink","pillow");  // exists
	bool t1 = access.login("pink","pillow"); // already logged in- returns true
	// F
	bool t2 = access.login("fusha","somePass"); // doesn't exists
	
	access.removeUser("pink","pillow","my@me.com");
	passOrFail(t && t1 && !t2, tests);
}
/** Test users logging out */
void logout(std::vector<bool> &tests,std::vector<std::string> &names,DB &access)
{
	addTestName("  Logout.",names);
	access.addUser("blue","mypass","mymail@somewhere.com");
	access.login("blue","mypass");
	bool t = access.logout("blue");		// exists
	bool t1 = access.logout("green");	// doesn't exists
	access.removeUser("blue","mypass","mymail@somewhere.com");
	passOrFail(t && !t1, tests);
}	
/** Test starting a game. */
void startgame(std::vector<bool> &tests,std::vector<std::string> &names,DB &access)
{
	addTestName("  Start a game.",names);
	int a = access.addUser("blue2","mypass","mymail@somewhere.com");
	print(a);
	bool l = access.login("blue2","mypass");
	print(l);
	int a2 = access.addUser("orange2","superSecret","my@where.com");
	print(a2);
	bool l2 = access.login("orange2","superSecret");
	print (l2);
	// T - both exists and logged in
	int t8 = access.startGame("blue2","orange2"); 
	/*
	// F - one or more not logged in or do not exists
	access.logout("blue2");
	int t1 = access.startGame("blue2","orange2"); 	// one logged out
	int t2 = access.startGame("orange2","green"); 	// one doesnt exist
	access.logout("orange2");	
	int t3 = access.startGame("blue2","orange2");	//both logged out 
	int t4 = access.startGame("green","buffulo"); 	// neither exists
	printl("start game");
*/
	print(t8);
/*	print("end");
	print(t1);
	print(t2);
	print(t3);
	print(t4);
	
	access.removeUser("blue","mypass","mymail@somewhere.com");
	access.removeUser("orange","superSecret","my@where.com");
	passOrFail((t8>0 && t1<0 && t2<0 && t3<0 && t4<0), tests);
*/
}
/** Test ending a game. */
void endgame(std::vector<bool> &tests,std::vector<std::string> &names,DB &access)
{
	addTestName("  End a game.",names);

	access.addUser("blue","mypass","mymail@somewhere.com");
	access.login("blue","mypass");
	access.addUser("orange","superSecret","my@where.com");
	access.login("orange","superSecret");

	// T - Users in game
	int g = access.startGame("blue","orange");
	bool t1 = access.endGame("blue","orange",1,g); // blue wins

	g = access.startGame("blue","orange"); 
	bool t2 = access.endGame("blue","orange",2, g); // orange wins
	
	// T game not finished or no winner it should be recorded.
	g = access.startGame("blue","orange"); 
	bool t3 = access.endGame("blue","orange",0,g); 
	// F - one or more users not in game
	bool t5 = access.endGame("green","orange",0,-325); // one doesn't exists
	access.logout("orange");
	access.logout("blue");
	g = access.startGame("blue","orange"); 
	bool t8 = access.endGame("blue","orange",2,g ); // neither logged in

	access.removeUser("blue","mypass","mymail@somewhere.com");
	access.removeUser("orange","superSecret","my@where.com");

	passOrFail( (t1 && t2 && t3 && !t5 && !t8), tests);
}
/** Test that user's profile is updated in database after games have been played or won. */
void getProfile(std::vector<bool> &tests,std::vector<std::string> &names,DB &access)
{
	addTestName("  Get new user's profile.",names);

	access.addUser("blue","mypass","mymail@somewhere.com");
	access.login("blue","mypass");
	access.addUser("orange","superSecret","my@where.com");
	access.login("orange","superSecret");
	
	User blue0 = access.getProfile("blue");	// exists
	int b0p = blue0.getPlayed();
	int b0w = blue0.getWon();
	int b0t = blue0.getTied();
	passOrFail((b0t==0 && b0p==0 && b0w==0),tests); 
	
	addTestName("  Get user's profile after games.",names);
	int g1 = access.startGame("blue","orange");
	access.endGame("blue","orange",1,g1); 	// blue wins
	int g2 = access.startGame("blue","orange");
	access.endGame("blue","orange",1,g2); 	// blue wins
	int g3 = access.startGame("blue","orange");
	access.endGame("blue","orange",0, g3);	// no winner 

	User blue2 = access.getProfile("blue");	// exists
	int bp = blue2.getPlayed();
	int bw = blue2.getWon();
	int bt = blue2.getTied();

	User orange = access.getProfile("orange");	// exists
	int op = orange.getPlayed();
	int ow = orange.getWon();
	int ot = orange.getTied();

	passOrFail((bp==3 && op==3 && bw==2 && ow==0 && ot==1 && bt ==1), tests);

	access.removeUser("blue","mypass","mymail@somewhere.com");
	access.removeUser("orange","superSecret","my@where.com");
}

// Test passing in name when creating accesses to DB
// change return values for adduser
/** Test account class */
bool accountClass()
{
	std::vector<bool> tests;
	std::vector<std::string> names;

	// Test 1
	addTestName("  Instance of accounts DB.",names);
	DB access = DB();
	passOrFail(true,tests);
	// Test 2
	addUsers(tests,names,access);
	// Test 3
	usersExists(tests,names,access);
	// Test 4
	removeUsers(tests,names,access);
	// Test 5
	updatePassword(tests,names,access);
	// Test 6
	login(tests,names,access);
	// Test 7
	logout(tests,names,access);
	// Test 8
	startgame(tests,names,access);
 	// Test 9
	endgame(tests,names,access);	
	// Test 10
	getProfile(tests,names,access);
	// Test 11
	addTestName("  Close database.",names);
	bool t11 = access.close();
	passOrFail(t11,tests);
	return passed(tests,names);

}


int main()
{
	std::vector<bool> tests;
	std::vector<std::string> UnitNames;
	printl("Unit tests -- ");

	addUnitName("Test Account Class.", UnitNames);
	bool t2 = accountClass();
	tests.push_back(t2);

	bool finished = passed(tests,UnitNames);
	if (!finished)
	{
		return -1;
	}
	return 0;
}
