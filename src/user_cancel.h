
#ifndef USER_CANCEL_H
#define USER_CANCEL_H

// CSV File paths
#define BOOKING_CSV_FILE "bookings.csv"
#define TEMP_CSV_FILE "temp_bookings.csv"
#define CANCELED_IDS_FILE "canceled_ids.csv"
#define MAX_GUEST_NAME_LENGTH 100

typedef struct {
    int roomNumber;
    char guestName[100];
    int checkInYear, checkInMonth, checkInDay;
    int checkOutYear, checkOutMonth, checkOutDay;
    int bookingID;
    float totalPrice;
    float upfrontPayment;
} BookingCancel;
int cancelBooking();
#endif
