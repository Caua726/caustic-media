/* window/win32/tools/ole_dnd_peer.c — an independent OLE drag-and-drop peer
 * for the window layer's Windows tests, written against the Windows SDK
 * alone (none of this repository's code), built with mingw-w64:
 *
 *   x86_64-w64-mingw32-gcc -O2 -o build/ole_dnd_peer.exe \
 *       window/win32/tools/ole_dnd_peer.c -lole32 -lshell32 -luuid -lgdi32
 *
 *   ole_dnd_peer.exe <log>
 *
 * Two undecorated windows of 300 by 300: a drop target at 600, 100 and a
 * drag source at 600, 450. The target takes CF_HDROP and CF_UNICODETEXT,
 * files first, a move while Shift is held and the source allows it, else a
 * copy — at the drop, what the last DragOver said (Wine passes no key held
 * to Drop); each drop is logged "drop files|text copy|move <bytes>", its
 * bytes (the paths, each ended by a newline; the text, UTF-8) written to
 * <log>.<n>, n counting the drops from 1. A press in the source activates
 * it, as a click would, and starts DoDragDrop with
 * copy and move allowed of what the band pressed holds — the top third two
 * files (C:\peer one.txt, C:\peer-two), the middle a short text, the bottom
 * 3 MB of text — dropped when the button is let go, cancelled on Escape; its
 * end is logged "end copy|move|none". "ready" is logged once both windows
 * are shown and the target registered. A line each, flushed. */
#define COBJMACROS
#include <windows.h>
#include <ole2.h>
#include <shlobj.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static FILE *logf;
static const char *log_path;
static int drops;

static void say(const char *line) {
    fprintf(logf, "%s\n", line);
    fflush(logf);
}

static void dump(const char *bytes, size_t n) {
    char path[512];
    drops++;
    snprintf(path, sizeof path, "%s.%d", log_path, drops);
    FILE *f = fopen(path, "wb");
    if (f) {
        fwrite(bytes, 1, n, f);
        fclose(f);
    }
}

static char *utf8_of(const WCHAR *w, int wn, int *out_n) {
    int n = WideCharToMultiByte(CP_UTF8, 0, w, wn, NULL, 0, NULL, NULL);
    char *s = malloc(n + 1);
    WideCharToMultiByte(CP_UTF8, 0, w, wn, s, n, NULL, NULL);
    s[n] = 0;
    *out_n = n;
    return s;
}

/* --- The target --- */

typedef struct {
    IDropTarget iface;
    LONG refs;
    int has_files, has_text;
    DWORD last;   /* the effect said last */
} Target;

