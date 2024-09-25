#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <CommonCrypto/CommonDigest.h>

// Maximum number of accounts 
    #define MAXaccounts 100        // Paul: Probably more standard to use a #define for a constant. Using a variable suggests the value can change. 
    #define PASSWORD_LENGTH 32
    #define NAME_LENGTH 25

// Define a structure to represent a bank account with it's attributes
    typedef struct 
    {
        int accountNumber;
        char accountHolder[25];
        float balance;
        unsigned char passwordHash[CC_SHA256_DIGEST_LENGTH];
    } BankAccount;                              // Paul:    Really you are defining a new type here and it makes the code neater to do that.
                                    //          For example:
                                    //                          typedef struct 
                                    //                          {
                                    //                              int accountNumber;
                                    //                              char accountHolder[25];
                                    //                              float balance; 
                                    //                              unsigned char passwordHash[CC_SHA256_DIGEST_LENGTH];
                                    //                          } BankAccount; 
                                    // 
                                    // Now you can use BankAccount as a variable type:
                                    // e.g. BankAccount new_account;
// prototypes
void createAccount(BankAccount *account, int accountNumber, char accountHolder[], float initialBalance, unsigned char password[]);
void deposit(BankAccount *account);
void withdraw(BankAccount *account);
void checkBalance(BankAccount *account);
bool checkPassword(BankAccount *account);
bool checkUser(BankAccount *account, char name[NAME_LENGTH]);
void saveAccounts(BankAccount *account, int numAccounts);
int loadAccounts(BankAccount *accounts, int maxAccounts);
void get_password_from_user(char password[PASSWORD_LENGTH]);
void get_user_name(char name[NAME_LENGTH]);
int get_matching_account(BankAccount *accounts, char name[NAME_LENGTH], int numAccounts);
void sha256_hash(const char *password, unsigned char hash[CC_SHA256_DIGEST_LENGTH]);



// Function to create a new bank account

void createAccount(BankAccount *account, int accountNumber, char accountHolder[], float initialBalance, unsigned char password[])
{
    account -> accountNumber = accountNumber; // 'account' represnts BankAccount, '->' represnets pointer to data attribute 'accountNumber' in data structure BankAccount 
    strcpy(account -> accountHolder, accountHolder);
    account -> balance = initialBalance;
    memcpy(account->passwordHash, password, CC_SHA256_DIGEST_LENGTH);

}

// Function to deposit money

void deposit(BankAccount *account)
{
    float newBalance = 0.00;
    printf("How much would you like to deposit? \n");
    scanf("%f", &newBalance);       // Paul: Perhaps check the return value is 1
    getchar();
    account -> balance += newBalance;
    printf("\nDeposit was succesful, new balance is %.2f£. Press ENTER to continue. \n", account -> balance);
}

// Function to withdraw money

void withdraw(BankAccount *account)
{
    
    float amount = 0.00;
    printf("How much would you like to withdraw? \n");
    scanf("%f", &amount);       // Paul: Perhaps check the return value is 1
    getchar();
    if(amount <= account -> balance)
    {
       account -> balance -= amount;
       printf("\n\nWithdrawal was succesful, new balance is %.2f£. Press ENTER to continue. \n", account -> balance); 
    }
    else
    {
        printf("\nInsuficient Funds! Press ENTER to continue. \n");
    }
    
}
// Function to check balance

void checkBalance(BankAccount *account)
{
    printf("\nCurrent balance for %s is: %.2f£. Press ENTER to continue. \n", account->accountHolder, account->balance);
    getchar();
}


// Function to check if the password is correct

