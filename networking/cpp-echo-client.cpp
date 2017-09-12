#include "Client.h"
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <iostream>

int main(int argc, char **argv)
{
	std::string hostname;
	int port = 0;
	int c;

	while((c = getopt(argc,argv,"p:h:")) != -1)
		{
			switch(c)
				{
				case 'p': port     = atoi(optarg); break;
				case 'h': hostname = optarg; break;
				default:
					std::cout << "Unknown option: " << c << std::endl;
					std::cout << "-p port" << std::endl;
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

	Client client;
	client.prepareSocket(hostname, port);
	client.mainLoop();
	client.closeSocket();
	return 0;
}
