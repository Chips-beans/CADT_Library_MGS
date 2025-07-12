#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "staff_check_in.h"

// Function prototypes
int find_booking(const char *guest_name, int booking_id, BookingSCheckIn bookings[], int max, int *found_count);
int load_room_status(RoomStatusSCheckIn rooms[], int max);
void save_room_status(RoomStatusSCheckIn rooms[], int count);
int generate_booking_id();
int assign_rooms(RoomStatusSCheckIn rooms[], int room_count, int single_req, int double_req, int checkin_year, int checkin_month, int checkin_day, int checkout_year, int checkout_month, int checkout_day, char guest_name[], BookingSCheckIn bookings[], int *booking_count);

int staffCheckIN()
{
    char choice;
    printf("================================================================================\n");
    printf("                              === Staff Check-in ===                            \n");
    printf("================================================================================\n");
    printf("                         Has the guest booked before? (Y/N): ");
    scanf(" %c", &choice);
    getchar(); // consume newline

    if (choice == 'Y' || choice == 'y')
    {
        char guest_name[MAX_NAME];
        int booking_id;
        printf("================================================================================\n");
        printf("                                  Enter guest name:                             \n");
        printf("================================================================================\n");
        fgets(guest_name, sizeof(guest_name), stdin);
        guest_name[strcspn(guest_name, "\n")] = 0;
        printf("================================================================================\n");
        printf("                                 Enter booking ID:                             \n");
        printf("================================================================================\n");
        scanf("%d", &booking_id);

        BookingSCheckIn bookings[100];
        int found_count = 0;
        if (!find_booking(guest_name, booking_id, bookings, 100, &found_count))
        {
            printf("================================================================================\n");
            printf("                    No matching booking found for %s with ID %d.               \n", guest_name, booking_id);
            printf("================================================================================\n");
            return 1;
        }

        printf("================================================================================\n");
        printf("                            Booking(s) found for %s:                            \n", guest_name);
        printf("================================================================================\n");
        for (int i = 0; i < found_count; i++)
        {
            printf("                         Room %d from %s to %s                             \n",
                   bookings[i].room_number, bookings[i].checkin_date, bookings[i].checkout_date);
        }

        RoomStatusSCheckIn rooms[100];
        int room_count = load_room_status(rooms, 100);
        int any_occupied = 0;
        for (int i = 0; i < found_count; i++)
        {
            int rn = bookings[i].room_number;
            int found_room = 0;

            for (int j = 0; j < room_count; j++)
            {
                if (rooms[j].room_number == rn)
                {
                    found_room = 1;
                    if (strcmp(rooms[j].status, "Occupied") == 0)
                    {
                        printf("================================================================================\n");
                        printf("                               Room %d already checked in.                       \n", rn);
                        printf("================================================================================\n");
                        any_occupied = 1;
                    }
                    else
                    {
                        strcpy(rooms[j].status, "Occupied");
                        strcpy(rooms[j].guest, guest_name);
                        printf("================================================================================\n");
                        printf("                                 Room %d checked in.                             \n", rn);
                        printf("================================================================================\n");
                        any_occupied = 1;
                    }
                    break;
                }
            }
            if (!found_room)
            {
                printf("================================================================================\n");
                printf("                            Room %d not found in room status file.               \n", rn);
                printf("================================================================================\n");
            }
        }

        if (any_occupied)
            save_room_status(rooms, room_count);
    }
    else if (choice == 'N' || choice == 'n')
    {
        // New booking flow inside staff check-in

        char guest_name[MAX_NAME];
        int single_rooms, double_rooms, nights;
        int checkin_day, checkin_month, checkin_year;
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        int today = tm.tm_mday;
        int this_month = tm.tm_mon + 1;
        int this_year = tm.tm_year + 1900;
        int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

        // Leap year check
        if ((this_year % 4 == 0 && this_year % 100 != 0) || (this_year % 400 == 0))
            days_in_month[2] = 29;

        printf("================================================================================\n");
        printf("                                 Enter guest name:                             \n");
        printf("================================================================================\n");
        fgets(guest_name, sizeof(guest_name), stdin);
        guest_name[strcspn(guest_name, "\n")] = 0;

        // Room counts input
        do
        {
            printf("================================================================================\n");
            printf("                        Number of single rooms to book:                        \n");
            printf("================================================================================\n");
            scanf("%d", &single_rooms);
            printf("================================================================================\n");
            printf("                        Number of double rooms to book:                        \n");
            printf("================================================================================\n");
            scanf("%d", &double_rooms);
            if (single_rooms < 0 || double_rooms < 0 || (single_rooms + double_rooms == 0))
            {
                printf("================================================================================\n");
                printf("                          You must book at least one room.                    \n");
                printf("================================================================================\n");
            }
        } while (single_rooms < 0 || double_rooms < 0 || (single_rooms + double_rooms == 0));

        do
        {
            printf("================================================================================\n");
            printf("                               Number of nights:                              \n");
            printf("================================================================================\n");
            scanf("%d", &nights);
        } while (nights <= 0);

        do
        {
            printf("================================================================================\n");
            printf("                               Check-in month (1-12):                        \n");
            printf("================================================================================\n");
            scanf("%d", &checkin_month);
        } while (checkin_month < 1 || checkin_month > 12);

        if (checkin_month == 2 &&
            ((this_year % 4 == 0 && this_year % 100 != 0) || (this_year % 400 == 0)))
            days_in_month[2] = 29;
        else
            days_in_month[2] = 28;

        do
        {
            printf("================================================================================\n");
            printf("                            Check-in day (1-%d):                             \n", days_in_month[checkin_month]);
            printf("================================================================================\n");
            scanf("%d", &checkin_day);
        } while (checkin_day < 1 || checkin_day > days_in_month[checkin_month]);

        checkin_year = this_year;
        if (checkin_month < this_month ||
            (checkin_month == this_month && checkin_day < today))
            checkin_year++;

        // Calculate checkout date
        struct tm checkin_tm = {0};
        checkin_tm.tm_year = checkin_year - 1900;
        checkin_tm.tm_mon = checkin_month - 1;
        checkin_tm.tm_mday = checkin_day + nights;
        mktime(&checkin_tm);

        int checkout_day = checkin_tm.tm_mday;
        int checkout_month = checkin_tm.tm_mon + 1;
        int checkout_year = checkin_tm.tm_year + 1900;

        // Load rooms and assign
        RoomStatusSCheckIn rooms[MAX_ROOMS_STAFF_CHECK_IN];
        int room_count = load_room_status(rooms, MAX_ROOMS_STAFF_CHECK_IN);

        BookingSCheckIn new_bookings[20];
        int booking_count = 0;

        int assigned_count = assign_rooms(rooms, room_count, single_rooms, double_rooms,
                                          checkin_year, checkin_month, checkin_day,
                                          checkout_year, checkout_month, checkout_day,
                                          guest_name, new_bookings, &booking_count);

        if (assigned_count == 0)
        {
            printf("================================================================================\n");
            printf("                         No rooms available to assign. Booking failed.          \n");
            printf("================================================================================\n");
            return 1;
        }

        // Save booking info to bookings.txt
        FILE *fpb = fopen(BOOKING_FILE_STAFF_CHECKIN, "a");
        if (!fpb)
        {
            printf("================================================================================\n");
            printf("                             Error saving bookings.                            \n");
            printf("================================================================================\n");
            return 1;
        }
        for (int i = 0; i < booking_count; i++)
        {
            fprintf(fpb, "%d,%s,%s,%s,%d,%.2f,%.2f\n",
                    new_bookings[i].room_number, new_bookings[i].guest,
                    new_bookings[i].checkin_date, new_bookings[i].checkout_date,
                    new_bookings[i].booking_id, new_bookings[i].total_price,
                    new_bookings[i].upfront_payment);
        }
        fclose(fpb);

        // Save updated room status
        save_room_status(rooms, room_count);

        printf("================================================================================\n");
        printf("                          Booking successful! Rooms assigned:                   \n");
        printf("================================================================================\n");
        for (int i = 0; i < booking_count; i++)
        {
            printf("                              Room %d (%s)                                   \n", new_bookings[i].room_number, rooms[i].room_type);
        }
    }
    else
    {
        printf("================================================================================\n");
        printf("                                Invalid choice.                                \n");
        printf("================================================================================\n");
        return 1;
    }

    return 0;
}

