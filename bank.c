#include "user.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// over here we need to save everything to a file
void saveEverything() {

}

// get the bank account information from the file
void loadBankAccounts() {

}

void updateBankAccounts() {

}

// get the transaction using a pipe
void getTransaction() {
	int fd[2];

	if (mkfifo(PIPE_NAME, 0644) == -1) {
		perror("did not open");
		exit(1);
	}

	printf("Created a new named pipe %s\n", PIPE_NAME);

	
}
