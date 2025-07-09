#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "checkIn.h"

int readCheckInFromCSV(FILE *file, CheckIN *booking) {
    char line[256];
    if (fgets(line, sizeof(line), file) == NULL) return 0;

    // Remove newline character if present
    line[strcspn(line, "\n")] = 0;

    // Tokenize line
    char *token = strtok(line, ",");
    if (!token) return 0;
    booking->roomNumber = atoi(token);

    token = strtok(NULL, ",");
    if (!token) return 0;
    strcpy(booking->guestName, token);

    token = strtok(NULL, ",");
    if (!token) return 0;
    sscanf(token, "%d-%d-%d", &booking->checkInYear, &booking->checkInMonth, &booking->checkInDay);

    token = strtok(NULL, ",");
    if (!token) return 0;
    sscanf(token, "%d-%d-%d", &booking->checkOutYear, &booking->checkOutMonth, &booking->checkOutDay);

    token = strtok(NULL, ",");
    if (!token) return 0;
    booking->bookingID = atoi(token);

    token = strtok(NULL, ",");
    if (!token) return 0;
    booking->totalPrice = atof(token);

    token = strtok(NULL, ",");
    if (!token) return 0;
    booking->upfrontPayment = atof(token);

    return 1;
}


// Write a booking record to file
void writeCheckInToCSV(FILE *file, const CheckIN *booking) {
    fprintf(file, "%d,%s,%04d-%02d-%02d,%04d-%02d-%02d,%d,%.2f,%.2f\n",
            booking->roomNumber, booking->guestName,
            booking->checkInYear, booking->checkInMonth, booking->checkInDay,
            booking->checkOutYear, booking->checkOutMonth, booking->checkOutDay,
            booking->bookingID, booking->totalPrice, booking->upfrontPayment);
}

int checkInGuest() {
    int inputBookingID;
    char inputGuestName[100];
    CheckIN booking;
    int found = 0;

    printf("========== GUEST CHECK-IN ==========\n");
    printf("Enter 4-digit Booking ID to check in: ");
    scanf("%d", &inputBookingID);
    getchar();
    printf("Enter Guest Name (or press Enter to skip): ");
    fgets(inputGuestName, sizeof(inputGuestName), stdin);

    // Remove trailing newline
    inputGuestName[strcspn(inputGuestName, "\n")] = '\0';

    FILE *bookingFile = fopen(BOOKING_CSV_FILE, "r");
    FILE *checkinFile = fopen(CHECKIN_CSV_FILE, "a");

    if (!bookingFile || !checkinFile) {
        printf("Error opening files.\n");
        return 1;
    }

    time_t now = time(NULL);
    struct tm today = *localtime(&now);
    int currentYear = today.tm_year + 1900;
    int currentMonth = today.tm_mon + 1;
    int currentDay = today.tm_mday;

    // Read and skip CSV header if present
    char header[256];

    while (readCheckInFromCSV(bookingFile, &booking)) {
        if (booking.bookingID == inputBookingID &&
            (strlen(inputGuestName) == 0 || strcmp(booking.guestName, inputGuestName) == 0)) {
            found = 1;

            if (booking.checkInYear == currentYear &&
                booking.checkInMonth == currentMonth &&
                booking.checkInDay == currentDay) {

                writeCheckInToCSV(checkinFile, &booking);

                printf("\nCheck-in successful!\n");
                printf(" Guest: %s\n", booking.guestName);
                printf(" Room: %d\n", booking.roomNumber);
                printf(" Check-in Date: %04d-%02d-%02d\n",
                       booking.checkInYear, booking.checkInMonth, booking.checkInDay);
                printf("=====================================\n");

            } else {
                printf("\nCannot check in guest %s today.\n", booking.guestName);
                printf(" Scheduled date: %04d-%02d-%02d\n", 
                        booking.checkInYear, booking.checkInMonth, booking.checkInDay);
            }
        }
    }

    if (!found) {
        printf("No matching booking found for Booking ID %d.\n", inputBookingID);
    }

    fclose(bookingFile);
    fclose(checkinFile);
    return 0;
}
