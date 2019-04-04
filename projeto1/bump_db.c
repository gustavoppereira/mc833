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
  user input3 = {"teste1@email.com", "Teste", "1", "teste1.jpg", "Campinas", "Maconheiro",  "Fumar paiero", "Nada"};
  user input4 = {"teste2@email.com", "Teste", "2", "teste2.jpg", "Campinas", "Maconheiro Senior",  "Fumar paiero", "Nada"};
  user input5 = {"teste3@email.com", "Teste", "3", "teste3.jpg", "Campinas", "Maconheiro",  "Fumar paiero", "Nada"};
  user input6 = {"teste4@email.com", "Teste", "4", "teste4.jpg", "Campinas", "Maconheiro Senior",  "Fumar paiero", "Nada"};
  user input7 = {"teste5@email.com", "Teste", "5", "teste5.jpg", "Campinas", "Maconheiro Deluxe",  "Fumar paiero", "Nada"};
  user input8 = {"teste6@email.com", "Teste", "6", "teste6.jpg", "Campinas", "Maconheiro Pica",  "Fumar paiero", "Nada"};
  user input9 = {"teste7@email.com", "Teste", "7", "teste7.jpg", "Campinas", "Jogador de Fifa",  "Jogar fifa", "Nada"};
  user input10 = {"teste8@email.com", "Teste", "8", "teste8.jpg", "Campinas", "Maconheiro",  "Fumar paiero", "Nada"};

  file = fopen("database", "w");

  if(file == NULL) {
    printf("Error opening file\n");
    exit(1);
  }

  fwrite(&input1, sizeof(user), 1, file);
  fwrite(&input2, sizeof(user), 1, file);
  fwrite(&input3, sizeof(user), 1, file);
  fwrite(&input4, sizeof(user), 1, file);
  fwrite(&input5, sizeof(user), 1, file);
  fwrite(&input6, sizeof(user), 1, file);
  fwrite(&input7, sizeof(user), 1, file);
  fwrite(&input8, sizeof(user), 1, file);
  fwrite(&input9, sizeof(user), 1, file);
  fwrite(&input10, sizeof(user), 1, file);

  fclose(file);

  return 0;
}
