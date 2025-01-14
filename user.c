#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "user.h"

void createUser(){
    struct User user;
    printf("Enter new username: ");
    fgets(user.name, 64, stdin);
    printf("Enter a 4-digit PIN: ");
    scanf("%d", &user.PIN);
    if (user.PIN < 1000 || user.PIN > 9999){
        printf("Invalid PIN\n");
        return;
    }
    user.wallet = 100;
    int file = open(USER_FILE, O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (file < 0){
        printf("File Open Error\n");
        return;
    }
    if (write(file, &user, sizeof(struct User)) != sizeof(struct User)){
        printf("File Write Error\n");
        close(file);
        return;
    }
    close(file);
}

void transaction(){
		struct Transaction *transaction = malloc(sizeof(struct Transaction));

		printf("Enter username: ");
    fgets(transaction->sender, 64, stdin);
    printf("Enter receiver's username: ");
    fgets(transaction->receiver, 64, stdin);
    printf("Enter amount of $: ");
    scanf("%d", &transaction->amount);
		getchar();
    printf("Enter PIN: ");
    scanf("%d", &transaction->confirmedPIN);
		getchar();

    if (strcmp(*(searchuser(transaction->sender))->name,transaction->sender)==0){
      printf("Search success\n");
    }

		//printf("Here are the vars: %s %s %d %d\n", transaction->sender, transaction->receiver, transaction->amount, transaction->confirmedPIN);

    int fd = open(PIPE_NAME, O_WRONLY);
    if (fd < 0){
        printf("Pipe Opening Error\n");
        return;
    }

    if (write(fd, transaction, sizeof(struct Transaction)) == -1) {
				printf("Transaction Send Error\n");
        return;
    } else {
				printf("success!\n");
		}

		close(fd);
		free(transaction);
}

struct User* searchuser(char* username){
  //Reads user file
  printf("I made it\n");
  int r_file = open(USER_FILE, O_RDONLY);
  struct User *user;
  int fsize;
  fstat(r_file, fsize);
  int esize = sizeof(struct User);
  int count = fsize/esize; //count is the number of users
  struct User *users = (struct User *)malloc(fsize);
  read(r_file, users, fsize);
  close(r_file);
  printf("I made it2\n");
  //Go through user file
  int i = 0; //Index
  while (i < count){
    printf("I made it 3\n");
    /*
    if(strcmp(*users[i].name, *username)==0){
      printf("User search Successful.\n");
      free(users);
      return &(users[i]);
    }
    */
    i++;
  }
  printf("Invalid username.\n");
  free(users);
  return NULL;
}
