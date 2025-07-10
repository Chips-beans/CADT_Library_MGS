#include "user_booking.h"
#include "user_view_rooms.h"
#include "updateFile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// Function to validate name input
int isRoomFree(const char *filename, int roomNum) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Failed to open room file");
        return 0;
    }

    char line[256];
    int roomID;
    char type[20], status[10], note[10];

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%d,%19[^,],%9[^,],%9[^\n]", &roomID, type, status, note) == 4) {
            if (roomID == roomNum && strcmp(status, "Free") == 0) {
                fclose(fp);
                return 1; // Room is Free
            }
        }
    }

    fclose(fp);
    return 0; // Room is not Free
}
int valid_name(const char *str)
{
    for (int i = 0; str[i]; i++)
    {
        if (!((str[i] >= 'A' && str[i] <= 'Z') ||
              (str[i] >= 'a' && str[i] <= 'z') ||
              str[i] == ' '))
            return 0;
    }
    return 1;
}
int isStringEmptyOrSpaces(const char *str)
{
    while (*str)
    {
        if (!isspace((unsigned char)*str))
        {
            return 0; // found a non-space char
        }
        str++;
    }
    return 1; // all spaces or empty
}

// Function to reuse or generate new booking ID
int getAvailableBookingID()
{
    FILE *fp = fopen("booking_ids.txt", "r");
    int id = -1;

    if (fp)
    {
        if (fscanf(fp, "%d", &id) == 1)
        {
            FILE *temp = fopen("temp_ids.txt", "w");
            int temp_id;
            while (fscanf(fp, "%d", &temp_id) == 1)
            {
                if (temp_id != id)
                    fprintf(temp, "%d\n", temp_id);
            }
            fclose(fp);
            fclose(temp);
            remove("booking_ids.txt");
            rename("temp_ids.txt", "booking_ids.txt");
            return id;
        }
        fclose(fp);
    }

    FILE *last = fopen("last_id.txt", "r");
    int last_id = 999;
    if (last)
    {
        fscanf(last, "%d", &last_id);
        fclose(last);
    }

    id = last_id + 1;
    if (id > 9999)
        id = 1000;

    last = fopen("last_id.txt", "w");
    fprintf(last, "%d", id);
    fclose(last);

    return id;
}

