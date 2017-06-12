/***********************************************************************
 * Program:
 *    Lab RockClientT2, Rock/Paper/Scissors with Sockets - Client Code
 *    Brother Jones, CS 460
 * Author:
 *    Colton Kopsa
 * Summary:
 *    Rock, paper, scissors client to connect to properly created RPS server
 ***************************************************************************
 * Changes made to my code for the re-submission:
 *   - No changes were made
 ***************************************************************************/
/*
** client.c -- a stream socket client demo
**
** This is code from http://beej.us/guide/bgnet/
** It was modified to partly conform to BYU-Idaho style.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <iostream>
#include <string>
#include <arpa/inet.h>

#define PORT "3490" // the port client will be connecting to

#define MAXDATASIZE 100 // max number of bytes we can get at once

char validInput(char playerInput);
void computeRes(char player1, char player2);

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
  if (sa->sa_family == AF_INET)
    {
      return &(((struct sockaddr_in*)sa)->sin_addr);
    }

  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
  int sockfd;
  int numbytes;
  char buf[MAXDATASIZE] = "";
  struct addrinfo hints;
  struct addrinfo *servinfo;
  struct addrinfo *p;
  int rv;
  char s[INET6_ADDRSTRLEN];

  if (argc != 2)
    {
      fprintf(stderr,"usage: client hostname\n");
      exit(1);
    }

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  std::string userInput;
  while (userInput[0] != 'q' && buf[0] != 'q')
    {
      if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0)
        {
          fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
          return 1;
        }

      // loop through all the results and connect to the first we can
      for (p = servinfo; p != NULL; p = p->ai_next)
        {
          if ((sockfd = socket(p->ai_family, p->ai_socktype,
                               p->ai_protocol)) == -1)
            {
              perror("client: socket");
              continue;
            }

          if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
            {
              perror("client: connect");
              close(sockfd);
              continue;
            }

          break;
        }

      if (p == NULL)
        {
          fprintf(stderr, "client: failed to connect\n");
          return 2;
        }

      inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
                s, sizeof s);
      printf("client: connecting to %s\n", s);

      freeaddrinfo(servinfo); // all done with this structure
      if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1)
        {
          perror("recv");
          exit(1);
        }
      printf("client: received '%s'\n",buf);
	while (true) {
      std::cin >> userInput;
	userInput[0] = validInput(userInput[0]);
      if (send(sockfd, userInput.c_str(), 1, 0) == -1)
        perror("send");
        if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1)
          {
            perror("recv");
            exit(1);
          }
        if (validInput(buf[0]) != 'q') {
          buf[numbytes] = '\0';
          printf("client: received '%s'\n",buf);
          computeRes(userInput[0], buf[0]);
        } else {
break;
}
    }
}
      close(sockfd);
  return 0;
}

char validInput(char playerInput) {
  switch (tolower(playerInput)) {
  case 'r':
  case 'p':
  case 's':
    return playerInput;
  }
  return 'q';
}

void computeRes(char player1, char player2) {
  if (player2 == 'q') {
    std::cout << "Your opponent quit. You win!" << std::endl;
    return;
  }
  if (player1 == 'q') {
    std::cout << "You quit. You lose!" << std::endl;
    return;
  }
  switch (player1 - player2) {
  case 0:
    std::cout << "draw" << std::endl;
    break;
  case 'r' - 's':
    std::cout << "win" << std::endl;
    break;
  case 'r' - 'p':
    std::cout << "lose" << std::endl;
    break;
  case 's' - 'p':
    std::cout << "win" << std::endl;
    break;
  case 's' - 'r':
    std::cout << "lose" << std::endl;
    break;
  case 'p' - 's':
    std::cout << "lose" << std::endl;
    break;
  case 'p' - 'r':
    std::cout << "win" << std::endl;
    break;
  default:
    std::cout << "error" << std::endl;
    break;
  }
}
