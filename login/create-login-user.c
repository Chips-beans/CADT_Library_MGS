#include "create-login-user.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int loginUser(const char *username, const char *password)
{
    FILE *file = fopen("login/login-info.txt", "r"); // Open the file in read mode
    if (file == NULL)
    {
        printf("Error opening file");
        return 0;
    }

    char fileUsername[100];
    char filePassword[100];
    while (fscanf(file, "%s %s", fileUsername, filePassword) == 2)
    {
        if (strcmp(fileUsername, username) == 0 && strcmp(filePassword, password) == 0)
        {
            printf("Login successful for user: %s\n", username);
            fclose(file);
            return 0;
        }
    }
    printf("Login failed for user: %s\n", username);
    fclose(file);
    return 0;
}

int loginStaff(const char *ID, const char *password)
{
    FILE *file = fopen("login/staff-login-info.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file");
        return 0;
    }
    
    char fileID[100];
    char filePassword[100];
    while (fscanf(file, "%s %s", fileID, filePassword) == 2)
    {  
        if (strcmp(fileID, ID) == 0 && strcmp(filePassword, password) == 0)
        {
            printf("Login successful for staff ID: %s\n", ID);
            fclose(file);
            return 0;
        }
    }
    printf("Login failed for staff ID: %s\n", ID);
    fclose(file);
    return 0;
}