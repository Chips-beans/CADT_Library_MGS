#ifndef INCOME_H
#define INCOME_H

#include <time.h>

// Constants
#define BOOKING_FILE "bookings.csv"
#define REPORT_FILE_STAFF_INCOME "income_report.csv"

// Struct for income calculation
typedef struct {
    int room_number;
    char guest_name[100];
    char checkin_date[11];    // YYYY-MM-DD
    char checkout_date[11];   // YYYY-MM-DD
    int booking_id;
    double total_price;
    double upfront_payment;
} BookingSIncome;

// Date parsing and logic
int parse_date(const char *date_str, struct tm *date_tm);
time_t date_to_time(const char *date_str);
int is_overlap(time_t startA, time_t endA, time_t startB, time_t endB);

// Menu and date input
void print_menu();
void get_date_input(struct tm *date_tm);

// Main function to calculate income
int staffCalsulateIncome();

#endif // INCOME_H
