#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>
#include <iostream>
#include <string.h>

typedef struct
{
  int                sock;
  struct sockaddr_in server_addr;
} Client;

int prepare_client(Client *c, char *hostname, int port)
{
  struct hostent *host_ent;
  
  host_ent = gethostbyname(hostname);
  if(host_ent == 0)
    {
      perror("Could not lookup host by name");
      return 0;
    }
  
  c->server_addr.sin_family      = AF_INET;
  c->server_addr.sin_port        = htons((unsigned short)port);
  c->server_addr.sin_addr.s_addr = *((u_int32_t *)host_ent->h_addr_list[0]);

  if((c->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
      perror("Can't Create Socket");
      return 0;
    }
  
  if(connect(c->sock, (struct sockaddr *)&(c->server_addr), sizeof(c->server_addr)) < 0)
    {
      if(errno == ECONNREFUSED)
	{
	  perror("Connectiono Refused");
	}
      else if(errno == ENETUNREACH)
	{
	  perror("Network Unreachable");
	}
      else if(errno == ETIMEDOUT)
	{
	  perror("Connection Timed Out");
	}
      else if(errno == EHOSTUNREACH)
	{
	  perror("Host Unreachable");
	}
      else
	{
	  perror("Could not connect to host.");
	}
      close(c->sock);
      return 0;
    }
  return 1;
}

int main(int argc, char **argv)
{
  char *hostname = 0;
  int port = 0;
  int c;

  while((c = getopt(argc,argv,"p:h:")) != -1)
    {
      switch(c)
	{
	case 'p': port     = atoi(optarg); break;
	case 'h': hostname = optarg; break;
	default:
	  printf("Unknown option: %c\n", c);
	  printf("-p port\n");
	  printf("-h host\n");
	  exit(1);
	  break;
	}
    }
  if(!port)
    {
      printf("must specify server port to use with -p !\n");
      exit(1);
    }
  if(!hostname)
    {
      printf("must specify server hostname to use with -h !\n");
      exit(1);
    }

  Client client;

  prepare_client(&client, hostname, port);

  char buf[1024];
  // zero out buffer so no garbage is sent to client or server on first pass
  memset(buf, 0, sizeof(buf));
  int readOut;

  int nfds, n;
  nfds = getdtablesize();
  fd_set rfds;

  while(1) {
    FD_ZERO(&rfds);
    FD_SET(client.sock, &rfds);
    FD_SET(fileno(stdin), &rfds);

    if((n = select(nfds, &rfds, 0, 0, 0)) < 0)
      {
	perror("Bad select on socket or keyboard!");
	return 0;
      }

    if(FD_ISSET(client.sock, &rfds))
      { // socket data
	// read from server
	readOut = read(client.sock, buf, sizeof(buf));
	if(readOut > 0) {
	  // null terminate string
	  buf[readOut] = 0;
	  // write to screen
	  write(fileno(stdout), buf, readOut);
	}
      }
    if(FD_ISSET(fileno(stdin), &rfds))
      { // keyboard data
	// read from keyboard
	readOut = read(fileno(stdin), buf, sizeof(buf));
	if(readOut > 0){
	  // null terminate string
	  buf[readOut] = 0;
	  // write to server
	  write(client.sock, buf, readOut);
	}
      }
	  
  }
  close(client.sock);

  return 0;
}
