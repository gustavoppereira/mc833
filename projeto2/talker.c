/*
** talker.c -- a datagram "client" demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define SERVERPORT "4950"	// the port users will be connecting to

#define MAXDATASIZE 1024 // max number of bytes we can get at once

#define MAXINPUTSIZE 50 // max size of input

struct user {
  char email[50];
  char first_name[50];
  char last_name[50];
  char image[50];
  char city[50];
  char formation[50];
  char skills[50];
  char experience[200];
};

typedef struct user user;

int main(int argc, char *argv[]) {
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;

	char result[MAXDATASIZE];

	if (argc != 3) {
		fprintf(stderr,"usage: talker hostname message\n");
		exit(1);
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if ((rv = getaddrinfo(argv[1], SERVERPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and make a socket
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("talker: socket");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "talker: failed to create socket\n");
		return 2;
	}
  
  struct timeval start, stop;
  gettimeofday(&start, NULL);
  printf("%ld.%06d,", start.tv_sec, start.tv_usec);

	if ((numbytes = sendto(sockfd, argv[2], strlen(argv[2]), 0,
			 p->ai_addr, p->ai_addrlen)) == -1) {
		perror("talker: sendto");
		exit(1);
	}

  gettimeofday(&stop, NULL);
  printf("%ld.%06d,", stop.tv_sec, stop.tv_usec);
  printf("%ld.%06d,", stop.tv_sec-start.tv_sec, stop.tv_usec-start.tv_usec);
//  printf("talker: sent %d bytes to %s\n", numbytes, argv[1]);
  
  
  gettimeofday(&start, NULL);
  printf("%ld.%06d,", start.tv_sec, start.tv_usec);

	if ((numbytes = recvfrom(sockfd, result, MAXDATASIZE, 0, p->ai_addr, &p->ai_addrlen)) == -1) {
		perror("error receiving response");
		exit(1);
	}
  
  
  gettimeofday(&stop, NULL);
  printf("%ld.%06d,", stop.tv_sec, stop.tv_usec);
  printf("%ld.%06d\n", stop.tv_sec-start.tv_sec, stop.tv_usec-start.tv_usec);

	freeaddrinfo(servinfo);

//  printf("talker: received %s\n", result);
	close(sockfd);

	return 0;
}
