#include "user.h"
#include "bank.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <semaphore.h>
#include <errno.h>
#include <string.h>

// signal handler
void sigint_handler(int sig) {
	char *filename = "bankPipe"; 

    // Attempt to delete the file
    if (remove(filename) == 0) {
        printf("File deleted successfully.\n");
    } else {
        printf("Error: Unable to delete the file.\n");
    }

	sem_unlink(SEM_NAME);
	exit(0);
}

void makeTransaction(struct Transaction transaction, struct User * user1, struct User * user2) {
		user1->wallet -= transaction.amount;
		user2->wallet += transaction.amount;
		// changeUser(char* username, struct User * userToChange)
		changeUser(transaction.sender, user1);
		changeUser(transaction.receiver, user2);

		sleep(20);
}

// get the transaction using a pipe
void getTransaction(sem_t *sem) {
	// this is the waiting for the semaphore
	sem_wait(sem);

	int fd = open(PIPE_NAME, O_RDONLY);
	if (fd < 0) {
		perror("error with the pipe");
		// increment the semaphore to allow the next transaction to go through
		sem_post(sem);
		return;
	}

	struct Transaction transaction;
	ssize_t bytes_read = read(fd, &transaction, sizeof(struct Transaction));
	close(fd);

	if (bytes_read != sizeof(struct Transaction)) {
		perror("error with the bytes");
		sem_post(sem);
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
		free(user1);
		free(user2);
		sem_post(sem);
		return;
	}
	// See if user1's pin is right
	if (transaction.confirmedPIN != user1->PIN) {
		free(user1);
		free(user2);
		sem_post(sem);
		return;
	}
	// Make sure user1's bank account has enough money
	if (user1->wallet < transaction.amount) {
		free(user1);
		free(user2);
		sem_post(sem);
		return;
	}

	// At this point, if we are still here, that means that we are good and can send the money
	sleep(20);
	makeTransaction(transaction, user1, user2);

	free(user1);
	free(user2);
	sem_post(sem);
}

// in the future, for every 10 transactions, we will write to the disk; also when the file is closed
int main() {
	signal(SIGINT, sigint_handler);

	// create the semaphore
	sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);
	if (sem == SEM_FAILED) {
		perror("sem_open");
		exit(1);
	}

	if (mkfifo(PIPE_NAME, 0644) == -1) {
		perror("did not open");
		sem_close(sem);
		sem_unlink(SEM_NAME);
		exit(1);
	}

  	printf("Created a new named pipe %s\n", PIPE_NAME);

	while (1) {
		getTransaction(sem);
	}

	sem_close(sem);
	sem_unlink(SEM_NAME);
}
