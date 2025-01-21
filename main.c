#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include "user.h"

void printError() {
	printf("~~~~~~~~~~~~~~~~~~~~\nWelcome to the bank. First, in another terminal, run ./bank. Then, enter ./user Create, ./user Transaction, ./user Deposit, ./user Withdraw, or ./user Info below to get started. Each of them does the following:\n1.   'Create' makes a user.\n2.   'Transaction' lets you give money to another user.\n3.   'Deposit' lets you deposit money.\n4.   'Withdraw' lets you withdraw money.\n5.   'Info' gives you the information for one user.\n~~~~~~~~~~~~~~~~~~~~\n");
}

int main(int argc, char* argv[]){
  if (argc == 2){
    if (strcmp(argv[1],"Create")==0){
      createUser();
    }
    else if (strcmp(argv[1],"Transaction")==0){
      transaction();
    }
    else if (strcmp(argv[1],"Info")==0){
      getInfo();
    }
    else if (strcmp(argv[1],"Deposit")==0){
      deposit();
    }
    else if (strcmp(argv[1],"Withdraw")==0){
      withdraw();
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
