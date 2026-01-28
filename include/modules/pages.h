#ifndef PAGES_H
#define PAGES_H

#include <gdiplus.h>
#include <vector>
#include <string>
#include "auth.h"
#include "database.h"

using namespace Gdiplus;

namespace Pages {
    inline void DrawPageHeader(Graphics& g, const wchar_t* title) {
        FontFamily ff(L"Segoe UI");
        Font f(&ff, 16, FontStyleBold, UnitPoint);
        SolidBrush b(Color(255, 33, 37, 41));
        g.DrawString(title, -1, &f, PointF(280.0f, 30.0f), &b);
        Pen p(Color(50, 0, 0, 0), 1.0f);
        g.DrawLine(&p, 280.0f, 75.0f, 1150.0f, 75.0f);
    }

    inline void DrawDashboard(Graphics& g, Session& s) {
        DrawPageHeader(g, L"DANH SÁCH TÀI KHOẢN HỆ THỐNG");
        FontFamily ff(L"Segoe UI");
        Font fRow(&ff, 11, FontStyleRegular, UnitPoint);
        SolidBrush bText(Color(255, 50, 50, 50));

        if (g_users.empty()) {
            g.DrawString(L"Không có dữ liệu người dùng...", -1, &fRow, PointF(300.0f, 120.0f), &bText);
        } else {
            for (size_t i = 0; i < g_users.size(); i++) {
                std::wstring row = std::to_wstring(i + 1) + L".  " + g_users[i].username + 
                                   (g_users[i].role == 0 ? L" (Admin)" : L" (User)");
                g.DrawString(row.c_str(), -1, &fRow, PointF(300.0f, 120.0f + (i * 40.0f)), &bText);
            }
        }
    }

    inline void DrawCourse(Graphics& g) { DrawPageHeader(g, L"DANH SÁCH KHÓA HỌC"); }
    inline void DrawSchedule(Graphics& g) { DrawPageHeader(g, L"LỊCH HỌC CHI TIẾT"); }
    inline void DrawAttendance(Graphics& g) { DrawPageHeader(g, L"HỆ THỐNG ĐIỂM DANH"); }
    inline void DrawScore(Graphics& g) { DrawPageHeader(g, L"BẢNG ĐIỂM SINH VIÊN"); }
}
#endif