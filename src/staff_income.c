#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BOOKING_FILE "bookings.csv"
#define REPORT_FILE "income_report.csv"

typedef struct {
    int room_number;
    char guest_name[100];
    char checkin_date[11];   // YYYY-MM-DD
    char checkout_date[11];  // YYYY-MM-DD
    int booking_id;
    double total_price;
    double upfront_payment;
} Booking;

int parse_date(const char *date_str, struct tm *date_tm) {
    return sscanf(date_str, "%d-%d-%d",
        &date_tm->tm_year, &date_tm->tm_mon, &date_tm->tm_mday) == 3;
}

// Convert YYYY-MM-DD string to time_t
time_t date_to_time(const char *date_str) {
    struct tm tm = {0};
    if (!parse_date(date_str, &tm)) return -1;
    tm.tm_year -= 1900;  // tm_year is years since 1900
    tm.tm_mon -= 1;      // tm_mon is 0-11
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    return mktime(&tm);
}

// Check if two date ranges overlap (A_start to A_end and B_start to B_end)
int is_overlap(time_t startA, time_t endA, time_t startB, time_t endB) {
    return !(endA < startB || endB < startA);
}

void print_menu() {
    printf("Choose income report type:\n");
    printf("1. Daily\n");
    printf("2. Weekly\n");
    printf("3. Monthly\n");
    printf("4. Yearly\n");
    printf("5. Total (All time)\n");
    printf("Enter choice: ");
}

void get_date_input(struct tm *date_tm) {
    int year, month, day;
    printf("Enter year (e.g., 2025): ");
    scanf("%d", &year);
    printf("Enter month (1-12): ");
    scanf("%d", &month);
    printf("Enter day (1-31): ");
    scanf("%d", &day);
    date_tm->tm_year = year - 1900;
    date_tm->tm_mon = month - 1;
    date_tm->tm_mday = day;
    date_tm->tm_hour = 0;
    date_tm->tm_min = 0;
    date_tm->tm_sec = 0;
}

int main() {
    FILE *fp = fopen(BOOKING_FILE, "r");
    if (!fp) {
        printf("Error: Could not open %s\n", BOOKING_FILE);
        return 1;
    }

    int choice;
    print_menu();
    scanf("%d", &choice);

    struct tm input_date = {0};
    time_t start_period = 0, end_period = 0;

    time_t now = time(NULL);

    switch(choice) {
        case 1: // Daily
            get_date_input(&input_date);
            start_period = mktime(&input_date);
            end_period = start_period + 86400 - 1; // one day range
            break;
        case 2: // Weekly (starting Sunday)
            get_date_input(&input_date);
            start_period = mktime(&input_date);
            // Adjust back to previous Sunday
            {
                struct tm *tmp = localtime(&start_period);
                int wday = tmp->tm_wday; // 0=Sunday
                start_period -= wday * 86400;
                end_period = start_period + 7 * 86400 - 1;
            }
            break;
        case 3: // Monthly
            printf("Enter year (e.g., 2025): ");
            int year, month;
            scanf("%d", &year);
            printf("Enter month (1-12): ");
            scanf("%d", &month);
            input_date.tm_year = year - 1900;
            input_date.tm_mon = month - 1;
            input_date.tm_mday = 1;
            start_period = mktime(&input_date);

            input_date.tm_mon += 1;  // next month
            input_date.tm_mday = 1;
            end_period = mktime(&input_date) - 1;
            break;
        case 4: // Yearly
            printf("Enter year (e.g., 2025): ");
            scanf("%d", &year);
            input_date.tm_year = year - 1900;
            input_date.tm_mon = 0;
            input_date.tm_mday = 1;
            start_period = mktime(&input_date);

            input_date.tm_year += 1;
            end_period = mktime(&input_date) - 1;
            break;
        case 5: // Total
            start_period = 0;
            end_period = now;
            break;
        default:
            printf("Invalid choice\n");
            fclose(fp);
            return 1;
    }

    // Read bookings
    char line[512];
    Booking bookings[1000];
    int count = 0;

    while (fgets(line, sizeof(line), fp)) {
        Booking b;
        char total_price_str[20], upfront_str[20];
        // Format:
        // room_number,guest_name,checkin_date,checkout_date,booking_id,total_price,upfront_payment
        if (sscanf(line, "%d,%99[^,],%10[^,],%10[^,],%d,%19[^,],%19s",
            &b.room_number, b.guest_name, b.checkin_date, b.checkout_date,
            &b.booking_id, total_price_str, upfront_str) == 7) {
            
            b.total_price = atof(total_price_str);
            b.upfront_payment = atof(upfront_str);
            bookings[count++] = b;
        }
    }
    fclose(fp);

    double total_income = 0;

    for (int i = 0; i < count; i++) {
        time_t ci = date_to_time(bookings[i].checkin_date);
        time_t co = date_to_time(bookings[i].checkout_date);
        if (ci == -1 || co == -1) continue;

        // Check if booking period overlaps income period
        if (is_overlap(ci, co, start_period, end_period)) {
            total_income += bookings[i].total_price;
        }
    }

    printf("Total income for selected period: $%.2f\n", total_income);

    // Write report
    FILE *rpt = fopen(REPORT_FILE, "w");
    if (!rpt) {
        printf("Error creating report file.\n");
        return 1;
    }

    fprintf(rpt, "Income Report\n");
    fprintf(rpt, "Period Start,%s\n", ctime(&start_period));
    fprintf(rpt, "Period End,%s\n", ctime(&end_period));
    fprintf(rpt, "Total Income,%.2f\n", total_income);
    fclose(rpt);

    printf("Report saved to %s\n", REPORT_FILE);

    return 0;
}
