// Maximum number of accounts 
#define MAX_ACCOUNTS 100 
#define PASSWORD_LENGTH 15
#define NAME_LENGTH 25

// Define a structure to represent a bank account with it's attributes
typedef struct 
{
    int accountNumber;
    char accountHolder[25];
    float balance;
    unsigned char passwordHash[CC_SHA256_DIGEST_LENGTH];
} BankAccount;
                                  
// prototypes
void createAccount(BankAccount *account, int accountNumber, char accountHolder[], float initialBalance, unsigned char password[]);
void deposit(BankAccount *account);
void withdraw(BankAccount *account);
void checkBalance(BankAccount *account);
bool checkPassword(BankAccount *account,char inputPassword[PASSWORD_LENGTH]);
bool checkUser(BankAccount *account, char name[NAME_LENGTH]);
void saveAccounts(BankAccount *account, int numAccounts);
int loadAccounts(BankAccount *accounts, int maxAccounts);
void get_password_from_user(char password[PASSWORD_LENGTH],char passwordCheck[PASSWORD_LENGTH]);
void get_user_name(char name[NAME_LENGTH]);
int get_matching_account(BankAccount *accounts, char name[NAME_LENGTH], int numAccounts);
void sha256_hash(const char *password, unsigned char hash[CC_SHA256_DIGEST_LENGTH]);
