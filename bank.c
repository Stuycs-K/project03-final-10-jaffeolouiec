#include "user.h"
#include <stdio.h>
#include <stdlib.h>

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

	if (pipe(fd) == -1) {
		perror("did not open");
		exit(1);
	}
}
