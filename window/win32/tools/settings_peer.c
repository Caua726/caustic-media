/* window/win32/tools/settings_peer.c — an independent peer that changes the
 * desktop's settings the way the Settings app does, for the window layer's
 * Windows tests; written against the Windows SDK alone (none of this
 * repository's code), built with mingw-w64:
 *
 *   x86_64-w64-mingw32-gcc -O2 -o build/settings_peer.exe \
 *       window/win32/tools/settings_peer.c -lgdi32
 *
 *   settings_peer.exe dark 0|1             Personalize\AppsUseLightTheme, then
 *                                          WM_SETTINGCHANGE "ImmersiveColorSet"
 *   settings_peer.exe highlight <rrggbb>   SetSysColors(COLOR_HIGHLIGHT): the
 *                                          system sends WM_SYSCOLORCHANGE
 *   settings_peer.exe text-scale <percent> Accessibility\TextScaleFactor, then
 *                                          WM_SETTINGCHANGE
 *   settings_peer.exe double-click <ms>    SPI_SETDOUBLECLICKTIME
 *   settings_peer.exe animations 0|1       SPI_SETCLIENTAREAANIMATION
 *   settings_peer.exe high-contrast 0|1    SPI_SETHIGHCONTRAST
 *   settings_peer.exe font <face> <points> SPI_SETNONCLIENTMETRICS, the
 *                                          message font
 *
 * Each SPI_SET* is made with SPIF_UPDATEINIFILE | SPIF_SENDCHANGE, so the
 * system broadcasts WM_SETTINGCHANGE itself. Exit 0 when the change was
 * made, 1 when it was refused. */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int set_dword(const WCHAR *path, const WCHAR *name, DWORD value) {
    return RegSetKeyValueW(HKEY_CURRENT_USER, path, name, REG_DWORD, &value, sizeof value) == ERROR_SUCCESS;
}

static void broadcast(const WCHAR *what) {
    DWORD_PTR result;
    SendMessageTimeoutW(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)what, SMTO_ABORTIFHUNG, 2000, &result);
}

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "usage: settings_peer.exe dark|text-scale|double-click|animations|high-contrast|font ...\n");
        return 2;
    }
    const char *what = argv[1];
    int value = atoi(argv[2]);
    const UINT send = SPIF_UPDATEINIFILE | SPIF_SENDCHANGE;
    if (strcmp(what, "dark") == 0) {
        if (!set_dword(L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
                       L"AppsUseLightTheme", value ? 0 : 1)) return 1;
        broadcast(L"ImmersiveColorSet");
        return 0;
    }
    if (strcmp(what, "highlight") == 0) {
        unsigned long rgb = strtoul(argv[2], NULL, 16);
        INT element = COLOR_HIGHLIGHT;
        COLORREF color = RGB((rgb >> 16) & 255, (rgb >> 8) & 255, rgb & 255);
        return SetSysColors(1, &element, &color) ? 0 : 1;
    }
    if (strcmp(what, "text-scale") == 0) {
        if (!set_dword(L"Software\\Microsoft\\Accessibility", L"TextScaleFactor", (DWORD)value)) return 1;
        broadcast(NULL);
        return 0;
    }
    if (strcmp(what, "double-click") == 0) {
        return SystemParametersInfoW(SPI_SETDOUBLECLICKTIME, (UINT)value, NULL, send) ? 0 : 1;
    }
    if (strcmp(what, "animations") == 0) {
        return SystemParametersInfoW(SPI_SETCLIENTAREAANIMATION, 0, (PVOID)(INT_PTR)(value != 0), send) ? 0 : 1;
    }
    if (strcmp(what, "high-contrast") == 0) {
        HIGHCONTRASTW hc;
        memset(&hc, 0, sizeof hc);
        hc.cbSize = sizeof hc;
        hc.dwFlags = value ? HCF_HIGHCONTRASTON : 0;
        return SystemParametersInfoW(SPI_SETHIGHCONTRAST, sizeof hc, &hc, send) ? 0 : 1;
    }
    if (strcmp(what, "font") == 0 && argc > 3) {
        NONCLIENTMETRICSW m;
        memset(&m, 0, sizeof m);
        m.cbSize = sizeof m;
        if (!SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, sizeof m, &m, 0)) return 1;
        MultiByteToWideChar(CP_UTF8, 0, argv[2], -1, m.lfMessageFont.lfFaceName, LF_FACESIZE);
        HDC dc = GetDC(NULL);
        m.lfMessageFont.lfHeight = -MulDiv(atoi(argv[3]), GetDeviceCaps(dc, LOGPIXELSY), 72);
        ReleaseDC(NULL, dc);
        return SystemParametersInfoW(SPI_SETNONCLIENTMETRICS, sizeof m, &m, send) ? 0 : 1;
    }
    return 2;
}
