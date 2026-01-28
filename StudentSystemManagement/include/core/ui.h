#ifndef UI_H
#define UI_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

/* ===== GIỮ NGUYÊN TỪ CODE GỐC ===== */
enum Role {
    NONE = 0,
    STUDENT = 1,
    LECTURER = 2,
    ADMIN = 3
};

struct Session {
    string username;
    Role role;
    bool isLoggedIn;
};

struct User {
    string username;
    string password;
    Role role;
    bool isApproved;
};

/* ===== UI CONSOLE ===== */

void DrawModernPortal(bool isRegisterMode) {
    cout << "\n==============================\n";
    cout << " HE THONG QUAN LY SINH VIEN\n";
    cout << "==============================\n";
    if (isRegisterMode)
        cout << "=== CHE DO DANG KY ===\n";
    else
        cout << "=== CHE DO DANG NHAP ===\n";
}

void DrawDashboard(const Session& session, const vector<User>& users) {
    cout << "\n==============================\n";
    cout << "Xin chao: " << session.username << endl;

    if (session.role == STUDENT) cout << "Vai tro: Sinh vien\n";
    if (session.role == LECTURER) cout << "Vai tro: Giang vien\n";
    if (session.role == ADMIN) cout << "Vai tro: Admin\n";

    cout << "==============================\n";

    if (session.role == ADMIN) {
        cout << "\n--- DANH SACH TAI KHOAN ---\n";
        for (const auto& u : users) {
            if (u.username == "admin") continue;
            cout << u.username
                 << " | role: " << u.role
                 << " | duyet: " << (u.isApproved ? "YES" : "NO")
                 << endl;
        }
    }

    cout << "\n1. Dang xuat\n";
}

#endif