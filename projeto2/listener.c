/*
** listener.c -- a datagram sockets "server" demo
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

#define MYPORT "4950"	// the port users will be connecting to

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

void send_result(void* value, int numbytes, int sockfd, struct sockaddr_storage addr, socklen_t addr_len) {
  struct timeval start;
  gettimeofday(&start, NULL);
  printf("%ld.%06d\n", start.tv_sec, start.tv_usec);
	if (sendto(sockfd, value, numbytes, 0, (struct sockaddr_storage *)&addr, addr_len) == -1) {
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

void get_by_email(user* database, char* email, int sockfd, struct sockaddr_storage addr, socklen_t addr_len) {
	char result[MAXUSERBYTES] = "";

	for(int i = 0; i < DB_ENTRY_SIZE; i++) {
		if (strcmp(database[i].email, email) == 0) {
			concat_user(result, database[i]);
		}
	}
	send_result(&result, MAXUSERBYTES, sockfd, addr, addr_len);
}

void read_request(char* email, int sockfd, struct sockaddr_storage addr, socklen_t addr_len) {
  struct timeval stop, start;
  gettimeofday(&start, NULL);

	printf("%ld.%06d,", start.tv_sec, start.tv_usec);
	user database[DB_ENTRY_SIZE];

	fetch_users(database, DB_ENTRY_SIZE);

	get_by_email(database, email, sockfd, addr, addr_len);

  gettimeofday(&stop, NULL);
	printf("%ld.%06d,", stop.tv_sec, stop.tv_usec);
  printf("%ld.%06d\n", stop.tv_sec-start2.tv_sec, stop.tv_usec-start2.tv_usec);
}

int main(void)
{
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;
	struct sockaddr_storage their_addr;
	char buf[MAXUSERBYTES];
	socklen_t addr_len;
	char s[INET6_ADDRSTRLEN];

	char email[50];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("listener: socket");
			continue;
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("listener: bind");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "listener: failed to bind socket\n");
		return 2;
	}

	freeaddrinfo(servinfo);

//  printf("listener: waiting to recvfrom...\n");

	addr_len = sizeof their_addr;
	if ((numbytes = recvfrom(sockfd, email, MAXUSERBYTES-1 , 0,
		(struct sockaddr *)&their_addr, &addr_len)) == -1) {
		perror("recvfrom");
		exit(1);
	}

//  printf("Received email %s\n", email);

	read_request(email, sockfd, their_addr, addr_len);

	// printf("listener: got packet from %s\n",
	// 	inet_ntop(their_addr.ss_family,
	// 		get_in_addr((struct sockaddr *)&their_addr),
	// 		s, sizeof s));
	// printf("listener: packet is %d bytes long\n", numbytes);
	// buf[numbytes] = '\0';
	// printf("listener: packet contains \"%s\"\n", buf);

	close(sockfd);

	return 0;
}
