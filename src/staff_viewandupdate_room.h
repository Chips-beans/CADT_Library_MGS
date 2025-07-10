#ifndef VIEW_UPDATE_ROOM_STATUS_H
#define VIEW_UPDATE_ROOM_STATUS_H

#include <time.h>

#define MAX_ROOMS_VIEW_AND_UPDATE 100
#define MAX_LINE 256

typedef struct
{
    int room_number;
    char room_type[10];     // "Single" or "Double"
    char status[10];        // "Free", "Occupied", "Down"
    char guest_name[50];
    char status_start[11];  // YYYY-MM-DD
    char status_end[11];    // YYYY-MM-DD
} RoomStatusVU;

// Function declarations
int parse_date(const char *date_str, struct tm *date_tm);
int date_in_range(struct tm check_date, struct tm start_date, struct tm end_date);

int load_rooms(RoomStatusVU rooms[], int *count);
void save_rooms(RoomStatusVU rooms[], int count);

void print_rooms_for_date(RoomStatusVU rooms[], int count, const char *date_str);
void update_room_status_vu(RoomStatusVU rooms[], int count, const char *date_str);
int staffViewAndUpdateRoomStatus();

#endif // ROOM_STATUS_H
