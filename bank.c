#include "user.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

// signal handler
void sigint_handler(int sig) {
		char *filename = "bankPipe"; 

    // Attempt to delete the file
    if (remove(filename) == 0) {
        printf("File deleted successfully.\n");
    } else {
        printf("Error: Unable to delete the file.\n");
    }
}

// over here we need to save everything to a file
void saveEverything() {

}

// get the bank account information from the file
void loadBankAccounts() {

}

void updateBankAccounts(struct Transaction transaction) {
	
}

// get the transaction using a pipe
void getTransaction(int fd[2], struct Transaction * transaction) {
	if (open(PIPE_NAME, O_RDONLY) == -1) {
		perror("failed to open pipe");
		exit(1);
	}	

	printf("getTransaction - before bytes_read\n");

	ssize_t bytes_read = read(fd, &transaction, sizeof(struct Transaction));

	printf("got the transaction\n");
	printf("Sender: %s\n", transaction->sender);
	printf("Receiver: %s\n", transaction->receiver);
	printf("Amount: $%d\n", transaction->amount);
	printf("PIN: %d\n", transaction->confirmedPIN);

	close(fd);
}

// in the future, for every 10 transactions, we will write to the disk; also when the file is closed
int main() {
	signal(SIGINT, sigint_handler);

	int fd; 
  struct Transaction * transaction;
  transaction = (struct transaction *) malloc(sizeof(struct Transaction));
  
  if (mkfifo(PIPE_NAME, 0644) == -1) {
    perror("did not open");
    exit(1);
  }

  printf("Created a new named pipe %s\n", PIPE_NAME);

	while (1) {
		getTransaction(fd, transaction);
	}
}
