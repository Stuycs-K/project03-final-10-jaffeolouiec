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
	int fd;

	if (mkfifo(PIPE_NAME, 0644) == -1) {
		perror("did not open");
		exit(1);
	}

	printf("Created a new named pipe %s\n", PIPE_NAME);

	if (open(PIPE_NAME, O_RDONLY) == -1) {
		perror("failed to open pipe");
		exit(1);
	}	

	ssize_t bytes_read = read(fd, &transaction, sizeof(struct Transaction));

	printf("got the transaction");
	printf("Sender: %s", transaction.sender);
	printf("Receiver: %s", transaction.receiver);
	printf("Amount: $%d\n", transaction.amount);
	printf("PIN: %d\n", transaction.confirmedPIN);
}
