#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "user.h"
#include <dirent.h>
#include <errno.h>

void log(char * message) {
    int fd = open(LOG_FILE, O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (fd < 0){
        printf("File Open Error\n");
        return;
    }
    if (write(fd, message, strlen(message)) != strlen(message)){
        printf("File Write Error\n");
        close(fd);
        return;
    }
    close(fd);
}

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

    // check if the user already exists
    //first check if USER_FILE exists, if it does, continue to check within that file with search user
    if (access(USER_FILE, F_OK) == 0) {
      struct User * existing_user = searchuser(user.name);
      if (existing_user != NULL) {
          printf("User already exists\n");
          free(existing_user);
          return;
      }
      free(existing_user);
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

    char * log_message;
    asprintf(&log_message, "user.c: User %s created\n", user.name);
    log(log_message);
    free(log_message);
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
        perror("Pipe Opening Error\n");
        return;
    }

    if (write(fd, transaction, sizeof(struct Transaction)) == -1) {
				perror("Transaction Send Error\n");
        close(fd);
        free(transaction);

        // log message
        char * log_message;
        asprintf(&log_message, "user.c: Transaction failed: send error\n");
        log(log_message);

        return;
    }

    // else we can write to the log file success
    char * log_message;
    asprintf(&log_message, "user.c: Transaction request sent: from %s to %s for $%d\n", transaction->sender, transaction->receiver, transaction->amount);
    log(log_message);

    sleep(1);
    printf("Transaction Send Success!\n");

    close(fd);

    int fd2 = open(BANK_TO_USER_PIPE, O_RDONLY);
    if (fd2 < 0){
        perror("Pipe Opening Error\n");

        // log message
        char * log_message;
        asprintf(&log_message, "user.c: Transaction failed: receive error\n");
        log(log_message);

        return;
    }
    
    free(transaction);
    char receipt[50];
    ssize_t bytes_read = read(fd2, receipt, 49);
    if (bytes_read <= 0) {
      perror("Error reading from file descriptor");
      close(fd2);

      // log message
      char * log_message;
      asprintf(&log_message, "user.c: Transaction failed: read error\n");
      log(log_message);

      return;
    }
    receipt[bytes_read] = '\0';
    printf("Transaction Status (From Bank): %s\n", receipt);
    close(fd2);
}

void deposit(){
		struct Transaction *transaction = malloc(sizeof(struct Transaction));

		printf("Enter username: ");
    fgets(transaction->sender, 64, stdin);
    transaction->sender[strcspn(transaction->sender, "\n")] = '\0';
    
    printf("Enter amount of $: ");
    scanf("%d", &transaction->amount);
		getchar();
    
    printf("Enter PIN: ");
    scanf("%d", &transaction->confirmedPIN);
		getchar();

    struct User * user1 = searchuser(transaction->sender);

    // See if the transaction should fail
	  // See if both users exist
	  if (user1 == NULL) {
   		char* message = "One or both users do not exist.";
      printf("%s",message);
    	return;
	  }
	  // See if user1's pin is right
	  if (transaction->confirmedPIN != user1->PIN) {
    	char* message = "Incorrect PIN.";
      printf("%s",message);
    	return;
	  }
	  // Make sure the transaction.amount is >= 0
	  if (transaction->amount < 0) {
		  char* message = "The amount should be a positive number.";
		  printf("%s",message);
    	return;
	  }
    user1->wallet+=transaction->amount;
    printf("Deposit Success!\n");
    free(transaction);
}

void withdraw(){
		struct Transaction *transaction = malloc(sizeof(struct Transaction));

		printf("Enter username: ");
    fgets(transaction->sender, 64, stdin);
    transaction->sender[strcspn(transaction->sender, "\n")] = '\0';
    
    printf("Enter amount of $: ");
    scanf("%d", &transaction->amount);
		getchar();
    
    printf("Enter PIN: ");
    scanf("%d", &transaction->confirmedPIN);
		getchar();

    struct User * user1 = searchuser(transaction->sender);

    // See if the transaction should fail
	  // See if both users exist
	  if (user1 == NULL) {
   		char* message = "One or both users do not exist.";
      printf("%s",message);
    	return;
	  }
	  // See if user1's pin is right
	  if (transaction->confirmedPIN != user1->PIN) {
    	char* message = "Incorrect PIN.";
      printf("%s",message);
    	return;
	  }
    // Make sure user1's bank account has enough money
	  if (user1->wallet < transaction->amount) {
    	char* message = "Insufficient funds.";
		  printf("%s",message);
    	return;
	  }
	  // Make sure the transaction.amount is >= 0
	  if (transaction->amount < 0) {
		  char* message = "The amount should be a positive number.";
		  printf("%s",message);
    	return;
	  }
    user1->wallet-=transaction->amount;
    printf("Withdraw Success!\n");
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

  // now let's log the change
  char * log_message;
  asprintf(&log_message, "user.c: User %s updated\n", username);
  log(log_message);
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

	// get the pin
  int pin;
	printf("Enter PIN: ");
  scanf("%d", &pin);
  getchar();
  
  // search for the user
  struct User * user = searchuser(username);
  if (user == NULL) {
    printf("User not found\n");
    return;
  }

  // check if the PIN is correct
  if (pin == user->PIN) {
    // print the user's info
    printf("Name: %s\n", user->name);
    printf("Wallet: $%d\n", user->wallet);
  } else {
    printf("Incorrect PIN\n");
  }

  // free the memory
  free(user);

  // log the action
  char * log_message;
  asprintf(&log_message, "user.c: User %s info requested\n", username);
  log(log_message);
}
