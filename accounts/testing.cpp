#include "accounts.h"
#include "user.h"
#include <string>
#include <iostream>


int main()
{

	DB users = DB();

	cout << users.addUser("carita'lynn", "mypassword", "m2yemail@a.com") << "==7" << endl;
	cout << users.addUser("caritalynn","mypassword","myemail@a.com") << "==7" << endl;
	cout << "Login caritalynn: " << users.login("caritalynn", "mypassword") << std::endl;
	cout << "loging carita'lynn: " << users.login("carita'lynn","mypassword") << std::endl;
	users.addUser("otherperson", "superpass", "my@email.com");
	cout << "login otherperson: " << users.login("my@email.com", "superpass") << std::endl;
	users.addUser("otherperson2", "superpass", "my2@email.com");
	cout << "login otherperson: " << users.login("my2@email.com", "superpass") << std::endl;

	//	cout << "remove user: " << users.removeUser("otherperson", "superpass", "my@email.com") << "==1" << endl;
	
	int game = users.startGame("otherperson","caritalynn");
	cout << "game id: " << game << endl;
	int game2 = users.startGame("otherperson","caritalynn");
	cout << "2nd game same players: " << game2 << endl;
	cout << users.getGameID("otherperson","caritalynn") << endl;
	int game3 = users.startGame("otherperson","otherperson2");
	cout << "3rd game different players: " << game3 << endl;
	//	cout << "end game 1: " << users.endGame("otherperson","caritalynn",1,game)<<endl;;
	//	cout << "end game 2: " << users.endGame("otherperson","caritalynn",1,game2)<<endl;
	//	cout << "end game 3: " << users.endGame("otherperson","otherperson2",1,game3)<<endl;


	users.close();
	return 0;
}
