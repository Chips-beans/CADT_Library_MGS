#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE 256
#define MAX_BOOKINGS 100
#define MAX_PROMO 50
#define BOOKINGS_FILE "bookings.csv"
#define ROOMS_FILE "rooms.csv"
#define PROMO_FILE "promocodes.csv"

// Struct to hold booking info
typedef struct {
    int room_number;
    char guest_name[100];
    char checkin[11];
    char checkout[11];
    int booking_id;
    float total_price;
    float upfront_paid;
} Booking;

// Struct for promo code
typedef struct {
    char code[50];
    int discount; // percentage
    char expiry[11];
} PromoCode;

// Parse date string "YYYY-MM-DD" into struct tm
int parse_date(const char *date_str, struct tm *date) {
    return sscanf(date_str, "%4d-%2d-%2d",
                  &date->tm_year, &date->tm_mon, &date->tm_mday) == 3;
}

// Compare two dates (struct tm), return days difference (date2 - date1)
int days_diff(struct tm date1, struct tm date2) {
    time_t t1 = mktime(&date1);
    time_t t2 = mktime(&date2);
    double diff = difftime(t2, t1);
    return (int)(diff / (60 * 60 * 24));
}

// Load promocodes from CSV into array, return count
int load_promocodes(PromoCode promos[]) {
    FILE *fp = fopen(PROMO_FILE, "r");
    if (!fp) return 0;

    char line[MAX_LINE];
    int count = 0;
    // skip header
    fgets(line, sizeof(line), fp);
    while (fgets(line, sizeof(line), fp) && count < MAX_PROMO) {
        char *token = strtok(line, ",");
        if (!token) continue;
        strcpy(promos[count].code, token);

        token = strtok(NULL, ",");
        if (!token) continue;
        promos[count].discount = atoi(token);

        token = strtok(NULL, ",\n");
        if (!token) continue;
        strcpy(promos[count].expiry, token);

        count++;
    }
    fclose(fp);
    return count;
}

// Check if promo code is valid & not expired, return discount % or 0
int check_promocode(const char *input_code, PromoCode promos[], int promo_count) {
    time_t now = time(NULL);
    struct tm today = *localtime(&now);
    for (int i = 0; i < promo_count; i++) {
        if (strcasecmp(input_code, promos[i].code) == 0) {
            struct tm expiry_date = {0};
            if (!parse_date(promos[i].expiry, &expiry_date))
                return 0; // invalid expiry date in file
            expiry_date.tm_year -= 1900;
            expiry_date.tm_mon -= 1;
            expiry_date.tm_isdst = -1;

            time_t exp_time = mktime(&expiry_date);
            if (difftime(exp_time, now) >= 0) {
                return promos[i].discount;
            } else {
                printf("Promo code '%s' is expired.\n", input_code);
                return 0;
            }
        }
    }
    printf("Promo code '%s' not found.\n", input_code);
    return 0;
}

// Update rooms.csv to set status "Free" and guest "-" for checked-out rooms
void update_room_status(int room_number) {
    FILE *fp = fopen(ROOMS_FILE, "r");
    FILE *temp = fopen("rooms_temp.csv", "w");
    if (!fp || !temp) {
        printf("Error opening room files.\n");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), fp)) {
        int rn;
        char rtype[20], status[20], guest[100];
        sscanf(line, "%d,%19[^,],%19[^,],%99[^\n]", &rn, rtype, status, guest);

        if (rn == room_number) {
            strcpy(status, "Free");
            strcpy(guest, "-");
        }
        fprintf(temp, "%d,%s,%s,%s\n", rn, rtype, status, guest);
    }

    fclose(fp);
    fclose(temp);

    remove(ROOMS_FILE);
    rename("rooms_temp.csv", ROOMS_FILE);
}

// Delete bookings by booking IDs
void delete_bookings(int booking_ids[], int count) {
    FILE *fp = fopen(BOOKINGS_FILE, "r");
    FILE *temp = fopen("bookings_temp.csv", "w");
    if (!fp || !temp) {
        printf("Error opening booking files.\n");
        if (fp) fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), fp)) {
        int bid = -1;
        sscanf(line, "%*d,%*[^,],%*[^,],%*[^,],%d,%*f,%*f", &bid);
        int keep = 1;
        for (int i = 0; i < count; i++) {
            if (bid == booking_ids[i]) {
                keep = 0; // delete this booking
                break;
            }
        }
        if (keep) fputs(line, temp);
    }
    fclose(fp);
    fclose(temp);

    remove(BOOKINGS_FILE);
    rename("bookings_temp.csv", BOOKINGS_FILE);
}

