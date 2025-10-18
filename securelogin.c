#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>  

#ifdef _WIN32
#include <windows.h> 
#endif

#define SALT_LEN 8
#define MAX_USERNAME_LEN 31
#define MAX_PASSWORD_LEN 64


void user_greeting(void);
void user_menu(bool has_account);
void user_signup(bool *has_account);
bool username_exists(const char *username);
bool validate_username(const char *username);
void generate_salt(char *salt, size_t len);
unsigned long toy_hash(const char *str, const char *salt);
void user_login(void);


int main(void)
{
    srand((unsigned)time(NULL));

    user_greeting();

    bool has_account = false;
    user_menu(has_account);

    return 0;
}

//User greeting
void user_greeting(void)
{
    printf("\nWelcome to SecureLogin!\n");
    printf("Where your credentials are safe with us.\n");
    sleep(1);
}

//User menu options
void user_menu(bool has_account)
{
    char choice;

    while (true)
    {
        if (!has_account)
        {
            printf("\nType the number of your option.\n");
            printf("1. Login\n");
            printf("2. Sign up\n");
            printf("3. Exit\n");
        }
        else
        {
            printf("\nType the number of your option.\n");
            printf("1. Login\n");
            printf("2. Exit\n");
        }

        printf("\nChoice: ");
        scanf(" %c", &choice);
         
        if (!has_account)
        {
            if (choice == '1') 
            {
                printf("Login selected\n");
                user_login();
                exit(0);
            } 
            else if (choice == '2') 
            {
                printf("Sign up selected\n");
                user_signup(&has_account);
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
        else
        {
            if (choice == '1')
            {
                printf("Login selected\n");
                user_login();
                exit(0);
            }
            else if (choice == '2')
            {
                exit(0);
            }
            else
            {
                printf("Invalid option. Please try again\n");
            }
        }
    }
}

//User signup
void user_signup(bool *has_account)
{
    char username[32];
    char *password;
    char salt[SALT_LEN + 1];
    unsigned long hash_value;
    
    while (getchar() != '\n' && !feof(stdin)) 
    { 
        //clear stdin 
    }

    while (true)
    {
         printf("\nUsername: ");
          fgets(username, sizeof(username), stdin); 
          size_t len = strlen(username);
          if (len > 0 && username[len-1] == '\n')
          {
            username[len-1] = '\0';
          }
          
          else if (len == sizeof(username) - 1)
          {
            printf("Error: Username too long. Limit %d characters.\n", MAX_USERNAME_LEN);
            while (getchar() != '\n' && !feof(stdin));
            continue;
          }
          
          if (strlen(username) == 0)
          {   
            printf("Error: Username cannot be empty. \n");
            continue;
          }
        
        if (!validate_username(username))
        {
            printf("Error: Username cannot contain colons (:) or spaces. \n");
            continue;
        }
        
        if (username_exists(username))
        {
            printf("Error: Username already exists. Please choose another.\n");
            continue;
        }
        break;
}
    
    password = getpass("Password: ");

    size_t password_len = strlen(password);
    if (password_len == 0)
    {
        printf("Error: Password cannot be empty. \n");
        memset(password, 0, password_len);
        return;
    }

    if (password_len > MAX_PASSWORD_LEN)
    {
        printf("Error: Password too long. limit %d characters. \n", MAX_PASSWORD_LEN);
        memset(password, 0, password_len);
        return;
    }

    //Generate salt + hash
    generate_salt(salt, SALT_LEN + 1);
    hash_value = toy_hash(password, salt);

    //Save to users.txt
    FILE *fp = fopen("users.txt", "a");
    if (!fp)
    {
        perror("Error opening users.txt");
        memset(password, 0, password_len);
        return;
    }
    #ifdef _WIN32
    SetFileAttributes("users.txt", FILE_ATTRIBUTE_HIDDEN);
    #else
    chmod("users.txt", 0600);
    #endif

    fprintf(fp, "%s:%s:%lu\n", username, salt, hash_value);
    fclose(fp);

    //Clear memory
    memset(password, 0, password_len);

    printf("Account created successfully!\n");

    *has_account = true;
}

//Check if username exists in txt file
bool username_exists(const char *username)
{
    FILE *fp = fopen("users.txt", "r");
    if (!fp)
    {
        return false;
    }
    
    char line[256];
    char stored_username[MAX_USERNAME_LEN + 1];

    while(fgets(line, sizeof(line), fp))
    {
        if (sscanf(line, "%31[^:]", stored_username) == 1)
        {
            if (strcmp(username, stored_username) == 0)
            {
                fclose(fp);
                return true;
            }
        }

    }

    fclose(fp);
    return false;
}

//Validate username - no spaces or colons
bool validate_username(const char *username)
{
    size_t len = strlen(username);
    for (size_t i = 0; i < len; i++)
    {
        if (username[i] == ':' || username[i] == ' ' || username[i] == '\t'
        || username[i] == '\n' || username[i] == '\r')
        {
            return false;
        }
    }
    return true;
 }

//Salt generator
void generate_salt(char *salt, size_t len)
{
    const char charset[] = 
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    for (size_t i = 0; i < len - 1 ; i++)
    {
        salt[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    salt[len -1] = '\0';
}

//Toy hash (djb2 variant)
unsigned long toy_hash(const char *str, const char *salt)
{
    unsigned long hash = 5381;
    int c;

    //Mix in the salt first
    while ((c = *salt++))
    {
        hash = (( hash << 5) + hash)+ c;
    }

    //Mix in the password
    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

//User login
void user_login(void)
{
    char username[32];
    char *password;
    char salt[SALT_LEN + 1];
    unsigned long stored_hash;
    unsigned long input_hash;
    bool login_success = false;

    while (getchar() != '\n' && !feof(stdin)) 
    { 
        //clear stdin 
    }

    printf("\nUsername: ");
    fgets(username, sizeof(username), stdin); 
    
    size_t len = strlen(username);
    if (len > 0 && username[len-1] == '\n')
    {
        username[len-1] = '\0';
    }
    else if (len == sizeof(username) - 1)
    {
        while (getchar() != '\n' && !feof(stdin));
    }
    
    // Validate username format
    if (strlen(username) == 0)
    {   
        printf("Login failed: Invalid username or password.\n");
        return;
    }

    if (!validate_username(username))
    {
        printf("Login failed: Invalid username or password.\n");
        return;
    }

    password = getpass("Password: ");

    size_t password_len = strlen(password);
    if (password_len == 0)
    {
        printf("Login failed: Invalid username or password.\n");
        memset(password, 0, password_len);
        return;
    }

    FILE *fp = fopen("users.txt", "r");
    if (!fp)
    {
        printf("Login failed: Invalid username or password.\n");
        memset(password, 0, password_len);
        return;
    }

    char line[256];
    char stored_username[MAX_USERNAME_LEN + 1];
    
    while (fgets(line, sizeof(line), fp))
    {
        // Parse line: username:salt:hash
        if (sscanf(line, "%31[^:]:%8[^:]:%lu", stored_username, salt, &stored_hash) == 3)
        {
            if (strcmp(username, stored_username) == 0)
            {
                // Username found . Hash the input password with stored salt
                input_hash = toy_hash(password, salt);
                
                // Compare hashes
                if (input_hash == stored_hash)
                {
                    //Art to confirm login before end of prototype
                    time_t now = time(NULL);
                    struct tm *t = localtime(&now);
                    char date_str[20];
                    char time_str[20];
                    
                    strftime(date_str, sizeof(date_str), "%m/%d/%Y", t);
                    strftime(time_str, sizeof(time_str), "%I:%M %p", t);
                    
                    printf("\n");
                    printf("╔═══════════════════════════════════════╗\n");
                    printf("║  Date: %-15s                ║\n", date_str);
                    printf("║  Time: %-15s                ║\n", time_str);
                    printf("║                                       ║\n");
                    printf("║      ✓ LOGIN SUCCESSFUL!              ║\n");
                    printf("║      Welcome, %-23s ║\n", username);
                    printf("║                                       ║\n");
                    printf("║    Thanks for testing my program!     ║\n");
                    printf("╚═══════════════════════════════════════╝\n");

                    login_success = true;
                }
                
                break;
            }
        }
    }
    
    // If login failed for any reason
    if (!login_success)
    {
        printf("Login failed: Invalid username or password.\n");
    }

    fclose(fp);
    
    memset(password, 0, password_len);
}
