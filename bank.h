#ifndef BANK_H
#define BANK_H 
#define SEM_NAME "/bank_semaphore"
void sigint_handler(int sig);
void getTransaction();
void makeTransaction(struct Transaction transaction, struct User * user1, struct User * user2);
#endif
