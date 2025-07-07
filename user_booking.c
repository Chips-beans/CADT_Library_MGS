#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define FLOORS 9
#define ROOMS_PER_FLOOR 10

int valid_name(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!((str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= 'a' && str[i] <= 'z') || str[i] == ' '))
            return 0;
    }
    return 1;
}

int isRoomAvailable(const char *filename, int roomNum, int checkinY, int checkinM, int checkinD, int checkoutY, int checkoutM, int checkoutD) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return 1; // no bookings yet means available

    int bookedRoom;
    char guest[100];
    int y1, m1, d1, y2, m2, d2;
    int startA = checkinY * 10000 + checkinM * 100 + checkinD;
    int endA = checkoutY * 10000 + checkoutM * 100 + checkoutD;

    while (fscanf(fp, "%d,%[^,],%d-%d-%d,%d-%d-%d\n",
                  &bookedRoom, guest, &y1, &m1, &d1, &y2, &m2, &d2) == 8) {
        if (bookedRoom == roomNum) {
            int startB = y1 * 10000 + m1 * 100 + d1;
            int endB = y2 * 10000 + m2 * 100 + d2;

            if (!(endA <= startB || endB <= startA)) {
                fclose(fp);
                return 0;
            }
        }
    }
    fclose(fp);
    return 1;
}

void saveBooking(const char *filename, int roomNum, const char *guest, int y1, int m1, int d1, int y2, int m2, int d2) {
    FILE *fp = fopen(filename, "a");
    if (!fp) {
        printf("Error writing to booking file.\n");
        return;
    }
    fprintf(fp, "%d,%s,%04d-%02d-%02d,%04d-%02d-%02d\n",
            roomNum, guest, y1, m1, d1, y2, m2, d2);
    fclose(fp);
}

int main() {
    char name[100];
    int single_rooms, double_rooms, nights;
    int checkin_day, checkin_month, checkin_year;
    float total, upfront;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int days_in_month[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};

    printf("==============================================================\n");
    printf("                Welcome to the Hotel Booking System!         \n");
    printf("==============================================================\n");

    do {
        printf("Enter your name: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = 0;
        if (!valid_name(name) || strlen(name) == 0) {
            printf("Invalid name. Please enter a valid name (letters and spaces only).\n");
        }
    } while (!valid_name(name) || strlen(name) == 0);

    do {
        printf("Enter number of single bed rooms to book: ");
        scanf("%d", &single_rooms);
    } while (single_rooms < 0);

    do {
        printf("Enter number of double bed rooms to book: ");
        scanf("%d", &double_rooms);
    } while (double_rooms < 0);

    int total_rooms = single_rooms + double_rooms;
    while (total_rooms == 0) {
        printf("You must book at least one room.\n");
        do {
            printf("Enter number of single bed rooms to book: ");
            scanf("%d", &single_rooms);
        } while (single_rooms < 0);
        do {
            printf("Enter number of double bed rooms to book: ");
            scanf("%d", &double_rooms);
        } while (double_rooms < 0);
        total_rooms = single_rooms + double_rooms;
    }

    do {
        printf("Enter number of nights: ");
        scanf("%d", &nights);
    } while (nights <= 0);

    do {
        printf("Enter check-in month (1-12): ");
        scanf("%d", &checkin_month);
    } while (checkin_month < 1 || checkin_month > 12);

    checkin_year = tm.tm_year + 1900;
    if (checkin_month == 2 && ((checkin_year % 4 == 0 && checkin_year % 100 != 0) || (checkin_year % 400 == 0))) {
        days_in_month[2] = 29;
    } else {
        days_in_month[2] = 28;
    }

    do {
        printf("Enter check-in day (1-%d): ", days_in_month[checkin_month]);
        scanf("%d", &checkin_day);
    } while (checkin_day < 1 || checkin_day > days_in_month[checkin_month]);

    struct tm checkin_tm = {0};
    checkin_tm.tm_year = checkin_year - 1900;
    checkin_tm.tm_mon = checkin_month - 1;
    checkin_tm.tm_mday = checkin_day;

    time_t checkin_time = mktime(&checkin_tm);
    checkin_time += nights * 86400;

    struct tm checkout_tm = *localtime(&checkin_time);
    int checkout_year = checkout_tm.tm_year + 1900;
    int checkout_month = checkout_tm.tm_mon + 1;
    int checkout_day = checkout_tm.tm_mday;

    int assignedRooms[20];
    int assignedCount = 0;

    // Assign rooms floor by floor, singles (0-4) then doubles (5-9)
    int singlesLeft = single_rooms;
    int doublesLeft = double_rooms;

    for (int floor = 1; floor <= FLOORS && (singlesLeft > 0 || doublesLeft > 0); floor++) {
        // Assign single rooms on this floor
        for (int r = 0; r <=4 && singlesLeft > 0; r++) {
            int roomNumber = floor * 100 + r;
            if (isRoomAvailable("bookings.txt", roomNumber, checkin_year, checkin_month, checkin_day, checkout_year, checkout_month, checkout_day)) {
                assignedRooms[assignedCount++] = roomNumber;
                saveBooking("bookings.txt", roomNumber, name, checkin_year, checkin_month, checkin_day, checkout_year, checkout_month, checkout_day);
                singlesLeft--;
            }
        }
        // Assign double rooms on this floor
        for (int r = 5; r <=9 && doublesLeft > 0; r++) {
            int roomNumber = floor * 100 + r;
            if (isRoomAvailable("bookings.txt", roomNumber, checkin_year, checkin_month, checkin_day, checkout_year, checkout_month, checkout_day)) {
                assignedRooms[assignedCount++] = roomNumber;
                saveBooking("bookings.txt", roomNumber, name, checkin_year, checkin_month, checkin_day, checkout_year, checkout_month, checkout_day);
                doublesLeft--;
            }
        }
    }

    if (assignedCount == 0) {
        printf("No rooms could be assigned. Booking failed.\n");
        return 0;
    }

    if (singlesLeft > 0 || doublesLeft > 0) {
        printf("Warning: Not all requested rooms were available.\n");
    }

    total = (single_rooms * 50.0f + double_rooms * 75.0f) * nights;
    upfront = total * 0.4f;

    char weekday[20];
    strftime(weekday, sizeof(weekday), "%A", &checkin_tm);

    srand((unsigned int)time(NULL));
    int booking_id = 1000 + rand() % 9000;

    printf("\n==============================================================\n");
    printf("                    BOOKING SUMMARY                          \n");
    printf("==============================================================\n");
    printf("Hotel Name      : Villa Lumiere du MexisCADTel\n");
    printf("Guest Name      : %s\n", name);
    printf("Room(s) Booked  : ");
    for (int i = 0; i < assignedCount; i++) {
        printf("%d ", assignedRooms[i]);
    }
    printf("\n");
    printf("Room Type(s)    : ");
    if (single_rooms > 0 && double_rooms > 0) printf("Single & Double\n");
    else if (single_rooms > 0) printf("Single\n");
    else printf("Double\n");
    printf("Nights          : %d\n", nights);
    printf("Check-in Date   : %s, %04d-%02d-%02d at 2:00pm\n", weekday, checkin_year, checkin_month, checkin_day);
    printf("Total Price     : $%.2f\n", total);
    printf("Upfront Payment : $%.2f (40%%)\n", upfront);
    printf("--------------------------------------------------------------\n");
    printf("*** Your Booking ID: %04d ***\n", booking_id);
    printf("==============================================================\n");

    return 0;
}
