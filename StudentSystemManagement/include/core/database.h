#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include "auth.h"

// --- Cấu trúc dữ liệu ---
struct Course {
    std::wstring name;
    std::wstring schedule; 
    std::wstring room;
};

struct StudentRecord {
    std::wstring studentName;
    std::wstring courseName;
    float score;
    int absentCount;
};

// --- Dữ liệu bộ nhớ tạm ---
inline std::vector<UserData> g_users;
inline std::vector<Course> g_courses;
inline std::vector<StudentRecord> g_records;

// --- Hàm quản lý File ---
inline void SaveDatabase() {
    std::wofstream file("users_data.txt");
    for (const auto& u : g_users) {
        file << u.username << L"|" << u.password << L"|" << (int)u.role << L"|" << (u.isApproved ? 1 : 0) << std::endl;
    }
}

inline void LoadDatabase() {
    g_users.clear();
    std::wifstream file("users_data.txt");
    std::wstring line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        size_t p1 = line.find(L'|'), p2 = line.find(L'|', p1 + 1), p3 = line.find(L'|', p2 + 1);
        if (p1 != std::wstring::npos && p2 != std::wstring::npos && p3 != std::wstring::npos) {
            std::wstring u = line.substr(0, p1);
            std::wstring p = line.substr(p1 + 1, p2 - p1 - 1);
            int r = std::stoi(line.substr(p2 + 1, p3 - p2 - 1));
            int ap = std::stoi(line.substr(p3 + 1));
            g_users.push_back({u, p, (Role)r, ap == 1});
        }
    }
    if (g_users.empty()) {
        g_users.push_back({L"admin", L"123", ADMIN, true});
        SaveDatabase();
    }
}

inline void SaveStudyData() {
    std::wofstream fC("courses.txt");
    for (const auto& c : g_courses) fC << c.name << L"|" << c.schedule << L"|" << c.room << std::endl;
    std::wofstream fR("records.txt");
    for (const auto& rec : g_records) fR << rec.studentName << L"|" << rec.courseName << L"|" << rec.score << L"|" << rec.absentCount << std::endl;
}

inline void LoadStudyData() {
    g_courses.clear(); g_records.clear();
    std::wifstream fC("courses.txt"); std::wstring line;
    while (std::getline(fC, line)) {
        size_t p1 = line.find(L'|'), p2 = line.find(L'|', p1 + 1);
        if (p1 != std::wstring::npos && p2 != std::wstring::npos)
            g_courses.push_back({line.substr(0, p1), line.substr(p1+1, p2-p1-1), line.substr(p2+1)});
    }
    std::wifstream fR("records.txt");
    while (std::getline(fR, line)) {
        size_t p1 = line.find(L'|'), p2 = line.find(L'|', p1 + 1), p3 = line.find(L'|', p2 + 1);
        if (p1 != std::wstring::npos && p2 != std::wstring::npos && p3 != std::wstring::npos)
            g_records.push_back({line.substr(0, p1), line.substr(p1+1, p2-p1-1), std::stof(line.substr(p2+1, p3-p2-1)), std::stoi(line.substr(p3+1))});
    }
    if (g_courses.empty()) {
        g_courses.push_back({L"Lap Trinh C++", L"Thu 2", L"A101"});
        SaveStudyData();
    }
}

inline Role AuthenticateUser(std::wstring u, std::wstring p, Role r) {
    for (const auto& user : g_users) if (user.username == u && user.password == p && user.role == r && user.isApproved) return user.role;
    return NONE;
}

inline void RegisterAccount(std::wstring u, std::wstring p, int r) {
    g_users.push_back({u, p, (Role)r, false});
    SaveDatabase();
}

#endif