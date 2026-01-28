#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include <string>
#include "include/core/auth.h"
#include "include/core/database.h"
#include "include/core/Frontend.h"

#pragma comment(lib, "gdiplus.lib")
#define ID_BTN_ACTION 101
#define ID_BTN_SWITCH 102

// Biến toàn cục quản lý trạng thái
Session currentSession = { L"", NONE, false };
bool isRegisterMode = false;
HWND hU, hP, hR, hB, hS;

// Căn chỉnh các ô nhập liệu về phía bên phải (khu vực Card trắng)
void CenterControls(HWND hwnd) {
    int startX = 770, startY = 235;
    if (!currentSession.isLoggedIn) {
        MoveWindow(hU, startX, startY, 280, 35, TRUE);
        MoveWindow(hP, startX, startY + 70, 280, 35, TRUE);
        MoveWindow(hR, startX, startY + 160, 280, 35, TRUE);
        MoveWindow(hB, startX, 460, 280, 50, TRUE);
        MoveWindow(hS, startX, 530, 280, 30, TRUE);
        
        ShowWindow(hU, SW_SHOW); ShowWindow(hP, SW_SHOW);
        ShowWindow(hR, SW_SHOW); ShowWindow(hB, SW_SHOW); ShowWindow(hS, SW_SHOW);
    } else {
        ShowWindow(hU, SW_HIDE); ShowWindow(hP, SW_HIDE);
        ShowWindow(hR, SW_HIDE); ShowWindow(hB, SW_HIDE); ShowWindow(hS, SW_HIDE);
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_CREATE: {
        HFONT hF = CreateFontW(18, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, L"Segoe UI");
        hU = CreateWindowW(L"Edit", L"", WS_CHILD | WS_BORDER, 0, 0, 0, 0, hwnd, 0, 0, 0);
        hP = CreateWindowW(L"Edit", L"", WS_CHILD | WS_BORDER | ES_PASSWORD, 0, 0, 0, 0, hwnd, 0, 0, 0);
        hR = CreateWindowW(L"Edit", L"2", WS_CHILD | WS_BORDER, 0, 0, 0, 0, hwnd, 0, 0, 0);
        hB = CreateWindowW(L"Button", L"XÁC NHẬN", WS_CHILD | BS_FLAT, 0, 0, 0, 0, hwnd, (HMENU)ID_BTN_ACTION, 0, 0);
        hS = CreateWindowW(L"Button", L"Chưa có tài khoản? Đăng ký", WS_CHILD | BS_FLAT, 0, 0, 0, 0, hwnd, (HMENU)ID_BTN_SWITCH, 0, 0);
        
        SendMessage(hU, WM_SETFONT, (WPARAM)hF, TRUE); 
        SendMessage(hP, WM_SETFONT, (WPARAM)hF, TRUE);
        SendMessage(hR, WM_SETFONT, (WPARAM)hF, TRUE); 
        SendMessage(hB, WM_SETFONT, (WPARAM)hF, TRUE);
        
        CenterControls(hwnd); 
        break;
    }

    case WM_SIZE: {
        CenterControls(hwnd);
        break;
    }

    case WM_LBUTTONDOWN: {
        int x = LOWORD(lp), y = HIWORD(lp);
        
        // 1. XỬ LÝ CLICK SIDEBAR (Chuyển Tab & Đăng xuất)
        if (currentSession.isLoggedIn && x <= 250) {
            if (y >= 140 && y <= 185) g_currentTab = DASHBOARD;
            else if (y >= 200 && y <= 245) g_currentTab = ATTENDANCE;
            else if (y >= 260 && y <= 305) g_currentTab = COURSE;
            else if (y >= 320 && y <= 365) g_currentTab = SCORE;
            else if (y >= 380 && y <= 425) g_currentTab = SCHEDULE;
            
            RECT rc; GetClientRect(hwnd, &rc);
            if (y >= rc.bottom - 80) { // Nút Đăng xuất
                currentSession.isLoggedIn = false;
                CenterControls(hwnd);
            }
            InvalidateRect(hwnd, NULL, TRUE);
        }

        // 2. LOGIC TƯƠNG TÁC CHO ADMIN (Duyệt/Xóa)
        if (currentSession.isLoggedIn && currentSession.role == 0 && g_currentTab == DASHBOARD) {
            int cX = 280, rowY = 120 + 50 + 40; 
            for (size_t i = 0; i < g_users.size(); i++) {
                if (g_users[i].username == L"admin") continue;
                // Nút Duyệt
                if (!g_users[i].isApproved && x >= cX + 450 && x <= cX + 520 && y >= rowY - 5 && y <= rowY + 23) {
                    g_users[i].isApproved = true;
                    SaveDatabase();
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                }
                // Nút Xóa
                if (x >= cX + 530 && x <= cX + 590 && y >= rowY - 5 && y <= rowY + 23) {
                    g_users.erase(g_users.begin() + i);
                    SaveDatabase();
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                }
                rowY += 45;
            }
        }

        // 3. LOGIC TƯƠNG TÁC CHO GIẢNG VIÊN (Điểm danh/Nhập điểm)
        if (currentSession.isLoggedIn && currentSession.role == 1) {
            int cX = 280, cY = 120;
            if (g_currentTab == ATTENDANCE) {
                for (int i = 0; i < (int)g_records.size(); i++) {
                    int btnY = cY + 35 + (i * 40);
                    if (x >= cX + 200 && x <= cX + 300 && y >= btnY && y <= btnY + 25) {
                        MessageBoxW(hwnd, (L"Đã đánh dấu có mặt cho: " + g_records[i].studentName).c_str(), L"Điểm danh", MB_OK);
                        break;
                    }
                }
            }
            if (g_currentTab == SCORE) {
                for (int i = 0; i < (int)g_records.size(); i++) {
                    int btnY = cY + 35 + (i * 40);
                    if (x >= cX + 350 && x <= cX + 440 && y >= btnY && y <= btnY + 25) {
                        MessageBoxW(hwnd, L"Chức năng mở bảng nhập điểm chi tiết...", L"Nhập điểm", MB_OK);
                        break;
                    }
                }
            }
        }
        break;
    }

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        Graphics g(hdc);
        RECT r;
        GetClientRect(hwnd, &r);

        if (!currentSession.isLoggedIn) {
            DrawModernPortal(g, r, isRegisterMode);
        } else {
            DrawDashboard(g, r, currentSession, g_users);
        }

        EndPaint(hwnd, &ps);
        break;
    }

    case WM_COMMAND:
        if (LOWORD(wp) == ID_BTN_SWITCH) {
            isRegisterMode = !isRegisterMode;
            SetWindowTextW(hS, isRegisterMode ? L"Quay lại Đăng nhập" : L"Chưa có tài khoản? Đăng ký");
            InvalidateRect(hwnd, NULL, TRUE);
        }
        if (LOWORD(wp) == ID_BTN_ACTION) {
            wchar_t u[50], p[50], rStr[10];
            GetWindowTextW(hU, u, 50);
            GetWindowTextW(hP, p, 50);
            GetWindowTextW(hR, rStr, 10);
            
            if (isRegisterMode) {
                RegisterAccount(u, p, _wtoi(rStr));
                MessageBoxW(hwnd, L"Đăng ký thành công! Vui lòng chờ Admin duyệt.", L"Thông báo", MB_OK);
                SendMessage(hwnd, WM_COMMAND, ID_BTN_SWITCH, 0);
            } else {
                Role role = AuthenticateUser(u, p, (Role)_wtoi(rStr));
                if (role != NONE) {
                    currentSession = { u, role, true };
                    CenterControls(hwnd); // Ẩn các ô nhập liệu
                    InvalidateRect(hwnd, NULL, TRUE);
                } else {
                    MessageBoxW(hwnd, L"Sai thông tin hoặc tài khoản chưa được duyệt!", L"Lỗi", MB_ICONERROR);
                }
            }
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProcW(hwnd, msg, wp, lp);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE h, HINSTANCE, LPSTR, int n) {
    // Tải toàn bộ dữ liệu từ file
    LoadDatabase();
    LoadStudyData();

    // Khởi tạo GDI+
    ULONG_PTR t;
    GdiplusStartupInput gsi;
    GdiplusStartup(&t, &gsi, NULL);

    // Đăng ký lớp cửa sổ
    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = h;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"STUDENT_MGMT_APP";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClassW(&wc);

    // Tạo cửa sổ chính
    HWND hwnd = CreateWindowW(L"STUDENT_MGMT_APP", L"Hệ thống Quản lý Sinh viên & Giảng viên", 
                              WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1200, 750, 
                              NULL, NULL, h, NULL);

    ShowWindow(hwnd, n);
    UpdateWindow(hwnd);

    // Vòng lặp tin nhắn
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Giải phóng GDI+
    GdiplusShutdown(t);
    return (int)msg.wParam;
}