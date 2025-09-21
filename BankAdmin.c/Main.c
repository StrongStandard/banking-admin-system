#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <CommonCrypto/CommonDigest.h>
#include "BankPrototypes.h"


// Main function
int main(){
    
BankAccount accounts[MAX_ACCOUNTS];
int numAccounts = loadAccounts(accounts, MAX_ACCOUNTS);
int accountNumber = 0;
char option = 0; 
bool validInput = false; 
char name[NAME_LENGTH] = ""; 
char password[PASSWORD_LENGTH] = ""; 
char passwordCheck[PASSWORD_LENGTH] = "";
char inputPassword[PASSWORD_LENGTH];

// global options menu
while (true)
{  
    validInput = false;
    while (validInput == false)
    {
        printf("\nWelcome in the Strong Bank! How can we help you today ?\n" 
        "\n"
        "Select an option:\n" 
        "\n"
        "c - create an account\n"
        "l - login to existing account\n"
        "e - end\n");


        option = tolower(getchar());

        if (option == 'c' || option == 'l')
        {
            validInput = true;
        }
        else if (option == 'e')
        {
            saveAccounts(accounts, numAccounts); 
            return 0;
        }
        else
        {
            printf("**************\n"
                "Invalid input: %c!\n"
                "**************\n", option);
        }
    } 

    // Initialise actions taken for each option
    if (option == 'c')
    {   
        // create new bank account
        const int MAX_ACCOUNT_NUMBER = 999999;       
        const int MIN_ACCOUNT_NUMBER = 100000;
        
        get_user_name(name);

        // Get account number
        srand(time(0));
        accountNumber = (rand() % MAX_ACCOUNT_NUMBER) + MIN_ACCOUNT_NUMBER;
        // Get password from user

        get_password_from_user(password, passwordCheck);
        int account_index = numAccounts; // index to keep track of current accounts

        // Hash the password using SHA-256
        sha256_hash(password, accounts[account_index].passwordHash);
                    
        createAccount(&accounts[account_index], accountNumber, name, 0.00, accounts[account_index].passwordHash);
        numAccounts++;     
                        
        saveAccounts(accounts, numAccounts); 

        printf("\nAccount was succesfuly created!\n"
            "\n"
            "Account holder: %s \n"
            "Account number: %i\n"
            "Balance: %.2f\n"
            "\nPress enter to continue.", accounts[account_index].accountHolder, accounts[account_index].accountNumber, accounts[account_index].balance);
        getchar();
    }

    // log in option menu 
    else if (option == 'l')
    {  
        int matching_account_counter = get_matching_account(accounts, name, numAccounts);
        if (matching_account_counter != -1)      
        {   
            BankAccount *account = &accounts[matching_account_counter];
            //printf("Type your password for account %d ", account->accountNumber);
            validInput = false;
            while(validInput == false)
            {
                if(checkPassword(account,inputPassword))
                {
                    validInput = true;
                }
                else
                {
                    printf("Invalid passsword! Try again. \n");
                    validInput = false;
                    getchar();
                }
            }
            
            option = '\0';
            while (option != 'x')
            {
                validInput = false;
                while (validInput == false)
                {   
                    // user menu  
                    printf("\nWelcome back %s, what would you like to do? \n"
                    "\nw - withdraw\n" 
                    "d - deposit\n"
                    "b - check balance\n" 
                    "x - exit\n", accounts[matching_account_counter].accountHolder);

                    scanf("%1c", &option);      
                    option = tolower(option);
                        

                    if (option == 'w' || option == 'd' || option == 'b' || option == 'x')
                    {
                        validInput = true; 
                    }
                    else
                    {
                            validInput = false;
                    }
                }

                if(option == 'w')
                {
                    withdraw(account);
                }
                else if(option == 'd')
                {
                    deposit(account);
                }
                else if(option == 'b')
                {
                    checkBalance(account);
                }
        
            }                 
        }
    }
} 

return 0;
}
