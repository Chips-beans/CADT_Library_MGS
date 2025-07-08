#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "checkIn.h"  // Make sure this contains the BookingCancel struct

int readBookingFromCSV(FILE *file, CheckIN *booking);
void writeBookingToCSV(FILE *file, const CheckIN *booking);

int checkInGuest() {
    int inputBookingID;
    char inputGuestName[100];
    CheckIN booking;
    int found = 0;

    printf("========== GUEST CHECK-IN ==========\n");
    printf("Enter 4-digit Booking ID to check in: ");
    scanf("%d", &inputBookingID);
    getchar();
    printf("Enter Guest Name: ");
    fgets(inputGuestName, sizeof(inputGuestName),stdin);

    FILE *bookingFile = fopen(BOOKING_CSV_FILE, "r");
    FILE *checkinFile = fopen(CHECKIN_CSV_FILE, "a"); // Append mode

    if (!bookingFile || !checkinFile) {
        printf("Error opening booking/check-in file.\n");
        return 1;
    }

    char header[256];

    // Get today's date
    time_t now = time(NULL);
    struct tm today = *localtime(&now);
    int currentYear = today.tm_year + 1900;
    int currentMonth = today.tm_mon + 1;
    int currentDay = today.tm_mday;

    while (readBookingFromCSV(bookingFile, &booking)) {
        if (booking.bookingID == inputBookingID && booking.guestName == inputGuestName) {
            found = 1;

            // Check if today matches the check-in date
            if (booking.checkInYear == currentYear &&
                booking.checkInMonth == currentMonth &&
                booking.checkInDay == currentDay) {

                // Log the check-in
                writeBookingToCSV(checkinFile, &booking);

                printf("\n✅ Check-in successful!\n");
                printf(" Guest: %s\n", booking.guestName);
                printf(" Room: %d\n", booking.roomNumber);
                printf(" Check-in Date: %04d-%02d-%02d\n",
                       booking.checkInYear, booking.checkInMonth, booking.checkInDay);
                printf("=====================================\n");

            } else {
                printf("\nCannot check in today.\n");
                printf(" Guest %s is scheduled to check in on %04d-%02d-%02d.\n",
                       booking.guestName,
                       booking.checkInYear, booking.checkInMonth, booking.checkInDay);
                printf(" Today's date is %04d-%02d-%02d.\n",
                       currentYear, currentMonth, currentDay);
            }

            break; // Stop searching
        }
    }

    if (!found) {
        printf("Booking ID %d not found.\n", inputBookingID);
    }

    fclose(bookingFile);
    fclose(checkinFile);
    return 0;
}
