void mine();
int proofOfWork();
int getTransaction();
int shutdown();
void sigintHandler(int sig);
int saveBlockchain(struct Blockchain input);
struct Blockchain getBlockchain(); //Retrieves blockchain
int getLedgerSize(char* filename); //Use stat here
int sharedMemory(Struct transaction input); //Return nonzero if successful, zero if unsuccesful
int hash(char* bytes);

struct Blockchain {
	Block * head;
	Block * curr;
	int len;
}

struct Transaction {
	char sender[64];
	char receiver[64];
	int amount;
	char signature[256];
}

struct Block {
	int index;
	int timestamp;
	char prevHash;
	char currHash;
	int nonce;
	Transaction transactions[10];
	int tx_count;
}
