#ifndef USER_VIEW_BOOKING_H
#define USER_VIEW_BOOKING_H

// Struct to store all details of a booking record
typedef struct {
    int roomNumber;
    char guestName[100];
    int checkInYear, checkInMonth, checkInDay;
    int checkOutYear, checkOutMonth, checkOutDay;
    int bookingID;
    float totalAmount;
    float upfrontPaid;
} BookingViewRoom;
int userViewBooking();
#endif