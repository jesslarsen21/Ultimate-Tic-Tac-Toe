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

typedef struct
{
  struct sockaddr_in addr;
  int                sock;
} Client;

//Client, rename to Clientconnection and can add it to the class server

typedef struct
{
  struct sockaddr_in addr;
  int                sock;
  
  Client            *clients;
  unsigned int       nclient;
} Server;

int init_server(Server *s)
{
  s->sock    = 0; //-1
  s->clients = 0;
  s->nclient = 0;
  return 0;
}

//close socket if open and set to -1 when closed
int close_server(Server *s)
{
  close(s->sock);
  return 0;
}

//server method: no need pass it in, pass in container (vector) in, client object by refrence
int add_client(Server *s, Client *c)
{
  Client *cs = (Client *) malloc((s->nclient + 1) * sizeof(Client));
  memcpy(cs, s->clients, sizeof(Client) * s->nclient);
  memcpy(&(cs[s->nclient]), c, sizeof(Client));
  if(s->clients)
    {
      free(s->clients);
    }
  s->clients = cs;
  s->nclient ++;
  return 0;
}

int remove_client(Server *s, int which_client)
{
  int count = s->nclient - which_client - 1;

  close(s->clients[which_client].sock);
  
  if(count > 0)
    {
      memcpy(&(s->clients[which_client]), &(s->clients[which_client+1]), sizeof(Client) * count);
    }
  
  s->nclient --;
  
  return 0;
}

//consstruct server that isn't listening yet, 
//address error system... deal with later
//return -1 one for failer and 0 for success
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

//rename: mainLoop
int server_loop(Server *s)
{
  fd_set rfds;
  int nfds;
  int i;
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
  struct timeval curr_time;
  struct timezone tz;
    
  gettimeofday(&last_time, &tz);
  
  nfds = getdtablesize();
  while(1)
    {
      FD_ZERO(&rfds);
      FD_SET(s->sock, &rfds);
      for(i = 0; i < s->nclient; i++)
		{
		  FD_SET(s->clients[i].sock, &rfds);
		}
	  //Set top on to 100 seconds
      time_out.tv_sec  = 1; //100
      time_out.tv_usec = 0;
      
      if((n = select(nfds, &rfds, 0, 0, &time_out)) < 0)
		{
		  perror("Bad select on socket!");
		  return 0;
		}

      if(n == 0)
		{ /* time out */
		}
      else if(FD_ISSET(s->sock, &rfds))
		{ /* accept a new connection */
			//roll this into a method
		  len = sizeof(c->addr);
		  
		  if((c->sock = accept(s->sock, (struct sockaddr *)&(c->addr), &len)) < 0)
			{
			  perror("accept failed");
			  return 0;
			}
		  std::cout << "new client connected" << std::endl;
		  add_client(s, c);
		  //roll this into a method
		}
      else
		{
		  found = 0;
		  for(i = 0; i < s->nclient; i++)
			{
			  if(FD_ISSET(s->clients[i].sock, &rfds))
				{
					//Method that handles client, tell which client reads sends echo
					//Will make a virtual method that gets over ridden.... later
				  found = 1;
				  n = read(s->clients[i].sock, buf, sizeof(buf));
				  if(n > 0)
					{
					  bytes_read += n;
					  write(fileno(stdout), buf, n);
					  write(s->clients[i].sock, buf, n);
					}
				  else if(n == 0)
					{
					  remove_client(s, i);
					}
				  else
					{
					  perror("bad socket read");
					  remove_client(s, i);
					}
				}
			  //Method -----
			}
		  if(!found)
			{
			  perror("Select on unknown file descriptor!");
			  return 0;
			}
		}
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
  
  //be like our main
  //seperate method for more readability
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
