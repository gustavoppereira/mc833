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

void print_user(user data) {
  printf("user.first_name : %s\n", data.first_name);
  printf("user.last_name : %s\n\n", data.last_name);
}

user str2user (char* str) {
  char delim[] = ",";
  
  char *ptr = strtok(str, delim);
  int i = 0 ;
  user usr;
  
  while(ptr != NULL)
  {
    switch(i){
      case 0:
        strcpy(usr.email,ptr);
        break;
      case 1:
        strcpy(usr.first_name,ptr);
        break;
      case 2:
        strcpy(usr.last_name,ptr);
        break;
      case 3:
        strcpy(usr.image,ptr);
        break;
      case 4:
        strcpy(usr.city,ptr);
        break;
      case 5:
        strcpy(usr.formation,ptr);
        break;
      case 6:
        strcpy(usr.skills,ptr);
        break;
      case 7:
        strcpy(usr.experience,ptr);
        break;
        
    }
    
    i++;
    ptr = strtok(NULL, delim);
  }
  return usr;
}

user* str2userlist(char* str){
  char delim[] = ";";
  
  char *ptr = strtok(str, delim);
  int i = 0 ;
  user *usrs = calloc(1, sizeof *usrs * 10);
  
  while(ptr != NULL)
  {
    printf("user: %s\n", ptr);
    usrs[i] = str2user(ptr);
    
    i++;
    ptr = strtok(NULL, delim);
  }
  return usrs;
}

int main(int argc, char *argv[])
{
  int sockfd, numbytes;
  char buf[MAXDATASIZE+2];
  char input[MAXINPUTSIZE];
  struct addrinfo hints, *servinfo, *p;
  int rv, i;
  int method_number = 6;
  int selected_method = -1;
  int space_char = 0;
  user* results;
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
    
    fprintf(stdout, "Input selected %s:\n", method_fields[selected_method]);
    fgets (input, MAXINPUTSIZE, stdin);
    strtok(input, "\n");
    switch(selected_method){
      case 0:
        strcpy(send_request.formation,input);
        break;
      case 1:
        strcpy(send_request.city,input);
        break;
      case 2:
        strcpy(send_request.experience,input);
        
        fprintf(stdout, "Input selected %s:\n", method_fields[selected_method+1]);
        fgets (input, MAXINPUTSIZE, stdin);
        strtok(input, "\n");
      case 3:
      case 5:
        strcpy(send_request.email,input);
        break;
    }
  }
  // print input for debug
  fprintf(stdout, "Request: {\n");
  fprintf(stdout, "method: %i\n", send_request.operation);
  fprintf(stdout, "exp: %s\n", send_request.experience);
  fprintf(stdout, "form: %s\n", send_request.formation);
  fprintf(stdout, "email: %s\n", send_request.email);
  fprintf(stdout, "city: %s\n", send_request.city);
  fprintf(stdout, "}\n");
  
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
  printf("client: connecting to %s\n", s);
  
  freeaddrinfo(servinfo); // all done with this structure
  
  // Send method
  printf("client: sending operation\n");
  if ((numbytes = send(sockfd, &(send_request.operation), sizeof(int), 0)) == -1) {
    perror("failed sending operation");
    exit(1);
  }
  
  switch(selected_method){
    case 0:
      printf("client: sending formation\n");
      if ((numbytes = send(sockfd, &(send_request.formation), sizeof(char[50]), 0)) == -1) {
        perror("failed sending formation");
        exit(1);
      }
      space_char = 1;
      break;
    case 1:
      printf("client: sending city\n");
      if ((numbytes = send(sockfd, &(send_request.city), sizeof(char[50]), 0)) == -1) {
        perror("failed sending city");
        exit(1);
      }
      break;
    case 2:
      printf("client: sending experience\n");
      if ((numbytes = send(sockfd, &(send_request.experience), sizeof(char[50]), 0)) == -1) {
        perror("failed sending experience");
        exit(1);
      }
    case 3:
    case 5:
      printf("client: sending email\n");
      if ((numbytes = send(sockfd, &(send_request.email), sizeof(char[50]), 0)) == -1) {
        perror("failed sending email");
        exit(1);
      }
      break;
  }
  
  numbytes = -1;
  if ((numbytes = recv(sockfd, buf, MAXDATASIZE, 0)) == -1) {
    perror("recv");
    exit(1);
  }
  
  if(numbytes != 0) {
    
    buf[numbytes] = '\0';
    
    printf("%s\n", buf);
    
    results = str2userlist(buf);
    
    for(i=0; i<10; i++){
      print_user(results[i]);
    }
    
  }
  
  close(sockfd);
  
  return 0;
}
