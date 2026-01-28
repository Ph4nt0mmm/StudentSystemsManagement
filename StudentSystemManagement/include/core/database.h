#ifndef DATABASE_H
#define DATABASE_H
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include "auth.h"

inline std::vector<UserData> g_users;

inline void SaveDatabase() {
    std::wofstream file("users_data.txt");
    for (const auto& u : g_users) {
        file << u.username << L" " << u.password << L" " << (int)u.role << L" " << (u.isApproved ? 1 : 0) << std::endl;
    }
}

inline void LoadDatabase() {
    g_users.clear();
    std::wifstream file("users_data.txt");
    std::wstring u, p; int r, ap;
    while (file >> u >> p >> r >> ap) {
        g_users.push_back({u, p, (Role)r, ap == 1});
    }
    if (g_users.empty()) {
        g_users.push_back({L"admin", L"123", ADMIN, true});
        SaveDatabase();
    }
}

inline Role AuthenticateUser(std::wstring u, std::wstring p, Role selectedRole) {
    for (const auto& user : g_users) {
        if (user.username == u && user.password == p && user.role == selectedRole) {
            if (user.isApproved) return user.role;
        }
    }
    return NONE;
}

inline void RegisterAccount(std::wstring u, std::wstring p, int r) {
    g_users.push_back({u, p, (Role)r, false});
    SaveDatabase();
}

inline void ApproveUser(std::wstring username) {
    for (auto& u : g_users) { if (u.username == username) u.isApproved = true; }
    SaveDatabase();
}

inline void DeleteUser(std::wstring username) {
    g_users.erase(std::remove_if(g_users.begin(), g_users.end(), [&](const UserData& u) {
        return u.username == username;
    }), g_users.end());
    SaveDatabase();
}
#endif