#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include "user.h"

void printError() {
	printf("~~~~~~~~~~~~~~~~~~~~\nWelcome to the bank. First, in another terminal, run ./bank. Then, enter ./user Create, ./user Transaction, or ./user Info below to get started. Each of them does the following:\n1.   'Create' makes a user.\n2.   'Transaction' lets you give money to another user.\n3.   'Info' gives you the information for one user.\n~~~~~~~~~~~~~~~~~~~~\n");
}

int main(int argc, char* argv[]){
  if (argc == 2){
    if (strcmp(argv[1],"Create")==0){
      createUser();
    }
    else if (strcmp(argv[1],"Transaction")==0){
      transaction();
    }
    // get a user's info
    else if (strcmp(argv[1],"Info")==0){
      getInfo();
    } 
    else{
	printError();
    }
  }
  else{
	printError();
  }
  return 0;
}
