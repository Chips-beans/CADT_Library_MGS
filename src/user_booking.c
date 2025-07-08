#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME 100
#define ROOM_FILE "bookings.txt"
#define MAX_ROOMS 90

// Function to validate name input
int valid_name(const char *str) {
    for (int i = 0; str[i]; i++) {
        if (!((str[i] >= 'A' && str[i] <= 'Z') ||
              (str[i] >= 'a' && str[i] <= 'z') ||
              str[i] == ' '))
            return 0;
    }
    return 1;
}

// Function to reuse or generate new booking ID
int getAvailableBookingID() {
    FILE *fp = fopen("booking_ids.txt", "r");
    int id = -1;

    if (fp) {
        if (fscanf(fp, "%d", &id) == 1) {
            FILE *temp = fopen("temp_ids.txt", "w");
            int temp_id;
            while (fscanf(fp, "%d", &temp_id) == 1) {
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
    if (last) {
        fscanf(last, "%d", &last_id);
        fclose(last);
    }

    id = last_id + 1;
    if (id > 9999) id = 1000;

    last = fopen("last_id.txt", "w");
    fprintf(last, "%d", id);
    fclose(last);

    return id;
}

int main() {
    char name[MAX_NAME];
    int single_rooms, double_rooms, nights;
    int checkin_day, checkin_month, checkin_year;
    float total = 0, upfront = 0;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int today = tm.tm_mday;
    int this_month = tm.tm_mon + 1;
    int this_year = tm.tm_year + 1900;
    int days_in_month[] = {0, 31,28,31,30,31,30,31,31,30,31,30,31};

    // Leap year check
    if ((this_year % 4 == 0 && this_year % 100 != 0) || (this_year % 400 == 0))
        days_in_month[2] = 29;

    printf("============== HOTEL BOOKING SYSTEM ==============\n");

    // Name input
    do {
        printf("Enter your name: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = 0;
        if (!valid_name(name) || strlen(name) == 0)
            printf("Invalid name. Try again.\n");
    } while (!valid_name(name) || strlen(name) == 0);

    // Room selection
    do {
        printf("Enter number of single rooms: ");
        scanf("%d", &single_rooms);
        printf("Enter number of double rooms: ");
        scanf("%d", &double_rooms);
        if (single_rooms < 0 || double_rooms < 0 || (single_rooms + double_rooms == 0))
            printf("You must book at least one room.\n");
    } while (single_rooms < 0 || double_rooms < 0 || (single_rooms + double_rooms == 0));

    // Nights
    do {
        printf("Enter number of nights: ");
        scanf("%d", &nights);
    } while (nights <= 0);

    // Check-in date
    do {
        printf("Enter check-in month (1–12): ");
        scanf("%d", &checkin_month);
    } while (checkin_month < 1 || checkin_month > 12);

    if (checkin_month == 2 &&
        ((this_year % 4 == 0 && this_year % 100 != 0) || (this_year % 400 == 0)))
        days_in_month[2] = 29;
    else
        days_in_month[2] = 28;

    do {
        printf("Enter check-in day: ");
        scanf("%d", &checkin_day);
    } while (checkin_day < 1 || checkin_day > days_in_month[checkin_month]);

    checkin_year = this_year;
    if (checkin_month < this_month || 
       (checkin_month == this_month && checkin_day < today))
        checkin_year++;

    // Check-out date calculation
    struct tm checkin_tm = {0};
    checkin_tm.tm_year = checkin_year - 1900;
    checkin_tm.tm_mon = checkin_month - 1;
    checkin_tm.tm_mday = checkin_day + nights;
    mktime(&checkin_tm);

    int checkout_day = checkin_tm.tm_mday;
    int checkout_month = checkin_tm.tm_mon + 1;
    int checkout_year = checkin_tm.tm_year + 1900;

    // Booking ID
    int booking_id = getAvailableBookingID();

    // Assign rooms and save to file
    FILE *fp = fopen(ROOM_FILE, "a");
    if (!fp) {
        printf("Error: could not open bookings.txt\n");
        return 1;
    }

    int assigned = 0;
    int remaining_single = single_rooms;
    int remaining_double = double_rooms;

    for (int floor = 1; floor <= 9; floor++) {
        for (int i = 0; i < 10; i++) {
            int roomNum = floor * 100 + i;
            if (remaining_single > 0 && i <= 4) {
                fprintf(fp, "%d,%s,%04d-%02d-%02d,%04d-%02d-%02d,%d,%.2f,%.2f\n",
                        roomNum, name,
                        checkin_year, checkin_month, checkin_day,
                        checkout_year, checkout_month, checkout_day,
                        booking_id, 50.0f * nights, 50.0f * nights * 0.4f);
                remaining_single--;
                assigned++;
            } else if (remaining_double > 0 && i >= 5) {
                fprintf(fp, "%d,%s,%04d-%02d-%02d,%04d-%02d-%02d,%d,%.2f,%.2f\n",
                        roomNum, name,
                        checkin_year, checkin_month, checkin_day,
                        checkout_year, checkout_month, checkout_day,
                        booking_id, 75.0f * nights, 75.0f * nights * 0.4f);
                remaining_double--;
                assigned++;
            }

            if (remaining_single == 0 && remaining_double == 0)
                break;
        }
        if (remaining_single == 0 && remaining_double == 0)
            break;
    }

    fclose(fp);

    float single_total = single_rooms * 50.0f * nights;
    float double_total = double_rooms * 75.0f * nights;
    total = single_total + double_total;
    upfront = total * 0.4f;

    // Display
    printf("\n========== BOOKING SUMMARY ==========\n");
    printf("Guest: %s\n", name);
    printf("Booking ID: %04d\n", booking_id);
    printf("Check-in: %04d-%02d-%02d\n", checkin_year, checkin_month, checkin_day);
    printf("Check-out: %04d-%02d-%02d\n", checkout_year, checkout_month, checkout_day);
    printf("Room(s) Booked: %d (Single: %d, Double: %d)\n", assigned, single_rooms, double_rooms);
    printf("Total Price     : $%.2f\n", total);
    printf("Upfront Payment : $%.2f (40%%)\n", upfront);
    printf("=====================================\n");

    return 0;
}
