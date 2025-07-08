#ifndef CHECKIN_H
#define CHECKIN_H

typedef struct {
    int roomNumber;
    char guestName[100];
    int checkInYear, checkInMonth, checkInDay;
    int checkOutYear, checkOutMonth, checkOutDay;
    int bookingID;
    float totalPrice;
    float upfrontPayment;
} CheckIN;

#define BOOKING_CSV_FILE "bookings.csv"
#define CHECKIN_CSV_FILE "checkin.csv"
int checkInGuest();
#endif
