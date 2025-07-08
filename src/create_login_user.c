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
        printf("Error opening file\n");
        return false;
    }

    char line[256];
    File fileData;

    while (fgets(line, sizeof(line), file))
    {
        // Tokenize by comma
        char *token = strtok(line, ",");
        if (token)
            strcpy(fileData.fileUsername, token);

        token = strtok(NULL, ",");
        if (token)
            strcpy(fileData.filePassword, token);

        // Remove newline from encrypted password
        fileData.filePassword[strcspn(fileData.filePassword, "\n")] = 0;

        decryptPassword(fileData.filePassword, fileData.dycryptedPassword);

        if (strcmp(fileData.fileUsername, username) == 0 && strcmp(fileData.dycryptedPassword, password) == 0)
        {
            printf("Login successful for user: %s\n", username);
            fclose(file);
            return true;
        }
    }

    printf("Login failed for user: %s\n", username);
    fclose(file);
    return false;
}

int loginStaff(const char *ID, const char *password)
{
    FILE *file = fopen("staff_login_info.csv", "r");
    if (file == NULL)
    {
        printf("Error opening file\n");
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
            printf("Login successful for staff ID: %s\n", ID);
            fclose(file);
            return 1;
        }
    }

    printf("Login failed for staff ID: %s\n", ID);
    fclose(file);
    return 0;
}

void createAccountUser(User user)
{
    FILE *file = fopen("login_info.csv", "a");
    if (file == NULL)
    {
        printf("Error opening file\n");
        return;
    }

    encryptPassword(user.password, user.encryptedPassword);
    fprintf(file, "%s,%s,%s,%s\n", user.username, user.encryptedPassword, user.emailAddress, user.phoneNumber);
    fclose(file);
    printf("Account created successfully for user: %s\n", user.username);
}

void createAccountStaff(Staff staff)
{
    FILE *file = fopen("staff_login_info.csv", "a");
    if (file == NULL)
    {
        printf("Error opening file\n");
        return;
    }

    encryptPassword(staff.password, staff.encryptedPassword);
    fprintf(file, "%s,%s,%s,%s\n", staff.staffID, staff.encryptedPassword, staff.emailAddress, staff.phoneNumber);
    fclose(file);
    printf("Account created successfully for staff ID: %s\n", staff.staffID);
}
