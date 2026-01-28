#include <windows.h>
#include <gdiplus.h>
#include "auth.h"
#include "helpers.h"
#include "database.h"
#include "Frontend.h"
#include "pages.h"

using namespace Gdiplus;

Session currentSession = { L"", NONE, false, MENU_DASHBOARD };
bool isRegisterMode = false;
HWND hU, hP, hR, hB, hS;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE: {
            hU = CreateWindowExW(0, L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 770, 230, 280, 35, hwnd, NULL, NULL, NULL);
            hP = CreateWindowExW(0, L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD, 770, 300, 280, 35, hwnd, NULL, NULL, NULL);
            hR = CreateWindowExW(0, L"Edit", L"0", WS_VISIBLE | WS_CHILD | WS_BORDER, 770, 390, 280, 35, hwnd, NULL, NULL, NULL);
            hB = CreateWindowW(L"Button", L"XÁC NHẬN", WS_VISIBLE | WS_CHILD, 770, 460, 280, 50, hwnd, (HMENU)101, NULL, NULL);
            hS = CreateWindowW(L"Button", L"Đăng ký", WS_VISIBLE | WS_CHILD, 770, 520, 280, 30, hwnd, (HMENU)102, NULL, NULL);
            break;
        }
        case WM_LBUTTONDOWN: {
            int x = LOWORD(lp), y = HIWORD(lp);
            if (currentSession.isLoggedIn && x < 250) {
                if (y >= 120 && y < 180) currentSession.currentView = MENU_DASHBOARD;
                else if (y >= 180 && y < 240) currentSession.currentView = MENU_COURSE;
                else if (y >= 240 && y < 300) currentSession.currentView = MENU_SCHEDULE;
                else if (y >= 300 && y < 360) currentSession.currentView = MENU_ATTENDANCE;
                else if (y >= 360 && y < 420) currentSession.currentView = MENU_SCORE;
                else if (y >= 640) { // Logout
                    currentSession.isLoggedIn = false;
                    ShowWindow(hU, SW_SHOW); ShowWindow(hP, SW_SHOW); 
                    ShowWindow(hR, SW_SHOW); ShowWindow(hB, SW_SHOW); ShowWindow(hS, SW_SHOW);
                }
                InvalidateRect(hwnd, NULL, TRUE);
            }
            break;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps; HDC hdc = BeginPaint(hwnd, &ps);
            Graphics g(hdc); RECT r; GetClientRect(hwnd, &r);
            if (!currentSession.isLoggedIn) DrawModernPortal(g, r, isRegisterMode);
            else {
                DrawDashboardFrame(g, r, currentSession);
                switch (currentSession.currentView) {
                    case MENU_DASHBOARD: Pages::DrawDashboard(g, currentSession); break;
                    case MENU_COURSE: Pages::DrawCourse(g); break;
                    case MENU_SCHEDULE: Pages::DrawSchedule(g); break;
                    case MENU_ATTENDANCE: Pages::DrawAttendance(g); break;
                    case MENU_SCORE: Pages::DrawScore(g); break;
                }
            }
            EndPaint(hwnd, &ps); break;
        }
        case WM_COMMAND: {
            if (LOWORD(wp) == 101) {
                wchar_t u[50], p[50], rs[10];
                GetWindowTextW(hU, u, 50); GetWindowTextW(hP, p, 50); GetWindowTextW(hR, rs, 10);
                Role r = Database::AuthenticateUser(u, p, (Role)_wtoi(rs));
                if (r != NONE) {
                    currentSession.isLoggedIn = true; currentSession.role = r; currentSession.username = u;
                    ShowWindow(hU, SW_HIDE); ShowWindow(hP, SW_HIDE); ShowWindow(hR, SW_HIDE);
                    ShowWindow(hB, SW_HIDE); ShowWindow(hS, SW_HIDE);
                } else MessageBoxW(hwnd, L"Sai tài khoản!", L"Lỗi", MB_ICONERROR);
                InvalidateRect(hwnd, NULL, TRUE);
            }
            break;
        }
        case WM_DESTROY: PostQuitMessage(0); break;
        default: return DefWindowProcW(hwnd, msg, wp, lp);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hI, HINSTANCE hP, LPSTR l, int n) {
    Database::LoadDatabase();
    ULONG_PTR t; GdiplusStartupInput si; GdiplusStartup(&t, &si, NULL);
    WNDCLASSW wc = {0}; wc.lpfnWndProc = WndProc; wc.hInstance = hI; wc.lpszClassName = L"App";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); RegisterClassW(&wc);
    HWND hwnd = CreateWindowExW(0, L"App", L"Hệ thống Quản lý", WS_OVERLAPPEDWINDOW, 100, 50, 1200, 750, 0, 0, hI, 0);
    ShowWindow(hwnd, n);
    MSG msg; while (GetMessage(&msg, 0, 0, 0)) { TranslateMessage(&msg); DispatchMessage(&msg); }
    GdiplusShutdown(t); return 0;
}