#include <stdio.h>
#include <string.h>

#define MAX_LINE 256

int main() {
    char inputName[100];
    char line[MAX_LINE];
    int found = 0;

    printf("========== VIEW BOOKING ==========\n");
    printf("Enter your name: ");
    fgets(inputName, sizeof(inputName), stdin);
    inputName[strcspn(inputName, "\n")] = 0; // remove trailing newline

    FILE *fp = fopen("bookings.txt", "r");
    if (!fp) {
        printf("Error: could not open bookings.txt\n");
        return 1;
    }

    printf("\n========= BOOKING INFO =========\n");
    while (fgets(line, sizeof(line), fp)) {
        int roomNum, bookingID;
        char username[100];
        int y1, m1, d1, y2, m2, d2;
        float totalPrice, upfrontPayment;

        // Parse line with 7 fields
        int count = sscanf(line, "%d,%99[^,],%d-%d-%d,%d-%d-%d,%d,%f,%f",
                           &roomNum, username,
                           &y1, &m1, &d1,
                           &y2, &m2, &d2,
                           &bookingID, &totalPrice, &upfrontPayment);

        if (count == 11) { // All fields parsed correctly
            if (strcmp(username, inputName) == 0) {
                found = 1;
                printf("Room Number : %d\n", roomNum);
                printf("Check-in    : %04d-%02d-%02d\n", y1, m1, d1);
                printf("Check-out   : %04d-%02d-%02d\n", y2, m2, d2);
                printf("Booking ID  : %04d\n", bookingID);
                printf("Total Price : $%.2f\n", totalPrice);
                printf("Upfront Paid: $%.2f\n", upfrontPayment);
                printf("-----------------------------\n");
            }
        }
    }

    if (!found) {
        printf("No bookings found for '%s'.\n", inputName);
    }
    printf("==============================\n");

    fclose(fp);
    return 0;
}
