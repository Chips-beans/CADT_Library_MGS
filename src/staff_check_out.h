#ifndef STAFF_CHECK_OUT_H
#define STAFF_CHECK_OUT_H


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
} BookingSCheckOUT;

// Struct for promo code
typedef struct {
    char code[50];
    int discount; // percentage
    char expiry[11];
} PromoCode;
int parse_date(const char *date_str, struct tm *date);
int days_diff(struct tm date1, struct tm date2);

// Promo code logic
int load_promocodes(PromoCode promos[]);
int check_promocode(const char *input_code, PromoCode promos[], int promo_count);

// Room & booking management
void update_room_status_co(int room_number);
void delete_bookings(int booking_ids[], int count);
int staffCheckOut();
#endif