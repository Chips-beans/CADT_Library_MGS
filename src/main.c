#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "create_login_user.h"
#include "user_booking.h"
#include "user_cancel.h"
#include "user_view_booking.h"
#include "user_view_rooms.h"
#include "checkIn.h"

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
                int isIn = loginStaff(staff.staffID, staff.password);
                if(isIn){
                    int choice;
                    do{
                        printf("Welcome, Staff\n");
                        printf(" 1. Check or Change room availability\n");
                        printf(" 2. View all room\n");
                        printf(" 3. CheckIN/OUT\n");
                        printf(" 4. Calculate Income\n");
                        printf(" 5. View booking\n");
                        printf(" 6. Back\n");
                        scanf("%d", &choice);
                        getchar();
                        if(choice == 3){
                            checkInGuest();
                        }
                    }while(choice != 6);
                }
            }
            else if (loginChoice == 3)
                continue;
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
            getchar();
            if (loginChoice == 2)
            {
                User user;
                do
                {
                    printf("---------------------------------------------------You have chosen to sign up as Customer.----------------------------------------------------\n");
                    printf("----------------------------------------------------------------------------------------------------------------------------------------------\n");
                    printf("                                        Please enter your Customer Username, password, confirm password, email address, and phone number.\n");
                    do
                    {
                        printf("                                            Username: ");
                        fgets(user.username, sizeof(user.username), stdin);
                        user.username[strcspn(user.username, "\n")] = 0;
                    } while (!valid_name(user.username) || isStringEmptyOrSpaces(user.username));
                    do
                    {
                        printf("                                            Password: ");
                        fgets(user.password, sizeof(user.password), stdin);
                        user.password[strcspn(user.password, "\n")] = 0;
                    } while (isStringEmptyOrSpaces(user.password));
                    do
                    {
                        printf("                                            Confirm Password: ");
                        fgets(user.confirmPassword, sizeof(user.confirmPassword), stdin);
                        user.confirmPassword[strcspn(user.confirmPassword, "\n")] = 0;
                    } while (isStringEmptyOrSpaces(user.confirmPassword));
                    do
                    {
                        printf("                                            Email Address: ");
                        fgets(user.emailAddress, sizeof(user.emailAddress), stdin);
                        user.emailAddress[strcspn(user.emailAddress, "\n")] = 0;
                    } while (isStringEmptyOrSpaces(user.emailAddress));
                    do
                    {
                        printf("                                            Phone Number: ");
                        fgets(user.phoneNumber, sizeof(user.phoneNumber), stdin);
                        user.phoneNumber[strcspn(user.phoneNumber, "\n")] = 0;
                    } while (isStringEmptyOrSpaces(user.phoneNumber));
                } while (strcmp(user.password, user.confirmPassword) != 0);
                createAccountUser(user);
            }
            else if (loginChoice == 1)
            {
                User user;
                printf("----------------------------------------------------You have chosen to log in as Customer.----------------------------------------------------\n");
                printf("----------------------------------------------------------------------------------------------------------------------------------------------\n");
                printf("                                            Please enter your Customer Username and Password.\n");
                do
                {
                    printf("                                            Username: ");
                    fgets(user.username, sizeof(user.username), stdin);
                    user.username[strcspn(user.username, "\n")] = 0;
                } while (!valid_name(user.username) || isStringEmptyOrSpaces(user.username));
                do
                {
                    printf("                                            Password: ");
                    fgets(user.password, sizeof(user.password), stdin);
                    user.password[strcspn(user.password, "\n")] = 0;
                } while (isStringEmptyOrSpaces(user.password));
                bool loginResult = loginUser(user.username, user.password);
                if (loginResult)
                {
                    int userChoice;
                    do
                    {
                        printf("Welcome %s\n", user.username);
                        printf(" 1. Booking Room\n");
                        printf(" 2. View Booking\n");
                        printf(" 3. Cancel Booking\n");
                        printf(" 4. Exit\n");
                        scanf("%d", &userChoice);
                        getchar();
                        if (userChoice == 1)
                        {
                            int bookingRoomUserChoice;
                            do{
                                printf("Booking Room\n");
                                printf(" 1. View Room\n");
                                printf(" 2. Book Room\n");
                                printf(" 3. Back\n");
                                scanf("%d", &bookingRoomUserChoice);
                                getchar();
                                if(bookingRoomUserChoice == 1) userViewRoom();
                                if(bookingRoomUserChoice == 2) bookRoom();
                            }while(bookingRoomUserChoice != 3);
                        }
                        if (userChoice == 2)
                        {
                            userViewBooking(user.username);
                        }
                        if (userChoice == 3)
                        {
                            cancelBooking();
                        }
                    } while (userChoice != 4);
                }
            }
            else if (loginChoice == 3)
                continue;
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
