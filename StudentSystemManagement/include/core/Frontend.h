#ifndef FRONTEND_H
#define FRONTEND_H
#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <vector>
#include "auth.h"

using namespace Gdiplus;

enum ActiveTab { DASHBOARD, ATTENDANCE, COURSE, SCORE, SCHEDULE };
inline ActiveTab g_currentTab = DASHBOARD;

inline void FillRoundRect(Graphics& g, Brush* b, int x, int y, int w, int h, int r) {
    GraphicsPath path;
    path.AddArc(x, y, r, r, 180, 90);
    path.AddArc(x + w - r, y, r, r, 270, 90);
    path.AddArc(x + w - r, y + h - r, r, r, 0, 90);
    path.AddArc(x, y + h - r, r, r, 90, 90);
    g.FillPath(b, &path);
}

inline void DrawDashboard(Graphics& g, RECT r, Session session, const std::vector<UserData>& users) {
    g.SetSmoothingMode(SmoothingModeAntiAlias);
    g.Clear(Color(255, 240, 242, 245)); 

    // --- SIDEBAR ---
    SolidBrush sideBr(Color(255, 0, 80, 82));
    g.FillRectangle(&sideBr, 0, 0, 250, r.bottom);

    FontFamily ff(L"Segoe UI");
    Font fMenu(&ff, 11, FontStyleBold, UnitPoint);
    Font fTitle(&ff, 20, FontStyleBold, UnitPoint);
    Font fContent(&ff, 11, FontStyleRegular, UnitPoint);
    
    SolidBrush white(Color(255, 255, 255));
    SolidBrush black(Color(255, 33, 37, 41));
    SolidBrush greenBrush(Color(255, 40, 167, 69));
    SolidBrush redBrush(Color(255, 220, 53, 69));
    SolidBrush highlight(Color(100, 255, 255, 255));
    Pen greyPen(Color(255, 200, 200, 200), 1);

    // Vẽ Menu Sidebar
    std::wstring menus[] = { L"📊 DASHBOARD", L"📅 ATTENDANCE", L"📚 COURSE", L"📝 SCORE", L"🕒 SCHEDULE" };
    int menuY = 150;
    for (int i = 0; i < 5; i++) {
        if (g_currentTab == i) g.FillRectangle(&highlight, 10, menuY - 10, 230, 45);
        g.DrawString(menus[i].c_str(), -1, &fMenu, PointF(40, (REAL)menuY), &white);
        menuY += 60;
    }

    // --- HEADER ---
    std::wstring headerText = L"Xin chào " + std::wstring(session.role == 0 ? L"Quản trị viên" : L"Người dùng") + L": " + session.username;
    g.DrawString(headerText.c_str(), -1, &fTitle, PointF(280, 40), &black);
    g.DrawLine(&greyPen, 280, 85, r.right - 50, 85);

    // --- NỘI DUNG ---
    int cX = 280, cY = 110;
    if (session.role == 0 && g_currentTab == DASHBOARD) {
        g.DrawString(L"DANH SÁCH NGƯỜI DÙNG HỆ THỐNG", -1, &fMenu, PointF(cX, cY), &black);
        
        int rowY = cY + 50;
        g.DrawString(L"Tài khoản", -1, &fMenu, PointF(cX, rowY), &black);
        g.DrawString(L"Vai trò", -1, &fMenu, PointF(cX + 150, rowY), &black);
        g.DrawString(L"Trạng thái", -1, &fMenu, PointF(cX + 300, rowY), &black);
        g.DrawString(L"Thao tác", -1, &fMenu, PointF(cX + 450, rowY), &black);
        
        rowY += 40;
        for (const auto& user : users) {
            if (user.username == L"admin") continue;

            g.DrawString(user.username.c_str(), -1, &fContent, PointF(cX, rowY), &black);
            std::wstring rName = (user.role == 1 ? L"Giảng viên" : L"Sinh viên");
            g.DrawString(rName.c_str(), -1, &fContent, PointF(cX + 150, rowY), &black);

            if (user.isApproved) {
                g.DrawString(L"Đã duyệt", -1, &fContent, PointF(cX + 300, rowY), &greenBrush);
            } else {
                g.DrawString(L"Chờ duyệt", -1, &fContent, PointF(cX + 300, rowY), &redBrush);
                FillRoundRect(g, &greenBrush, cX + 450, rowY - 5, 70, 28, 5);
                g.DrawString(L"DUYỆT", -1, &fMenu, PointF(cX + 458, rowY), &white);
            }

            FillRoundRect(g, &redBrush, cX + 530, rowY - 5, 60, 28, 5);
            g.DrawString(L"XÓA", -1, &fMenu, PointF(cX + 542, rowY), &white);
            rowY += 45;
        }
    } else {
        g.DrawString(L"Vui lòng chọn chức năng quản lý bên trái.", -1, &fContent, PointF(cX, cY + 50), &black);
    }

    // Nút Đăng xuất
    FillRoundRect(g, &redBrush, 30, r.bottom - 80, 190, 45, 10);
    g.DrawString(L"↪ ĐĂNG XUẤT", -1, &fMenu, PointF(65, r.bottom - 68), &white);
}

inline void DrawModernPortal(Graphics& g, RECT r, bool isRegister) {
    g.SetSmoothingMode(SmoothingModeAntiAlias);
    LinearGradientBrush bg(Rect(0, 0, r.right, r.bottom), Color(255, 0, 121, 124), Color(255, 0, 70, 72), LinearGradientModeVertical);
    g.FillRectangle(&bg, 0, 0, r.right, r.bottom);
    SolidBrush card(Color(255, 255, 255));
    FillRoundRect(g, &card, 730, 100, 360, 580, 25);
    FontFamily ff(L"Segoe UI");
    Font fT(&ff, 20, FontStyleBold, UnitPoint);
    Font fS(&ff, 9, FontStyleRegular, UnitPoint);
    SolidBrush mBr(Color(255, 0, 80, 82));
    SolidBrush gBr(Color(255, 120, 120, 120));

    g.DrawString(isRegister ? L"ĐĂNG KÝ" : L"ĐĂNG NHẬP", -1, &fT, PointF(770, 150), &mBr);
    g.DrawString(L"Tên đăng nhập:", -1, &fS, PointF(770, 210), &gBr);
    g.DrawString(L"Mật khẩu:", -1, &fS, PointF(770, 280), &gBr);
    g.DrawString(L"Vai trò (0:Admin, 1:GV, 2:SV):", -1, &fS, PointF(770, 365), &mBr);
}

#endif