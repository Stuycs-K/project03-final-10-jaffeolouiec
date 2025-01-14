#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "user.h"

void createUser(){
    struct User user;
    printf("Enter new username: ");
    fgets(user.name, 64, stdin);
    printf("Enter a 4-digit PIN: ");
    scanf("%d", &user.PIN);
    if (user.PIN < 1000 || user.PIN > 9999){
        printf("Invalid PIN\n");
        return;
    }
    user.wallet = 0;
    int file = open(USER_FILE, O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (file < 0){
        printf("File Open Error\n");
        return;
    }
    if (write(file, &user, sizeof(struct User)) != sizeof(struct User)){
        printf("File Write Error\n");
        close(file);
        return;
    }
    close(file);
}

void transaction(){
		struct Transaction *transaction = malloc(sizeof(struct Transaction));   
 
		printf("Enter username: ");
    fgets(transaction->sender, 64, stdin);
    printf("Enter receiver's username: ");
    fgets(transaction->receiver, 64, stdin);
    printf("Enter amount of $: ");
    scanf("%d", &transaction->amount);
		getchar();
    printf("Enter PIN: ");
    scanf("%d", &transaction->confirmedPIN);
		getchar();

		//printf("Here are the vars: %s %s %d %d\n", transaction->sender, transaction->receiver, transaction->amount, transaction->confirmedPIN);

    int fd = open(PIPE_NAME, O_WRONLY);
    if (fd < 0){
        printf("Pipe Opening Error\n");
        return;
    }

    if (write(fd, transaction, sizeof(struct Transaction)) == -1) {
				printf("Transaction Send Error\n");
        return;
    } else {
				printf("success!\n");
		}

		close(fd);
		free(transaction);
}