int main() {
    printf("=== Staff Check-out ===\n");
    char guest_name[100];
    printf("Enter guest name for check-out: ");
    fgets(guest_name, sizeof(guest_name), stdin);
    guest_name[strcspn(guest_name, "\n")] = 0; // remove newline

    // Load all bookings for guest
    FILE *fp = fopen(BOOKINGS_FILE, "r");
    if (!fp) {
        printf("No bookings found.\n");
        return 1;
    }

    Booking bookings[MAX_BOOKINGS];
    int count = 0;
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), fp) && count < MAX_BOOKINGS) {
        int rn, bid;
        char name[100], ci[11], co[11];
        float total, upfront;
        sscanf(line, "%d,%99[^,],%10[^,],%10[^,],%d,%f,%f",
               &rn, name, ci, co, &bid, &total, &upfront);

        if (strcmp(name, guest_name) == 0) {
            bookings[count].room_number = rn;
            strcpy(bookings[count].guest_name, name);
            strcpy(bookings[count].checkin, ci);
            strcpy(bookings[count].checkout, co);
            bookings[count].booking_id = bid;
            bookings[count].total_price = total;
            bookings[count].upfront_paid = upfront;
            count++;
        }
    }
    fclose(fp);

    if (count == 0) {
        printf("No bookings found for guest '%s'.\n", guest_name);
        return 1;
    }

    printf("\nFound %d booking(s) for '%s':\n", count, guest_name);
    for (int i = 0; i < count; i++) {
        printf("%d) Room %d: Check-in %s, Check-out %s, Booking ID: %d, Total: $%.2f, Upfront Paid: $%.2f\n",
               i + 1,
               bookings[i].room_number,
               bookings[i].checkin,
               bookings[i].checkout,
               bookings[i].booking_id,
               bookings[i].total_price,
               bookings[i].upfront_paid);
    }

    // Ask which booking to check out (allow multiple)
    printf("Enter booking number(s) to check out (comma separated, e.g. 1,3): ");
    char input[100];
    fgets(input, sizeof(input), stdin);

    int selected_ids[MAX_BOOKINGS];
    int selected_count = 0;

    char *token = strtok(input, ",");
    while (token) {
        int idx = atoi(token) - 1;
        if (idx >= 0 && idx < count) {
            selected_ids[selected_count++] = bookings[idx].booking_id;
        } else {
            printf("Invalid selection: %s\n", token);
        }
        token = strtok(NULL, ",");
    }
    if (selected_count == 0) {
        printf("No valid bookings selected. Aborting.\n");
        return 1;
    }

    // Load promocodes
    PromoCode promos[MAX_PROMO];
    int promo_count = load_promocodes(promos);

    // Ask promocode
    char promo_input[50];
    int discount = 0;
    printf("Enter promocode (or leave empty): ");
    fgets(promo_input, sizeof(promo_input), stdin);
    promo_input[strcspn(promo_input, "\n")] = 0;
    if (strlen(promo_input) > 0) {
        discount = check_promocode(promo_input, promos, promo_count);
        if (discount > 0)
            printf("Promo code applied! Discount: %d%%\n", discount);
        else
            printf("No discount applied.\n");
    }

    // Current date for late checkout check
    time_t now = time(NULL);
    struct tm today = *localtime(&now);

    printf("\n=== CHECKOUT RECEIPT ===\n");
    float grand_total = 0.0f;
    for (int i = 0; i < selected_count; i++) {
        // Find booking struct by booking_id
        Booking *b = NULL;
        for (int j = 0; j < count; j++) {
            if (bookings[j].booking_id == selected_ids[i]) {
                b = &bookings[j];
                break;
            }
        }
        if (!b) continue;

        // Calculate remaining payment = total_price - upfront_paid
        float remaining = b->total_price - b->upfront_paid;

        // Check late checkout days
        struct tm co_date = {0};
        parse_date(b->checkout, &co_date);
        co_date.tm_year -= 1900;
        co_date.tm_mon -= 1;
        co_date.tm_isdst = -1;
        int late_days = days_diff(co_date, today);
        if (late_days > 0) {
            printf("Note: Late checkout by %d day(s) for room %d.\n", late_days, b->room_number);
            float late_fee = late_days * 20.0f; // e.g. $20 per extra day
            printf("Late fee added: $%.2f\n", late_fee);
            remaining += late_fee;
        }

        // Apply promo discount on remaining
        float discount_amount = 0.0f;
        if (discount > 0) {
            discount_amount = remaining * discount / 100.0f;
            remaining -= discount_amount;
        }

        grand_total += remaining;

        printf("---------------------------------------\n");
        printf("Room: %d\n", b->room_number);
        printf("Guest: %s\n", b->guest_name);
        printf("Check-in: %s\n", b->checkin);
        printf("Check-out: %s\n", b->checkout);
        printf("Total Price: $%.2f\n", b->total_price);
        printf("Upfront Paid: $%.2f\n", b->upfront_paid);
        printf("Remaining Due: $%.2f\n", remaining);
        if (discount > 0) {
            printf("Discount Applied: %d%% ($%.2f)\n", discount, discount_amount);
        }
        if (late_days > 0) {
            printf("Late Checkout Fee: %d day(s) x $20 = $%.2f\n", late_days, late_days * 20.0f);
        }
    }
    printf("---------------------------------------\n");
    printf("GRAND TOTAL DUE: $%.2f\n", grand_total);
    printf("=== Thank you! ===\n");

    // Delete bookings & update rooms
    delete_bookings(selected_ids, selected_count);
    for (int i = 0; i < selected_count; i++) {
        // Find booking room number to update room status
        for (int j = 0; j < count; j++) {
            if (bookings[j].booking_id == selected_ids[i]) {
                update_room_status(bookings[j].room_number);
                break;
            }
        }
    }

    return 0;
}
