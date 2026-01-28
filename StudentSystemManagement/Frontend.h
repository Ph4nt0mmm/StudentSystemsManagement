#ifndef FRONTEND_H
#define FRONTEND_H
#include <windows.h>
#include <gdiplus.h>
#include "auth.h"
#include "database.h"

using namespace Gdiplus;

inline void FillRoundRect(Graphics& g, Brush* b, int x, int y, int w, int h, int r) {
    GraphicsPath path;
    path.AddArc(x, y, r, r, 180, 90);
    path.AddArc(x + w - r, y, r, r, 270, 90);
    path.AddArc(x + w - r, y + h - r, r, r, 0, 90);
    path.AddArc(x, y + h - r, r, r, 90, 90);
    g.FillPath(b, &path);
}

inline void DrawModernPortal(Graphics& g, RECT r, bool isRegister) {
    g.SetSmoothingMode(SmoothingModeAntiAlias);
    LinearGradientBrush bg(Rect(0, 0, r.right, r.bottom), Color(255, 0, 121, 124), Color(255, 0, 70, 72), LinearGradientModeVertical);
    g.FillRectangle(&bg, 0, 0, r.right, r.bottom);
    SolidBrush card(Color(255, 255, 255));
    FillRoundRect(g, &card, 730, 100, 360, 580, 25);
    FontFamily ff(L"Segoe UI");
    Font fT(&ff, 18, FontStyleBold, UnitPoint);
    Font fS(&ff, 9, FontStyleRegular, UnitPoint);
    SolidBrush mBr(Color(255, 0, 80, 82));
    SolidBrush gBr(Color(255, 120, 120, 120));
    g.DrawString(isRegister ? L"ĐĂNG KÝ" : L"ĐĂNG NHẬP", -1, &fT, PointF(765, 150), &mBr);
    g.DrawString(L"Tên đăng nhập:", -1, &fS, PointF(770, 210), &gBr);
    g.DrawString(L"Mật khẩu:", -1, &fS, PointF(770, 280), &gBr);
    g.DrawString(L"Vai trò (1:GV, 2:SV):", -1, &fS, PointF(770, 365), &mBr);
}

inline void DrawDashboard(Graphics& g, RECT r, Session session) {
    g.SetSmoothingMode(SmoothingModeAntiAlias);
    g.Clear(Color(255, 240, 242, 245));
    SolidBrush sideBr(Color(255, 0, 80, 82));
    g.FillRectangle(&sideBr, 0, 0, 250, r.bottom);
    FontFamily ff(L"Segoe UI");
    Font fT(&ff, 13, FontStyleBold, UnitPoint);
    Font fWelcome(&ff, 16, FontStyleBold, UnitPoint);
    Font fM(&ff, 10, FontStyleRegular, UnitPoint);
    
    SolidBrush w(Color(255, 255, 255)), b(Color(255, 33, 37, 41));
    SolidBrush red(Color(255, 220, 53, 69)), green(Color(255, 40, 167, 69));

    // Nút Đăng xuất
    FillRoundRect(g, &red, 30, r.bottom - 80, 190, 45, 10);
    g.DrawString(L"↪ ĐĂNG XUẤT", -1, &fM, PointF(65, r.bottom - 68), &w);

    // Sidebar Role
    std::wstring rSide = (session.role == ADMIN) ? L"QUẢN TRỊ VIÊN" : (session.role == LECTURER ? L"GIẢNG VIÊN" : L"SINH VIÊN");
    g.DrawString(rSide.c_str(), -1, &fT, PointF(30, 80), &w);

    // Lời chào động
    std::wstring msg = (session.role == ADMIN) ? L"Chào mừng Quản trị viên: " : (session.role == LECTURER ? L"Chào mừng Giảng viên: " : L"Chào mừng Sinh viên: ");
    msg += session.username;
    g.DrawString(msg.c_str(), -1, &fWelcome, PointF(280, 40), &b);

    if (session.role == ADMIN) {
        g.DrawString(L"QUẢN LÝ TÀI KHOẢN HỆ THỐNG", -1, &fT, PointF(280, 90), &b);
        int y = 140;
        for (const auto& u : g_users) {
            if (u.username == L"admin") continue;
            std::wstring info = u.username + (u.role == LECTURER ? L" (GV) " : L" (SV) ") + (u.isApproved ? L"[Đã duyệt]" : L"[Chờ]");
            g.DrawString(info.c_str(), -1, &fM, PointF(280, y), &b);
            if (!u.isApproved) {
                g.FillRectangle(&green, 650, y - 5, 70, 25);
                g.DrawString(L"Duyệt", -1, &fM, PointF(662, y - 2), &w);
            }
            g.FillRectangle(&red, 730, y - 5, 70, 25);
            g.DrawString(L"Xóa", -1, &fM, PointF(750, y - 2), &w);
            y += 40;
        }
    } else {
        g.DrawString(L"Bạn đã đăng nhập thành công vào trang cá nhân.", -1, &fM, PointF(280, 100), &b);
    }
}
#endif