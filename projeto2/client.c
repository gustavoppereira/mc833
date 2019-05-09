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
#include <time.h>
#include <arpa/inet.h>

#define PORT "3490" // the port client will be connecting to

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

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }

  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void print_user(user data) {
  printf("user.email : %s\n", data.email);
  printf("user.first_name : %s\n", data.first_name);
  printf("user.last_name : %s\n", data.last_name);
  printf("user.image : %s\n", data.image);
  printf("user.city : %s\n", data.city);
  printf("user.formation : %s\n", data.formation);
  printf("user.skills : %s\n", data.skills);
  printf("user.experience : %s\n", data.experience);
  printf("\n");
}

void print_user_skill(user data) {
  printf("skill : %s\n", data.skills);
}

user* str2userlist(char* str){
  char delim[] = ";,";
  char copy[1024];

  strcpy(copy, str);

  char *ptr = strtok(str, delim);
  int i = 0, end = 0;
  user *usrs = calloc(1, sizeof *usrs * 10);

  while(ptr != NULL && !end)
  {
    user usr;

    strcpy(usr.email,ptr);

    ptr = strtok(NULL, delim);
    strcpy(usr.first_name,ptr);

    ptr = strtok(NULL, delim);
    strcpy(usr.last_name,ptr);

    ptr = strtok(NULL, delim);
    strcpy(usr.image,ptr);

    ptr = strtok(NULL, delim);
    strcpy(usr.city,ptr);

    ptr = strtok(NULL, delim);
    strcpy(usr.formation,ptr);

    ptr = strtok(NULL, delim);
    strcpy(usr.skills,ptr);

    ptr = strtok(NULL, delim);
    strcpy(usr.experience,ptr);

    usrs[i] = usr;

    ptr = strtok(NULL, delim);
    if(ptr == NULL || i == 10)
      end = 1;
    i++;
  }
  return usrs;
}

int main(int argc, char *argv[])
{
  int sockfd, numbytes;
  char buf[MAXDATASIZE+2];
  struct addrinfo hints, *servinfo, *p;
  int rv, i;
  int space_char = 0;
  user* results;
  char email[50];

  char s[INET6_ADDRSTRLEN];

  if (argc != 3) {
    fprintf(stderr,"usage: client hostname email\n");
    exit(1);
  }

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  // get inputed mail
  strcpy(email,argv[2]);

  // loop through all the results and connect to the first we can
  for(p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype,
                         p->ai_protocol)) == -1) {
      perror("client");
      continue;
    }

    if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      perror("client");
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
//  printf("client: connecting to %s\n", s);

  freeaddrinfo(servinfo); // all done with this structure
  
  struct timeval start, stop;
  gettimeofday(&start, NULL);
  printf("%ld.%06d,", start.tv_sec, start.tv_usec);
  
  // Send email
//  printf("client: sending email\n");
  if ((numbytes = send(sockfd, &(email), sizeof(char[50]), 0)) == -1) {
    perror("failed sending email");
    exit(1);
  }

  gettimeofday(&stop, NULL);
  printf("%ld.%06d,", stop.tv_sec, stop.tv_usec);
  printf("%ld.%06d,", stop.tv_sec-start.tv_sec, stop.tv_usec-start.tv_usec);

  struct timeval start2;
  gettimeofday(&start2, NULL);
  printf("%ld.%06d,", start2.tv_sec, start2.tv_usec);

  numbytes = -1;
  if ((numbytes = recv(sockfd, buf, MAXDATASIZE, 0)) == -1) {
    perror("recv");
    exit(1);
  }

  gettimeofday(&stop, NULL);
  printf("%ld.%06d,", stop.tv_sec, stop.tv_usec);
  printf("%ld.%06d\n", stop.tv_sec-start2.tv_sec, stop.tv_usec-start2.tv_usec);

//  printf("Time for full operation: %ld.%d\n", stop.tv_sec-start.tv_sec, stop.tv_usec-start.tv_usec);

  if(numbytes != 0) {

    buf[numbytes] = '\0';

    results = str2userlist(buf);

    for(i=0; i<10 && results[i].email[0] != ' '; i++){
//        if(selected_method == 1)
//          print_user_skill(results[i]);
//        else print_user(results[i]);
    }

  }

  close(sockfd);

  return 0;
}
