#ifndef _SERVER_H_
#define _SERVER_H_

#include "ByteBuffer.h"
#include "WebSocketValidator.h"

#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <string>

/* required for:
 * struct sockaddr_in,
 */
#include <netinet/ip.h>
/* required for:
 * bind(), socket(), listen(), accept(),
 */
#include <sys/types.h>
#include <sys/socket.h>
/* required for:
 * select(), FD_*(), 
 */
#include <sys/select.h>
/* required for:
 * read(), close()
 */
#include <unistd.h>
/*
 */
#include <sys/time.h>
/* required for:
 * gettimeofday()
 */

class ConnectionClient {
public:
	ConnectionClient();
	ConnectionClient(int state_in);
	~ConnectionClient();
	int closeSocket();
  int forgetSocket();
	int setTCP();
	bool isTCP() const;
	int setWeb();
	bool isWeb() const;
	int setWebWait();
	bool isWebWait() const;
	int setWebFailed();
	bool isWebFailed() const;
  
	int setState(int bits);
	int clearState(int bits);
	bool isStateSet(int bits) const;
	bool setData(const std::string &key, const std::string &value);
	std::string getData(const std::string &key) const;

	/* reads as much message as possible from socket
	 * returns number of bytes read, < 0 on error */
	ssize_t read();
	ssize_t write(const std::vector<char> &msg);

	std::string getAddress() const;
	double timeSinceLast() const;

	
	ByteBuffer m_buffer;
	struct sockaddr_in addr;
	int sock;
	int state;
	enum {
		STATE_TCP        = 0x0001,
		STATE_WEB        = 0x0002,
		STATE_WEB_WAIT   = 0x0004,
		STATE_WEB_FAILED = 0x0008,
    STATE_SOCK_TYPE  = 0x000F,
    STATE_NON_SOCK   = 0x7FF0
	};
	std::map<std::string, std::string> data;
	struct timeval m_last_time;
};

std::ostream &operator<<(std::ostream &os, const ConnectionClient &v);


class Server {
public:
	//Methods: return 0 for success and -1 for failer
	Server();
	virtual ~Server();
	virtual int initServer();
	virtual int closeServer();
	virtual int addClient(ConnectionClient *client);
	virtual int deleteClient(int client);
	virtual int countReadyClients() const;
	virtual int acceptClient(int which_server);
	virtual int prepareServer(int port, int backlog, int which_server);
	virtual int handleClient(ConnectionClient *client, int i);
	virtual int heartbeat(bool timedout);
	virtual int mainLoop();
	virtual int setTimeOut(const struct timeval &timeout);

	enum {
		SERVER_TCP,
		SERVER_WEB
	};

protected:
	virtual int buildResponse(ConnectionClient *client, int i, const std::vector<char> &msg,
														std::vector<char> &response);
	//Variables
	struct sockaddr_in tcp_addr;
	struct sockaddr_in web_addr;
	int tcp_sock;
	int web_sock;
	std::vector<ConnectionClient*> clients;
  Net::WebSocket::WebSocketValidator ws_validator;
	struct timeval m_timeout;
	
};

#endif 