// --- FUNCTIONS IMPLEMENTATION ---

int find_booking(const char *guest_name, int booking_id, BookingSCheckIn bookings[], int max, int *found_count)
{
    FILE *fp = fopen(BOOKING_FILE_STAFF_CHECKIN, "r");
    if (!fp)
        return 0;

    char line[256];
    int count = 0;
    while (fgets(line, sizeof(line), fp) && count < max)
    {
        BookingSCheckIn b;
        if (sscanf(line, "%d,%99[^,],%10[^,],%10[^,],%d,%f,%f",
                   &b.room_number, b.guest, b.checkin_date, b.checkout_date,
                   &b.booking_id, &b.total_price, &b.upfront_payment) == 7)
        {
            if (strcmp(b.guest, guest_name) == 0 && b.booking_id == booking_id)
            {
                bookings[count++] = b;
            }
        }
    }
    fclose(fp);
    *found_count = count;
    return count > 0;
}

int load_room_status(RoomStatusSCheckIn rooms[], int max)
{
    FILE *fp = fopen(ROOM_STATUS_FILE, "r");
    if (!fp)
    {
        printf("Failed to open %s\n", ROOM_STATUS_FILE);
        return 0;
    }
    int count = 0;
    char line[256];

    while (fgets(line, sizeof(line), fp) && count < max)
    {
        RoomStatusSCheckIn r;
        int fields = sscanf(line, "%d,%9[^,],%19[^,],%[^,]",
                            &r.room_number, r.room_type, r.status, r.guest);
        if (fields == 4)
        {
            rooms[count++] = r;
        }
    }
    fclose(fp);
    return count;
}

