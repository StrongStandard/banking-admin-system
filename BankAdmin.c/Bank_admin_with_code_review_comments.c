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
    // Paul:    You say "initialise variables" here but actually most of them are just declared.
    //          Uninitialised variables are the cause of a lot of bugs so often best to initialise everything.
    //          For example:
    //                          char name[25] = "";
    //                          char option = 0;
    // initialise variables
    BankAccount accounts[MAXaccounts];
    int numAccounts = loadAccounts(accounts, MAXaccounts);
    int account_counter = 0;      // Paul: Better to use a #define for the string length rather than a "magic number". Especially if you use it again later like you do for this length.
    int accountNumber = 0;
    char option = 0; // Martin: I found that with this variable, because it is selected based upon key pressed, some loops never get started and i keep getting error that variable is undeclared ?
    bool validInput = false; // Martin: same issue with this variable
    char name[NAME_LENGTH] = ""; // Martin: same here
    char password[PASSWORD_LENGTH] = ""; // Martin: same here

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
        getchar();      // Paul: Might be nice to add a comment as to why this is needed. e.g. Calling getchar() to wait for user to hit another key after making menu selection

        // Paul:    I'm not sure what version of C you are using, but the more recent ones allow for variables to be declared at any point (rather than just at the top of a function)
        //          This allows us to keep variable declarations "close" to variable usage which is a big advantage to code readability and bug reduction
        //          For example I would be putting: "bool validInput = false;" here 
        // repeat while wrong input
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
            const int MAX_ACCOUNT_NUMBER = 999999;       // Paul:    If MAX and MIN aren't changing they should be #defines or const int. Also all upper case names are used to indicate #defines so don't use them for variables
            const int MIN_ACCOUNT_NUMBER = 100000;
            //          Also "MAX" is not a good choice for a variable name. A better choice would be: 
                                    //const int max_account_number = 999999;


            get_user_name(name);

            // Get account number
            srand(time(0));
            accountNumber = (rand() % MAX_ACCOUNT_NUMBER) + MIN_ACCOUNT_NUMBER;
            // Get password from user

            get_password_from_user(password);
            int account_index = numAccounts; // index to keep track of current accounts

            // Hash the password using SHA-256
            sha256_hash(password, accounts[account_index].passwordHash);
            
                                                                        //Paul: I would split "get_password_from_user()" into a function that you can then call twice. Google the "DRY principle"
                      
            getchar();
            createAccount(&accounts[account_index], accountNumber, name, 0.00, accounts[account_index].passwordHash);
            numAccounts++;      // Paul:    It feels like you are using one variable for two purposes here which is confusing and not recommended. 
                                //          Perhaps have numAccounts and accountIndex then you won't need to do the decrementing which looks odd. 

            saveAccounts(accounts, numAccounts); 

            printf("\nAccount was succesfuly created!\n"
                "\n"
                "Account holder: %s \n"
                "Account number: %i\n"
                "Balance: %.2f\n"
                "\nPress enter to continue.", accounts[account_index].accountHolder, accounts[account_index].accountNumber, accounts[account_index].balance);

                  // Paul: Why do you need to reload here. You should get back the same information I think?
        }
 // Paul:    I realised we are still in the main() function. 240 lines is too long for any single function. I think it would be better to split
        //          the individual menu option handling into their own functions. e.g. create_account(), account_login() etc. Of course you will need
        //          to pass some parameters to get the data you need.
        //          As a general rule, functions shouldn't be much longer than you can see on the screen in one go.

        // log in option menu 
        else if(option == 'l')
        
        {  // Paul: The indenting of this do-while loop is all over the place. Yes it will compile but it is very hard to read and definitely not recommended

                    // Paul: You could actually use 'name' here rather than declaring a new string. That would save memory and maybe be clearer?
                    // find the user account in the database 
                    // Paul:    As before, it's better to initialise the string as empty and ask for one less char than the string can hold (to leave space for the NULL terminator)
                                                            //          But more critically this is a bug. strlen(name2) does NOT give the size of name2 (i.e. 25) it gives the length of the string held in name2.
                                                            //          strlen(name2) != sizeof(name2)
                                                            //          The docs for fgets() state: "Parsing stops if a newline character is found", so if the user just hits enter we might end up with strlen(name2) == 0
                                                            //          So this line would then be: name2[-1] = '\0';   which is definitely not intended and has a good chance of crashing the program.
                              // Paul:    This line does nothing because there is a NULL terminator before it 
                             // Paul: Perhaps a getMatchingUserAccount() function that returns a pointer to the account would make this clearer
            
             
                account_counter = get_matching_account(accounts, name, numAccounts);

                

                if(account_counter != -1)      // Paul: Not really necessary to check this because found must be 1, but might be worth keeping in case you add an option above to exit the "enter account holder" without finding one
                {   
                    BankAccount * account = &accounts[account_counter];
                
                              // Paul:    To decrement always use "counter--;" but I don't like this anyway. Why is the variable called "counter" when it is an account index? Why don't you populate counter 
                                            //          with the value you want rather than adding 1 above and then subtracting 1 here?
                        do
                        {
                            if(checkPassword(account))
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
                        
                            // Paul: Better to have "struct BankAccount * account = accounts[counter];" and then use "account" from then onwards
                        
                            // Paul:    It feels like everything inside this "if" should be held in a separate function.
                            //          I wonder why you haven't used more functions (they are good practice!). Perhaps because of the 
                            //          hassle of passing multiple parameters? If that's the case then I would make a struct of what you need to pass
                            //          and then you only need to pass the struct as a paramter. Also I suspect some variables can be local to the new function e.g. validInput

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

                                        scanf("%1c", &option);      // Paul: Perhaps check the return value is 1
                                        option = tolower(option);
                                           // Paul: Generally you shouldn't leave commented out code 

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


// function check_user deleted