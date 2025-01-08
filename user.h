void createUser();
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
void create_user();
void transaction();