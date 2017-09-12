Game Server
-----------
The game server will have 2 TCP sockets for communication,
one for each player.  The main algorithm is:

	create game instance for two players;
	send initial game state to both players;
	
	until game over
	{
		wait for message from a client;
		process the message, calling appropriate methods on game instance;
		send required response messages to client, if any;
		send all game state changes to clients;
	}
	
	close connection to clients;
	record game results;
	destroy game instance;
	
	exit;
	
