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
#include <time.h>

#define PORT "3490"  // the port users will be connecting to

#define BACKLOG 10	 // how many pending connections queue will hold

#define DB_ENTRY_SIZE 10

#define MAXUSERBYTES 1024

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

FILE *open_file(char* mode) {
	return fopen("database", mode);
}

void fetch_users(user* result, int count) {
	FILE *stream = open_file("r+");

	if(!fread(result, sizeof(user), count, stream)) {
		perror("Database error");
	}
	fclose(stream);
}

void send_result(void* value, int numbytes, int sockfd) {
  struct timeval start;
  gettimeofday(&start, NULL);
  printf("Sending result at : %ld.%d\n", start.tv_sec, start.tv_usec);
	if (send(sockfd, value, numbytes, 0) == -1) {
		perror("error sending result");
	}
}

char *user2str (user ap)
{
  /* get lenght of string required to hold struct values */
  size_t len = 0;
  len = snprintf (NULL, len, "%s,%s,%s,%s,%s,%s,%s,%s", ap.email, ap.first_name, ap.last_name, ap.image, ap.city, ap.formation, ap.skills, ap.experience);

  /* allocate/validate string to hold all values (+1 to null-terminate) */
  char *apstr = calloc (1, sizeof *apstr * len + 1);
  if (!apstr) {
    fprintf (stderr, "%s() error: virtual memory allocation failed.\n", __func__);
  }

  /* write/validate struct values to apstr */
  if (snprintf (apstr, len + 1, "%s,%s,%s,%s,%s,%s,%s,%s", ap.email, ap.first_name, ap.last_name, ap.image, ap.city, ap.formation, ap.skills, ap.experience) > len + 1)
  {
    fprintf (stderr, "%s() error: snprintf returned truncated result.\n", __func__);
    return NULL;
  }
	printf("User -> %s\n", apstr);
  return apstr;
}

void concat_user(char* result, user data) {
	if(strcmp(result, "") == 0) {
		strcpy(result, user2str(data));
	} else {
		strcat(result, ";");
		strcat(result, user2str(data));
	}
}

void get_by_email(char* email, user* database, int sockfd) {
	char result[MAXUSERBYTES] = "";

	if (recv(sockfd, &email, 50, 0) == -1) {
		perror("list_by_formation: recv formation");
		exit(1);
	}
	for(int i = 0; i < DB_ENTRY_SIZE; i++) {
		if (strcmp(database[i].email, email) == 0) {
			concat_user(result, database[i]);
		}
	}
	send_result(&result, MAXUSERBYTES, sockfd);
}

void read_request(char* email, int sockfd) {
  struct timeval stop, start;
  gettimeofday(&start, NULL);

	printf("Started operation at : %ld.%d\n", start.tv_sec, start.tv_usec);
	user database[DB_ENTRY_SIZE];

	fetch_users(database, DB_ENTRY_SIZE);

	get_by_email(email, database, sockfd);

  gettimeofday(&stop, NULL);
	printf("Operation ended at : %ld.%d\n", stop.tv_sec, stop.tv_usec);
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

	char email[50];
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

			if ((numbytes = recv(new_fd, email, sizeof(int), 0)) == -1) {
				perror("error receiving request");
			}
      struct timeval start;
      gettimeofday(&start, NULL);
			printf("Received request at : %ld.%d\n", start.tv_sec, start.tv_usec);
			read_request(email, new_fd);

			close(new_fd);
			exit(0);
		}
		close(new_fd);  // parent doesn't need this
	}

	return 0;
}
