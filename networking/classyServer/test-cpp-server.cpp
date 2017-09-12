#include "Server.h"
#include <unistd.h>
#include <cstdlib>

int client_stuff()
{
	Server server;
	int i;
	int error_count = 0;
	const int max_clients = 10;
	for(i = 0; i < max_clients; i++)
		{
			ConnectionClient *c = new ConnectionClient;
			if(server.addClient(c) != 0)
				{
					std::cerr << "Error: unable to add a client." << std::endl;
					error_count ++;
				}
			else
				{
					std::cout << "Added client " << i << "." << std::endl;
				}
		}
	if(server.deleteClient(-1) >= 0)
		{
			std::cerr << "Error: deleted -1 client." << std::endl;
			error_count ++;
		}
	if(server.deleteClient(max_clients) >= 0)
		{
			std::cerr << "Error: deleted " << max_clients << " client." << std::endl;
			error_count ++;
		}
	
	for(i = max_clients-1; i >= 0; i--)
		{
			if(server.deleteClient(i) != 0)
				{
					std::cerr << "Error: unable to delete " << i << " client." << std::endl;
					error_count ++;
				}
			else
				{
					std::cout << "Deleted client " << i << "." << std::endl;
				}
		}

	if(server.deleteClient(0) >= 0)
		{
			std::cerr << "Error: deleted " << max_clients << " client." << std::endl;
			error_count ++;
		}

	return error_count;
}

int socket_stuff()
{
	int port = 4321, backlog = 511;
	int error_count = 0;
	int max_tries = 10;
	int i;

	for(i = 0; i < max_tries; i++)
		{
			Server s;
			if(s.prepareServer(port, backlog, Server::SERVER_TCP) < 0)
				{
					std::cerr << "Error unable to prepare server." << std::endl;
					error_count ++;
				}
			if(s.closeServer() < 0)
				{
					std::cerr << "Error unable to close server." << std::endl;
					error_count ++;
				}
		}
	
	return error_count;
}

int main(int argc, char **argv)
{
	int ec;
	ec = client_stuff();
	if(ec == 0)
		{
			std::cout << "No errors for client add/delete." << std::endl;
		}
	else
		{
			std::cerr << "Errors for client add/delete: " << ec << "." << std::endl;
		}

	int es;
	es = socket_stuff();
	
	if(es == 0)
		{
			std::cout << "No errors for socket prepare/close." << std::endl;
		}
	else
		{
			std::cerr << "Errors for socket prepare/close: " << ec << "." << std::endl;
		}
	
	return 0;
}