int bookRoom()
{
    Booking book;
    float total = 0, upfront = 0;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int today = tm.tm_mday;
    int this_month = tm.tm_mon + 1;
    int this_year = tm.tm_year + 1900;
    int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Leap year check
    if ((this_year % 4 == 0 && this_year % 100 != 0) || (this_year % 400 == 0))
        days_in_month[2] = 29;

    printf("============== HOTEL BOOKING SYSTEM ==============\n");

    // Name input
    do
    {
        printf("Enter your name: ");
        fgets(book.name, sizeof(book.name), stdin);
        book.name[strcspn(book.name, "\n")] = 0;
        if (!valid_name(book.name) || isStringEmptyOrSpaces(book.name))
            printf("Invalid name. Try again.\n");
    } while (!valid_name(book.name) || isStringEmptyOrSpaces(book.name));

    // Room selection
    do
    {
        printf("Enter number of single rooms: ");
        scanf("%d", &book.single_rooms);
        printf("Enter number of double rooms: ");
        scanf("%d", &book.double_rooms);
        if (book.single_rooms < 0 || book.double_rooms < 0 || (book.single_rooms + book.double_rooms == 0))
            printf("You must book at least one room.\n");
    } while (book.single_rooms < 0 || book.double_rooms < 0 || (book.single_rooms + book.double_rooms == 0));

    // Nights
    do
    {
        printf("Enter number of nights: ");
        scanf("%d", &book.nights);
    } while (book.nights <= 0);

    // Check-in date
    do
    {
        printf("Enter check-in month (1-12): ");
        scanf("%d", &book.checkin_month);
    } while (book.checkin_month < 1 || book.checkin_month > 12);

    if (book.checkin_month == 2 &&
        ((this_year % 4 == 0 && this_year % 100 != 0) || (this_year % 400 == 0)))
        days_in_month[2] = 29;
    else
        days_in_month[2] = 28;

    do
    {
        printf("Enter check-in day: ");
        scanf("%d", &book.checkin_day);
    } while (book.checkin_day < 1 || book.checkin_day > days_in_month[book.checkin_month]);

    book.checkin_year = this_year;
    if (book.checkin_month < this_month ||
        (book.checkin_month == this_month && book.checkin_day < today))
        book.checkin_year++;

    // Check-out date calculation
    struct tm checkin_tm = {0};
    checkin_tm.tm_year = book.checkin_year - 1900;
    checkin_tm.tm_mon = book.checkin_month - 1;
    checkin_tm.tm_mday = book.checkin_day + book.nights;
    mktime(&checkin_tm);

    int checkout_day = checkin_tm.tm_mday;
    int checkout_month = checkin_tm.tm_mon + 1;
    int checkout_year = checkin_tm.tm_year + 1900;

    // Booking ID
    int booking_id = getAvailableBookingID();

    // Assign rooms and save to file
    FILE *fp = fopen(ROOM_FILE, "a");
    if (!fp)
    {
        printf("Error: could not open bookings.csv\n");
        return 1;
    }

    int assigned = 0;
    int remaining_single = book.single_rooms;
    int remaining_double = book.double_rooms;
    Date date;
            date.day = book.checkin_day;
            date.month = book.checkin_month;
            date.year = book.checkin_year;
    for (int floor = 1; floor <= 9; floor++)
    {
        for (int i = 0; i < 10; i++)
        {
            int roomNum = floor * 100 + i;
            
            int isBook = isRoomBooked(ROOM_FILE, roomNum, date);
            if (!isBook)
            {
                if (remaining_single > 0 && i <= 4)
                {
                    fprintf(fp, "%d,%s,%04d-%02d-%02d,%04d-%02d-%02d,%d,%.2f,%.2f\n",
                            roomNum, book.name,
                            book.checkin_year, book.checkin_month, book.checkin_day,
                            checkout_year, checkout_month, checkout_day,
                            booking_id, 50.0f * book.nights, 50.0f * book.nights * 0.4f);
                    updateRoomStatus("rooms.csv", roomNum, "Booked");
                    remaining_single--;
                    assigned++;
                }
                else if (remaining_double > 0 && i >= 5)
                {
                    fprintf(fp, "%d,%s,%04d-%02d-%02d,%04d-%02d-%02d,%d,%.2f,%.2f\n",
                            roomNum, book.name,
                            book.checkin_year, book.checkin_month, book.checkin_day,
                            checkout_year, checkout_month, checkout_day,
                            booking_id, 75.0f * book.nights, 75.0f * book.nights * 0.4f);
                    updateRoomStatus("rooms.csv", roomNum, "Booked");
                    remaining_double--;
                    assigned++;
                }
            }

            if (remaining_single == 0 && remaining_double == 0)
                break;
        }
        if (remaining_single == 0 && remaining_double == 0)
            break;
    }

    fclose(fp);

    float single_total = book.single_rooms * 50.0f * book.nights;
    float double_total = book.double_rooms * 75.0f * book.nights;
    total = single_total + double_total;
    upfront = total * 0.4f;

    // Display
    printf("\n========== BOOKING SUMMARY ==========\n");
    printf("Guest: %s\n", book.name);
    printf("Booking ID: %04d\n", booking_id);
    printf("Check-in: %04d-%02d-%02d\n", book.checkin_year, book.checkin_month, book.checkin_day);
    printf("Check-out: %04d-%02d-%02d\n", checkout_year, checkout_month, checkout_day);
    printf("Room(s) Booked: %d (Single: %d, Double: %d)\n", assigned, book.single_rooms, book.double_rooms);
    printf("Total Price     : $%.2f\n", total);
    printf("Upfront Payment : $%.2f (40%%)\n", upfront);
    printf("=====================================\n");

    return 0;
}
int staffBookRoom()
{
    Booking book;
    float total = 0, upfront = 0;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int today = tm.tm_mday;
    int this_month = tm.tm_mon + 1;
    int this_year = tm.tm_year + 1900;
    int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Leap year check
    if ((this_year % 4 == 0 && this_year % 100 != 0) || (this_year % 400 == 0))
        days_in_month[2] = 29;

    printf("============== HOTEL BOOKING SYSTEM ==============\n");

    // Name input
    do
    {
        printf("Enter your name: ");
        fgets(book.name, sizeof(book.name), stdin);
        book.name[strcspn(book.name, "\n")] = 0;
        if (!valid_name(book.name) || isStringEmptyOrSpaces(book.name))
            printf("Invalid name. Try again.\n");
    } while (!valid_name(book.name) || isStringEmptyOrSpaces(book.name));

    // Room selection
    do
    {
        printf("Enter number of single rooms: ");
        scanf("%d", &book.single_rooms);
        printf("Enter number of double rooms: ");
        scanf("%d", &book.double_rooms);
        if (book.single_rooms < 0 || book.double_rooms < 0 || (book.single_rooms + book.double_rooms == 0))
            printf("You must book at least one room.\n");
    } while (book.single_rooms < 0 || book.double_rooms < 0 || (book.single_rooms + book.double_rooms == 0));

    // Nights
    do
    {
        printf("Enter number of nights: ");
        scanf("%d", &book.nights);
    } while (book.nights <= 0);

    // Check-in date
    do
    {
        printf("Enter check-in month (1-12): ");
        scanf("%d", &book.checkin_month);
    } while (book.checkin_month < 1 || book.checkin_month > 12);

    if (book.checkin_month == 2 &&
        ((this_year % 4 == 0 && this_year % 100 != 0) || (this_year % 400 == 0)))
        days_in_month[2] = 29;
    else
        days_in_month[2] = 28;

    do
    {
        printf("Enter check-in day: ");
        scanf("%d", &book.checkin_day);
    } while (book.checkin_day < 1 || book.checkin_day > days_in_month[book.checkin_month]);

    book.checkin_year = this_year;
    if (book.checkin_month < this_month ||
        (book.checkin_month == this_month && book.checkin_day < today))
        book.checkin_year++;

    // Check-out date calculation
    struct tm checkin_tm = {0};
    checkin_tm.tm_year = book.checkin_year - 1900;
    checkin_tm.tm_mon = book.checkin_month - 1;
    checkin_tm.tm_mday = book.checkin_day + book.nights;
    mktime(&checkin_tm);

    int checkout_day = checkin_tm.tm_mday;
    int checkout_month = checkin_tm.tm_mon + 1;
    int checkout_year = checkin_tm.tm_year + 1900;

    // Booking ID
    int booking_id = getAvailableBookingID();

    // Assign rooms and save to file
    FILE *fp = fopen(ROOM_FILE, "a");
    if (!fp)
    {
        printf("Error: could not open bookings.csv\n");
        return 1;
    }

    int assigned = 0;
    int remaining_single = book.single_rooms;
    int remaining_double = book.double_rooms;
    Date date;
            date.day = book.checkin_day;
            date.month = book.checkin_month;
            date.year = book.checkin_year;
    for (int floor = 1; floor <= 9; floor++)
    {
        for (int i = 0; i < 10; i++)
        {
            int roomNum = floor * 100 + i;
            
            int isFree = isRoomFree("rooms.csv", roomNum);
            if (isFree)
            {
                if (remaining_single > 0 && i <= 4)
                {
                    fprintf(fp, "%d,%s,%04d-%02d-%02d,%04d-%02d-%02d,%d,%.2f,%.2f\n",
                            roomNum, book.name,
                            book.checkin_year, book.checkin_month, book.checkin_day,
                            checkout_year, checkout_month, checkout_day,
                            booking_id, 50.0f * book.nights, 50.0f * book.nights * 0.4f);
                    updateRoomStatus("rooms.csv", roomNum, "Booked");
                    remaining_single--;
                    assigned++;
                }
                else if (remaining_double > 0 && i >= 5)
                {
                    fprintf(fp, "%d,%s,%04d-%02d-%02d,%04d-%02d-%02d,%d,%.2f,%.2f\n",
                            roomNum, book.name,
                            book.checkin_year, book.checkin_month, book.checkin_day,
                            checkout_year, checkout_month, checkout_day,
                            booking_id, 75.0f * book.nights, 75.0f * book.nights * 0.4f);
                    updateRoomStatus("rooms.csv", roomNum, "Booked");
                    remaining_double--;
                    assigned++;
                }
            }

            if (remaining_single == 0 && remaining_double == 0)
                break;
        }
        if (remaining_single == 0 && remaining_double == 0)
            break;
    }

    fclose(fp);

    float single_total = book.single_rooms * 50.0f * book.nights;
    float double_total = book.double_rooms * 75.0f * book.nights;
    total = single_total + double_total;
    upfront = total * 0.4f;

    // Display
    printf("\n========== BOOKING SUMMARY ==========\n");
    printf("Guest: %s\n", book.name);
    printf("Booking ID: %04d\n", booking_id);
    printf("Check-in: %04d-%02d-%02d\n", book.checkin_year, book.checkin_month, book.checkin_day);
    printf("Check-out: %04d-%02d-%02d\n", checkout_year, checkout_month, checkout_day);
    printf("Room(s) Booked: %d (Single: %d, Double: %d)\n", assigned, book.single_rooms, book.double_rooms);
    printf("Total Price     : $%.2f\n", total);
    printf("Upfront Payment : $%.2f (40%%)\n", upfront);
    printf("=====================================\n");

    return 0;
}
