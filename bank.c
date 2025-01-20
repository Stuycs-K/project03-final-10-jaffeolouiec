#include "user.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

// signal handler
void sigint_handler(int sig) {
	char *filename = PIPE_NAME;
	char *filename2 = BANK_TO_USER_PIPE;

    // Attempt to delete the file
    if (remove(filename) == 0) {
        printf("File 1 deleted successfully.\n");
    } else {
        printf("Error: Unable to delete the file.\n");
    }

	// Attempt to delete the file
	if (remove(filename2) == 0) {
		printf("File 2 deleted successfully.\n");
	} else {
		printf("Error: Unable to delete the file.\n");
	}

	exit(0);
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
	if (fd < 0) {
		perror("error opening the pipe");
		return;
	}

	printf("getTransaction - before bytes_read\n");

	struct Transaction transaction;
	ssize_t bytes_read = read(fd, &transaction, sizeof(struct Transaction));

	if (bytes_read != sizeof(struct Transaction)) {
		perror("error with the bytes");
		close(fd);
		return;
	}
	close(fd);

	printf("got the transaction\n");
	printf("Sender: %s\n", transaction.sender);
	printf("Receiver: %s\n", transaction.receiver);
	printf("Amount: $%d\n", transaction.amount);
	printf("PIN: %d\n", transaction.confirmedPIN);

	struct User * user1 = searchuser(transaction.sender);
	struct User * user2 = searchuser(transaction.receiver);

	// opening the other pipe
	int fd2 = open(BANK_TO_USER_PIPE, O_WRONLY);
	if (fd2 < 0) {
		perror("error opening the pipe");
		return;
	}

	// See if the transaction should fail
	// See if both users exist
	if (user1 == NULL || user2 == NULL) {
   		char* message = "One or both users do not exist.";
    	write(fd2, message, strlen(message));
		sleep(1);
    	return;
	}
	// See if user1's pin is right
	if (transaction.confirmedPIN != user1->PIN) {
    	char* message = "Incorrect PIN.";
    	write(fd2, message, strlen(message));
		sleep(1);
    	return;
	}
	// Make sure user1's bank account has enough money
	if (user1->wallet < transaction.amount) {
    	char* message = "Insufficient funds.";
		printf("%s",message);
    	write(fd2, message, strlen(message));
		sleep(1);
    	return;
	}
	printf("Successful Transaction\n");

	// At this point, if we are still here, that means that we are good and can send the money
	makeTransaction(transaction, user1, user2);
	char* message = "Transaction Successful.";
	write(fd2, message, strlen(message)); // Write the success message
	close(fd);
	close(fd2);
	sleep(1);
}

// in the future, for every 10 transactions, we will write to the disk; also when the file is closed
int main() {
	signal(SIGINT, sigint_handler);

	if (mkfifo(PIPE_NAME, 0644) == -1) {
		perror("did not open");
		exit(1);
	}

	printf("Created a new named pipe %s\n", PIPE_NAME);

	// create a pipe for the bank to send messages to the user
	if (mkfifo(BANK_TO_USER_PIPE, 0644) == -1) {
		perror("did not open");
		exit(1);
	}

	printf("Created a new named pipe %s\n", BANK_TO_USER_PIPE);

	while (1) {
		getTransaction();
	}

	return 0;
}
