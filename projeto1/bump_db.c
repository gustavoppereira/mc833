#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main(void) {

  FILE *file;

  user input1 = {"buliojarros@gmail.com", "Julio", "Barros", "paulinho.jpg", "Taubate", "Maconheiro",  "Bolar Beck", "Varias porra"};
  user input2 = {"gustavoppereira@gmail.com", "Gustavo", "Pereira", "gus.jpg", "Rio", "Maconheiro Mirim",  "Fumar paiero", "Varios nada"};
  user input3 = {"teste@email.com", "Teste", "1", "teste1.jpg", "Campinas", "Maconheiro",  "Fumar paiero", "Nada"};

  file = fopen("database", "w");

  if(file == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  fwrite(&input1, sizeof(user), 1, file);
  fwrite(&input2, sizeof(user), 1, file);
  fwrite(&input3, sizeof(user), 1, file);

  fclose(file);

  return 0;
}
