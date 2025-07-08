#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user_view_booking.h"

#define MAX_LINE_LENGTH 256
#define BOOKING_CSV_FILENAME "bookings.csv"

// Parses a line from CSV into a Booking struct
int parseBookingFromCSVLine(const char *csvLine, BookingViewRoom *booking) {
    char checkInDateStr[11], checkOutDateStr[11];
    int parsedFields = sscanf(csvLine, "%d,%99[^,],%10[^,],%10[^,],%d,%f,%f",
                              &booking->roomNumber, booking->guestName,
                              checkInDateStr, checkOutDateStr,
                              &booking->bookingID, &booking->totalAmount, &booking->upfrontPaid);

    if (parsedFields != 7)
        return 0;

    // Parse check-in and check-out date strings into integers
    sscanf(checkInDateStr, "%d-%d-%d", &booking->checkInYear, &booking->checkInMonth, &booking->checkInDay);
    sscanf(checkOutDateStr, "%d-%d-%d", &booking->checkOutYear, &booking->checkOutMonth, &booking->checkOutDay);

    return 1; // Success
}

// Displays a booking in a user-friendly format
void displayBookingInfo(const BookingViewRoom *booking) {
    printf("Room Number : %d\n", booking->roomNumber);
    printf("Guest Name  : %s\n", booking->guestName);
    printf("Check-in    : %04d-%02d-%02d\n", booking->checkInYear, booking->checkInMonth, booking->checkInDay);
    printf("Check-out   : %04d-%02d-%02d\n", booking->checkOutYear, booking->checkOutMonth, booking->checkOutDay);
    printf("Booking ID  : %04d\n", booking->bookingID);
    printf("Total Amount: $%.2f\n", booking->totalAmount);
    printf("Upfront Paid: $%.2f\n", booking->upfrontPaid);
    printf("-----------------------------\n");
}

int userViewBooking(char userName[]) {
    char searchName[100];                      // Name input from user
    char csvLine[MAX_LINE_LENGTH];             // Buffer to store each line from file
    int isBookingFound = 0;                    // Flag to track if any matching booking was found

    printf("========== VIEW BOOKING ==========\n");
    strcpy(searchName, userName);
    // Open the CSV file for reading
    FILE *bookingFile = fopen(BOOKING_CSV_FILENAME, "r");
    if (!bookingFile) {
        printf("Error: Could not open file '%s'\n", BOOKING_CSV_FILENAME);
        return 1;
    }

    printf("\n========= BOOKING INFO =========\n");

    // Read and process each line of the file
    while (fgets(csvLine, sizeof(csvLine), bookingFile)) {
        BookingViewRoom booking;
        if (parseBookingFromCSVLine(csvLine, &booking)) {
            // Compare input name with guest name from the file
            if (strcmp(booking.guestName, searchName) == 0) {
                displayBookingInfo(&booking);
                isBookingFound = 1;
            }
        }
    }

    // If no bookings matched the input name
    if (!isBookingFound) {
        printf("No bookings found for '%s'.\n", searchName);
    }

    printf("==============================\n");

    fclose(bookingFile); // Close the file
    return 0;
}
