#ifndef BANK_H
#define BANK_H 
void getTransaction(int fd, struct Transaction * transaction);
void saveEverything();
void loadBankAccounts();
void updateBankAccounts(struct Transaction transaction);
#endif
