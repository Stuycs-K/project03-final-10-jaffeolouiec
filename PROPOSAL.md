# Blockchain/Cryptocurrency Project Proposal

## 1. Group Members

1. **Chris**
2. **Otzar**

## 2. Statement of the Problem / High-Level Description

We propose to build a **simplified blockchain application** that demonstrates the functionality and concepts of cryptocurrency. Our blockchain project will:

- Store blocks of data in a chain in memory and on disk
- Simulate mining with multiple miners (proof-of-work) to mine new blocks
- Simulate transactions between “users” with public/private key pairs

We aim to create an **MVP (Minimum Viable Product)** quickly, focusing on a basic blockchain (reading/writing blocks and verifying transactions).

## 3. User Interface / Usage

1. **User Generation**  
   A user will run `./create_user` to generate their personal public/private key pair.

2. **Transactions**  
   To send a transaction, a user would run `./client` with these args:
   - Their private key
   - The other user’s public key
   - The amount  
   The transaction will be signed by the user’s private key and then be sent to “/miner”

3. **Mining**  
   The miner process, run with `./miner`, will listen for transactions, gather them, and then perform the proof-of-work. If the block is valid, it will be written to memory.

4. **Shutdown**  
   When the miner process receives the `SIGINT` signal, it will write the data to disk.

## 4. Technical Design

### 4.1 Using the Topics Covered in Class

1. **Allocating Memory**  
   We will allocate memory for blocks, the blockchain, and lists of transactions.  
   We may use shared memory and use a queue to process transactions.

2. **Files**  
   The blockchain will be stored on `blockchain.dat`  
   On startup, the blockchain will be read into memory  
   After mining each block, we will append to `blockchain.dat`

3. **Information about Files**  
   We can use `stat()` to find the size of the ledger.

4. **Processes**  
   We plan to have two types of processes:
   - **Miner Process**: Continually runs, listens for transactions and verifies them, and mines blocks.
   - **Client Process**: Sends transactions through a socket  
   We can implement multiple miner processes to simulate a decentralized network

5. **Signals**  
   The miner process will catch `SIGINT` and do this before shutdown:
   - Finish mining the current block
   - Write data to `blockchain.dat`
   - Close the socket

6. **Shared Memory**  
   We will use shared memory to hold a queue of incoming transactions.  
   All miners will confirm the work.

7. **Semaphores**  
   This will synchronize access to the transaction pool to handle multiple miners.

8. **Pipes/Sockets**  
   Clients will send their transactions to the miners through pipes or sockets. The transactions will be added to the queue.

### 4.2 Division of Responsibilities (Subject to Change)

**Otzar**

- Blockchain data structure
- Hashing function
- Proof of work for mining/verification
- Reading/writing the chain
- Signal handlers

**Chris**

- Setting up named pipe to wait for transactions 
- Client program that sends transactions
- Semaphores to make sure one thing is done at once
- Reading transactions and sending them to the mining logic
- User Management

**Collaborative Tasks**

- Testing
- Documentation
- Implementing advanced features

### 4.3 Algorithms / Data Structures

We will be using hashing algorithms using a library. We will need to create public and private keys for users, as well as creating a signature for the private keys. We will use a struct that will store the blockchain.

```c
struct Transaction {
    char sender[64];
    char receiver[64];
    int amount;
    char signature[?];
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

struct Blockchain {
    Block * head;
    Block * curr;
    int len;
}

```

### 5. Timeline with Milestones

- **Jan 5**: Finish proposal
- **Jan 6**: Repo available; Set up simple block structures; Implement file I/O
- **Jan 7**: Develop `./create_user`
- **Jan 8**: Begin coding `./client` to accept transactions; hashing functions
- **Jan 9**: Proof-of-work (finding valid nonces) and the basic structure of `./miner`
- **Jan 10**: Shared memory for transaction pool
- **Jan 11**: Named pipes for client to miner communication
- **Jan 12**: Semaphore usage and finish `./client`
- **Jan 13**: Optimize mining logic and checks for transactions
- **Jan 14**: Signal Handling
- **Jan 15**: Test multiple miners
- **Jan 16**: Block broadcasting and resolving conflicts
- **Jan 17**: Multiple transactions per block
- **Jan 18**: Extensive testing
- **Jan 19**: Create video (5-8 minutes)
- **Jan 20**: Finalize documentation
- **Jan 21**: Double check code. Project due at 8am



