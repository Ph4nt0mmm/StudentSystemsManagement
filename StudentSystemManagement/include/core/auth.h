#ifndef AUTH_H
#define AUTH_H
#include <string>

enum Role { ADMIN = 0, LECTURER = 1, STUDENT = 2, NONE = -1 };

struct Session {
    std::wstring username;
    Role role;
    bool isLoggedIn;
};

struct UserData {
    std::wstring username;
    std::wstring password;
    Role role;
    bool isApproved;
};
#endif