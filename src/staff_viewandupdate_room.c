#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ROOMS 100
#define MAX_LINE 256

typedef struct {
    int room_number;
    char room_type[10];    // "Single" or "Double"
    char status[10];       // "Free", "Occupied", "Down"
    char guest_name[50];
    char status_start[11]; // YYYY-MM-DD or empty
    char status_end[11];   // YYYY-MM-DD or empty
} RoomStatus;

int parse_date(const char *date_str, struct tm *date_tm) {
    if (strlen(date_str) != 10) return 0;
    if (sscanf(date_str, "%4d-%2d-%2d", &date_tm->tm_year, &date_tm->tm_mon, &date_tm->tm_mday) != 3)
        return 0;
    date_tm->tm_year -= 1900;
    date_tm->tm_mon -= 1;
    date_tm->tm_hour = 0; date_tm->tm_min = 0; date_tm->tm_sec = 0;
    return 1;
}

int date_in_range(struct tm check_date, struct tm start_date, struct tm end_date) {
    time_t t_check = mktime(&check_date);
    time_t t_start = mktime(&start_date);
    time_t t_end = mktime(&end_date);
    return (t_check >= t_start && t_check <= t_end);
}

int load_rooms(RoomStatus rooms[], int *count) {
    FILE *fp = fopen("rooms.csv", "r");
    if (!fp) {
        printf("Error opening rooms.csv\n");
        return 0;
    }
    char line[MAX_LINE];
    int idx = 0;
    // Skip header line if you have one, or comment this out if no header:
    fgets(line, sizeof(line), fp);
    while (fgets(line, sizeof(line), fp) && idx < MAX_ROOMS) {
        char *token = strtok(line, ",");
        if (!token) continue;
        rooms[idx].room_number = atoi(token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy(rooms[idx].room_type, token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy(rooms[idx].status, token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy(rooms[idx].guest_name, token);

        token = strtok(NULL, ",");
        if (token) {
            token[strcspn(token, "\n")] = 0; // remove newline
            strcpy(rooms[idx].status_start, token);
        } else {
            rooms[idx].status_start[0] = 0;
        }

        token = strtok(NULL, ",");
        if (token) {
            token[strcspn(token, "\n")] = 0;
            strcpy(rooms[idx].status_end, token);
        } else {
            rooms[idx].status_end[0] = 0;
        }
        idx++;
    }
    fclose(fp);
    *count = idx;
    return 1;
}

void save_rooms(RoomStatus rooms[], int count) {
    FILE *fp = fopen("rooms.csv", "w");
    if (!fp) {
        printf("Error writing to rooms.csv\n");
        return;
    }
    // Write header (optional, comment if not needed)
    fprintf(fp, "RoomNumber,RoomType,Status,GuestName,StatusStartDate,StatusEndDate\n");
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d,%s,%s,%s,%s,%s\n",
            rooms[i].room_number,
            rooms[i].room_type,
            rooms[i].status,
            rooms[i].guest_name,
            rooms[i].status_start,
            rooms[i].status_end);
    }
    fclose(fp);
}

void print_rooms_for_date(RoomStatus rooms[], int count, const char *date_str) {
    struct tm check_date = {0};
    if (!parse_date(date_str, &check_date)) {
        printf("Invalid date format.\n");
        return;
    }

    printf("\nRoom Status on %s:\n", date_str);
    printf("RoomNumber | RoomType | Status   | GuestName\n");
    printf("---------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        int is_down = 0;
        if (strcmp(rooms[i].status, "Down") == 0 && strlen(rooms[i].status_start) > 0 && strlen(rooms[i].status_end) > 0) {
            struct tm start_date = {0}, end_date = {0};
            parse_date(rooms[i].status_start, &start_date);
            parse_date(rooms[i].status_end, &end_date);
            if (date_in_range(check_date, start_date, end_date))
                is_down = 1;
        }
        if (is_down) {
            printf("%9d | %-8s | %-8s | %s\n", rooms[i].room_number, rooms[i].room_type, "Down", rooms[i].guest_name);
        } else {
            printf("%9d | %-8s | %-8s | %s\n", rooms[i].room_number, rooms[i].room_type, rooms[i].status, rooms[i].guest_name);
        }
    }
}

void update_room_status(RoomStatus rooms[], int count, const char *date_str) {
    int room_to_update;
    char new_status[10];
    char start_date[11], end_date[11];

    printf("Enter room number to update: ");
    scanf("%d", &room_to_update);

    int idx = -1;
    for (int i = 0; i < count; i++) {
        if (rooms[i].room_number == room_to_update) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        printf("Room number %d not found.\n", room_to_update);
        return;
    }

    printf("Current status of room %d is %s.\n", room_to_update, rooms[idx].status);

    printf("Enter new status (Free, Down): ");
    scanf("%s", new_status);

    if (strcmp(new_status, "Down") == 0) {
        printf("Enter downtime start date (YYYY-MM-DD): ");
        scanf("%s", start_date);
        printf("Enter downtime end date (YYYY-MM-DD): ");
        scanf("%s", end_date);

        struct tm tmp;
        if (!parse_date(start_date, &tmp) || !parse_date(end_date, &tmp)) {
            printf("Invalid date format.\n");
            return;
        }

        strcpy(rooms[idx].status, "Down");
        strcpy(rooms[idx].status_start, start_date);
        strcpy(rooms[idx].status_end, end_date);
        strcpy(rooms[idx].guest_name, "-");
        printf("Room %d marked as Down from %s to %s.\n", room_to_update, start_date, end_date);
    } else if (strcmp(new_status, "Free") == 0) {
        strcpy(rooms[idx].status, "Free");
        rooms[idx].status_start[0] = 0;
        rooms[idx].status_end[0] = 0;
        strcpy(rooms[idx].guest_name, "-");
        printf("Room %d status updated to Free.\n", room_to_update);
    } else {
        printf("Unsupported status update.\n");
    }
}

int main() {
    RoomStatus rooms[MAX_ROOMS];
    int room_count = 0;
    char input_date[11];
    int choice;

    if (!load_rooms(rooms, &room_count)) {
        return 1;
    }

    printf("=== Staff View/Update Room Status ===\n");
    printf("Enter date to view/update room status (YYYY-MM-DD): ");
    scanf("%10s", input_date);

    print_rooms_for_date(rooms, room_count, input_date);

    printf("\nDo you want to update room status? (1=Yes, 0=No): ");
    scanf("%d", &choice);

    if (choice == 1) {
        update_room_status(rooms, room_count, input_date);
        save_rooms(rooms, room_count);
    }

    printf("Done.\n");
    return 0;
}
