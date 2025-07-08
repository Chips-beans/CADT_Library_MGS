#ifndef USER_BOOKING_h
#define USER_BOOKING_h

int valid_name(const char *str);
int isRoomAvailable(const char *filename, 
    int roomNum, 
    int checkinY, 
    int checkinM, 
    int checkinD, 
    int checkoutY, 
    int checkoutM, 
    int checkoutD);
void saveBooking(const char *filename, 
    int roomNum, 
    const char *guest, 
    int y1, 
    int m1, 
    int d1, 
    int y2, 
    int m2, 
    int d2);
#endif