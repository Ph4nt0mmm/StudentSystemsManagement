#ifndef AUTH_H
#define AUTH_H
#include <string>

// Định nghĩa vai trò: Admin=0, Giảng viên=1, Sinh viên=2
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