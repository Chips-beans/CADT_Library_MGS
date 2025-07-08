#ifndef USER_BOOKING_h
#define USER_BOOKING_h

#define MAX_NAME 100
typedef struct 
{
    char name[MAX_NAME];
    int single_rooms, double_rooms, nights;
    int checkin_day, checkin_month, checkin_year;
}Booking;


int valid_name(const char *str);
int getAvailableBookingID();
int bookRoom();
#endif