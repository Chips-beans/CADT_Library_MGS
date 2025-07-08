#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "user_cancel.h"


// Calculate the number of days between two calendar dates
int calculateDaysBetween(int year1, int month1, int day1, int year2, int month2, int day2) {
    struct tm startDate = {0}, endDate = {0};
    startDate.tm_year = year1 - 1900;
    startDate.tm_mon = month1 - 1;
    startDate.tm_mday = day1;

    endDate.tm_year = year2 - 1900;
    endDate.tm_mon = month2 - 1;
    endDate.tm_mday = day2;

    time_t startTimestamp = mktime(&startDate);
    time_t endTimestamp = mktime(&endDate);
    double timeDifference = difftime(endTimestamp, startTimestamp);

    return (int)(timeDifference / (60 * 60 * 24)); // Convert seconds to days
}

// Read a booking record from file
int readBookingFromCSV(FILE *file, BookingCancel *booking) {
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
void writeBookingToCSV(FILE *file, const BookingCancel *booking) {
    fprintf(file, "%d,%s,%04d-%02d-%02d,%04d-%02d-%02d,%d,%.2f,%.2f\n",
            booking->roomNumber, booking->guestName,
            booking->checkInYear, booking->checkInMonth, booking->checkInDay,
            booking->checkOutYear, booking->checkOutMonth, booking->checkOutDay,
            booking->bookingID, booking->totalPrice, booking->upfrontPayment);
}


// Main function to cancel a booking by booking ID
int cancelBooking() {
    int enteredBookingID;                     // Booking ID entered by the user
    int isBookingFound = 0;                   // Flag to track if the booking ID was found
    int daysBeforeCheckIn = -1;               // Days remaining before check-in (used for refund policy)

    BookingCancel currentBooking;                   // Struct to hold each booking as it's read
    BookingCancel earliestCheckInBooking;           // Tracks the earliest check-in among matched bookings

    // Prompt user to enter booking ID
    printf("========== CANCEL BOOKING ==========\n");
    printf("Enter your 4-digit Booking ID: ");
    scanf("%d", &enteredBookingID);

    // Open main bookings file (for reading) and temporary file (for filtered bookings)
    FILE *bookingFile = fopen(BOOKING_CSV_FILE, "r");
    FILE *tempFile = fopen(TEMP_CSV_FILE, "w");
    if (!bookingFile || !tempFile) {
        printf("Error accessing booking files.\n");
        return 1;
    }

    // Get current date (used to compare with check-in date)
    time_t now = time(NULL);
    struct tm today = *localtime(&now);
    int currentYear = today.tm_year + 1900;
    int currentMonth = today.tm_mon + 1;
    int currentDay = today.tm_mday;

    int isFirstMatch = 1; // Used to track and store the earliest check-in among matching records

    // Process each booking record from the file
    while (readBookingFromCSV(bookingFile, &currentBooking)) {
        // Check if current booking matches the user's entered ID
        if (currentBooking.bookingID == enteredBookingID) {
            isBookingFound = 1;

            // Store the first matching booking or update if there's an earlier check-in
            if (isFirstMatch) {
                earliestCheckInBooking = currentBooking;
                isFirstMatch = 0;
            } else {
                // Compare current booking's check-in date with stored earliest
                int newCheckInDiff = calculateDaysBetween(currentYear, currentMonth, currentDay,
                                                          currentBooking.checkInYear, currentBooking.checkInMonth, currentBooking.checkInDay);
                int existingCheckInDiff = calculateDaysBetween(currentYear, currentMonth, currentDay,
                                                               earliestCheckInBooking.checkInYear, earliestCheckInBooking.checkInMonth, earliestCheckInBooking.checkInDay);
                // Update to earlier check-in date if needed
                if (newCheckInDiff < existingCheckInDiff) {
                    earliestCheckInBooking = currentBooking;
                }
            }

            // Show which room is being canceled
            printf("Canceled room %d for guest %s\n", currentBooking.roomNumber, currentBooking.guestName);
        } else {
            // Keep non-canceled bookings by writing them to the temp file
            writeBookingToCSV(tempFile, &currentBooking);
        }
    }

    // Close both booking files
    fclose(bookingFile);
    fclose(tempFile);

    // If the booking was found and canceled
    if (isBookingFound) {
        // Replace the old bookings file with the updated temp file
        remove(BOOKING_CSV_FILE);
        rename(TEMP_CSV_FILE, BOOKING_CSV_FILE);

        // Log the canceled booking ID for record-keeping
        FILE *canceledIDsFile = fopen(CANCELED_IDS_FILE, "a");
        if (canceledIDsFile) {
            fprintf(canceledIDsFile, "%d\n", enteredBookingID);
            fclose(canceledIDsFile);
        }

        // Calculate how many days before check-in the booking was canceled
        daysBeforeCheckIn = calculateDaysBetween(currentYear, currentMonth, currentDay,
                                                 earliestCheckInBooking.checkInYear,
                                                 earliestCheckInBooking.checkInMonth,
                                                 earliestCheckInBooking.checkInDay);

        // Display cancellation summary and refund policy
        printf("\n=========================================\n");
        printf(" Booking ID %d has been successfully canceled.\n", enteredBookingID);
        if (daysBeforeCheckIn >= 3) {
            printf(" You will receive a 40%% refund within 7 business days.\n");
        } else if (daysBeforeCheckIn >= 0) {
            printf(" No refund due to our hotel policy (less than 3 days before check-in).\n");
        } else {
            printf(" Your check-in date is already past. No refund issued.\n");
        }
        printf("=========================================\n");
    } else {
        // If booking wasn't found, clean up temp file and inform user
        remove(TEMP_CSV_FILE);
        printf(" Booking ID %d not found.\n", enteredBookingID);
    }

    return 0; // Program completed successfully
}
