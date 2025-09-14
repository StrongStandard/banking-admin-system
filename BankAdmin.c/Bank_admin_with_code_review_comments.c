#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <CommonCrypto/CommonDigest.h>
#include "BankFunctions.c"


// Main function
int main()
{
    BankAccount accounts[MAXaccounts];
    int numAccounts = loadAccounts(accounts, MAXaccounts);
    int account_counter = 0;      
    int accountNumber = 0;
    char option = 0; 
    bool validInput = false; 
    char name[NAME_LENGTH] = ""; 
    char password[PASSWORD_LENGTH] = ""; 
    char passwordCheck[PASSWORD_LENGTH] = "";
    char inputPassword[PASSWORD_LENGTH];

    // global options menu
    do
    {  
        do
        {
            printf("\nWelcome in the Strong Bank! How can we help you today ?\n" 
            "\n"
            "Select an option:\n" 
            "\n"
            "c - create an account\n"
            "l - login to existing account\n"
            "e - end\n");

        scanf("%1c", &option);
        option = tolower(option);
        getchar();      

        if(option == 'c' || option == 'l' || option == 'e')
        {
            validInput = true;
        }
        else
        {
            printf("**************\n" 
                "Invalid input!\n"
                "**************\n");
        }
        } while (validInput == false);

        // Initialise actions taken for each option
        if(option == 'c' )
        {   // create new bank account
            const int MAX_ACCOUNT_NUMBER = 999999;       
            const int MIN_ACCOUNT_NUMBER = 100000;
            


            get_user_name(name);

            // Get account number
            srand(time(0));
            accountNumber = (rand() % MAX_ACCOUNT_NUMBER) + MIN_ACCOUNT_NUMBER;
            // Get password from user

            get_password_from_user(password,passwordCheck);
            int account_index = numAccounts; // index to keep track of current accounts

            // Hash the password using SHA-256
            sha256_hash(password, accounts[account_index].passwordHash);
            
                                                                       
                      
            getchar();
            createAccount(&accounts[account_index], accountNumber, name, 0.00, accounts[account_index].passwordHash);
            numAccounts++;     
                            

            saveAccounts(accounts, numAccounts); 

            printf("\nAccount was succesfuly created!\n"
                "\n"
                "Account holder: %s \n"
                "Account number: %i\n"
                "Balance: %.2f\n"
                "\nPress enter to continue.", accounts[account_index].accountHolder, accounts[account_index].accountNumber, accounts[account_index].balance);
    
        }

        // log in option menu 
        else if(option == 'l')
        
        {  
             
                account_counter = get_matching_account(accounts, name, numAccounts);

                if(account_counter != -1)      
                {   
                    BankAccount *account = &accounts[account_counter];
                    printf("Type your password for account %d ", account->accountNumber);
                        do
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

                        } while(validInput == false);
                        
                            do
                            {
                                    do
                                    {   // user menu  
                                        getchar();
                                        printf("\nWelcome back %s, what would you like to do? \n"
                                        "\nw - withdraw\n" 
                                        "d - deposit\n"
                                        "b - check balance\n" 
                                        "x - exit\n", accounts[account_counter].accountHolder);

                                        scanf("%1c", &option);      
                                        option = tolower(option);
                                            

                                        if(option == 'w' || option == 'd' || option == 'b' || option == 'x')
                                        {
                                            validInput = true; 
                                        }
                                        else
                                        {
                                             validInput = false;
                                        }

                                    } while (validInput == false);

                                    if(option == 'w')
                                    {
                                        withdraw(account);
                                    }

                                    if(option == 'd')
                                    {
                                        deposit(account);
                                    }
                                    if(option == 'b')
                                    {
                                        checkBalance(account);
                                    }
                        
                            } while (option != 'x');                   
                }
        }
        getchar();
    } while(option != 'e');
    saveAccounts(accounts, numAccounts); 

    return 0;
}
