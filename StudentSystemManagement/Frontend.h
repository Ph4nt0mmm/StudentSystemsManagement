#ifndef FRONTEND_H
#define FRONTEND_H

#include <windows.h>
#include <gdiplus.h>
#include <string>
#include "auth.h"
#include "helpers.h"

using namespace Gdiplus;

inline void DrawModernPortal(Graphics& g, RECT r, bool isReg) {
    SolidBrush bg(Color(255, 240, 245, 245));
    g.FillRectangle(&bg, 0, 0, r.right, r.bottom);

    SolidBrush card(Color(255, 255, 255, 255));
    Helpers::FillRoundRect(g, &card, 700, 80, 420, 600, 30);

    // Dòng này sẽ hết lỗi sau khi bạn sửa helpers.h ở trên
    Pen shadowPen(Color(20, 0, 0, 0), 2.0f);
    Helpers::DrawRoundRect(g, &shadowPen, 700, 80, 420, 600, 30);

    FontFamily ff(L"Segoe UI");
    Font fTitle(&ff, 24, FontStyleBold, UnitPoint);
    SolidBrush bText(Color(255, 45, 52, 54));
    g.DrawString(isReg ? L"ĐĂNG KÝ" : L"ĐĂNG NHẬP", -1, &fTitle, PointF(770.0f, 140.0f), &bText);
    
    Font fHint(&ff, 10, FontStyleRegular, UnitPoint);
    SolidBrush bHint(Color(255, 99, 110, 114));
    g.DrawString(L"Vai trò: 0-Admin | 1-GV | 2-SV", -1, &fHint, PointF(775.0f, 365.0f), &bHint);

    SolidBrush decor(Color(255, 0, 60, 60));
    g.FillEllipse(&decor, 200, 250, 200, 200);
}

inline void DrawDashboardFrame(Graphics& g, RECT r, Session& s) {
    SolidBrush sideBg(Color(255, 0, 60, 60));
    g.FillRectangle(&sideBg, 0, 0, 250, r.bottom);

    FontFamily ff(L"Segoe UI");
    Font fMenu(&ff, 11, FontStyleBold, UnitPoint);
    Font fHeader(&ff, 16, FontStyleBold, UnitPoint);
    SolidBrush white(Color(255, 255, 255, 255));
    SolidBrush darkText(Color(255, 33, 37, 41));

    const wchar_t* roleName = (s.role == 0) ? L"QUẢN TRỊ VIÊN" : (s.role == 1 ? L"GIẢNG VIÊN" : L"SINH VIÊN");
    g.DrawString(roleName, -1, &fMenu, PointF(30.0f, 45.0f), &white);
    
    Pen lineWhite(Color(100, 255, 255, 255), 1.0f);
    g.DrawLine(&lineWhite, 30.0f, 75.0f, 220.0f, 75.0f);

    const wchar_t* menuLabels[] = { L"🏠   TRANG CHỦ", L"📚   KHÓA HỌC", L"📅   LỊCH HỌC", L"✅   ĐIỂM DANH", L"📊   BẢNG ĐIỂM" };

    for (int i = 0; i < 5; i++) {
        REAL yPos = 120.0f + (REAL)(i * 60); 
        if ((int)s.currentView == i) {
            SolidBrush active(Color(80, 255, 255, 255));
            g.FillRectangle(&active, 0.0f, yPos, 250.0f, 50.0f); 
            SolidBrush indicator(Color(255, 255, 255, 255));
            g.FillRectangle(&indicator, 0.0f, yPos, 5.0f, 50.0f);
        }
        g.DrawString(menuLabels[i], -1, &fMenu, PointF(40.0f, yPos + 15.0f), &white);
    }

    SolidBrush brRed(Color(255, 220, 53, 69)); 
    Helpers::FillRoundRect(g, &brRed, 20, 650, 210, 45, 10); 
    g.DrawString(L"↔   ĐĂNG XUẤT", -1, &fMenu, PointF(60.0f, 662.0f), &white);

    SolidBrush headBg(Color(255, 255, 255, 255));
    g.FillRectangle(&headBg, 250, 0, r.right - 250, 80);
    
    Pen separator(Color(40, 0, 0, 0), 1.0f);
    g.DrawLine(&separator, 250.0f, 80.0f, (REAL)r.right, 80.0f);

    const wchar_t* pageTitles[] = { L"Trang chủ hệ thống", L"Quản lý Khóa học", L"Lịch học & Giảng dạy", L"Hệ thống Điểm danh", L"Kết quả học tập" };
    g.DrawString(pageTitles[(int)s.currentView], -1, &fHeader, PointF(280.0f, 25.0f), &darkText);
    
    Font fUser(&ff, 10, FontStyleRegular, UnitPoint);
    std::wstring welcome = L"Xin chào, " + s.username;
    g.DrawString(welcome.c_str(), -1, &fUser, PointF((REAL)r.right - 220.0f, 30.0f), &darkText);
}

#endif