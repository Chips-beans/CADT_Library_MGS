#include <stdio.h>
#include <string.h>

void viewFreeRoomsWithPrice(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: Could not open rooms file.\n");
        return;
    }

    char roomNumber[10], roomType[10], status[10], guest[50];
    int foundFree = 0;
    int price = 0;

    printf("Available Rooms:\n");
    printf("-----------------\n");

    while (fscanf(fp, "%[^,],%[^,],%[^,],%s\n", roomNumber, roomType, status, guest) == 4) {
        if (strcmp(status, "Free") == 0) {
            if (strcmp(roomType, "Single") == 0) {
                price = 50;
            } else if (strcmp(roomType, "Double") == 0) {
                price = 75;
            } else {
                price = 0; // default price if unknown type
            }
            printf("Room %s (%s) - $%d per night\n", roomNumber, roomType, price);
            foundFree = 1;
        }
    }

    if (!foundFree) {
        printf("No free rooms available right now.\n");
    }

    fclose(fp);
}

int main() {
    viewFreeRoomsWithPrice("rooms.txt");
    return 0;
}
