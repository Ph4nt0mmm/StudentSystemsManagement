#ifndef AUTH_H
#define AUTH_H
#include <string>

enum Role { NONE = -1, ADMIN = 0, TEACHER = 1, STUDENT = 2 };
enum ViewState { MENU_DASHBOARD, MENU_COURSE, MENU_SCHEDULE, MENU_ATTENDANCE, MENU_SCORE };

struct UserData {
    std::wstring username;
    std::wstring password;
    Role role;
    bool isApproved;
    std::wstring fullname;
    std::wstring id;
    std::wstring email;
};

struct Session {
    std::wstring username;
    Role role;
    bool isLoggedIn;
    ViewState currentView;
    std::wstring fullname;
    std::wstring id;
    std::wstring email;
};
#endif