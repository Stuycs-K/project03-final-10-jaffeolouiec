# Bank Project Proposal

## 1. Group Members
1. Chris  
2. Otzar  

---

## 2. Statement of the Problem / High-Level Description
We propose to build a bank application that demonstrates the functionality and concepts of the banking system. Our bank project will simulate transactions between users.

---

## 3. User Interface / Usage
### 3.1 User Generation
A user will run `./create_user` to generate their account.

### 3.2 Transactions
To send a transaction, a user would run `./transaction` with these arguments:
- Their PIN  
- The other user’s account number  
- The amount  

### 3.3 Deposit
Enter PIN and amount to deposit money from the wallet.

### 3.4 Withdrawal
Enter PIN and amount to withdraw money into the wallet.

---

## 4. Technical Design

### 4.1 Using the Topics Covered in Class
1. **Allocating Memory**  
   - We will allocate memory for transactions and monetary amounts.  

2. **Files**  
   - File with the bank data.  

3. **Information About Files**  
   - We can use `stat()` to find the bank size.  

4. **Processes**  
   - We plan to have two types of processes:  
     - **Bank Process**: Continually runs, listens for transactions, and verifies them.  
     - **Transaction Process**: Sends transactions through a pipe.  

5. **Signals**  
   - The bank process will catch `SIGINT` and do the following before shutdown:  
     - Finish remaining transactions.  
     - Write data to a data file.  

6. **Semaphores**  
   - Restrict transactions to only one at a time.  

7. **Pipes**  
   - Transactions will send their data to the bank through pipes.

### 4.2 Division of Responsibilities (subject to change)

- **Otzar**  
  - Basic bank server  
  - Memory allocation  
  - Bank data file  

- **Chris**  
  - Create user  
  - User struct  
  - User transaction send  

- **After Completion of Above Tasks**:  
  - Transactions, semaphores, signals  

### 4.3 Algorithms / Data Structures

#### Struct Definitions

```c
struct User {
    char[64] name;
    int PIN;
    int wallet;
};

struct Transaction {
    char sender[64];
    char receiver[64];
    int amount;
    int confirmedPIN;
};
```

---

## 5. Timeline with Milestones

- **Jan 7**: Finish proposal  
- **Jan 8**: Repo available; Set up simple block structures; Implement file I/O  
- **Jan 9**: Develop `./create_user`  
- **Jan 10**: Begin coding `./transaction` to accept transactions  
- **Jan 11**: Basic structure of `./bank`  
- **Jan 12**: Named pipes for transaction-to-bank communication  
- **Jan 13**: Semaphore usage and finish `./bank`  
- **Jan 14**: Optimize logic and checks for transactions  
- **Jan 15**: Signal handling  
- **Jan 16**: Extensive testing  
- **Jan 17**: Create video (5-8 minutes)  
- **Jan 18**: Finalize documentation  
- **Jan 19**: Double-check code. Project due on **1/21 at 8 AM**.

