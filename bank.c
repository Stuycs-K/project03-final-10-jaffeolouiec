#include "user.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

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

void makeTransaction(struct Transaction transaction, struct User * user1, struct User * user2) {
		user1->wallet -= transaction.amount;
		user2->wallet += transaction.amount;
		// changeUser(char* username, struct User * userToChange)
		changeUser(transaction.sender, user1);
		changeUser(transaction.receiver, user2);
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

	struct User * user1 = searchuser(transaction.sender);
	struct User * user2 = searchuser(transaction.receiver);

	// See if the transaction should fail
	// See if both users exist
	if (user1 == NULL || user2 == NULL) {
   		char* message = "One or both users do not exist.";
    	write(fd, message, strlen(message));
    	return;
	}
	// See if user1's pin is right
	if (transaction.confirmedPIN != user1->PIN) {
    	char* message = "Incorrect PIN.";
    	write(fd, message, strlen(message));
    	return;
	}
	// Make sure user1's bank account has enough money
	if (user1->wallet < transaction.amount) {
    	char* message = "Insufficient funds.";
		printf("%s",message);
    	write(fd, message, strlen(message)); // Write the error message
    	return;
	}
	printf("Successful Transaction");

	// At this point, if we are still here, that means that we are good and can send the money
	makeTransaction(transaction, user1, user2);
	char* message = "Transaction Successful.";
	write(fd, message, strlen(message)); // Write the success message
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
