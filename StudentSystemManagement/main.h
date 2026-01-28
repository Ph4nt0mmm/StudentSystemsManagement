#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include "auth.h"
#include "database.h"
#include "Frontend.h"

#pragma comment(lib, "gdiplus.lib")

#define ID_BTN_ACTION 101
#define ID_BTN_SWITCH 102

Session currentSession = { L"", NONE, false };
bool isRegisterMode = false;
HWND hU, hP, hR, hB, hS;

HFONT hFontMain;

void CenterControls(HWND hwnd) {
    RECT rc;
    GetClientRect(hwnd, &rc);

    int centerX = (rc.right - 280) / 2;
    int startY  = rc.bottom / 2 - 150;

    MoveWindow(hU, centerX, startY,       280, 40, TRUE);
    MoveWindow(hP, centerX, startY + 60,  280, 40, TRUE);
    MoveWindow(hR, centerX, startY + 120, 280, 40, TRUE);
    MoveWindow(hB, centerX, startY + 180, 280, 50, TRUE);
    MoveWindow(hS, centerX, startY + 240, 280, 30, TRUE);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {

    case WM_CREATE: {
        hFontMain = CreateFontW(
            20, 0, 0, 0,
            FW_SEMIBOLD,
            FALSE, FALSE, FALSE,
            DEFAULT_CHARSET,
            OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY,
            VARIABLE_PITCH,
            L"Segoe UI"
        );

        hU = CreateWindowW(L"Edit", L"", WS_CHILD | WS_BORDER | WS_VISIBLE,
            0, 0, 0, 0, hwnd, NULL, NULL, NULL);

        hP = CreateWindowW(L"Edit", L"", WS_CHILD | WS_BORDER | WS_VISIBLE | ES_PASSWORD,
            0, 0, 0, 0, hwnd, NULL, NULL, NULL);

        hR = CreateWindowW(L"Edit", L"2", WS_CHILD | WS_BORDER | WS_VISIBLE,
            0, 0, 0, 0, hwnd, NULL, NULL, NULL);

        hB = CreateWindowW(L"Button", L"XÁC NHẬN", WS_CHILD | WS_VISIBLE,
            0, 0, 0, 0, hwnd, (HMENU)ID_BTN_ACTION, NULL, NULL);

        hS = CreateWindowW(L"Button", L"Chưa có tài khoản? Đăng ký",
            WS_CHILD | WS_VISIBLE | BS_FLAT,
            0, 0, 0, 0, hwnd, (HMENU)ID_BTN_SWITCH, NULL, NULL);

        SendMessage(hU, WM_SETFONT, (WPARAM)hFontMain, TRUE);
        SendMessage(hP, WM_SETFONT, (WPARAM)hFontMain, TRUE);
        SendMessage(hR, WM_SETFONT, (WPARAM)hFontMain, TRUE);
        SendMessage(hB, WM_SETFONT, (WPARAM)hFontMain, TRUE);
        SendMessage(hS, WM_SETFONT, (WPARAM)hFontMain, TRUE);

        CenterControls(hwnd);
        break;
    }

    case WM_SIZE:
        CenterControls(hwnd);
        break;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        Graphics g(hdc);
        RECT r;
        GetClientRect(hwnd, &r);

        if (!currentSession.isLoggedIn)
            DrawModernPortal(g, r, isRegisterMode);
        else
            DrawDashboard(g, r, currentSession, g_users);

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

            int roleInt = _wtoi(rStr);

            if (isRegisterMode) {
                RegisterAccount(u, p, roleInt);
                MessageBoxW(hwnd, L"Đăng ký xong! Chờ Admin duyệt.", L"Thông báo", MB_OK);
                SendMessage(hwnd, WM_COMMAND, ID_BTN_SWITCH, 0);
            }
            else {
                Role role = AuthenticateUser(u, p, (Role)roleInt);
                if (role != NONE) {
                    currentSession = { u, role, true };
                    ShowWindow(hU, SW_HIDE);
                    ShowWindow(hP, SW_HIDE);
                    ShowWindow(hR, SW_HIDE);
                    ShowWindow(hB, SW_HIDE);
                    ShowWindow(hS, SW_HIDE);
                    InvalidateRect(hwnd, NULL, TRUE);
                }
                else {
                    MessageBoxW(hwnd, L"Lỗi đăng nhập hoặc tài khoản chưa duyệt!", L"Lỗi", MB_ICONERROR);
                }
            }
        }
        break;

    case WM_DESTROY:
        DeleteObject(hFontMain);
        PostQuitMessage(0);
        break;
    }

    return DefWindowProcW(hwnd, msg, wp, lp);
}

int WINAPI WinMain(HINSTANCE h, HINSTANCE, LPSTR, int n) {
    LoadDatabase();

    ULONG_PTR token;
    GdiplusStartupInput gdi;
    GdiplusStartup(&token, &gdi, NULL);

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = h;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"STUDENT_SYS";

    RegisterClassW(&wc);

    HWND hwnd = CreateWindowW(
        L"STUDENT_SYS",
        L"Hệ thống Quản lý Sinh viên",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        1200, 750,
        NULL, NULL, h, NULL
    );

    ShowWindow(hwnd, n);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(token);
    return 0;
}
