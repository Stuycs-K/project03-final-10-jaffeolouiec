#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "user.h"
#include <dirent.h>

void createUser(){
    struct User user;
    printf("Enter new username: ");
    fgets(user.name, 64, stdin);
    // remove newline character
    user.name[strcspn(user.name, "\n")] = '\0';
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
    transaction->sender[strcspn(transaction->sender, "\n")] = '\0';

    printf("Enter receiver's username: ");
    fgets(transaction->receiver, 64, stdin);
    transaction->receiver[strcspn(transaction->receiver, "\n")] = '\0';
    
    printf("Enter amount of $: ");
    scanf("%d", &transaction->amount);
		getchar();
    
    printf("Enter PIN: ");
    scanf("%d", &transaction->confirmedPIN);
		getchar();

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

/*username would be a unique value*/
void changeUser(char* username, struct User * userToChange) {
	int fd = open(USER_FILE, O_RDWR);

	struct User *user;
  struct stat file;
  fstat(fd, &file);
  int fsize = file.st_size;
  int esize = sizeof(struct User);
  int count = fsize/esize; //count is the number of users
  struct User *users = (struct User *)malloc(fsize);
  read(fd, users, fsize);
  close(fd);
  //Go through user file
  int i = 0; //Index
  while (i < count){
    if(strcmp((&users[i])->name, username)==0){
      printf("User search Successful.\n");
      users[i] = *userToChange;
			break;
    }   
    i++;
  }

	// now we gotta save users
  fd = open(USER_FILE, O_WRONLY | O_TRUNC);
  if (fd < 0){
      printf("File Open Error\n");
      return;
  }

  if (write(fd, users, fsize) != fsize){
      printf("File Write Error\n");
      close(fd);
      return;
  }

  free(users);
}
                   

struct User* searchuser(char* username){
  //Reads user file
  int r_file = open(USER_FILE, O_RDONLY);
  if (r_file < 0){
    perror("open");
    return NULL;
  }

  struct stat file;
  fstat(r_file, &file);
  int fsize = file.st_size;
  int esize = sizeof(struct User);
  int count = fsize/esize; //count is the number of users

  struct User *users = (struct User *)malloc(fsize);
  read(r_file, users, fsize);
  close(r_file);

  //Go through user file
  struct User * foundUser = NULL;
  int i = 0; //Index

  while (i < count){
    if(strcmp((&users[i])->name, username)==0){
      foundUser = malloc(sizeof(struct User));
      *foundUser = users[i];
      break;
    }
    i++;
  }

  // Free the memory
  free(users);
  return foundUser;
}

void getInfo() {
  // ask the user for the username
  char username[64];
  printf("Enter the username: ");
  fgets(username, 64, stdin);
  // remove newline character
  username[strcspn(username, "\n")] = '\0';
  
  // search for the user
  struct User * user = searchuser(username);
  if (user == NULL) {
    printf("User not found\n");
    return;
  }
  // print the user's info
  printf("Name: %s\n", user->name);
  printf("PIN: %d\n", user->PIN);
  printf("Wallet: $%d\n", user->wallet);
}