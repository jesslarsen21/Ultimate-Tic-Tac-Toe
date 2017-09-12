#include "Server.h"
#include "signals.h"
#include <unistd.h>
#include <cstdlib>
#include <iostream>

Server *g_server = 0;

void my_handler(int signum)
{
	std::cerr << "Signal: " << signum << std::endl;
	if(g_server)
		{
			std::cerr << "Closing server." << std::endl;
			g_server->closeServer();
			delete g_server;
			g_server = 0;
		}
	{
		// Force Segmentation fault
		int *s = 0;
		*s = 1;
	}
}

int main(int argc, char **argv)
{
	int port = 3333;
	int backlog = 511;
	int c;

	while((c = getopt(argc,argv,"p:b:")) != -1)
		{
			switch(c)
				{
				case 'p': port     = atoi(optarg); break;
				case 'b': backlog  = atoi(optarg); break;
				default:
					std::cout << "Unknown option: " << c << std::endl;
					std::cout << "-p port" << std::endl;
					std::cout << "-b backlog" << std::endl;
					exit(1);
					break;
				}
		}
	if(!port)
		{
			std::cout << "must specify our port to use with -p !" << std::endl;
			exit(1);
		}
	if(!backlog)
		{
			std::cout << "must specify backlog to use with -b !" << std::endl;
			exit(1);
		}
	
	registerSignalHandlers(my_handler);

	/* That's right, a global variable.
	 * It allows the signal handler to try to close
	 * down the server.
	 */
	g_server = new Server;
	if(g_server->prepareServer(port, backlog, Server::SERVER_TCP) < 0)
		{
			std::cout << "Server preparations failed." << std::endl;
			exit(1);
		}
	g_server->mainLoop();
	g_server->closeServer();
	delete g_server;
	g_server = 0;
	
	return 0;
}
