#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "login/create-login-user.h"

int main()
{
    int choice;
    do
    {
        printf("----------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("                                                Welcome to MEXISCADTEL management system\n");
        printf("----------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("                                                Are you a Staff or Customer:\n");
        printf("                                                    1. Staff\n");
        printf("                                                    2. Customer\n");
        printf("                                                    3. Exit\n");
        printf("                                                Enter your choice(1 or 2): ");
        scanf("%d", &choice);
        if (choice == 1)
        {
            printf("-----------------------------------------------------------Welcome to the staff menu----------------------------------------------------------\n");
            printf("                                                Do you want to login or sign up.\n");
            printf("                                                    1. Login\n");
            printf("                                                    2. Sign up\n");
            printf("                                                    3. Exit\n");
            printf("                                                Enter your choice(1 or 2 or 3): ");
            int loginChoice;
            scanf("%d", &loginChoice);
            if (loginChoice == 2)
            {
                Staff staff;
                do
                {
                    printf("------------------------------------------------------You have chosen to sign up as Staff.----------------------------------------------------\n");
                    printf("                                        Please enter your Staff ID, password, confirm password, email address, and phone number.\n");
                    printf("                                            Staff ID: ");
                    scanf("%s", staff.staffID);
                    if (strncmp(staff.staffID, "IDTB11", 6) != 0)
                    {
                        printf("Invalid Staff ID.\n");
                        continue;
                    }
                    printf("                                            Password: ");
                    scanf("%s", staff.password);
                    printf("                                            Confirm Password: ");
                    scanf("%s", staff.confirmPassword);
                    printf("                                            Email Address: ");
                    scanf("%s", staff.emailAddress);
                    printf("                                            Phone Number: ");
                    scanf("%s", staff.phoneNumber);
                } while (strcmp(staff.password, staff.confirmPassword) != 0);
                createAccountStaff(staff);
            }
            else if (loginChoice == 1)
            {
                Staff staff;
                printf("------------------------------------------------------You have chosen to log in as Staff.-----------------------------------------------------\n");
                printf("                                            Please enter your Staff ID and password.\n");
                printf("                                                Staff ID: ");
                scanf("%s", staff.staffID);
                printf("                                                Password: ");
                scanf("%s", staff.password);
                int loginResult = loginStaff(staff.staffID, staff.password);
            }
            else if (loginChoice == 3) continue;
        }
        else if (choice == 2)
        {
            printf("----------------------------------------------------------Welcome to customer menu.-----------------------------------------------------------\n");
            printf("----------------------------------------------------------------------------------------------------------------------------------------------\n");
            int loginChoice;
            printf("                                                Do you want to login or sign up.\n");
            printf("                                                    1. Login\n");
            printf("                                                    2. Sign up\n");
            printf("                                                    3. Exit\n");
            printf("                                                Enter your choice(1 or 2 or 3): ");
            scanf("%d", &loginChoice);
            if (loginChoice == 2)
            {
                User user;
                do
                {
                    printf("---------------------------------------------------You have chosen to sign up as Customer.----------------------------------------------------\n");
                    printf("----------------------------------------------------------------------------------------------------------------------------------------------\n");
                    printf("                                        Please enter your Customer Username, password, confirm password, email address, and phone number.\n");
                    printf("                                            Username: ");
                    scanf("%s", user.username);
                    printf("                                            Password: ");
                    scanf("%s", user.password);
                    printf("                                            Confirm Password: ");
                    scanf("%s", user.confirmPassword);
                    printf("                                            Email Address: ");
                    scanf("%s", user.emailAddress);
                    printf("                                            Phone Number: ");
                    scanf("%s", user.phoneNumber);
                } while (strcmp(user.password, user.confirmPassword) != 0);
                createAccountUser(user);
            }
            else if (loginChoice == 1)
            {
                User user;
                printf("----------------------------------------------------You have chosen to log in as Customer.----------------------------------------------------\n");
                printf("----------------------------------------------------------------------------------------------------------------------------------------------\n");
                printf("                                            Please enter your Customer Username and Password.\n");
                printf("                                                Username: ");
                scanf("%s", user.username);
                printf("                                                Password: ");
                char password[100];
                scanf("%s", user.password);
                int loginResult = loginUser(user.username, user.password);
            }else if (loginChoice == 3) continue;
        }
        else if (choice == 3)
        {
            printf("--------------------------------------------------------Exiting the program. Goodbye!---------------------------------------------------------\n");
            printf("----------------------------------------------------------------------------------------------------------------------------------------------\n");
        }
        else
        {
            printf("------------------------------------------------------Invalid choice. Please try again.-------------------------------------------------------\n");
            printf("----------------------------------------------------------------------------------------------------------------------------------------------\n\n\n");
        }
    } while (choice != 3);
    
    return 0;
}
