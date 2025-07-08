#ifndef USER_BOOKING_h
#define USER_BOOKING_h

#define MAX_NAME 100

typedef struct
{
    int room_number;
    char guest_name[MAX_NAME];
    int checkin_year, checkin_month, checkin_day;
    int checkout_year, checkout_month, checkout_day;
    int booking_id;
} Booking;
int valid_name(const char *str);
int getAvailableBookingID();
void saveBookingToFile(Booking b);
#endif