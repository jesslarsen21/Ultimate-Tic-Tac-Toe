#ifndef _CLIENT_H_
#define _CLIENT_H_
#include "ByteBuffer.h"
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

class Client
{
public:
	Client();
	Client(const std::string &server_name_in, const int server_port_in);
	virtual ~Client();
	virtual int prepareSocket(const std::string &server_name_in, const int server_port_in);
	virtual int getSocketFd() const;
	virtual int closeSocket();
	virtual int mainLoop();
	virtual int handleServer();
	virtual int handleStdin();
	virtual int sendServer(const std::vector<char> &buf);
	virtual int heartbeat();
	virtual int setUseStdin(bool use_stdin);
	
protected:
	int sock;
	struct sockaddr_in server_addr;
	std::string server_name;
	int server_port;
	ByteBuffer m_buffer;
	bool m_use_stdin;
	
private:
};

#endif
