/***********************************************************************
 * Program:
 *    Lab RockSrvT2, Rock/Paper/Scissors with Sockets - Server Code
 *    Brother Jones, CS 460
 * Author:
 *    Colton Kopsa
 * Summary:
 *    A rock, paper, scissors game server for network gaming.
 ***************************************************************************
 * Changes made to my code for the re-submission:
 *   - No changes were made
 ************************************************************************/
/*
** server.c -- a stream socket server demo
**
** This is a modified of code from http://beej.us/guide/bgnet/
** The code was modified to take out the "fork" to start a new process and
** the signal handler.
**
** It was modified to partly conform to BYU-Idaho style.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <iostream>
#include <string>
//  #include <signal.h>

#define PORT "3490"  // the port users will be connecting to

#define BACKLOG 10   // how many pending connections queue will hold

#define MAXDATASIZE 100 // max number of bytes we can get at once

void getPlayerInput(int player_fd, char playerInputBuffer[MAXDATASIZE]);
char validInput(char playerInput);
// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
  if (sa->sa_family == AF_INET)
    {
      return &(((struct sockaddr_in*)sa)->sin_addr);
    }

  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
  char player1Input[MAXDATASIZE] = "";
  char player2Input[MAXDATASIZE] = "";
  int sockfd;      // listen on sock_fd
  int p1_fd = 0;      // new connection on new_fd
  int p2_fd = 0;      // new connection on new_fd
  struct addrinfo hints;
  struct addrinfo *servinfo;
  struct addrinfo *p;
  struct sockaddr_storage their_addr; // connector's address information
  socklen_t sin_size;
  //struct sigaction sa;
  int yes=1;
  char s[INET6_ADDRSTRLEN];
  int rv;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; // use my IP

  if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
    {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
      return 1;
    }

  // loop through all the results and bind to the first we can
  for(p = servinfo; p != NULL; p = p->ai_next)
    {
      if ((sockfd = socket(p->ai_family, p->ai_socktype,
                           p->ai_protocol)) == -1)
        {
          perror("server: socket");
          continue;
        }

      if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                     sizeof(int)) == -1)
        {
          perror("setsockopt");
          exit(1);
        }

      if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
          close(sockfd);
          perror("server: bind");
          continue;
        }

      break;
    }

  freeaddrinfo(servinfo); // all done with this structure

  if (p == NULL)
    {
      fprintf(stderr, "server: failed to bind\n");
      exit(1);
    }

  if (listen(sockfd, BACKLOG) == -1)
    {
      perror("listen");
      exit(1);
    }

  std::string welcomeString = "Welcome to the NextGen Rock Paper Scissors!\0";
  while (true)
    {
      printf("server: waiting for connections...\n");
      sin_size = sizeof their_addr;
      p1_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
      if (p1_fd == -1)
        {
          perror("accept");
          continue;
        }

      inet_ntop(their_addr.ss_family,
                get_in_addr((struct sockaddr *)&their_addr),
                s, sizeof s);
      printf("server: got connection from %s\n", s);
      while(p2_fd == 0) {
        p2_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (p2_fd == -1)
          {
            perror("accept");
            continue;
          }

        inet_ntop(their_addr.ss_family,
                  get_in_addr((struct sockaddr *)&their_addr),
                  s, sizeof s);
        printf("server: got connection from %s\n", s);
      }
      //sends welcome string
      if (send(p1_fd, welcomeString.c_str(), welcomeString.size(), 0) == -1)
        perror("send");
      if (send(p2_fd, welcomeString.c_str(), welcomeString.size(), 0) == -1)
        perror("send");
	while (true) {
//      std::thread player1(getPlayerInput, p1_fd, player1Input);
//      std::thread player2(getPlayerInput, p2_fd, player2Input);
getPlayerInput( p2_fd, player2Input);
getPlayerInput( p1_fd, player1Input);

      //player1.join();
      //player2.join();
      if (send(p1_fd, player2Input, 1, 0) == -1)
        perror("send");
      if (send(p2_fd, player1Input, 1, 0) == -1)
        perror("send");
	if (player1Input[0] == 'q' || player2Input[0] == 'q')
break;
	
}
      close(p1_fd);
      close(p2_fd);
      p1_fd = 0;
      p2_fd = 0;
    }
  return 0;
}

void getPlayerInput(int player_fd, char playerInputBuffer[MAXDATASIZE]) {
  //reads from client.
  int numbytes;
  if ((numbytes = recv(player_fd, playerInputBuffer, MAXDATASIZE-1, 0)) == -1)
    {
      perror("recv");
      exit(1);
    }
  playerInputBuffer[numbytes] = '\0';
  printf("server: received '%s'\n", playerInputBuffer);
  playerInputBuffer[0] = validInput(playerInputBuffer[0]);

}

char validInput(char playerInput) {
  switch (tolower(playerInput)) {
  case 'r':
  case 'p':
  case 's':
    return tolower(playerInput);
  }
  return 'q';
}
