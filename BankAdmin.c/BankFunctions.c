#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <CommonCrypto/CommonDigest.h>

// Maximum number of accounts 
    #define MAXaccounts 100        // Paul: Probably more standard to use a #define for a constant. Using a variable suggests the value can change. 
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
    scanf("%f", &newBalance);       
    getchar();
    account -> balance += newBalance;
    printf("\nDeposit was succesful, new balance is %.2f£. Press ENTER to continue. \n", account -> balance);
}

// Function to withdraw money

void withdraw(BankAccount *account)
{
    
    float amount = 0.00;
    printf("How much would you like to withdraw? \n");
    scanf("%f", &amount);      
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

bool checkPassword(BankAccount *account,char inputPassword[PASSWORD_LENGTH])
{

    //printf("Type your password for account %d ", account->accountNumber);
    char *input_temp_password = getpass("Type your password for account: ");
    memcpy(inputPassword,input_temp_password,PASSWORD_LENGTH);
    //scanf("%s", inputPassword);

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
    FILE *file = fopen("/Users/martincerveny/BankAdmin/accounts.txt", "w");

    if (file == NULL)
    {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < numAccounts; i++)
    {
        fprintf(file, "%d,%s,%.2f,", account[i].accountNumber, account[i].accountHolder, account[i].balance);


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
    FILE *file = fopen("/Users/martincerveny/BankAdmin/accounts.txt", "r");

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
   int Spaces = 0;
            do
            {
                InvalidcharacterCounter = 0;
                Spaces = 0;
                // Get account name
                printf("Enter your full name: ");
                fgets(name, NAME_LENGTH - 1, stdin);
                name[strlen(name) - 1] = '\0';
                for (int l = 0; l < strlen(name); l++)
                {                                      
                    name[l] = tolower(name[l]);
                    // printf("%c",name[l]);
                    if(!isalpha(name[l]) && name[l] != ' ')
                    {
                        InvalidcharacterCounter++;
                    }
                    if(name[l] == ' ')
                    {
                            Spaces++;
                    }
                    if(Spaces > 1)
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

            printf("Enter account holder: ");
            fgets(name, NAME_LENGTH - 1, stdin);
            name[strcspn(name, "\n")] = '\0';

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
            printf("%s/n",accounts[j].accountHolder);
            }
            
            printf("%s/n",name);
            printf("User not found! Try again.\n");

            return -1; 

}

// Function to get password from user

void get_password_from_user(char password[PASSWORD_LENGTH],char passwordCheck[PASSWORD_LENGTH])
{
    bool validInput = false;
        do
        {    // Get account password
            char *password_temp = getpass("Create new password: ");
            memcpy(password,password_temp,PASSWORD_LENGTH);
     
            bool is_at_least_one_uppercase = false ; 
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
         
            char *password_temp2 = getpass("Confirm the password: ");
            memcpy(passwordCheck, password_temp2, PASSWORD_LENGTH);

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
