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
void getTransaction() {
	int fd = open(PIPE_NAME, O_RDONLY);
	printf("getTransaction - before bytes_read\n");

	struct Transaction transaction;
	ssize_t bytes_read = read(fd, &transaction, sizeof(struct Transaction));

	if (bytes_read != sizeof(struct Transaction)) {
		perror("error with the bytes");
		close(fd);
		return;
	}

	printf("got the transaction\n");
	printf("Sender: %s\n", transaction.sender);
	printf("Receiver: %s\n", transaction.receiver);
	printf("Amount: $%d\n", transaction.amount);
	printf("PIN: %d\n", transaction.confirmedPIN);

	struct User * user1 = searchUser(transaction.sender);
	struct User *	 user2 = searchUser(transaction.receiver);

	// See if the transaction should fail
	// See if both users exist
	if (user1 == NULL || user2 == NULL) {return;}
	// See if user1's pin is right
	if (transaction.confirmedPIN == user1.PIN) {return;}
	// Make sure user1's bank account has enough money
	if (user1.wallet <= transaction.amount) {return;}

	// At this point, if we are still here, that means that we are good and can send the money
	make_transaction(transaction, user1, user2);

	close(fd);
}

// in the future, for every 10 transactions, we will write to the disk; also when the file is closed
int main() {
	signal(SIGINT, sigint_handler);

  if (mkfifo(PIPE_NAME, 0644) == -1) {
    perror("did not open");
    exit(1);
  }

  printf("Created a new named pipe %s\n", PIPE_NAME);

	while (1) {
		getTransaction();
	}
}
