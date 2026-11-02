#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#include <wrl/client.h>
#include <wrl/event.h>
#include <string>
#include <vector>
#include <memory>
#include <iostream>

#include "WebView2.h"
#include "json.hpp"

using Microsoft::WRL::ComPtr;
using Microsoft::WRL::Callback;
using json = nlohmann::json;

#define WM_TRAYICON (WM_USER + 1)
#define ID_TRAY_APP_ICON 1001
#define ID_TRAY_EXIT 2001
#define ID_TRAY_SHOW 2002
#define ID_TRAY_TOGGLE_MODE 2003

#define ID_ALERT_BTN_ACK 3001
#define ID_ALERT_BTN_OPEN_APP 3002

// Global State
HINSTANCE g_hInstance = NULL;
HWND g_hWnd = NULL;
HWND g_hAlertWnd = NULL;
NOTIFYICONDATAW g_nid = { sizeof(NOTIFYICONDATAW) };
ComPtr<ICoreWebView2Controller> g_webViewController = nullptr;
ComPtr<ICoreWebView2> g_webView = nullptr;
bool g_isWindowMode = false;
bool g_isVisible = false;
bool g_webViewReady = false;
std::string g_currentLanguage = "tr";

// Dimensions
const int FLYOUT_WIDTH = 680;
const int FLYOUT_HEIGHT = 750;
const int WINDOW_WIDTH = 1050;
const int WINDOW_HEIGHT = 780;

const int ALERT_WIDTH = 480;
const int ALERT_HEIGHT = 340;

// Forward declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK AlertWndProc(HWND, UINT, WPARAM, LPARAM);
void ShowFlyout(HWND hWnd);
void ToggleWindowMode(HWND hWnd);
void PositionFlyout(HWND hWnd);
void ShowDedicatedAlertWindow(const std::wstring& title, const std::wstring& message);

