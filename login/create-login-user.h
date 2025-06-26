#ifndef create_login_user_h
#define create_login_user_h


typedef struct 
{
    char username[100];
    char password[100];
    char emailAddress[100];
    char phoneNumber[100];
    char confirmPassword[100];
    char encryptedPassword[100];
} User;
typedef struct 
{
    char staffID[100];
    char password[100];
    char emailAddress[100];
    char phoneNumber[100];
    char confirmPassword[100];
    char encryptedPassword[100];
} Staff;
typedef struct 
{
    char fileUsername[100];
    char filePassword[100];
    char dycryptedPassword[100];
} File;
int loginUser(const char *username, const char *password);
int loginStaff(const char *ID, const char *password);
void createAccountUser(User user);
void createAccountStaff(Staff staff);


#endif