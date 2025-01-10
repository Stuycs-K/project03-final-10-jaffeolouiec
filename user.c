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
    struct Transaction transaction;
    printf("Enter username: ");
    fgets(transaction.sender, 64, stdin);
    printf("Enter receiver's username: ");
    fgets(transaction.receiver, 64, stdin);
    printf("Enter amount of $: ");
    scanf("%d", &transaction.amount);
    printf("Enter PIN: ");
    scanf("%d", &transaction.confirmedPIN);
    int file = open(PIPE_NAME, O_WRONLY);
    if (file < 0){
        printf("Pipe Opening Error\n");
        return;
    }
    if (write(file, &transaction, sizeof(struct Transaction)) != sizeof(struct Transaction)){
        printf("Transaction Send Error\n");
        close(file);
        return;
    }
    close(file);
}

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

  }
  if (username){
    printf("Invalid username.\n");
    free(entries);
    return;
  }
  //Data
  printf("Enter year pop boro: ");
  scanf("%d %d %[^\n]",&entries[index - 1].year,&entries[index - 1].population,entries[index - 1].boro);
  int w_file = open(USER_FILE, O_WRONLY);
  close(w_file);
  free(entries);
  printf("File updated successfully.\n");
}
