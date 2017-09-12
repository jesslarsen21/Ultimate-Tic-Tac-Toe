#include "UTTTClient.h"
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <iostream>

int main(int argc, char **argv)
{
	std::string hostname = "localhost";
	int port = 3333;
	int c;
	bool use_ai = false;
	std::string username = "";
	std::string password = "";

	while((c = getopt(argc,argv,"ap:U:P:h:")) != -1)
		{
			switch(c)
				{
				case 'p': port     = atoi(optarg); break;
				case 'h': hostname = optarg; break;
				case 'a': use_ai   = true; break;
				case 'U': username = optarg; break;
				case 'P': password = optarg; break;
				default:
					std::cout << "Unknown option: " << c << std::endl;
					std::cout << "-a DO_AI" << std::endl;
					std::cout << "-p port" << std::endl;
					std::cout << "-U username" << std::endl;
					std::cout << "-P password" << std::endl;
					std::cout << "-h host" << std::endl;
					exit(1);
					break;
				}
		}
	if(!port)
		{
			std::cout << "must specify server port to use with -p !" << std::endl;
			exit(1);
		}
	if(hostname == "")
		{
			std::cout << "must specify server hostname to use with -h !" << std::endl;
			exit(1);
		}

	UTTTClient client(use_ai);
	client.prepareSocket(hostname, port);
	if(username != "" && password != "")
		{
			client.setCredentials(username, password);
		}
	client.mainLoop();
	client.closeSocket();
	return 0;
}
