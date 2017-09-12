#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>
#include <cstring>
#include <netinet/ip.h>
#include <sys/time.h>
#include <ctime>
#include <iostream>
#include <set>
#include "websocket/WebSocketValidator.h"

typedef struct
{
  struct sockaddr_in addr;
  int                sock;
} Client;

typedef struct
{
  struct sockaddr_in addr;
  int                sock;
  
  Client            *clients;
  unsigned int       nclient;
  std::set<int> 	webSockClients;
} Server;

#define DEBUG 1
#ifdef DEBUG
#define DEBUG_FILE_DESCRIPTORS(msg, s) { debug_message(msg); debug_file_descriptors(s); }
#define DEBUG_DISPLAY_CLIENT(msg, c) { debug_message(msg); debug_display_client(c); }
#else
#define DEBUG_FILE_DESCRIPTORS(msg, s)
#define DEBUG_DISPLAY_CLIENT(msg, c)
#endif

void debug_message(const char *msg)
{
  fprintf(stderr, "%s: ", msg);
}

void debug_display_client(Client *c)
{
  fprintf(stderr, "sock: %d\n" , c->sock);
}

void debug_file_descriptors(Server *s)
{
  size_t i;
  fprintf(stderr, "FDS: %d ", s->sock);
  for(i = 0; i < s->nclient; i++)
    {
      fprintf(stderr, "%d ", s->clients[i].sock);
    }
  fprintf(stderr, "\n");
}

int init_server(Server *s)
{
  s->sock    = 0;
  s->clients = 0;
  s->nclient = 0;
  return 0;
}

int close_server(Server *s)
{
  close(s->sock);
  return 0;
}

int add_client(Server *s, Client *c)
{
  DEBUG_FILE_DESCRIPTORS("first-of-add_client", s);
  DEBUG_DISPLAY_CLIENT("first-of-add-client", c);
  Client *cs = (Client *) malloc((s->nclient + 1) * sizeof(Client));
  memcpy(cs, s->clients, sizeof(Client) * s->nclient);
  memcpy(&(cs[s->nclient]), c, sizeof(Client));
  if(s->clients)
    {
      free(s->clients);
    }
  s->clients = cs;
  s->nclient ++;
  fprintf(stderr, "nclient: %d\n", s->nclient);
  DEBUG_FILE_DESCRIPTORS("end-of-add_client", s);
  return 0;
}

int remove_client(Server *s, int which_client)
{
  DEBUG_FILE_DESCRIPTORS("first-of-remove_client", s);
  DEBUG_DISPLAY_CLIENT("first-of-remove-client", &(s->clients[which_client]));
  
  int count = s->nclient - which_client - 1;

  close(s->clients[which_client].sock);
  
  s->webSockClients.erase(which_client);
  
  if(count > 0)
    {
      memmove(&(s->clients[which_client]), &(s->clients[which_client+1]), sizeof(Client) * count);
    }
  
  s->nclient --;
  fprintf(stderr, "nclient: %d\n", s->nclient);
  DEBUG_FILE_DESCRIPTORS("end-of-remove_client", s);
  return 0;
}

