#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <unistd.h>

void user_greeting(void);

int main(void)
{
    user_greeting();
    return 0;
}

//login interface
void user_greeting(void)
{
    printf("Welcome to SecureLogin!\n");
    printf("Where you credentials are safe with us.\n");
    sleep(2);
}