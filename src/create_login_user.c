#include "create_login_user.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void encryptPassword(const char *password, char *encrypted)
{
    int i;
    for (i = 0; password[i] != '\0'; i++)
    {
        encrypted[i] = password[i] + 3;
    }
    encrypted[i] = '\0';
}

void decryptPassword(const char *encrypted, char *decrypted)
{
    int i;
    for (i = 0; encrypted[i] != '\0'; i++)
    {
        decrypted[i] = encrypted[i] - 3;
    }
    decrypted[i] = '\0';
}

bool loginUser(const char *username, const char *password)
{
    FILE *file = fopen("login_info.csv", "r");
    if (file == NULL)
    {
        printf("========================================================================================\n");
        printf("                            Error: Could not open login_info.csv                        \n");
        printf("========================================================================================\n");
        return false;
    }

    char line[256];
    File fileData;

    while (fgets(line, sizeof(line), file))
    {
        char *token = strtok(line, ",");
        if (token)
            strcpy(fileData.fileUsername, token);

        token = strtok(NULL, ",");
        if (token)
            strcpy(fileData.filePassword, token);

        fileData.filePassword[strcspn(fileData.filePassword, "\n")] = 0;

        decryptPassword(fileData.filePassword, fileData.dycryptedPassword);

        if (strcmp(fileData.fileUsername, username) == 0 && strcmp(fileData.dycryptedPassword, password) == 0)
        {
            printf("========================================================================================\n");
            printf("                     Login successful for user: %s                                      \n", username);
            printf("========================================================================================\n");
            fclose(file);
            return true;
        }
    }

    printf("========================================================================================\n");
    printf("                     Login failed for user: %s                                           \n", username);
    printf("========================================================================================\n");
    fclose(file);
    return false;
}

int loginStaff(const char *ID, const char *password)
{
    FILE *file = fopen("staff_login_info.csv", "r");
    if (file == NULL)
    {
        printf("========================================================================================\n");
        printf("                        Error: Could not open staff_login_info.csv                      \n");
        printf("========================================================================================\n");
        return 0;
    }

    char line[256];
    File fileData;

    while (fgets(line, sizeof(line), file))
    {
        char *token = strtok(line, ",");
        if (token)
            strcpy(fileData.fileUsername, token);

        token = strtok(NULL, ",");
        if (token)
            strcpy(fileData.filePassword, token);

        fileData.filePassword[strcspn(fileData.filePassword, "\n")] = 0;

        decryptPassword(fileData.filePassword, fileData.dycryptedPassword);
        if (strcmp(fileData.fileUsername, ID) == 0 && strcmp(fileData.dycryptedPassword, password) == 0)
        {
            printf("========================================================================================\n");
            printf("                     Login successful for staff ID: %s                                  \n", ID);
            printf("========================================================================================\n");
            fclose(file);
            return 1;
        }
    }

    printf("========================================================================================\n");
    printf("                     Login failed for staff ID: %s                                       \n", ID);
    printf("========================================================================================\n");
    fclose(file);
    return 0;
}

void createAccountUser(User user)
{
    FILE *file = fopen("login_info.csv", "a");
    if (file == NULL)
    {
        printf("========================================================================================\n");
        printf("                        Error: Could not open login_info.csv                            \n");
        printf("========================================================================================\n");
        return;
    }

    encryptPassword(user.password, user.encryptedPassword);
    fprintf(file, "%s,%s,%s,%s\n", user.username, user.encryptedPassword, user.emailAddress, user.phoneNumber);
    fclose(file);

    printf("========================================================================================\n");
    printf("                 Account created successfully for user: %s                              \n", user.username);
    printf("========================================================================================\n");
}

void createAccountStaff(Staff staff)
{
    FILE *file = fopen("staff_login_info.csv", "a");
    if (file == NULL)
    {
        printf("========================================================================================\n");
        printf("                      Error: Could not open staff_login_info.csv                        \n");
        printf("========================================================================================\n");
        return;
    }

    encryptPassword(staff.password, staff.encryptedPassword);
    fprintf(file, "%s,%s,%s,%s\n", staff.staffID, staff.encryptedPassword, staff.emailAddress, staff.phoneNumber);
    fclose(file);

    printf("========================================================================================\n");
    printf("             Account created successfully for staff ID: %s                              \n", staff.staffID);
    printf("========================================================================================\n");
}
