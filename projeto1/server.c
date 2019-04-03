/*
** server.c -- a stream socket server demo
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
#include <signal.h>

#define PORT "3490"  // the port users will be connecting to

#define BACKLOG 10	 // how many pending connections queue will hold

#define DB_ENTRY_SIZE 10

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

struct request {
	char experience[200];
	char formation[50];
	char email[50];
	char city[50];
	int operation;
};

typedef struct request request;
typedef struct user user;


void sigchld_handler(int s)
{
	(void)s; // quiet unused variable warning

	// waitpid() might overwrite errno, so we save and restore it:
	int saved_errno = errno;

	while(waitpid(-1, NULL, WNOHANG) > 0);

	errno = saved_errno;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

FILE *open_file() {
	return fopen("database", "r+");
}

void list_by_formation(char* formation, int sockfd) {
	printf("Executing list_by_formation (%s)\n", formation);
	FILE *stream = open_file();
	user database[DB_ENTRY_SIZE], result[DB_ENTRY_SIZE];
	int count = 0;

	if(!fread(database, sizeof(user), DB_ENTRY_SIZE, stream)) {
		perror("Database error");
	}

	fclose(stream);

	for(int i = 0; i < DB_ENTRY_SIZE; i++) {
		if (strcmp(database[i].formation, formation) == 0) {
			result[count++] = database[i];
		}
	}

	if (send(sockfd, &result, count * sizeof(user), 0) == -1) {
		perror("list_by_formation");
	}
}

void list_skills_by_city(char* city, int sockfd) {

}

void add_skill(char* email, int sockfd) {

}

void list_all(int sockfd) {

}

void get_experience(char* email, int sockfd) {

}

void get_user(char* email, int sockfd) {

}

void read_request(request req, int sockfd) {
	printf("Reading request for operation : %d\n", req.operation);
    
    
    fprintf(stdout, "Request: {\n");
    fprintf(stdout, "method: %i\n", req.operation);
    fprintf(stdout, "exp: %s\n", req.experience);
    fprintf(stdout, "form: %s\n", req.formation);
    fprintf(stdout, "email: %s\n", req.email);
    fprintf(stdout, "city: %s\n", req.city);
    fprintf(stdout, "}\n");
    
	switch (req.operation) {
		case 0:
			list_by_formation(req.formation, sockfd);
		break;
		case 1:
			list_skills_by_city(req.city, sockfd);
		break;
		case 2:
			add_skill(req.email, sockfd);
		break;
		case 3:
			get_experience(req.email, sockfd);
		break;
		case 4:
			list_all(sockfd);
		break;
		case 5:
			get_user(req.email, sockfd);
		break;
	}
}

int main(void)
{
	int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	struct sigaction sa;
	int yes=1;
	char s[INET6_ADDRSTRLEN];
	int rv;

	FILE *outfile;

	user input1;
	user input2;

	request req;
	int numbytes = 0;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (p == NULL) {
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	printf("server: waiting for connections...\n");

	while(1) {  // main accept() loop
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
			continue;
		}

		inet_ntop(their_addr.ss_family,
			get_in_addr((struct sockaddr *)&their_addr),
			s, sizeof s);
		printf("server: got connection from %s\n", s);

		if (!fork()) { // this is the child process
			close(sockfd); // child doesn't need the listener

			if ((numbytes = recv(new_fd, &req, sizeof(request), 0)) == -1) {
				perror("error receiving request");
			}

			read_request(req, new_fd);

			// if (send(new_fd, "Hello, world!", 13, 0) == -1)
			// 	perror("send");
			//
			// if ((numbytes = recv(new_fd, buffer, 100, 0)) == -1) {
			// 	perror("recv");
			// }

			printf("server received request with operation : %d\n", req.operation);

			close(new_fd);
			exit(0);
		}
		close(new_fd);  // parent doesn't need this
	}

	return 0;
}
