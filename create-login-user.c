#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int loginUser(const char *username, const char *password) {
    FILE *file = fopen("login-info.txt", "r"); // Open the file in read mode
    if (file == NULL) {
        printf("Error opening file");
        return 0;
    }

    char fileUsername[100];
    char filePassword[100];
    while(fgets(fileUsername, sizeof(fileUsername), file) != NULL) {
        fgets(filePassword, sizeof(filePassword), file);
        // Remove newline character from the end of the strings
        fileUsername[strcspn(fileUsername, "\n")] = 0;
        filePassword[strcspn(filePassword, "\n")] = 0;

        // Check if the username and password match
        if (strcmp(fileUsername, username) == 0 && strcmp(filePassword, password) == 0) {
            printf("Login successful for user: %s\n", username);
            fclose(file);
            
            return 0;
        }
    }
    
    fclose(file);
    return 0;
}

int loginStaff(const char *ID, const char *password) {
    FILE *file = fopen("staff-login-info.txt", "r");
    if (file == NULL) {
        printf("Error opening file");
        return 0;
    }

    char fileID[100];
    char filePassword[100];
    while(fgets(fileID, sizeof(fileID), file) != NULL) {
        fgets(filePassword, sizeof(filePassword), file);
        // Remove newline character from the end of the strings
        fileID[strcspn(fileID, "\n")] = 0;
        filePassword[strcspn(filePassword, "\n")] = 0;

   
        if (strcmp(fileID, ID) == 0 && strcmp(filePassword, password) == 0) {
            printf("Login successful for staff ID: %s\n", ID);
            fclose(file);
            return 0;
        }
    }
    
    fclose(file);
    return 0;
}