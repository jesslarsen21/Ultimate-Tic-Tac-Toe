#include "UTTTServer.h"
#include "signals.h"
#include "logging.h"

static UTTTServer *g_server = 0;

static void my_handler(int signum)
{
	DBGP_L(LOG_STREAM(std::cerr) << "Signal: " << signum << std::endl);
	if(g_server)
		{
			DBGP_L(LOG_STREAM(std::cerr) << "Closing server." << std::endl);
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

	while((c = getopt(argc,argv,"p:b:h?")) != -1)
		{
			switch(c)
				{
				case 'p': port     = atoi(optarg); break;
				case 'b': backlog  = atoi(optarg); break;
				default:
					std::cout << "Unknown option: " << (char)c << std::endl;
				case 'h':
				case '?':
					std::cout << "Opens one port for normal TCP." << std::endl;
					std::cout << "Opens next port for WebSocket TCP." << std::endl;
					std::cout << "-p port" << std::endl;
					std::cout << "-b backlog" << std::endl;
					exit(1);
					break;
				}
		}
	if(port <= 0)
		{
			std::cout << "Must specify our port to use with -p !" << std::endl;
			exit(1);
		}
	if(backlog <= 0)
		{
			std::cout << "Must specify backlog to use with -b !" << std::endl;
			exit(1);
		}

	/* be sure to close down server if signals occur */
	registerSignalHandlers(my_handler);

	/* That's right, a global variable.
	 * It allows the signal handler to try to close
	 * down the server.
	 */
	g_server = new UTTTServer;
	if(g_server->prepareServer(port, backlog, Server::SERVER_TCP) < 0)
		{
			std::cout << "Server preparations failed." << std::endl;
			exit(1);
		}
	if(g_server->prepareServer(port+1, backlog, Server::SERVER_WEB) < 0)
		{
			std::cout << "WebSocket Server preparations failed." << std::endl;
			exit(1);
		}
	DBGP_L(LOG_STREAM(std::cout) << "Entering mainLoop()" << std::endl);
	g_server->mainLoop();
	DBGP_L(LOG_STREAM(std::cout) << "Exited mainLoop()" << std::endl);
	g_server->closeServer();
	delete g_server;
	g_server = 0;
	
	DBGP_L(LOG_STREAM(std::cout) << "main() returning." << std::endl);
	return 0;
}
