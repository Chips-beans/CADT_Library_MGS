#ifndef USER_VIEW_ROOMS_H
#define USER_VIEW_ROOMS_H

#define FLOORS 9
#define ROOMS_PER_FLOOR 10

typedef struct {
    int year, month, day;
} Date;

typedef enum {
    SINGLE,
    DOUBLE
} RoomType;
typedef struct {
    int roomNumber;
    char username[100];
    Date checkinDate;
    Date checkoutDate;
    char bookingID[20];
    float totalPrice;
    float upfrontPayment;
} ViewRoom;
int isRoomBooked(const char *filename, int roomNum, Date targetDate);
int userViewRoom();

#endif