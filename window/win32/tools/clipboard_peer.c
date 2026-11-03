/* window/win32/tools/clipboard_peer.c — an independent clipboard peer for
 * the window layer's Windows tests, written against the Windows SDK alone
 * (none of this repository's code), built with mingw-w64:
 *
 *   x86_64-w64-mingw32-gcc -O2 -o build/clipboard_peer.exe \
 *       window/win32/tools/clipboard_peer.c
 *
 *   clipboard_peer.exe put <text|-> [png]   the clipboard emptied and owned:
 *                                           CF_UNICODETEXT of the UTF-8 file
 *                                           text ("-": none), the registered
 *                                           "PNG" of the file png
 *   clipboard_peer.exe get-text <out>       CF_UNICODETEXT written to out as
 *                                           UTF-8; exit 3 when not there
 *   clipboard_peer.exe get-png <out>        "PNG" written to out as it is;
 *                                           exit 3 when not there
 *   clipboard_peer.exe formats <out>        the formats there, a line each:
 *                                           CF_UNICODETEXT, CF_TEXT, PNG, or
 *                                           the number
 *   clipboard_peer.exe put-named <name> <file>   the clipboard emptied and
 *                                           owned: the format registered as
 *                                           name, the file's bytes as they are
 *   clipboard_peer.exe get-named <name> <out>    the format registered as name
 *                                           written to out; exit 3 when not
 *                                           there
 *   clipboard_peer.exe put-html <file>      "HTML Format" of the file's
 *                                           fragment, inside a page, with the
 *                                           header of offsets Windows' programs
 *                                           write
 *   clipboard_peer.exe put-cf <n> <file>    format number n (17: CF_DIBV5),
 *                                           the file's bytes
 *   clipboard_peer.exe get-cf <n> <out>     format number n written to out;
 *                                           exit 3 when not there
 *
 * Exit 0 when done, 1 when the clipboard could not be had. */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *read_all(const char *path, size_t *n) {
    FILE *f = fopen(path, "rb");
    if (!f) return NULL;
    size_t cap = 65536, len = 0;
    char *b = malloc(cap);
    size_t got;
    while ((got = fread(b + len, 1, cap - len, f)) > 0) {
        len += got;
        if (len == cap) b = realloc(b, cap *= 2);
    }
    fclose(f);
    *n = len;
    return b;
}

static int write_all(const char *path, const void *p, size_t n) {
    FILE *f = fopen(path, "wb");
    if (!f) return 0;
    fwrite(p, 1, n, f);
    fclose(f);
    return 1;
}

/* Opened for hwnd, tried for two seconds: another program may hold it. */
static int open_clipboard(HWND hwnd) {
    for (int i = 0; i < 200; i++) {
        if (OpenClipboard(hwnd)) return 1;
        Sleep(10);
    }
    return 0;
}