// Convert Wide string to UTF-8 std::string
std::string WideToUtf8(const std::wstring& wstr) {
    if (wstr.empty()) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

std::wstring Utf8ToWide(const std::string& str) {
    if (str.empty()) return std::wstring();
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

std::wstring FilePathToUri(const std::wstring& path) {
    std::wstring uri = L"file:///";
    for (wchar_t c : path) {
        if (c == L'\\') uri += L'/';
        else uri += c;
    }
    return uri;
}

std::wstring GetUiDirectory() {
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);
    std::wstring path(exePath);
    size_t lastBackslash = path.find_last_of(L"\\/");
    std::wstring dir = (lastBackslash != std::wstring::npos) ? path.substr(0, lastBackslash) : L".";

    std::wstring uiDir = dir + L"\\ui";
    DWORD attr = GetFileAttributesW((uiDir + L"\\index.html").c_str());
    if (attr != INVALID_FILE_ATTRIBUTES) {
        return uiDir;
    }

    uiDir = dir + L"\\..\\src\\ui";
    attr = GetFileAttributesW((uiDir + L"\\index.html").c_str());
    if (attr != INVALID_FILE_ATTRIBUTES) {
        return uiDir;
    }

    return dir;
}

void UpdateWebViewBounds(HWND hWnd) {
    if (g_webViewController) {
        RECT bounds;
        GetClientRect(hWnd, &bounds);
        g_webViewController->put_Bounds(bounds);
    }
}

// -------------------------------------------------------------
// Dedicated Native Alert Window (Bottom-Right, Responsive Buttons)
// -------------------------------------------------------------
static std::wstring g_alertTitle = L"⚠️ KRİTİK SUNUCU UYARISI";
static std::wstring g_alertMessage = L"Anormal değerler tespit edildi!";
static HFONT g_hFontTitle = NULL;
static HFONT g_hFontBody = NULL;
static HBRUSH g_hBgBrush = NULL;
static HBRUSH g_hCardBrush = NULL;

LRESULT CALLBACK AlertWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        g_hFontTitle = CreateFontW(17, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
        g_hFontBody = CreateFontW(13, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
        g_hBgBrush = CreateSolidBrush(RGB(15, 23, 42));       // Slate 900
        g_hCardBrush = CreateSolidBrush(RGB(30, 41, 59));     // Slate 800

        std::wstring openText = (g_currentLanguage == "en") ? L"Open Control Panel" : L"Kontrol Panelini Aç";
        std::wstring ackText  = (g_currentLanguage == "en") ? L"Acknowledge & Close" : L"Onayla ve Kapat";

        HWND btnOpen = CreateWindowW(L"BUTTON", openText.c_str(), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            18, 252, 210, 36, hWnd, (HMENU)ID_ALERT_BTN_OPEN_APP, g_hInstance, NULL);
        SendMessageW(btnOpen, WM_SETFONT, (WPARAM)g_hFontBody, TRUE);

        HWND btnAck = CreateWindowW(L"BUTTON", ackText.c_str(), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            242, 252, 215, 36, hWnd, (HMENU)ID_ALERT_BTN_ACK, g_hInstance, NULL);
        SendMessageW(btnAck, WM_SETFONT, (WPARAM)g_hFontBody, TRUE);

        break;
    }

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        RECT clientRect;
        GetClientRect(hWnd, &clientRect);
        FillRect(hdc, &clientRect, g_hBgBrush);

        // Header warning bar
        RECT headerRect = { 0, 0, clientRect.right, 42 };
        HBRUSH redBar = CreateSolidBrush(RGB(220, 38, 38)); // Red 600
        FillRect(hdc, &headerRect, redBar);
        DeleteObject(redBar);

        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(255, 255, 255));
        SelectObject(hdc, g_hFontTitle);

        RECT titleTextRect = { 18, 10, clientRect.right - 18, 38 };
        DrawTextW(hdc, g_alertTitle.c_str(), -1, &titleTextRect, DT_LEFT | DT_SINGLELINE);

        // Inner Card Box for Message Content
        RECT cardRect = { 18, 52, clientRect.right - 18, 240 };
        FillRect(hdc, &cardRect, g_hCardBrush);
        FrameRect(hdc, &cardRect, (HBRUSH)GetStockObject(GRAY_BRUSH));

        SelectObject(hdc, g_hFontBody);
        SetTextColor(hdc, RGB(241, 245, 249)); // Slate 100

        RECT bodyTextRect = { 28, 62, clientRect.right - 28, 230 };
        DrawTextW(hdc, g_alertMessage.c_str(), -1, &bodyTextRect, DT_LEFT | DT_WORDBREAK);

        EndPaint(hWnd, &ps);
        break;
    }

    case WM_COMMAND: {
        int wmId = LOWORD(wParam);
        if (wmId == ID_ALERT_BTN_ACK) {
            DestroyWindow(hWnd);
            g_hAlertWnd = NULL;
        } else if (wmId == ID_ALERT_BTN_OPEN_APP) {
            DestroyWindow(hWnd);
            g_hAlertWnd = NULL;
            ShowFlyout(g_hWnd);
        }
        break;
    }

    case WM_DESTROY:
        if (g_hFontTitle) DeleteObject(g_hFontTitle);
        if (g_hFontBody) DeleteObject(g_hFontBody);
        if (g_hBgBrush) DeleteObject(g_hBgBrush);
        if (g_hCardBrush) DeleteObject(g_hCardBrush);
        g_hAlertWnd = NULL;
        break;

    default:
        return DefWindowProcW(hWnd, msg, wParam, lParam);
    }
    return 0;
}

