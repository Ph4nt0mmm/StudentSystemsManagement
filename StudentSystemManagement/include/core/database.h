#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <fstream>
#include <sstream>
#include "auth.h"

inline std::vector<UserData> g_users;

namespace Database {
    inline void LoadDatabase() {
        g_users.clear();
        // Tài khoản mặc định
        g_users.push_back({L"admin", L"admin", ADMIN, true, L"Quản Trị Viên", L"AD001", L"admin@school.edu"});
        g_users.push_back({L"le", L"123", STUDENT, true, L"Lê Văn Anh", L"SV2024", L"le@school.edu"});
    }

    inline Role AuthenticateUser(const wchar_t* u, const wchar_t* p, Role r) {
        for (const auto& user : g_users) {
            if (user.username == u && user.password == p && user.role == r) return user.role;
        }
        return NONE;
    }

    inline void RegisterAccount(const wchar_t* u, const wchar_t* p, int r) {
        g_users.push_back({u, p, (Role)r, false, L"Chưa cập nhật", L"N/A", L"N/A"});
    }
}
#endif