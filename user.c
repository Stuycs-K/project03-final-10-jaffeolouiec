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
/*
void searchuser(char* username){
  //Get size + Count size needed (From readdata)
  int r_file = open(USER_FILE, O_RDONLY);
  struct User user;
  fstat(r_file, &user);
  int fsize = user.st_size;
  int esize = sizeof(struct User);
  int count = fsize/esize;
  struct User *users = (struct User *)malloc(fsize);
  read(r_file, users, fsize);
  close(r_file);
  //Index
  int i = 0;
  while (i < count){
    scanf("%d %d %[^\n]",&entries[i - 1].year,&entries[i - 1].population,entries[i - 1].boro);
    int w_file = open(USER_FILE, O_WRONLY);
  }
  if (username){
    printf("Invalid username.\n");
    free(entries);
    return;
  }
  printf("User search Successful.\n");
}
*/
