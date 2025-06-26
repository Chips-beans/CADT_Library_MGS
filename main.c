#include <stdio.h>
#include "login/create-login-user.h"

int main()
{
    int choice;

    printf("-------------------------------------------------------------\n");
    printf("         Welcome to MEXISCADTEL management system\n");
    printf("-------------------------------------------------------------\n");
    printf("         Are you logging in as:\n");
    printf("            1. Staff\n");
    printf("            2. Customer\n");
    printf("            3. Exit\n");
    printf("         Enter your choice(1 or 2): ");
    scanf("%d", &choice);
    if (choice == 1)
    {
        printf("------You have chosen to log in as Staff.------\n");
        printf("    Please enter your Staff ID and password.\n");
        printf("    Staff ID: ");
        char staffID[100];
        scanf("%s", staffID);
        printf("    Password: ");
        char password[100];
        scanf("%s", password);
        int loginResult = loginStaff(staffID, password);
        }
    else if (choice == 2)
    {
        printf("You have chosen to log in as Customer.\n");
        // Here you would typically call a function to handle customer login
    }
    else if (choice == 3)
    {
        printf("Exiting the system. Goodbye!\n");
        return 0;
    }
    else
    {
        printf("Invalid choice. Please restart the program and choose either 1 or 2.\n");
        return 1;
    }
    return 0;
}
