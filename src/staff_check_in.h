#ifndef STAFF_CHECK_IN_H
#define STAFF_CHECK_IN_H

#define MAX_NAME 100
#define ROOM_STATUS_FILE "rooms.csv"
#define BOOKING_FILE_STAFF_CHECKIN "bookings.csv"
#define MAX_ROOMS_STAFF_CHECK_IN 90

typedef struct
{
    int room_number;
    char room_type[10];
    char status[20];
    char guest[MAX_NAME];
    char status_start[11];
    char status_end[11];
} RoomStatusSCheckIn;

typedef struct
{
    int room_number;
    char guest[MAX_NAME];
    char checkin_date[11];  // YYYY-MM-DD
    char checkout_date[11]; // YYYY-MM-DD
    int booking_id;
    float total_price;
    float upfront_payment;
} BookingSCheckIn;

// Function prototypes
int find_booking(const char *guest_name, int booking_id, BookingSCheckIn bookings[], int max, int *found_count);
int load_room_status(RoomStatusSCheckIn rooms[], int max);
void save_room_status(RoomStatusSCheckIn rooms[], int count);
int generate_booking_id();
int assign_rooms(RoomStatusSCheckIn rooms[], int room_count, int single_req, int double_req, int checkin_year, int checkin_month, int checkin_day, int checkout_year, int checkout_month, int checkout_day, char guest_name[], BookingSCheckIn bookings[], int *booking_count);

int staffCheckIN();

#endif