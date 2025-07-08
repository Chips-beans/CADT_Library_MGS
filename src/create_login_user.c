#include "create_login_user.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void encryptPassword(const char *password, char *encrypted) {
    int i;
    for (i = 0; password[i] != '\0'; i++) {
        encrypted[i] = password[i] + 3;
    }
    encrypted[i] = '\0'; 
}
void decryptPassword(const char *encrypted, char *decrypted) {
    int i;
    for (i = 0; encrypted[i] != '\0'; i++) {
        decrypted[i] = encrypted[i] - 3;  // Shift back
    }
    decrypted[i] = '\0';
}

bool loginUser(const char *username, const char *password)
{
    FILE *file = fopen("login_info.txt", "r"); // Open the file in read mode
    if (file == NULL)
    {
        printf("Error opening file");
        return false;
    }
    File fileData;
    
    while (fscanf(file, "%s %s", fileData.fileUsername, fileData.filePassword) == 2)
    {
        decryptPassword(fileData.filePassword, fileData.dycryptedPassword);
        printf("Checking user: %s with decrypted password: %s\n", fileData.fileUsername, fileData.dycryptedPassword);
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
    FILE *file = fopen("staff_login_info.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file");
        return 0;
    }

    File fileData;
    while (fscanf(file, "%s %s", fileData.fileUsername, fileData.filePassword) == 2)
    {
        
        decryptPassword(fileData.filePassword, fileData.dycryptedPassword);
        if (strcmp(fileData.fileUsername, ID) == 0 && strcmp(fileData.dycryptedPassword, password) == 0)
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

void createAccountUser(User user)
{
    FILE *file = fopen("login_info.txt", "a"); // Open the file in append mode
    if (file == NULL)
    {
        printf("Error opening file");
    }
    encryptPassword(user.password, user.encryptedPassword);
    fprintf(file, "%s %s %s %s\n", user.username, user.encryptedPassword, user.emailAddress, user.phoneNumber);
    fclose(file);
    printf("Account created successfully for user: %s\n", user.username);
}
void createAccountStaff(Staff staff)
{
    FILE *file = fopen("staff_login_info.txt", "a"); // Open the file in append mode
    if (file == NULL)
    {
        printf("Error opening file");
    }
    encryptPassword(staff.password, staff.encryptedPassword);
    fprintf(file, "%s %s %s %s\n", staff.staffID, staff.encryptedPassword, staff.emailAddress, staff.phoneNumber);
    fclose(file);
    printf("Account created successfully for staff ID: %s\n", staff.staffID);
}

