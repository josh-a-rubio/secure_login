#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <unistd.h>

void user_greeting(void);
void user_menu(void);
void user_signup(void);

int main(void)
{
    user_greeting();
    user_menu();

    return 0;
    
}

//login interface
void user_greeting(void)
{
    printf("\nWelcome to SecureLogin!\n");
    printf("Where your credentials are safe with us.\n");
    sleep(2);
}

void user_menu(void)
{
    char choice;

    printf("\nType the number of your option.\n");
    printf("1. Login\n");
    printf("2. Sign up\n");
    printf("3. Exit\n");
    printf("\nChoice: ");

    scanf(" %c", &choice);

    if (choice == '1') 
    {
        printf("Login selected\n");
        
    } 
    else if (choice == '2') 
    {
       printf("Sign up selected\n");
       user_signup();
    } 
    else if (choice == '3')
    {
        exit(0);
    } 
    else
    {
        printf("Invalid option. Please try again\n");
    }

}
void user_signup(void)
{
    
    char username[32];
    char *password;
    
    
    while (getchar() != '\n' && !EOF) 
    { 
        //clear stdin 
    }
    
    printf("\nUsername: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n" )] = '\0';
    
    password = getpass("Password: ");
    printf("Account created successfully!\n");
    
}
