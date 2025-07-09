#include <stdio.h>
#include <string.h>
#define MAX_LINE 256

void updateRoomStatus(const char *filename, int targetRoomID, const char *newStatus) {
    FILE *fp = fopen(filename, "r");
    FILE *temp = fopen("temp.csv", "w");

    if (!fp || !temp) {
        printf("Error opening file.\n");
        return;
    }

    char line[MAX_LINE];
    int roomID;
    char type[20], status[10], note[10];

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%d,%19[^,],%9[^,],%9[^\n]", &roomID, type, status, note) == 4) {
            if (roomID == targetRoomID) {
                fprintf(temp, "%d,%s,%s,%s\n", roomID, type, newStatus, note);
            } else {
                fputs(line, temp);
            }
        }
    }

    fclose(fp);
    fclose(temp);

    remove(filename);
    rename("temp.csv", filename);
}
