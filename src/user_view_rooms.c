#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user_view_rooms.h"

// Convert a date string "YYYY-MM-DD" to Date struct
Date parseDate(const char *dateStr)
{
    Date d;
    sscanf(dateStr, "%d-%d-%d", &d.year, &d.month, &d.day);
    return d;
}

// Convert Date to YYYYMMDD integer for comparison
int dateToInt(Date d)
{
    return d.year * 10000 + d.month * 100 + d.day;
}

// Determine room type from index
RoomType getRoomType(int roomIndex)
{
    return (roomIndex <= 4) ? SINGLE : DOUBLE;
}

// Room type to string
const char *roomTypeToString(RoomType type)
{
    return (type == SINGLE) ? "Single" : "Double";
}

// Room price by type
int getRoomPrice(RoomType type)
{
    return (type == SINGLE) ? 50 : 70;
}

// Check if a room is booked on a specific date
int isRoomBooked(const char *filename, int roomNum, Date targetDate)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        perror("Failed to open booking file");
        return 0;
    }

    char line[256];
    ViewRoom booking;
    char checkinStr[11], checkoutStr[11];

    while (fgets(line, sizeof(line), fp))
    {
        int fields = sscanf(line, "%d,%99[^,],%10[^,],%10[^,],%19[^,],%f,%f",
                            &booking.roomNumber, booking.username,
                            checkinStr, checkoutStr,
                            booking.bookingID, &booking.totalPrice, &booking.upfrontPayment);
        if (fields == 7)
        {
            booking.checkinDate = parseDate(checkinStr);
            booking.checkoutDate = parseDate(checkoutStr);

            if (booking.roomNumber == roomNum)
            {
                int start = dateToInt(booking.checkinDate);
                int end = dateToInt(booking.checkoutDate);
                int target = dateToInt(targetDate);

                if (target >= start && target < end)
                {
                    fclose(fp);
                    return 1; // Booked
                }
            }
        }
    }

    fclose(fp);
    return 0; // Not booked
}

// Ask user for date
Date inputDate()
{
    Date d;
    printf("Enter the date to check room availability.\n");
    printf("Year (e.g., 2025): ");
    scanf("%d", &d.year);
    printf("Month (1-12): ");
    scanf("%d", &d.month);
    printf("Day (1-31): ");
    scanf("%d", &d.day);
    return d;
}

// Validate user input date
int isValidDate(Date d)
{
    return (d.year >= 2024 && d.month >= 1 && d.month <= 12 && d.day >= 1 && d.day <= 31);
}

int userViewRoom()
{
    Date checkDate = inputDate();

    if (!isValidDate(checkDate))
    {
        printf("Invalid date input.\n");
        return 1;
    }

    printf("\n=========== ROOM AVAILABILITY FOR %04d-%02d-%02d ===========\n",
           checkDate.year, checkDate.month, checkDate.day);
    printf("Room Number\tType\t\tStatus\t\tPrice\n");

    for (int floor = 1; floor <= FLOORS; floor++)
    {
        for (int room = 0; room < ROOMS_PER_FLOOR; room++)
        {
            int roomNum = floor * 100 + room;
            RoomType type = getRoomType(room);
            int price = getRoomPrice(type);
            int booked = isRoomBooked("bookings.csv", roomNum, checkDate);

            printf("%d\t\t%-7s\t%-7s\t$%d\n",
                   roomNum,
                   roomTypeToString(type),
                   booked ? "Booked" : "Free",
                   price);
        }
    }

    return 0;
}
