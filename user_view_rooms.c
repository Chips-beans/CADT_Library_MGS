#include <stdio.h>

#define FLOORS 9
#define ROOMS_PER_FLOOR 10

// Check if room is booked on a specific date
int isRoomBooked(const char *filename, int roomNum, int year, int month, int day)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
        return 0; // No bookings file means all rooms are free

    int bookedRoom;
    char guest[100];
    int startY, startM, startD, endY, endM, endD;

    int targetDate = year * 10000 + month * 100 + day;

    while (fscanf(fp, "%d,%[^,],%d-%d-%d,%d-%d-%d\n",
                  &bookedRoom, guest, &startY, &startM, &startD, &endY, &endM, &endD) == 8)
    {
        if (bookedRoom == roomNum)
        {
            int startDate = startY * 10000 + startM * 100 + startD;
            int endDate = endY * 10000 + endM * 100 + endD;
            if (targetDate >= startDate && targetDate < endDate)
            {
                fclose(fp);
                return 1;
            }
        }
    }

    fclose(fp);
    return 0;
}

int main()
{
    int year, month, day;

    // Ask the user for a date
    printf("Enter the date to check room availability.\n");
    printf("Year (e.g., 2025): ");
    scanf("%d", &year);
    printf("Month (1-12): ");
    scanf("%d", &month);
    printf("Day (1-31): ");
    scanf("%d", &day);

    // Basic validation
    if (year < 2024 || month < 1 || month > 12 || day < 1 || day > 31)
    {
        printf("Invalid date input.\n");
        return 1;
    }

    printf("\n=========== ROOM AVAILABILITY FOR %04d-%02d-%02d ===========\n", year, month, day);
    printf("Room Number\tType\tStatus\n");

    for (int floor = 1; floor <= FLOORS; floor++)
    {
        for (int room = 0; room < ROOMS_PER_FLOOR; room++)
        {
            int roomNum = floor * 100 + room;
            char *roomType = (room <= 4) ? "Single" : "Double";

            int booked = isRoomBooked("bookings.txt", roomNum, year, month, day);
            printf("%d\t\t%s\t%s\n", roomNum, roomType, booked ? "Booked" : "Free");
        }
    }

    return 0;
}
