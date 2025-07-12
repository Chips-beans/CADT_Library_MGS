#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "create_login_user.h"
#include "user_booking.h"
#include "user_cancel.h"
#include "user_view_booking.h"
#include "user_view_rooms.h"
#include "staff_viewandupdate_room.h"
#include "staff_check_in.h"
#include "staff_check_out.h"
#include "staff_income.h"

int main()
{
    int choice;

    do
    {
        printf("========================================================================================\n");
        printf("                          Welcome to MEXISCADTEL Management System                     \n");
        printf("========================================================================================\n");
        printf("                          Are you a Staff or Customer?                                 \n");
        printf("                              1. Staff                                                  \n");
        printf("                              2. Customer                                               \n");
        printf("                              3. Exit                                                   \n");
        printf("                          Enter your choice (1 or 2): ");
        scanf("%d", &choice);

        if (choice == 1)
        {
            printf("========================================================================================\n");
            printf("                              Welcome to the Staff Menu                                 \n");
            printf("========================================================================================\n");
            printf("                          Do you want to login or sign up?                              \n");
            printf("                              1. Login                                                  \n");
            printf("                              2. Sign up                                                \n");
            printf("                              3. Exit                                                   \n");
            printf("                          Enter your choice (1 or 2 or 3): ");
            int loginChoice;
            scanf("%d", &loginChoice);

            if (loginChoice == 2)
            {
                Staff staff;
                do
                {
                    printf("========================================================================================\n");
                    printf("                         You have chosen to sign up as Staff                            \n");
                    printf("========================================================================================\n");
                    printf("  Please enter your Staff ID, password, confirm password, email address, and phone number.\n");
                    printf("                              Staff ID: ");
                    scanf("%s", staff.staffID);
                    if (strncmp(staff.staffID, "IDTB11", 6) != 0)
                    {
                        printf("                          Invalid Staff ID. Please try again.                           \n");
                        continue;
                    }
                    printf("                                            Password: ");
                    scanf("%s", staff.password);
                    printf("                              Confirm Password: ");
                    scanf("%s", staff.confirmPassword);
                    printf("                                            Email Address: ");
                    scanf("%s", staff.emailAddress);
                    printf("                              Phone Number: ");
                    scanf("%s", staff.phoneNumber);
                } while (strcmp(staff.password, staff.confirmPassword) != 0);
                createAccountStaff(staff);
            }
            else if (loginChoice == 1)
            {
                Staff staff;
                printf("========================================================================================\n");
                printf("                         You have chosen to log in as Staff                             \n");
                printf("========================================================================================\n");
                printf("                          Please enter your Staff ID and password                       \n");
                printf("                              Staff ID: ");
                scanf("%s", staff.staffID);
                printf("                              Password: ");
                scanf("%s", staff.password);

                int isIn = loginStaff(staff.staffID, staff.password);
                if (isIn)
                {
                    int choice;
                    do
                    {
                        printf("========================================================================================\n");
                        printf("                                     Welcome, Staff                                      \n");
                        printf("========================================================================================\n");
                        printf("                              1. View all room                                          \n");
                        printf("                              2. CheckIN/OUT                                            \n");
                        printf("                              3. Calculate Income                                       \n");
                        printf("                              4. Back                                                   \n");
                        printf("                          Enter your choice: ");
                        scanf("%d", &choice);
                        getchar();

                        if (choice == 1)
                        {
                            staffViewAndUpdateRoomStatus();
                        }
                        else if (choice == 2)
                        {
                            int CheckInOutChoice;
                            do
                            {
                                printf("========================================================================================\n");
                                printf("                                 Check IN/OUT Menu                                       \n");
                                printf("========================================================================================\n");
                                printf("                              1. Check IN                                               \n");
                                printf("                              2. Check OUT                                              \n");
                                printf("                              3. Back                                                   \n");
                                printf("                          Enter your choice: ");
                                scanf("%d", &CheckInOutChoice);
                                getchar();
                                if(CheckInOutChoice == 1){
                                staffCheckIN();
                                }else if (CheckInOutChoice == 2){
                                    staffCheckOut();
                                }
                            } while (CheckInOutChoice != 3);
                        }else if (choice == 3){
                            staffCalsulateIncome();
                        }
                    } while (choice != 4);
                }
            }
            else if (loginChoice == 3)
                continue;
        }
        else if (choice == 2)
        {
            printf("========================================================================================\n");
            printf("                           Welcome to the Customer Menu                                 \n");
            printf("========================================================================================\n");
            int loginChoice;
            printf("                          Do you want to login or sign up?                              \n");
            printf("                              1. Login                                                  \n");
            printf("                              2. Sign up                                                \n");
            printf("                              3. Exit                                                   \n");
            printf("                          Enter your choice (1 or 2 or 3): ");
            scanf("%d", &loginChoice);
            getchar();
            if (loginChoice == 2)
            {
                User user;
                do
                {
                    printf("========================================================================================\n");
                    printf("                      You have chosen to sign up as Customer                             \n");
                    printf("========================================================================================\n");
                    printf("  Please enter your Username, password, confirm password, email, and phone number.       \n");
                    do
                    {
                        printf("                              Username: ");
                        fgets(user.username, sizeof(user.username), stdin);
                        user.username[strcspn(user.username, "\n")] = 0;
                    } while (!valid_name(user.username) || isStringEmptyOrSpaces(user.username));
                    do
                    {
                        printf("                              Password: ");
                        fgets(user.password, sizeof(user.password), stdin);
                        user.password[strcspn(user.password, "\n")] = 0;
                    } while (isStringEmptyOrSpaces(user.password));
                    do
                    {
                        printf("                              Confirm Password: ");
                        fgets(user.confirmPassword, sizeof(user.confirmPassword), stdin);
                        user.confirmPassword[strcspn(user.confirmPassword, "\n")] = 0;
                    } while (isStringEmptyOrSpaces(user.confirmPassword));
                    do
                    {
                        printf("                              Email Address: ");
                        fgets(user.emailAddress, sizeof(user.emailAddress), stdin);
                        user.emailAddress[strcspn(user.emailAddress, "\n")] = 0;
                    } while (isStringEmptyOrSpaces(user.emailAddress));
                    do
                    {
                        printf("                              Phone Number: ");
                        fgets(user.phoneNumber, sizeof(user.phoneNumber), stdin);
                        user.phoneNumber[strcspn(user.phoneNumber, "\n")] = 0;
                    } while (isStringEmptyOrSpaces(user.phoneNumber));
                } while (strcmp(user.password, user.confirmPassword) != 0);
                createAccountUser(user);
            }
            else if (loginChoice == 1)
            {
                User user;
                printf("========================================================================================\n");
                printf("                      You have chosen to log in as Customer                              \n");
                printf("========================================================================================\n");
                printf("                   Please enter your Username and Password                               \n");
                do
                {
                    printf("                              Username: ");
                    fgets(user.username, sizeof(user.username), stdin);
                    user.username[strcspn(user.username, "\n")] = 0;
                } while (!valid_name(user.username) || isStringEmptyOrSpaces(user.username));
                do
                {
                    printf("                              Password: ");
                    fgets(user.password, sizeof(user.password), stdin);
                    user.password[strcspn(user.password, "\n")] = 0;
                } while (isStringEmptyOrSpaces(user.password));
                bool loginResult = loginUser(user.username, user.password);
                if (loginResult)
                {
                    int userChoice;
                    do
                    {
                        printf("========================================================================================\n");
                        printf("                              Welcome %s                                                 \n", user.username);
                        printf("========================================================================================\n");
                        printf("                              1. Booking Room                                            \n");
                        printf("                              2. View Booking                                            \n");
                        printf("                              3. Cancel Booking                                          \n");
                        printf("                              4. Exit                                                    \n");
                        printf("                          Enter your choice: ");
                        scanf("%d", &userChoice);
                        getchar();
                        if (userChoice == 1)
                        {
                            int bookingRoomUserChoice;
                            do
                            {
                                printf("========================================================================================\n");
                                printf("                                 Booking Room Menu                                       \n");
                                printf("========================================================================================\n");
                                printf("                              1. View Room                                               \n");
                                printf("                              2. Book Room                                               \n");
                                printf("                              3. Back                                                    \n");
                                printf("                          Enter your choice: ");
                                scanf("%d", &bookingRoomUserChoice);
                                getchar();
                                if (bookingRoomUserChoice == 1)
                                    userViewRoom();
                                if (bookingRoomUserChoice == 2)
                                    bookRoom();
                            } while (bookingRoomUserChoice != 3);
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
            printf("========================================================================================\n");
            printf("                       Exiting the program. Goodbye!                                     \n");
            printf("========================================================================================\n");
        }
        else
        {
            printf("========================================================================================\n");
            printf("                     Invalid choice. Please try again.                                   \n");
            printf("========================================================================================\n\n\n");
        }
    } while (choice != 3);

    return 0;
}