void ShowDedicatedAlertWindow(const std::wstring& title, const std::wstring& message) {
    g_alertTitle = title;
    g_alertMessage = message;

    // Get work area (excludes taskbar)
    RECT workArea;
    SystemParametersInfoW(SPI_GETWORKAREA, 0, &workArea, 0);

    int w = ALERT_WIDTH;
    int h = ALERT_HEIGHT;
    // Position at Bottom-Right corner
    int x = workArea.right - w - 16;
    int y = workArea.bottom - h - 16;

    if (g_hAlertWnd != NULL && IsWindow(g_hAlertWnd)) {
        InvalidateRect(g_hAlertWnd, NULL, TRUE);
        SetWindowPos(g_hAlertWnd, HWND_TOPMOST, x, y, w, h, SWP_SHOWWINDOW);
        SetForegroundWindow(g_hAlertWnd);
        MessageBeep(MB_ICONWARNING);
        FlashWindow(g_hAlertWnd, TRUE);
        return;
    }

    const wchar_t ALERT_CLASS[] = L"LinuxServerHelperAlertWindowClass";
    WNDCLASSEXW awc = { sizeof(WNDCLASSEXW) };
    awc.lpfnWndProc = AlertWndProc;
    awc.hInstance = g_hInstance;
    awc.lpszClassName = ALERT_CLASS;
    awc.hCursor = LoadCursor(NULL, IDC_ARROW);
    awc.hbrBackground = NULL;
    awc.hIcon = LoadIcon(NULL, IDI_WARNING);
    RegisterClassExW(&awc);

    g_hAlertWnd = CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_APPWINDOW,
        ALERT_CLASS,
        title.c_str(),
        WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
        x, y, w, h,
        NULL, NULL, g_hInstance, NULL
    );

    if (g_hAlertWnd) {
        SetForegroundWindow(g_hAlertWnd);
        MessageBeep(MB_ICONWARNING);
        FlashWindow(g_hAlertWnd, TRUE);
    }
}

// -------------------------------------------------------------
// Main Application Window & WebView2
// -------------------------------------------------------------
void InitWebView(HWND hWnd) {
    SetEnvironmentVariableW(L"WEBVIEW2_ADDITIONAL_BROWSER_ARGUMENTS", 
        L"--disable-web-security --allow-running-insecure-content --ignore-certificate-errors");

    wchar_t localAppData[MAX_PATH];
    GetEnvironmentVariableW(L"LOCALAPPDATA", localAppData, MAX_PATH);
    std::wstring dataFolder = std::wstring(localAppData) + L"\\LinuxServerHelper\\WebView2Data";

    HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(
        nullptr,
        dataFolder.c_str(),
        nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [hWnd](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {
                if (FAILED(result) || !env) {
                    wchar_t errMsg[256];
                    swprintf_s(errMsg, L"WebView2 ortamı başlatılamadı!\nHRESULT: 0x%08X", result);
                    MessageBoxW(hWnd, errMsg, L"Hata", MB_OK | MB_ICONERROR);
                    return result;
                }

                env->CreateCoreWebView2Controller(
                    hWnd,
                    Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [hWnd](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
                            if (FAILED(result) || !controller) {
                                wchar_t errMsg[256];
                                swprintf_s(errMsg, L"WebView2 controller oluşturulamadı!\nHRESULT: 0x%08X", result);
                                MessageBoxW(hWnd, errMsg, L"Hata", MB_OK | MB_ICONERROR);
                                return result;
                            }

                            g_webViewController = controller;
                            g_webViewController->get_CoreWebView2(&g_webView);

                            g_webViewController->put_IsVisible(TRUE);
                            UpdateWebViewBounds(hWnd);

                            ComPtr<ICoreWebView2Settings> settings;
                            if (SUCCEEDED(g_webView->get_Settings(&settings)) && settings) {
                                settings->put_AreDefaultContextMenusEnabled(TRUE);
                                settings->put_AreDevToolsEnabled(TRUE);
                                settings->put_IsStatusBarEnabled(FALSE);
                            }

                            // Message handler for interop
                            EventRegistrationToken token;
                            g_webView->add_WebMessageReceived(
                                Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                                    [](ICoreWebView2*, ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT {
                                        LPWSTR messageRaw = nullptr;
                                        args->TryGetWebMessageAsString(&messageRaw);
                                        if (messageRaw) {
                                            std::wstring ws(messageRaw);
                                            CoTaskMemFree(messageRaw);
                                            std::string msg = WideToUtf8(ws);
                                            try {
                                                auto j = json::parse(msg);
                                                std::string action = j.value("action", "");
                                                if (action == "toggle_window_mode") {
                                                    ToggleWindowMode(g_hWnd);
                                                } else if (action == "minimize") {
                                                    ShowWindow(g_hWnd, SW_MINIMIZE);
                                                } else if (action == "hide") {
                                                    ShowWindow(g_hWnd, SW_HIDE);
                                                    g_isVisible = false;
                                                } else if (action == "exit_app") {
                                                    PostQuitMessage(0);
                                                } else if (action == "set_language") {
                                                    g_currentLanguage = j.value("lang", "tr");
                                                } else if (action == "show_alert_window") {
                                                    std::string titleStr = j.value("title", "⚠️ KRİTİK SUNUCU UYARISI");
                                                    std::string messageStr = j.value("message", "Anormal sistem değerleri!");
                                                    ShowDedicatedAlertWindow(Utf8ToWide(titleStr), Utf8ToWide(messageStr));
                                                }
                                            } catch(...) {}
                                        }
                                        return S_OK;
                                    }
                                ).Get(),
                                &token
                            );

                            std::wstring uiDir = GetUiDirectory();
                            std::wstring indexFile = uiDir + L"\\index.html";
                            std::wstring fileUri = FilePathToUri(indexFile);
                            g_webView->Navigate(fileUri.c_str());

                            g_webViewReady = true;
                            UpdateWebViewBounds(hWnd);

                            return S_OK;
                        }
                    ).Get()
                );
                return S_OK;
            }
        ).Get()
    );

    if (FAILED(hr)) {
        wchar_t errMsg[256];
        swprintf_s(errMsg, L"CreateCoreWebView2EnvironmentWithOptions çağrısı başarısız!\nHRESULT: 0x%08X", hr);
        MessageBoxW(hWnd, errMsg, L"Hata", MB_OK | MB_ICONERROR);
    }
}