void save_room_status(RoomStatusSCheckIn rooms[], int count)
{
    FILE *fp = fopen(ROOM_STATUS_FILE, "w");
    if (!fp)
    {
        printf("Error writing room status file.\n");
        return;
    }
    for (int i = 0; i < count; i++)
    {
        if (strlen(rooms[i].status_end) == 0)
        {
            strcpy(rooms[i].status_end, "-");
        }
        if (strlen(rooms[i].status_start) == 0)
        {
            strcpy(rooms[i].status_start, "-");
        }
        if (strlen(rooms[i].guest) == 0)
        {
            strcpy(rooms[i].guest, "-");
        }
    }
    for (int i = 0; i < count; i++)
    {
        fprintf(fp, "%d,%s,%s,%s,%s,%s\n",
                rooms[i].room_number, rooms[i].room_type,
                rooms[i].status, rooms[i].guest, rooms[i].status_start, rooms[i].status_end);
    }
    fclose(fp);
}

int generate_booking_id()
{
    FILE *fp = fopen("last_booking_id.txt", "r");
    int id = 999;
    if (fp)
    {
        fscanf(fp, "%d", &id);
        fclose(fp);
    }
    id++;
    if (id > 9999)
        id = 1000;
    fp = fopen("last_booking_id.txt", "w");
    if (fp)
    {
        fprintf(fp, "%d", id);
        fclose(fp);
    }
    return id;
}

// Assign rooms, update status, fill Booking array, return assigned count
int assign_rooms(RoomStatusSCheckIn rooms[], int room_count, int single_req, int double_req,
                 int checkin_year, int checkin_month, int checkin_day,
                 int checkout_year, int checkout_month, int checkout_day,
                 char guest_name[], BookingSCheckIn bookings[], int *booking_count)
{
    int assigned = 0;
    int single_left = single_req;
    int double_left = double_req;
    int booking_id = generate_booking_id();

    char checkin_date[11];
    char checkout_date[11];
    snprintf(checkin_date, sizeof(checkin_date), "%04d-%02d-%02d", checkin_year, checkin_month, checkin_day);
    snprintf(checkout_date, sizeof(checkout_date), "%04d-%02d-%02d", checkout_year, checkout_month, checkout_day);

    for (int floor = 1; floor <= 9 && (single_left > 0 || double_left > 0); floor++)
    {
        for (int i = 0; i < 10; i++)
        {
            int idx = (floor - 1) * 10 + i;
            if (idx >= room_count)
                break;
            if (single_left > 0 && i <= 4 && strcmp(rooms[idx].status, "Free") == 0)
            {
                rooms[idx].status[0] = 0;
                strcpy(rooms[idx].status, "Occupied");
                strcpy(rooms[idx].guest, guest_name);

                BookingSCheckIn b;
                b.room_number = rooms[idx].room_number;
                strcpy(b.guest, guest_name);
                strcpy(b.checkin_date, checkin_date);
                strcpy(b.checkout_date, checkout_date);
                b.booking_id = booking_id;
                b.total_price = 50.0f * (checkout_day - checkin_day);
                b.upfront_payment = b.total_price * 0.4f;

                bookings[(*booking_count)++] = b;

                single_left--;
                assigned++;
            }
            else if (double_left > 0 && i >= 5 && strcmp(rooms[idx].status, "Free") == 0)
            {
                rooms[idx].status[0] = 0;
                strcpy(rooms[idx].status, "Occupied");
                strcpy(rooms[idx].guest, guest_name);

                BookingSCheckIn b;
                b.room_number = rooms[idx].room_number;
                strcpy(b.guest, guest_name);
                strcpy(b.checkin_date, checkin_date);
                strcpy(b.checkout_date, checkout_date);
                b.booking_id = booking_id;
                b.total_price = 75.0f * (checkout_day - checkin_day);
                b.upfront_payment = b.total_price * 0.4f;

                bookings[(*booking_count)++] = b;

                double_left--;
                assigned++;
            }
            if (single_left == 0 && double_left == 0)
                break;
        }
    }
    return assigned;
}
