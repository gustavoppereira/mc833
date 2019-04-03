/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#define PORT "3490" // the port client will be connecting to

#define MAXDATASIZE 100 // max number of bytes we can get at once

#define MAXINPUTSIZE 50 // max size of input

struct user {
    char email[50];
    char first_name[50];
    char last_name[50];
    char image[50];
    char city[50];
    char formation[50];
    char skills[50];
    char experience[50];
};

typedef struct user user;

struct request {
    char experience[50];
    char formation[50];
    char email[50];
    char city[50];
    int operation;
};

typedef struct request request;

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
	int sockfd, numbytes;
	char buf[MAXDATASIZE];
    char input[MAXINPUTSIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv, i;
    int method_number = 6;
    int selected_method = -1;
    char * method_names[] = {"user-formation","hability-city","experience-add", "experience-email", "user-all", "user-email"};
    char * method_fields[] = {"formation","city","experience", "email", "", "email"};
    request send_request;
    
	char s[INET6_ADDRSTRLEN];

	if (argc != 3) {
	    fprintf(stderr,"usage: client hostname method\n");
	    exit(1);
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
    
    // reading method input
    for(i=0; i<method_number; i++) {
        if(!strcmp(method_names[i], argv[2])) {
            selected_method = i;
            break;
        }
    }
    
    if(selected_method == -1) {
        fprintf(stderr, "failed to select method\n");
        return 1;
    }
    
    // get input for selected methods
    if(selected_method != 4) {
        send_request.operation = selected_method;
        if(i==2) {
            fprintf(stdout, "Input selected %s:\n", method_fields[selected_method+1]);
            fgets (input, MAXINPUTSIZE, stdin);
            strcpy(send_request.email,input);
        }
        fprintf(stdout, "Input selected %s:\n", method_fields[selected_method]);
        fgets (input, MAXINPUTSIZE, stdin);
        strcpy(send_request.experience,input);
    }

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			perror("client: connect");
			close(sockfd);
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
			s, sizeof s);
	printf("client: connecting to %s\n", s);

	freeaddrinfo(servinfo); // all done with this structure

	if ((numbytes = send(sockfd, &send_request, sizeof(request), 0)) == -1) {
		perror("send");
		exit(1);
	}

	if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
	    perror("recv");
	    exit(1);
	}

	buf[numbytes] = '\0';

	printf("client: received '%s'\n",buf);

	close(sockfd);

	return 0;
}