static FORMATETC fmt_of(CLIPFORMAT cf) {
    FORMATETC f = {cf, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
    return f;
}

static DWORD choose(DWORD keys, DWORD allowed) {
    if ((keys & MK_SHIFT) && (allowed & DROPEFFECT_MOVE)) return DROPEFFECT_MOVE;
    if (allowed & DROPEFFECT_COPY) return DROPEFFECT_COPY;
    if (allowed & DROPEFFECT_MOVE) return DROPEFFECT_MOVE;
    return DROPEFFECT_NONE;
}

static HRESULT STDMETHODCALLTYPE t_qi(IDropTarget *self, REFIID riid, void **out) {
    if (IsEqualIID(riid, &IID_IUnknown) || IsEqualIID(riid, &IID_IDropTarget)) {
        *out = self;
        IDropTarget_AddRef(self);
        return S_OK;
    }
    *out = NULL;
    return E_NOINTERFACE;
}
static ULONG STDMETHODCALLTYPE t_addref(IDropTarget *self) { return InterlockedIncrement(&((Target *)self)->refs); }
static ULONG STDMETHODCALLTYPE t_release(IDropTarget *self) { return InterlockedDecrement(&((Target *)self)->refs); }

static HRESULT STDMETHODCALLTYPE t_enter(IDropTarget *self, IDataObject *data, DWORD keys, POINTL pt, DWORD *effect) {
    Target *t = (Target *)self;
    FORMATETC ff = fmt_of(CF_HDROP), ft = fmt_of(CF_UNICODETEXT);
    t->has_files = IDataObject_QueryGetData(data, &ff) == S_OK;
    t->has_text = IDataObject_QueryGetData(data, &ft) == S_OK;
    *effect = (t->has_files || t->has_text) ? choose(keys, *effect) : DROPEFFECT_NONE;
    t->last = *effect;
    return S_OK;
}
static HRESULT STDMETHODCALLTYPE t_over(IDropTarget *self, DWORD keys, POINTL pt, DWORD *effect) {
    Target *t = (Target *)self;
    *effect = (t->has_files || t->has_text) ? choose(keys, *effect) : DROPEFFECT_NONE;
    t->last = *effect;
    return S_OK;
}
static HRESULT STDMETHODCALLTYPE t_leave(IDropTarget *self) { return S_OK; }

static HRESULT STDMETHODCALLTYPE t_drop(IDropTarget *self, IDataObject *data, DWORD keys, POINTL pt, DWORD *effect) {
    Target *t = (Target *)self;
    DWORD chosen = t->last & *effect;
    if (chosen == DROPEFFECT_NONE) {
        *effect = DROPEFFECT_NONE;
        return S_OK;
    }
    const char *how = chosen == DROPEFFECT_MOVE ? "move" : "copy";
    STGMEDIUM m;
    char line[128];
    if (t->has_files) {
        FORMATETC ff = fmt_of(CF_HDROP);
        if (IDataObject_GetData(data, &ff, &m) == S_OK) {
            HDROP h = (HDROP)GlobalLock(m.hGlobal);
            UINT count = DragQueryFileW(h, 0xFFFFFFFF, NULL, 0);
            size_t cap = 4096, len = 0;
            char *all = malloc(cap);
            for (UINT i = 0; i < count; i++) {
                WCHAR path[1024];
                UINT pn = DragQueryFileW(h, i, path, 1024);
                int n;
                char *s = utf8_of(path, (int)pn, &n);
                while (len + n + 2 > cap) all = realloc(all, cap *= 2);
                memcpy(all + len, s, n);
                len += n;
                all[len++] = '\n';
                free(s);
            }
            GlobalUnlock(m.hGlobal);
            ReleaseStgMedium(&m);
            dump(all, len);
            snprintf(line, sizeof line, "drop files %s %zu", how, len);
            say(line);
            free(all);
            *effect = chosen;
            return S_OK;
        }
    }
    if (t->has_text) {
        FORMATETC ft = fmt_of(CF_UNICODETEXT);
        if (IDataObject_GetData(data, &ft, &m) == S_OK) {
            const WCHAR *w = GlobalLock(m.hGlobal);
            int n;
            char *s = utf8_of(w, (int)wcslen(w), &n);
            GlobalUnlock(m.hGlobal);
            ReleaseStgMedium(&m);
            dump(s, n);
            snprintf(line, sizeof line, "drop text %s %d", how, n);
            say(line);
            free(s);
            *effect = chosen;
            return S_OK;
        }
    }
    *effect = DROPEFFECT_NONE;
    return S_OK;
}

static IDropTargetVtbl target_vtbl = {t_qi, t_addref, t_release, t_enter, t_over, t_leave, t_drop};
static Target target = {{&target_vtbl}, 1, 0, 0};

/* --- The source: a data object of HGLOBALs, a drop source --- */

typedef struct {
    IDataObject iface;
    LONG refs;
    int count;
    FORMATETC formats[2];
    HGLOBAL blocks[2];
} Data;

static HGLOBAL global_of(const void *p, size_t n) {
    HGLOBAL g = GlobalAlloc(GMEM_MOVEABLE, n);
    memcpy(GlobalLock(g), p, n);
    GlobalUnlock(g);
    return g;
}

static HGLOBAL dup_global(HGLOBAL g) {
    size_t n = GlobalSize(g);
    HGLOBAL c = global_of(GlobalLock(g), n);
    GlobalUnlock(g);
    return c;
}

static int find(Data *d, const FORMATETC *f) {
    for (int i = 0; i < d->count; i++)
        if (d->formats[i].cfFormat == f->cfFormat && (f->tymed & TYMED_HGLOBAL)) return i;
    return -1;
}

static HRESULT STDMETHODCALLTYPE d_qi(IDataObject *self, REFIID riid, void **out) {
    if (IsEqualIID(riid, &IID_IUnknown) || IsEqualIID(riid, &IID_IDataObject)) {
        *out = self;
        IDataObject_AddRef(self);
        return S_OK;
    }
    *out = NULL;
    return E_NOINTERFACE;
}
static ULONG STDMETHODCALLTYPE d_addref(IDataObject *self) { return InterlockedIncrement(&((Data *)self)->refs); }
static ULONG STDMETHODCALLTYPE d_release(IDataObject *self) {
    Data *d = (Data *)self;
    LONG r = InterlockedDecrement(&d->refs);
    if (r == 0) {
        for (int i = 0; i < d->count; i++) GlobalFree(d->blocks[i]);
        free(d);
    }
    return r;
}
static HRESULT STDMETHODCALLTYPE d_get(IDataObject *self, FORMATETC *f, STGMEDIUM *m) {
    Data *d = (Data *)self;
    int i = find(d, f);
    if (i < 0) return DV_E_FORMATETC;
    m->tymed = TYMED_HGLOBAL;
    m->hGlobal = dup_global(d->blocks[i]);
    m->pUnkForRelease = NULL;
    return S_OK;
}
static HRESULT STDMETHODCALLTYPE d_get_here(IDataObject *self, FORMATETC *f, STGMEDIUM *m) { return E_NOTIMPL; }
static HRESULT STDMETHODCALLTYPE d_query(IDataObject *self, FORMATETC *f) {
    return find((Data *)self, f) >= 0 ? S_OK : DV_E_FORMATETC;
}
static HRESULT STDMETHODCALLTYPE d_canonical(IDataObject *self, FORMATETC *in, FORMATETC *out) {
    out->ptd = NULL;
    return E_NOTIMPL;
}
static HRESULT STDMETHODCALLTYPE d_set(IDataObject *self, FORMATETC *f, STGMEDIUM *m, BOOL release) { return E_NOTIMPL; }
static HRESULT STDMETHODCALLTYPE d_enum(IDataObject *self, DWORD dir, IEnumFORMATETC **out) {
    Data *d = (Data *)self;
    if (dir != DATADIR_GET) return E_NOTIMPL;
    return SHCreateStdEnumFmtEtc(d->count, d->formats, out);
}
static HRESULT STDMETHODCALLTYPE d_advise(IDataObject *self, FORMATETC *f, DWORD a, IAdviseSink *s, DWORD *c) { return OLE_E_ADVISENOTSUPPORTED; }
static HRESULT STDMETHODCALLTYPE d_unadvise(IDataObject *self, DWORD c) { return OLE_E_ADVISENOTSUPPORTED; }
static HRESULT STDMETHODCALLTYPE d_enum_advise(IDataObject *self, IEnumSTATDATA **out) { return OLE_E_ADVISENOTSUPPORTED; }

static IDataObjectVtbl data_vtbl = {d_qi, d_addref, d_release, d_get, d_get_here, d_query,
                                    d_canonical, d_set, d_enum, d_advise, d_unadvise, d_enum_advise};

static void add(Data *d, CLIPFORMAT cf, HGLOBAL g) {
    d->formats[d->count] = fmt_of(cf);
    d->blocks[d->count] = g;
    d->count++;
}

static HGLOBAL wide_text(const char *utf8, size_t n) {
    int wn = MultiByteToWideChar(CP_UTF8, 0, utf8, (int)n, NULL, 0);
    WCHAR *w = calloc(wn + 1, sizeof(WCHAR));
    MultiByteToWideChar(CP_UTF8, 0, utf8, (int)n, w, wn);
    HGLOBAL g = global_of(w, (wn + 1) * sizeof(WCHAR));
    free(w);
    return g;
}

static Data *data_of(int band) {
    Data *d = calloc(1, sizeof(Data));
    d->iface.lpVtbl = &data_vtbl;
    d->refs = 1;
    if (band == 0) {
        static const WCHAR paths[] = L"C:\\peer one.txt\0C:\\peer-two\0";
        size_t n = sizeof(DROPFILES) + sizeof paths;
        char *b = calloc(1, n);
        DROPFILES *df = (DROPFILES *)b;
        df->pFiles = sizeof(DROPFILES);
        df->fWide = TRUE;
        memcpy(b + sizeof(DROPFILES), paths, sizeof paths);
        add(d, CF_HDROP, global_of(b, n));
        free(b);
    } else if (band == 1) {
        const char *s = "peer text: ação ✓";
        add(d, CF_UNICODETEXT, wide_text(s, strlen(s)));
    } else {
        /* Whole lines until 3 MB or more: what the test makes too. */
        const char *line = "ole ação — 3 MB through a global ✓ ";
        size_t ln = strlen(line), n = 0, cap = 3145728 + ln;
        char *s = malloc(cap);
        while (n < 3145728) {
            memcpy(s + n, line, ln);
            n += ln;
        }
        add(d, CF_UNICODETEXT, wide_text(s, n));
        free(s);
    }
    return d;
}

typedef struct {
    IDropSource iface;
    LONG refs;
} Source;

static HRESULT STDMETHODCALLTYPE s_qi(IDropSource *self, REFIID riid, void **out) {
    if (IsEqualIID(riid, &IID_IUnknown) || IsEqualIID(riid, &IID_IDropSource)) {
        *out = self;
        IDropSource_AddRef(self);
        return S_OK;
    }
    *out = NULL;
    return E_NOINTERFACE;
}
static ULONG STDMETHODCALLTYPE s_addref(IDropSource *self) { return InterlockedIncrement(&((Source *)self)->refs); }
static ULONG STDMETHODCALLTYPE s_release(IDropSource *self) { return InterlockedDecrement(&((Source *)self)->refs); }
static HRESULT STDMETHODCALLTYPE s_continue(IDropSource *self, BOOL escape, DWORD keys) {
    if (escape) return DRAGDROP_S_CANCEL;
    if (!(keys & MK_LBUTTON)) return DRAGDROP_S_DROP;
    return S_OK;
}
static HRESULT STDMETHODCALLTYPE s_feedback(IDropSource *self, DWORD effect) { return DRAGDROP_S_USEDEFAULTCURSORS; }

static IDropSourceVtbl source_vtbl = {s_qi, s_addref, s_release, s_continue, s_feedback};
static Source source = {{&source_vtbl}, 1};

/* --- The windows --- */

static HWND target_hwnd, source_hwnd;

static LRESULT CALLBACK proc(HWND h, UINT msg, WPARAM wp, LPARAM lp) {
    if (msg == WM_LBUTTONDOWN && h == source_hwnd) {
        int band = (short)HIWORD(lp) / 100;
        if (band > 2) band = 2;
        SetForegroundWindow(h);
        SetFocus(h);
        Data *d = data_of(band);
        DWORD effect = DROPEFFECT_NONE;
        HRESULT r = DoDragDrop(&d->iface, &source.iface, DROPEFFECT_COPY | DROPEFFECT_MOVE, &effect);
        IDataObject_Release(&d->iface);
        if (r == DRAGDROP_S_DROP && effect == DROPEFFECT_MOVE) say("end move");
        else if (r == DRAGDROP_S_DROP && effect == DROPEFFECT_COPY) say("end copy");
        else say("end none");
        return 0;
    }
    if (msg == WM_PAINT) {
        PAINTSTRUCT ps;
        HDC dc = BeginPaint(h, &ps);
        FillRect(dc, &ps.rcPaint, (HBRUSH)GetStockObject(h == source_hwnd ? GRAY_BRUSH : DKGRAY_BRUSH));
        EndPaint(h, &ps);
        return 0;
    }
    if (msg == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(h, msg, wp, lp);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: ole_dnd_peer.exe <log>\n");
        return 2;
    }
    log_path = argv[1];
    logf = fopen(log_path, "w");
    if (!logf) return 1;
    if (OleInitialize(NULL) != S_OK) return 1;
    WNDCLASSW wc = {0};
    wc.lpfnWndProc = proc;
    wc.hInstance = GetModuleHandleW(NULL);
    wc.hCursor = LoadCursorW(NULL, (LPCWSTR)IDC_ARROW);
    wc.lpszClassName = L"caustic-ole-peer";
    RegisterClassW(&wc);
    target_hwnd = CreateWindowExW(WS_EX_TOPMOST, wc.lpszClassName, L"dnd-peer target", WS_POPUP | WS_VISIBLE,
                                  600, 100, 300, 300, NULL, NULL, wc.hInstance, NULL);
    source_hwnd = CreateWindowExW(WS_EX_TOPMOST, wc.lpszClassName, L"dnd-peer source", WS_POPUP | WS_VISIBLE,
                                  600, 450, 300, 300, NULL, NULL, wc.hInstance, NULL);
    if (!target_hwnd || !source_hwnd || RegisterDragDrop(target_hwnd, &target.iface) != S_OK) return 1;
    UpdateWindow(target_hwnd);
    UpdateWindow(source_hwnd);
    say("ready");
    MSG m;
    while (GetMessageW(&m, NULL, 0, 0) > 0) {
        TranslateMessage(&m);
        DispatchMessageW(&m);
    }
    RevokeDragDrop(target_hwnd);
    OleUninitialize();
    return 0;
}
