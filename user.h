#define USER_FILE "bankData.dat"
#define PIPE_NAME "userToBankPipe"
#define BANK_TO_USER_PIPE "bankToUserPipe"
#define BANK_FILE "allBankData.dat"
#define LOG_FILE "log.txt"

struct User {
    char name[64];
    int PIN;
    int wallet;
};

struct Transaction {
    char sender[64];
    char receiver[64];
    int amount;
    int confirmedPIN;
};

struct User* searchuser(char* username);
void createUser();
void transaction();
void getInfo();
void changeUser(char* username, struct User * userToChange);
void log(char* message);
