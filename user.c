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

    searchuser(transaction->sender);
    if (strcmp((searchuser(transaction->sender))->name,transaction->sender)==0){
      printf("First User Search Success\n");
      if (strcmp((searchuser(transaction->sender))->name,transaction->sender)==0){
        printf("Second User Search Success\n");
        if(searchuser(transaction->sender)->PIN==transaction->confirmedPIN){
          printf("Authenticated\n");
          if(searchuser(transaction->sender)->wallet-transaction->amount>=0){
            printf("Money Success\n");
          }
          else{
            printf("Not enough money failure :((((\n");
          }
        }
        else{
          printf("Fraud Attempt\n");
        }
      }
      else{
        printf("Second User Search Failure\n");
      }
    }
    else{
      printf("First User Search Failure\n");
    }

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
  fstat(r_file, &file);
  int fsize = file.st_size;
  int esize = sizeof(struct User);
  int count = fsize/esize; //count is the number of users
  struct User *users = (struct User *)malloc(fsize);
  read(r_file, users, fsize);
  close(r_file);
  //Go through user file
  int i = 0; //Index
  while (i < count){
    if(strcmp((&users[i])->name, username)==0){
      printf("User search Successful.\n");
			users[i] = userToChange;
			break;
    }   
    i++;
  }

	// now we gotta save users

  free(users);
  return user;
}
                   

struct User* searchuser(char* username){
  //Reads user file
  int r_file = open(USER_FILE, O_RDONLY);
  struct User *user;
  struct stat file;
  fstat(r_file, &file);
  int fsize = file.st_size;
  int esize = sizeof(struct User);
  int count = fsize/esize; //count is the number of users
  struct User *users = (struct User *)malloc(fsize);
  read(r_file, users, fsize);
  close(r_file);
  //Go through user file
  int i = 0; //Index
  while (i < count){
    if(strcmp((&users[i])->name, username)==0){
      printf("User search Successful.\n");
      free(users);
      return &(users[i]);
    }
    i++;
  }
  free(users);
  return user;
}
