#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BOOKING_FILE "bookings.txt"
#define TEMP_FILE "temp_bookings.txt"
#define ID_FILE "booking_ids.txt"

int daysBetween(int y1, int m1, int d1, int y2, int m2, int d2)
{
    struct tm date1 = {0}, date2 = {0};
    date1.tm_year = y1 - 1900;
    date1.tm_mon = m1 - 1;
    date1.tm_mday = d1;

    date2.tm_year = y2 - 1900;
    date2.tm_mon = m2 - 1;
    date2.tm_mday = d2;

    time_t t1 = mktime(&date1);
    time_t t2 = mktime(&date2);
    double seconds = difftime(t2, t1);

    return (int)(seconds / (60 * 60 * 24)); // convert to days
}

int main()
{
    int cancel_id;
    int found = 0;
    int earliest_y, earliest_m, earliest_d;
    int refund_days = -1;

    printf("========== CANCEL BOOKING ==========\n");
    printf("Enter your 4-digit Booking ID: ");
    scanf("%d", &cancel_id);

    FILE *in = fopen(BOOKING_FILE, "r");
    FILE *out = fopen(TEMP_FILE, "w");

    if (!in || !out)
    {
        printf("Error accessing booking files.\n");
        return 1;
    }

    int room, id;
    char name[100];
    int y1, m1, d1, y2, m2, d2;
    int first_found = 1;

    // Get today's date
    time_t t = time(NULL);
    struct tm today = *localtime(&t);
    int ty = today.tm_year + 1900;
    int tm = today.tm_mon + 1;
    int td = today.tm_mday;

    while (fscanf(in, "%d,%99[^,],%d-%d-%d,%d-%d-%d,%d\n",
                  &room, name,
                  &y1, &m1, &d1,
                  &y2, &m2, &d2, &id) == 9)
    {
        if (id == cancel_id)
        {
            found = 1;
            if (first_found)
            {
                earliest_y = y1;
                earliest_m = m1;
                earliest_d = d1;
                first_found = 0;
            }
            else
            {
                // Keep the earliest check-in if multiple rooms
                int cmp = daysBetween(ty, tm, td, y1, m1, d1);
                int prev = daysBetween(ty, tm, td, earliest_y, earliest_m, earliest_d);
                if (cmp < prev)
                {
                    earliest_y = y1;
                    earliest_m = m1;
                    earliest_d = d1;
                }
            }
            printf("Canceled room %d for guest %s\n", room, name);
        }
        else
        {
            fprintf(out, "%d,%s,%04d-%02d-%02d,%04d-%02d-%02d,%d\n",
                    room, name,
                    y1, m1, d1,
                    y2, m2, d2, id);
        }
    }

    fclose(in);
    fclose(out);

    if (found)
    {
        remove(BOOKING_FILE);
        rename(TEMP_FILE, BOOKING_FILE);

        FILE *idfile = fopen(ID_FILE, "a");
        if (idfile)
        {
            fprintf(idfile, "%d\n", cancel_id);
            fclose(idfile);
        }

        refund_days = daysBetween(ty, tm, td, earliest_y, earliest_m, earliest_d);

        printf("\n=========================================\n");
        printf(" Booking ID %d has been successfully canceled.\n", cancel_id);
        if (refund_days >= 3)
        {
            printf(" You will receive a 40%% refund within 7 business days.\n");
        }
        else if (refund_days >= 0)
        {
            printf(" No refund due to our hotel policy (less than 3 days before check-in).\n");
        }
        else
        {
            printf(" Your check-in date is already past. No refund issued.\n");
        }
        printf("=========================================\n");
    }
    else
    {
        remove(TEMP_FILE);
        printf(" Booking ID %d not found.\n", cancel_id);
    }

    return 0;
}