bool checkPassword(BankAccount *account)
{
    char inputPassword[32];
    printf("Type your password for account: %d: ", account->accountNumber);
    scanf("%s", inputPassword);

    // Hash the input password using SHA-256

    unsigned char inputHash[CC_SHA256_DIGEST_LENGTH];
    sha256_hash(inputPassword, inputHash);

    // Compare the hashed input password with the stored hashed password

    if (memcmp(inputHash, account->passwordHash, CC_SHA256_DIGEST_LENGTH) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// function to save accounts

void saveAccounts(BankAccount *account, int numAccounts)
{
    FILE *file = fopen("/Users/martincerveny/Desktop/VScode/C_Programming/Projects.c/accounts.txt", "w");

    if (file == NULL)
    {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < numAccounts; i++)
    {
        fprintf(file, "%d %s %.2f ", account[i].accountNumber, account[i].accountHolder, account[i].balance);


        // Save the hashed password as hexadecimal in the file

        for (int j = 0; j < CC_SHA256_DIGEST_LENGTH; j++)
        {
            fprintf(file, "%02x", account[i].passwordHash[j]);
        }

        fprintf(file, "\n");
    }

    fclose(file);
}

// Function to load accounts

int loadAccounts(BankAccount *accounts, int maxAccounts)
{
    FILE *file = fopen("/Users/martincerveny/Desktop/VScode/C_Programming/Projects.c/accounts.txt", "r");

    if (file == NULL)
    {
        printf("Error opening file for reading.\n");
        return 0;
    }

    int numAccounts = 0;
    char line[256];

    while (fscanf(file, "%d", &accounts[numAccounts].accountNumber) == 1)
    {
        fscanf(file, " %24[^0-9] %f ", accounts[numAccounts].accountHolder, &accounts[numAccounts].balance);

        // Read the hashed password as hexadecimal from the file

        for (int j = 0; j < CC_SHA256_DIGEST_LENGTH; j++)
        {
            int scanned = 0;
            fscanf(file, "%02x", &scanned);
            accounts[numAccounts].passwordHash[j] = (unsigned char)scanned;
        }

        // Move to the next line

        fscanf(file, "\n");

        numAccounts++;

        if (numAccounts >= maxAccounts)
        {
            printf("Reached the maximum number of accounts. Some data may be ignored.\n");
            break;
        }
    }

    fclose(file);

    return numAccounts;
}

// Function to get name from a user

void get_user_name(char name[NAME_LENGTH])
{
   int InvalidcharacterCounter = 0;
            do
            {
                InvalidcharacterCounter = 0;
                // Get account name
                printf("Enter your full name: ");
                 
                fgets(name, NAME_LENGTH - 1, stdin); 
                for (int l = 0; l < strlen(name); l++)
                {                                      
                    name[l] = tolower(name[l]);
                    
                    if(!isalpha(name[l]) && name[l] != ' ')
                    {
                        InvalidcharacterCounter++;
                    }  
                                                  
                }  
                if(InvalidcharacterCounter > 0)
                {
                    printf("Invalid characters! Try again. \n");
                    printf("%i\n", InvalidcharacterCounter);
                }  
             
            }while(InvalidcharacterCounter > 0);
}

// Function to check if account name is matching

int get_matching_account(BankAccount *accounts, char name[NAME_LENGTH], int numAccounts)
{

          // Paul: This should be a bool and perhaps "account_found" is a better variable name
            
            
            printf("Enter account holder: ");
            fgets(name, NAME_LENGTH - 1, stdin);
            name[strcspn(name, "\n")] = '\0'; /////////

            for (int i = 0; i < strlen(name); i++)
            {
                name[i] = tolower(name[i]);
            }
                    
                    
            for(int j = 0; j < numAccounts; j++)
            {
                        // If user exists 
                if(strcmp(name, accounts[j].accountHolder) == 0)
                {
                    return j;
                }
            }
            
                
            printf("User not found! Try again.\n");

            return -1; 

}

// Function to get password from user

void get_password_from_user(char password[PASSWORD_LENGTH])
{
    bool validInput = false;
    char passwordCheck[PASSWORD_LENGTH] = "";
        do
        {    // Get account password
            printf("Create new password: ");
            scanf("%s", password);

            bool is_at_least_one_uppercase = false ;    // Paul: It makes more sense for this to be a bool type. Also "is_at_least_one_uppercase" is a better name. At first I thought it meant is the whole string uppercase. 
            for(int p = 0; p < strlen(password); p++)
            {

                if(isupper(password[p]))
                {
                    is_at_least_one_uppercase = true;
                }
            }
            if(strlen(password) < 10 || strlen(password) > 15 || is_at_least_one_uppercase == false)
            {
                getchar();
                printf("\nThe length of a password is 10-15 characters, at least one uppercase and a digit! \n"
                           "\n");
            }
            else
            {
                    validInput = true;
            }
        } while(validInput == false);
        do
        {
            printf("Confirm the password: ");
            scanf("%s", passwordCheck);

        } while(strcmp(passwordCheck, password) != 0);

}
// Function to hash a password using SHA-256

void sha256_hash(const char *password, unsigned char hash[CC_SHA256_DIGEST_LENGTH]) 
{
    CC_SHA256_CTX sha256;
    CC_SHA256_Init(&sha256);
    CC_SHA256_Update(&sha256, password, strlen(password));
    CC_SHA256_Final(hash, &sha256);
}