void PositionFlyout(HWND hWnd) {
    APPBARDATA abd = { sizeof(APPBARDATA) };
    SHAppBarMessage(ABM_GETTASKBARPOS, &abd);

    RECT workArea;
    SystemParametersInfoW(SPI_GETWORKAREA, 0, &workArea, 0);

    int width = FLYOUT_WIDTH;
    int height = FLYOUT_HEIGHT;

    if (height > (workArea.bottom - workArea.top - 20)) {
        height = workArea.bottom - workArea.top - 20;
    }

    int x = workArea.right - width - 14;
    int y = workArea.bottom - height - 12;

    if (abd.uEdge == ABE_LEFT) {
        x = workArea.left + 14;
        y = workArea.bottom - height - 12;
    } else if (abd.uEdge == ABE_TOP) {
        x = workArea.right - width - 14;
        y = workArea.top + 12;
    }

    SetWindowPos(hWnd, HWND_TOPMOST, x, y, width, height, SWP_SHOWWINDOW);
    UpdateWebViewBounds(hWnd);
}

void ShowFlyout(HWND hWnd) {
    if (g_isVisible && !g_isWindowMode) {
        ShowWindow(hWnd, SW_HIDE);
        g_isVisible = false;
    } else {
        if (!g_isWindowMode) {
            PositionFlyout(hWnd);
        } else {
            ShowWindow(hWnd, SW_RESTORE);
        }
        SetForegroundWindow(hWnd);
        g_isVisible = true;
        UpdateWebViewBounds(hWnd);
    }
}

