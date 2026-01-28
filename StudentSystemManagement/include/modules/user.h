#ifndef USER_H
#define USER_H

#include <stdbool.h>

// Enum for user roles
typedef enum {
    ROLE_ADMIN,
    ROLE_LECTURER,
    ROLE_STUDENT
} UserRole;

// User structure
typedef struct {
    int id;
    char username[50];
    char password[100];  // Hashed password
    char email[100];
    UserRole role;
    char full_name[100];
    char phone[20];
    bool is_active;
    char created_at[20];
    char last_login[20];
} User;

// Function declarations
User* user_create(int id, const char* username, const char* password, 
                  const char* email, UserRole role, const char* full_name);
void user_destroy(User* user);
bool user_authenticate(User* user, const char* password);
bool user_change_password(User* user, const char* old_password, const char* new_password);
void user_update_profile(User* user, const char* email, const char* phone, const char* full_name);
bool user_validate_email(const char* email);
bool user_validate_phone(const char* phone);
const char* user_role_to_string(UserRole role);
UserRole string_to_user_role(const char* role_str);

#endif // USER_H