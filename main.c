#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include "user.h"

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
      printf("~~~~~~~~~~~~~~~~~~~~\nWelcome to the bank. Enter one of the ARGS='' below to get started:\n1.   'Create'\n2.   'Transaction'\n3.   'Info'~~~~~~~~~~~~~~~~~~~~\n");
    }
  }
  else{
    printf("~~~~~~~~~~~~~~~~~~~~\nWelcome to the bank. Enter one of the ARGS='' below to get started:\n1.   'Create'\n2.   'Transaction'\n~~~~~~~~~~~~~~~~~~~~\n");
  }
  return 0;
}