void ToggleWindowMode(HWND hWnd) {
    g_isWindowMode = !g_isWindowMode;

    if (g_isWindowMode) {
        SetWindowLongPtr(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
        SetWindowPos(hWnd, HWND_NOTOPMOST, 120, 80, WINDOW_WIDTH, WINDOW_HEIGHT, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    } else {
        SetWindowLongPtr(hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
        PositionFlyout(hWnd);
    }

    UpdateWebViewBounds(hWnd);
}

void ShowContextMenu(HWND hWnd) {
    POINT pt;
    GetCursorPos(&pt);

    HMENU hMenu = CreatePopupMenu();

    std::wstring showText = (g_currentLanguage == "en") ? L"Open / Show" : L"Aç / Göster";
    std::wstring toggleText;
    if (g_isWindowMode) {
        toggleText = (g_currentLanguage == "en") ? L"Switch to Flyout Mode" : L"Flyout Moduna Geç";
    } else {
        toggleText = (g_currentLanguage == "en") ? L"Switch to Window Mode" : L"Pencere Moduna Geç";
    }
    std::wstring exitText = (g_currentLanguage == "en") ? L"Exit" : L"Çıkış";

    InsertMenuW(hMenu, 0, MF_BYPOSITION | MF_STRING, ID_TRAY_SHOW, showText.c_str());
    InsertMenuW(hMenu, 1, MF_BYPOSITION | MF_STRING, ID_TRAY_TOGGLE_MODE, toggleText.c_str());
    InsertMenuW(hMenu, 2, MF_BYPOSITION | MF_SEPARATOR, 0, nullptr);
    InsertMenuW(hMenu, 3, MF_BYPOSITION | MF_STRING, ID_TRAY_EXIT, exitText.c_str());

    SetForegroundWindow(hWnd);
    TrackPopupMenu(hMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, pt.x, pt.y, 0, hWnd, NULL);
    DestroyMenu(hMenu);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_SIZE:
        UpdateWebViewBounds(hWnd);
        break;

    case WM_MOVE:
        if (g_webViewController) {
            g_webViewController->NotifyParentWindowPositionChanged();
        }
        break;

    case WM_ACTIVATE:
        if (LOWORD(wParam) == WA_INACTIVE && !g_isWindowMode && g_isVisible) {
            HWND foreground = GetForegroundWindow();
            if (foreground != g_hAlertWnd) {
                ShowWindow(hWnd, SW_HIDE);
                g_isVisible = false;
            }
        }
        break;

    case WM_TRAYICON:
        if (lParam == WM_LBUTTONUP) {
            ShowFlyout(hWnd);
        } else if (lParam == WM_RBUTTONUP) {
            ShowContextMenu(hWnd);
        }
        break;

    case WM_COMMAND: {
        int wmId = LOWORD(wParam);
        if (wmId == ID_TRAY_EXIT) {
            Shell_NotifyIconW(NIM_DELETE, &g_nid);
            PostQuitMessage(0);
        } else if (wmId == ID_TRAY_SHOW) {
            ShowFlyout(hWnd);
        } else if (wmId == ID_TRAY_TOGGLE_MODE) {
            ToggleWindowMode(hWnd);
        }
        break;
    }

    case WM_CLOSE:
        ShowWindow(hWnd, SW_HIDE);
        g_isVisible = false;
        return 0;

    case WM_DESTROY:
        Shell_NotifyIconW(NIM_DELETE, &g_nid);
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProcW(hWnd, message, wParam, lParam);
    }
    return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int) {
    g_hInstance = hInstance;
    HRESULT hrCom = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (FAILED(hrCom)) {
        MessageBoxW(NULL, L"COM başlatılamadı!", L"Kritik Hata", MB_OK | MB_ICONERROR);
        return 0;
    }

    HANDLE hMutex = CreateMutexW(NULL, TRUE, L"LinuxServerHelper_SingleInstance_Mutex");
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        MessageBoxW(NULL, L"Linux Server Helper zaten çalışıyor. Görev çubuğundaki simgeyi kontrol edin.", L"Bilgi", MB_OK | MB_ICONINFORMATION);
        CoUninitialize();
        return 0;
    }

    const wchar_t CLASS_NAME[] = L"LinuxServerHelperWindowClass";
    WNDCLASSEXW wc = { sizeof(WNDCLASSEXW) };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    HICON hAppIcon = (HICON)LoadImageW(hInstance, L"resources\\app.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
    if (!hAppIcon) {
        hAppIcon = LoadIcon(NULL, IDI_APPLICATION);
    }
    wc.hIcon = hAppIcon;
    wc.hIconSm = hAppIcon;

    RegisterClassExW(&wc);

    g_hWnd = CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
        CLASS_NAME,
        L"Linux Server Helper",
        WS_POPUP,
        0, 0, FLYOUT_WIDTH, FLYOUT_HEIGHT,
        NULL, NULL, hInstance, NULL
    );

    if (!g_hWnd) {
        CoUninitialize();
        return 0;
    }

    g_nid.hWnd = g_hWnd;
    g_nid.uID = ID_TRAY_APP_ICON;
    g_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    g_nid.uCallbackMessage = WM_TRAYICON;
    g_nid.hIcon = hAppIcon;
    wcscpy_s(g_nid.szTip, L"Linux Server Helper");
    Shell_NotifyIconW(NIM_ADD, &g_nid);

    InitWebView(g_hWnd);

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    if (hMutex) {
        CloseHandle(hMutex);
    }

    CoUninitialize();
    return (int)msg.wParam;
}
