#define USER_FILE "bankData.dat"
#define PIPE_NAME "bankPipe"

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
void createUser();
void transaction();