int prepare_server(Server *s, int port, int backlog)
{
  s->addr.sin_family      = AF_INET;
  s->addr.sin_port        = htons((unsigned short)port);
  s->addr.sin_addr.s_addr = INADDR_ANY;
  
  if((s->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
      perror("Can't create socket!");
      return 0;
    }
  
  if(bind(s->sock, (struct sockaddr *)&(s->addr), sizeof(s->addr)) != 0)
    {
      perror("Can't bind socket!");
      return 0;
    }
  
  if(listen(s->sock, backlog) != 0)
    {
      perror("Can't listen socket!");
      return 0;
    }
  
  return 1;
}

int server_loop(Server *s)
{
  fd_set rfds;
  int nfds;
  size_t i;
  int found;
  int n;
  char buf[1024];
  socklen_t len;	  
  Client client;
  Client *c = &client;
  struct timeval time_out;

  unsigned int count = 0;
  unsigned int bytes_read = 0;
  struct timeval last_time;
  struct timezone tz;
  
  Net::WebSocket::WebSocketValidator validator ("/");
    
  gettimeofday(&last_time, &tz);
  
  nfds = getdtablesize();
  while(1)
    {
      DEBUG_FILE_DESCRIPTORS("top-of-while", s);
      FD_ZERO(&rfds);
      FD_SET(s->sock, &rfds);
      for(i = 0; i < s->nclient; i++)
		{
		  FD_SET(s->clients[i].sock, &rfds);
		}
      time_out.tv_sec  = 100;
      time_out.tv_usec = 0;
      
      if((n = select(nfds, &rfds, 0, 0, &time_out)) < 0)
		{
		  DEBUG_FILE_DESCRIPTORS("select-failed", s);
		  perror("Bad select on socket!");
		  return 0;
		}

      if(n == 0)
		{ /* time out */
		}
      else if(FD_ISSET(s->sock, &rfds))
		{ /* accept a new connection */
		  len = sizeof(c->addr);
		  
		  if((c->sock = accept(s->sock, (struct sockaddr *)&(c->addr), &len)) < 0)
			{
			  perror("accept failed");
			  return 0;
			}
		  std::cout << "new client connected" << std::endl;
		  add_client(s, c);
		}
      else
		{
		  found = 0;
		  for(i = 0; i < s->nclient; i++)
			{
			  if(FD_ISSET(s->clients[i].sock, &rfds))
				{
				  found = 1;
				  n = read(s->clients[i].sock, buf, sizeof(buf));
				  if(n > 0)
					{
					  const std::string msg (buf);
					  
					  if (s->webSockClients.find(s->clients[i].sock) != s->webSockClients.end()) {
					  	std::cout << "Got WS frame!" << std::endl;
					  	
					  	const std::string reply = "Hello!";
					  	const std::string frame = validator.rules.MakeFinalTextFrame(reply);
					  	write(s->clients[i].sock, frame.c_str(), frame.length());
					  
				  	    //std::cout << "Got msg >>> " << msg << std::endl << std::endl;
				  	    
				  	    
				  	    //write(s->clients[i].sock, msg.c_str(), msg.size());
					  }
					  else {
				
						  std::string accept;
						  const std::string err = validator.Validate(msg, accept);
						  if (err == "") {
						  	std::cout << "Got valid handshake!" << std::endl;
						  	std::cout << "Response: " << accept << std::endl;
						  	
						  	write(s->clients[i].sock, accept.c_str(), accept.size());
						  	
						  	//todo : Add socket upgrade.
						  } else {
						  	std::cout << "Invalid handshake: " << err << std::endl;
						  
						  	const std::string httpResp = "HTTP/1.1 404\r\n\r\n";
						  	write(s->clients[i].sock, httpResp.c_str(), httpResp.size());
						  	std::cout << "Response: \t" << httpResp << std::endl;
						  }
						  
						  s->webSockClients.insert(s->clients[i].sock);
						  
						  bytes_read += n;
						  //write(fileno(stdout), buf, n);
						  //write(s->clients[i].sock, "ES: ", 4);
						  //write(s->clients[i].sock, buf, n);
					  }
					}
				  else if(n == 0)
					{
						std::cout << "Removed client: " << s->clients[i].sock << std::endl;
					  remove_client(s, i);
					}
				  else
					{
					  fprintf(stderr, "FD: %d\n", s->clients[i].sock);
					  perror("bad socket read");
					  remove_client(s, i);
					}
				}
			}
		  if(!found)
			{
			  perror("Select on unknown file descriptor!");
			  return 0;
			}
		}
	  memset(buf, 0, 1024);
      count ++;
    }
  return 1;
}



int main(int argc, char **argv)
{
  int port = 0;
  int c;

  while((c = getopt(argc,argv,"p:")) != -1)
    {
      switch(c)
	{
	case 'p': port = atoi(optarg); break;
	default:
	  printf("Unknown option: %c\n", c);
	  exit(1);
	  break;
	}
    }
  if(!port)
    {
      printf("must specify our port to use with -p !\n");
      exit(1);
    }
  
  Server server;
  init_server(&server);
  
  if(!prepare_server(&server, port, 511))
    {
      exit(1);
    }

  server_loop(&server);

  close_server(&server);
  
  return 0;
}