static HGLOBAL global_of(const void *p, size_t n) {
    HGLOBAL g = GlobalAlloc(GMEM_MOVEABLE, n);
    if (!g) return NULL;
    memcpy(GlobalLock(g), p, n);
    GlobalUnlock(g);
    return g;
}

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "usage: clipboard_peer.exe put|get-text|get-png|formats ...\n");
        return 2;
    }
    UINT png = RegisterClipboardFormatW(L"PNG");
    if (strcmp(argv[1], "put") == 0) {
        /* An owner: EmptyClipboard with none makes SetClipboardData fail. */
        HWND owner = CreateWindowExW(0, L"STATIC", L"clipboard-peer", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);
        if (!owner || !open_clipboard(owner)) return 1;
        EmptyClipboard();
        if (strcmp(argv[2], "-") != 0) {
            size_t n;
            char *s = read_all(argv[2], &n);
            if (!s) return 1;
            int wn = MultiByteToWideChar(CP_UTF8, 0, s, (int)n, NULL, 0);
            WCHAR *w = calloc(wn + 1, sizeof(WCHAR));
            MultiByteToWideChar(CP_UTF8, 0, s, (int)n, w, wn);
            SetClipboardData(CF_UNICODETEXT, global_of(w, (wn + 1) * sizeof(WCHAR)));
            free(w);
            free(s);
        }
        if (argc > 3) {
            size_t n;
            char *b = read_all(argv[3], &n);
            if (!b) return 1;
            SetClipboardData(png, global_of(b, n));
            free(b);
        }
        CloseClipboard();
        DestroyWindow(owner);
        return 0;
    }
    if (!open_clipboard(NULL)) return 1;
    int status = 0;
    if (strcmp(argv[1], "get-text") == 0) {
        HANDLE h = GetClipboardData(CF_UNICODETEXT);
        if (!h) {
            status = 3;
        } else {
            const WCHAR *w = GlobalLock(h);
            int n = WideCharToMultiByte(CP_UTF8, 0, w, (int)wcslen(w), NULL, 0, NULL, NULL);
            char *s = malloc(n + 1);
            WideCharToMultiByte(CP_UTF8, 0, w, (int)wcslen(w), s, n, NULL, NULL);
            GlobalUnlock(h);
            if (!write_all(argv[2], s, n)) status = 1;
            free(s);
        }
    } else if (strcmp(argv[1], "get-png") == 0) {
        HANDLE h = GetClipboardData(png);
        if (!h) {
            status = 3;
        } else {
            if (!write_all(argv[2], GlobalLock(h), GlobalSize(h))) status = 1;
            GlobalUnlock(h);
        }
    } else if (strcmp(argv[1], "put-html") == 0) {
        size_t n = 0;
        char *frag = read_all(argv[2], &n);
        if (!frag) {
            status = 1;
        } else {
            static const char pre[] = "<html><body><!--StartFragment-->";
            static const char post[] = "<!--EndFragment--></body></html>";
            static const char fmt[] = "Version:0.9\r\nStartHTML:%010d\r\nEndHTML:%010d\r\nStartFragment:%010d\r\nEndFragment:%010d\r\n";
            int head = snprintf(NULL, 0, fmt, 0, 0, 0, 0);
            size_t all = head + (sizeof pre - 1) + n + (sizeof post - 1);
            char *b = malloc(all + 1);
            int start = head + (int)(sizeof pre - 1);
            snprintf(b, head + 1, fmt, head, (int)all, start, start + (int)n);
            memcpy(b + head, pre, sizeof pre - 1);
            memcpy(b + start, frag, n);
            memcpy(b + start + n, post, sizeof post);
            if (!EmptyClipboard() || !SetClipboardData(RegisterClipboardFormatA("HTML Format"), global_of(b, all + 1))) status = 1;
            free(b);
            free(frag);
        }
    } else if ((strcmp(argv[1], "put-named") == 0 || strcmp(argv[1], "put-cf") == 0) && argc >= 4) {
        UINT cf = strcmp(argv[1], "put-cf") == 0 ? (UINT)atoi(argv[2]) : RegisterClipboardFormatA(argv[2]);
        size_t n = 0;
        char *b = read_all(argv[3], &n);
        if (!b || !EmptyClipboard() || !SetClipboardData(cf, global_of(b, n))) status = 1;
        free(b);
    } else if ((strcmp(argv[1], "get-named") == 0 || strcmp(argv[1], "get-cf") == 0) && argc >= 4) {
        UINT cf = strcmp(argv[1], "get-cf") == 0 ? (UINT)atoi(argv[2]) : RegisterClipboardFormatA(argv[2]);
        HANDLE h = GetClipboardData(cf);
        if (!h) {
            status = 3;
        } else {
            if (!write_all(argv[3], GlobalLock(h), GlobalSize(h))) status = 1;
            GlobalUnlock(h);
        }
    } else if (strcmp(argv[1], "formats") == 0) {
        FILE *f = fopen(argv[2], "w");
        if (!f) {
            status = 1;
        } else {
            for (UINT cf = EnumClipboardFormats(0); cf; cf = EnumClipboardFormats(cf)) {
                if (cf == CF_UNICODETEXT) fprintf(f, "CF_UNICODETEXT\n");
                else if (cf == CF_TEXT) fprintf(f, "CF_TEXT\n");
                else if (cf == png) fprintf(f, "PNG\n");
                else {
                    char name[128];
                    if (GetClipboardFormatNameA(cf, name, sizeof name) > 0) fprintf(f, "%s\n", name);
                    else fprintf(f, "%u\n", cf);
                }
            }
            fclose(f);
        }
    } else {
        status = 2;
    }
    CloseClipboard();
    return status;
}
