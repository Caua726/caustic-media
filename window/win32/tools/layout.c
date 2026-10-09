#include <windows.h>
#include <imm.h>
#include <commdlg.h>
#include <dwmapi.h>
#include <uxtheme.h>
#include <shellscalingapi.h>
#include <oleidl.h>
#include <shobjidl.h>
#include <shlobj.h>
#include <knownfolders.h>
#include <uiautomationcore.h>
#include <uiautomationcoreapi.h>
#include <uiautomationclient.h>
#include <stddef.h>
#include <stdio.h>
int main(void) {
    _Static_assert(sizeof(WNDCLASSEXW) == 80, "WNDCLASSEXW");
    printf("WNDCLASSEXW %zu\n", sizeof(WNDCLASSEXW));
    _Static_assert(sizeof(POINT) == 8, "POINT");
    printf("POINT %zu\n", sizeof(POINT));
    _Static_assert(sizeof(MSG) == 48, "MSG");
    printf("MSG %zu\n", sizeof(MSG));
    _Static_assert(sizeof(RECT) == 16, "RECT");
    printf("RECT %zu\n", sizeof(RECT));
    _Static_assert(sizeof(PAINTSTRUCT) == 72, "PAINTSTRUCT");
    printf("PAINTSTRUCT %zu\n", sizeof(PAINTSTRUCT));
    _Static_assert(sizeof(MINMAXINFO) == 40, "MINMAXINFO");
    printf("MINMAXINFO %zu\n", sizeof(MINMAXINFO));
    _Static_assert(sizeof(TRACKMOUSEEVENT) == 24, "TRACKMOUSEEVENT");
    printf("TRACKMOUSEEVENT %zu\n", sizeof(TRACKMOUSEEVENT));
    _Static_assert(sizeof(CREATESTRUCTW) == 80, "CREATESTRUCTW");
    printf("CREATESTRUCTW %zu\n", sizeof(CREATESTRUCTW));
    _Static_assert(sizeof(BITMAPINFOHEADER) == 40, "BITMAPINFOHEADER");
    printf("BITMAPINFOHEADER %zu\n", sizeof(BITMAPINFOHEADER));
    _Static_assert(sizeof(RGBQUAD) == 4, "RGBQUAD");
    printf("RGBQUAD %zu\n", sizeof(RGBQUAD));
    _Static_assert(sizeof(BITMAPINFO) == 44, "BITMAPINFO");
    printf("BITMAPINFO %zu\n", sizeof(BITMAPINFO));
    _Static_assert(sizeof(BLENDFUNCTION) == 4, "BLENDFUNCTION");
    printf("BLENDFUNCTION %zu\n", sizeof(BLENDFUNCTION));
    _Static_assert(sizeof(WINDOWPLACEMENT) == 44, "WINDOWPLACEMENT");
    printf("WINDOWPLACEMENT %zu\n", sizeof(WINDOWPLACEMENT));
    _Static_assert(sizeof(LOGFONTW) == 92, "LOGFONTW");
    printf("LOGFONTW %zu\n", sizeof(LOGFONTW));
    _Static_assert(sizeof(NONCLIENTMETRICSW) == 504, "NONCLIENTMETRICSW");
    printf("NONCLIENTMETRICSW %zu\n", sizeof(NONCLIENTMETRICSW));
    _Static_assert(sizeof(HIGHCONTRASTW) == 16, "HIGHCONTRASTW");
    printf("HIGHCONTRASTW %zu\n", sizeof(HIGHCONTRASTW));
    _Static_assert(sizeof(OPENFILENAMEW) == 152, "OPENFILENAMEW");
    printf("OPENFILENAMEW %zu\n", sizeof(OPENFILENAMEW));
    _Static_assert(sizeof(FILETIME) == 8, "FILETIME");
    printf("FILETIME %zu\n", sizeof(FILETIME));
    _Static_assert(sizeof(WIN32_FIND_DATAW) == 592, "WIN32_FIND_DATAW");
    printf("WIN32_FIND_DATAW %zu\n", sizeof(WIN32_FIND_DATAW));
    _Static_assert(sizeof(WIN32_FILE_ATTRIBUTE_DATA) == 36, "WIN32_FILE_ATTRIBUTE_DATA");
    printf("WIN32_FILE_ATTRIBUTE_DATA %zu\n", sizeof(WIN32_FILE_ATTRIBUTE_DATA));
    _Static_assert(sizeof(BY_HANDLE_FILE_INFORMATION) == 52, "BY_HANDLE_FILE_INFORMATION");
    printf("BY_HANDLE_FILE_INFORMATION %zu\n", sizeof(BY_HANDLE_FILE_INFORMATION));
    _Static_assert(sizeof(COMPOSITIONFORM) == 28, "COMPOSITIONFORM");
    printf("COMPOSITIONFORM %zu\n", sizeof(COMPOSITIONFORM));
    _Static_assert(sizeof(CANDIDATEFORM) == 32, "CANDIDATEFORM");
    printf("CANDIDATEFORM %zu\n", sizeof(CANDIDATEFORM));
    _Static_assert(sizeof(GUID) == 16, "GUID");
    printf("GUID %zu\n", sizeof(GUID));
    _Static_assert(sizeof(NOTIFYICONDATAW) == 976, "NOTIFYICONDATAW");
    printf("NOTIFYICONDATAW %zu\n", sizeof(NOTIFYICONDATAW));
    _Static_assert(sizeof(FORMATETC) == 32, "FORMATETC");
    printf("FORMATETC %zu\n", sizeof(FORMATETC));
    _Static_assert(sizeof(STGMEDIUM) == 24, "STGMEDIUM");
    printf("STGMEDIUM %zu\n", sizeof(STGMEDIUM));
    _Static_assert(sizeof(DROPFILES) == 20, "DROPFILES");
    printf("DROPFILES %zu\n", sizeof(DROPFILES));
    _Static_assert(sizeof(VARIANT) == 24, "VARIANT");
    printf("VARIANT %zu\n", sizeof(VARIANT));
    _Static_assert(sizeof(SAFEARRAYBOUND) == 8, "SAFEARRAYBOUND");
    printf("SAFEARRAYBOUND %zu\n", sizeof(SAFEARRAYBOUND));
    _Static_assert(sizeof(SAFEARRAY) == 32, "SAFEARRAY");
    printf("SAFEARRAY %zu\n", sizeof(SAFEARRAY));
    _Static_assert(sizeof(COMDLG_FILTERSPEC) == 16, "COMDLG_FILTERSPEC");
    printf("COMDLG_FILTERSPEC %zu\n", sizeof(COMDLG_FILTERSPEC));
    _Static_assert(sizeof(DISPPARAMS) == 24, "DISPPARAMS");
    printf("DISPPARAMS %zu\n", sizeof(DISPPARAMS));
    _Static_assert(sizeof(EXCEPINFO) == 64, "EXCEPINFO");
    printf("EXCEPINFO %zu\n", sizeof(EXCEPINFO));
    _Static_assert(sizeof(MOUSEINPUT) == 32, "MOUSEINPUT");
    printf("MOUSEINPUT %zu\n", sizeof(MOUSEINPUT));
    _Static_assert(sizeof(KEYBDINPUT) == 24, "KEYBDINPUT");
    printf("KEYBDINPUT %zu\n", sizeof(KEYBDINPUT));
    _Static_assert(sizeof(INPUT) == 40, "INPUT");
    printf("INPUT %zu\n", sizeof(INPUT));
    _Static_assert(sizeof(MARGINS) == 16, "MARGINS");
    printf("MARGINS %zu\n", sizeof(MARGINS));
    _Static_assert(sizeof(MONITORINFO) == 40, "MONITORINFO");
    printf("MONITORINFO %zu\n", sizeof(MONITORINFO));
    _Static_assert(sizeof(MONITORINFOEXW) == 104, "MONITORINFOEXW");
    printf("MONITORINFOEXW %zu\n", sizeof(MONITORINFOEXW));
    _Static_assert(sizeof(DEVMODEW) == 220, "DEVMODEW");
    printf("DEVMODEW %zu\n", sizeof(DEVMODEW));
    _Static_assert(sizeof(ICONINFO) == 32, "ICONINFO");
    printf("ICONINFO %zu\n", sizeof(ICONINFO));
    _Static_assert(sizeof(COPYDATASTRUCT) == 24, "COPYDATASTRUCT");
    printf("COPYDATASTRUCT %zu\n", sizeof(COPYDATASTRUCT));
    _Static_assert(sizeof(FLASHWINFO) == 32, "FLASHWINFO");
    printf("FLASHWINFO %zu\n", sizeof(FLASHWINFO));
    _Static_assert(sizeof(POINTER_INFO) == 96, "POINTER_INFO");
    printf("POINTER_INFO %zu\n", sizeof(POINTER_INFO));
    _Static_assert(sizeof(POINTER_PEN_INFO) == 120, "POINTER_PEN_INFO");
    printf("POINTER_PEN_INFO %zu\n", sizeof(POINTER_PEN_INFO));
    _Static_assert(sizeof(POINTER_TOUCH_INFO) == 144, "POINTER_TOUCH_INFO");
    printf("POINTER_TOUCH_INFO %zu\n", sizeof(POINTER_TOUCH_INFO));
    _Static_assert(sizeof(GESTUREINFO) == 56, "GESTUREINFO");
    printf("GESTUREINFO %zu\n", sizeof(GESTUREINFO));
    _Static_assert(sizeof(GESTURECONFIG) == 12, "GESTURECONFIG");
    printf("GESTURECONFIG %zu\n", sizeof(GESTURECONFIG));
    _Static_assert(sizeof(struct UiaRect) == 32, "UiaRect");
    printf("UiaRect %zu\n", sizeof(struct UiaRect));
    _Static_assert(sizeof(struct UiaPoint) == 16, "UiaPoint");
    printf("UiaPoint %zu\n", sizeof(struct UiaPoint));
    _Static_assert(sizeof(IUnknownVtbl) == 24, "IUnknownVtbl");
    printf("IUnknownVtbl %zu\n", sizeof(IUnknownVtbl));
    _Static_assert(sizeof(IUnknown) == 8, "IUnknown");
    printf("IUnknown %zu\n", sizeof(IUnknown));
    _Static_assert(sizeof(IDropTargetVtbl) == 56, "IDropTargetVtbl");
    printf("IDropTargetVtbl %zu\n", sizeof(IDropTargetVtbl));
    _Static_assert(sizeof(IDropTarget) == 8, "IDropTarget");
    printf("IDropTarget %zu\n", sizeof(IDropTarget));
    _Static_assert(sizeof(IDataObjectVtbl) == 96, "IDataObjectVtbl");
    printf("IDataObjectVtbl %zu\n", sizeof(IDataObjectVtbl));
    _Static_assert(sizeof(IDataObject) == 8, "IDataObject");
    printf("IDataObject %zu\n", sizeof(IDataObject));
    _Static_assert(sizeof(IEnumFORMATETCVtbl) == 56, "IEnumFORMATETCVtbl");
    printf("IEnumFORMATETCVtbl %zu\n", sizeof(IEnumFORMATETCVtbl));
    _Static_assert(sizeof(IEnumFORMATETC) == 8, "IEnumFORMATETC");
    printf("IEnumFORMATETC %zu\n", sizeof(IEnumFORMATETC));
    _Static_assert(sizeof(IDropSourceVtbl) == 40, "IDropSourceVtbl");
    printf("IDropSourceVtbl %zu\n", sizeof(IDropSourceVtbl));
    _Static_assert(sizeof(IDropSource) == 8, "IDropSource");
    printf("IDropSource %zu\n", sizeof(IDropSource));
    _Static_assert(sizeof(IFileOpenDialogVtbl) == 232, "IFileOpenDialogVtbl");
    printf("IFileOpenDialogVtbl %zu\n", sizeof(IFileOpenDialogVtbl));
    _Static_assert(sizeof(IFileOpenDialog) == 8, "IFileOpenDialog");
    printf("IFileOpenDialog %zu\n", sizeof(IFileOpenDialog));
    _Static_assert(sizeof(IFileSaveDialogVtbl) == 256, "IFileSaveDialogVtbl");
    printf("IFileSaveDialogVtbl %zu\n", sizeof(IFileSaveDialogVtbl));
    _Static_assert(sizeof(IFileSaveDialog) == 8, "IFileSaveDialog");
    printf("IFileSaveDialog %zu\n", sizeof(IFileSaveDialog));
    _Static_assert(sizeof(IShellItemVtbl) == 64, "IShellItemVtbl");
    printf("IShellItemVtbl %zu\n", sizeof(IShellItemVtbl));
    _Static_assert(sizeof(IShellItem) == 8, "IShellItem");
    printf("IShellItem %zu\n", sizeof(IShellItem));
    _Static_assert(sizeof(IShellItemArrayVtbl) == 80, "IShellItemArrayVtbl");
    printf("IShellItemArrayVtbl %zu\n", sizeof(IShellItemArrayVtbl));
    _Static_assert(sizeof(IShellItemArray) == 8, "IShellItemArray");
    printf("IShellItemArray %zu\n", sizeof(IShellItemArray));
    _Static_assert(sizeof(IRawElementProviderSimpleVtbl) == 56, "IRawElementProviderSimpleVtbl");
    printf("IRawElementProviderSimpleVtbl %zu\n", sizeof(IRawElementProviderSimpleVtbl));
    _Static_assert(sizeof(IRawElementProviderSimple) == 8, "IRawElementProviderSimple");
    printf("IRawElementProviderSimple %zu\n", sizeof(IRawElementProviderSimple));
    _Static_assert(sizeof(IRawElementProviderFragmentVtbl) == 72, "IRawElementProviderFragmentVtbl");
    printf("IRawElementProviderFragmentVtbl %zu\n", sizeof(IRawElementProviderFragmentVtbl));
    _Static_assert(sizeof(IRawElementProviderFragment) == 8, "IRawElementProviderFragment");
    printf("IRawElementProviderFragment %zu\n", sizeof(IRawElementProviderFragment));
    _Static_assert(sizeof(IRawElementProviderFragmentRootVtbl) == 40, "IRawElementProviderFragmentRootVtbl");
    printf("IRawElementProviderFragmentRootVtbl %zu\n", sizeof(IRawElementProviderFragmentRootVtbl));
    _Static_assert(sizeof(IRawElementProviderFragmentRoot) == 8, "IRawElementProviderFragmentRoot");
    printf("IRawElementProviderFragmentRoot %zu\n", sizeof(IRawElementProviderFragmentRoot));
    _Static_assert(sizeof(IInvokeProviderVtbl) == 32, "IInvokeProviderVtbl");
    printf("IInvokeProviderVtbl %zu\n", sizeof(IInvokeProviderVtbl));
    _Static_assert(sizeof(IInvokeProvider) == 8, "IInvokeProvider");
    printf("IInvokeProvider %zu\n", sizeof(IInvokeProvider));
    _Static_assert(sizeof(IToggleProviderVtbl) == 40, "IToggleProviderVtbl");
    printf("IToggleProviderVtbl %zu\n", sizeof(IToggleProviderVtbl));
    _Static_assert(sizeof(IToggleProvider) == 8, "IToggleProvider");
    printf("IToggleProvider %zu\n", sizeof(IToggleProvider));
    _Static_assert(sizeof(IValueProviderVtbl) == 48, "IValueProviderVtbl");
    printf("IValueProviderVtbl %zu\n", sizeof(IValueProviderVtbl));
    _Static_assert(sizeof(IValueProvider) == 8, "IValueProvider");
    printf("IValueProvider %zu\n", sizeof(IValueProvider));
    _Static_assert(sizeof(IRangeValueProviderVtbl) == 80, "IRangeValueProviderVtbl");
    printf("IRangeValueProviderVtbl %zu\n", sizeof(IRangeValueProviderVtbl));
    _Static_assert(sizeof(IRangeValueProvider) == 8, "IRangeValueProvider");
    printf("IRangeValueProvider %zu\n", sizeof(IRangeValueProvider));
    _Static_assert(sizeof(ITextProviderVtbl) == 72, "ITextProviderVtbl");
    printf("ITextProviderVtbl %zu\n", sizeof(ITextProviderVtbl));
    _Static_assert(sizeof(ITextProvider) == 8, "ITextProvider");
    printf("ITextProvider %zu\n", sizeof(ITextProvider));
    _Static_assert(sizeof(ITextRangeProviderVtbl) == 168, "ITextRangeProviderVtbl");
    printf("ITextRangeProviderVtbl %zu\n", sizeof(ITextRangeProviderVtbl));
    _Static_assert(sizeof(ITextRangeProvider) == 8, "ITextRangeProvider");
    printf("ITextRangeProvider %zu\n", sizeof(ITextRangeProvider));
    _Static_assert(sizeof(ISelectionProviderVtbl) == 48, "ISelectionProviderVtbl");
    printf("ISelectionProviderVtbl %zu\n", sizeof(ISelectionProviderVtbl));
    _Static_assert(sizeof(ISelectionProvider) == 8, "ISelectionProvider");
    printf("ISelectionProvider %zu\n", sizeof(ISelectionProvider));
    _Static_assert(sizeof(ISelectionItemProviderVtbl) == 64, "ISelectionItemProviderVtbl");
    printf("ISelectionItemProviderVtbl %zu\n", sizeof(ISelectionItemProviderVtbl));
    _Static_assert(sizeof(ISelectionItemProvider) == 8, "ISelectionItemProvider");
    printf("ISelectionItemProvider %zu\n", sizeof(ISelectionItemProvider));
    _Static_assert(sizeof(IExpandCollapseProviderVtbl) == 48, "IExpandCollapseProviderVtbl");
    printf("IExpandCollapseProviderVtbl %zu\n", sizeof(IExpandCollapseProviderVtbl));
    _Static_assert(sizeof(IExpandCollapseProvider) == 8, "IExpandCollapseProvider");
    printf("IExpandCollapseProvider %zu\n", sizeof(IExpandCollapseProvider));
    _Static_assert(sizeof(IScrollProviderVtbl) == 88, "IScrollProviderVtbl");
    printf("IScrollProviderVtbl %zu\n", sizeof(IScrollProviderVtbl));
    _Static_assert(sizeof(IScrollProvider) == 8, "IScrollProvider");
    printf("IScrollProvider %zu\n", sizeof(IScrollProvider));
    _Static_assert(sizeof(IScrollItemProviderVtbl) == 32, "IScrollItemProviderVtbl");
    printf("IScrollItemProviderVtbl %zu\n", sizeof(IScrollItemProviderVtbl));
    _Static_assert(sizeof(IScrollItemProvider) == 8, "IScrollItemProvider");
    printf("IScrollItemProvider %zu\n", sizeof(IScrollItemProvider));
    _Static_assert(sizeof(IGridProviderVtbl) == 48, "IGridProviderVtbl");
    printf("IGridProviderVtbl %zu\n", sizeof(IGridProviderVtbl));
    _Static_assert(sizeof(IGridProvider) == 8, "IGridProvider");
    printf("IGridProvider %zu\n", sizeof(IGridProvider));
    _Static_assert(sizeof(IGridItemProviderVtbl) == 64, "IGridItemProviderVtbl");
    printf("IGridItemProviderVtbl %zu\n", sizeof(IGridItemProviderVtbl));
    _Static_assert(sizeof(IGridItemProvider) == 8, "IGridItemProvider");
    printf("IGridItemProvider %zu\n", sizeof(IGridItemProvider));
    _Static_assert(sizeof(ITableProviderVtbl) == 48, "ITableProviderVtbl");
    printf("ITableProviderVtbl %zu\n", sizeof(ITableProviderVtbl));
    _Static_assert(sizeof(ITableProvider) == 8, "ITableProvider");
    printf("ITableProvider %zu\n", sizeof(ITableProvider));
    _Static_assert(sizeof(ITableItemProviderVtbl) == 40, "ITableItemProviderVtbl");
    printf("ITableItemProviderVtbl %zu\n", sizeof(ITableItemProviderVtbl));
    _Static_assert(sizeof(ITableItemProvider) == 8, "ITableItemProvider");
    printf("ITableItemProvider %zu\n", sizeof(ITableItemProvider));
    _Static_assert(offsetof(WNDCLASSEXW, cbSize) == 0, "WNDCLASSEXW.cbSize");
    _Static_assert(offsetof(WNDCLASSEXW, style) == 4, "WNDCLASSEXW.style");
    _Static_assert(offsetof(WNDCLASSEXW, lpfnWndProc) == 8, "WNDCLASSEXW.lpfnWndProc");
    _Static_assert(offsetof(WNDCLASSEXW, cbClsExtra) == 16, "WNDCLASSEXW.cbClsExtra");
    _Static_assert(offsetof(WNDCLASSEXW, cbWndExtra) == 20, "WNDCLASSEXW.cbWndExtra");
    _Static_assert(offsetof(WNDCLASSEXW, hInstance) == 24, "WNDCLASSEXW.hInstance");
    _Static_assert(offsetof(WNDCLASSEXW, hIcon) == 32, "WNDCLASSEXW.hIcon");
    _Static_assert(offsetof(WNDCLASSEXW, hCursor) == 40, "WNDCLASSEXW.hCursor");
    _Static_assert(offsetof(WNDCLASSEXW, hbrBackground) == 48, "WNDCLASSEXW.hbrBackground");
    _Static_assert(offsetof(WNDCLASSEXW, lpszMenuName) == 56, "WNDCLASSEXW.lpszMenuName");
    _Static_assert(offsetof(WNDCLASSEXW, lpszClassName) == 64, "WNDCLASSEXW.lpszClassName");
    _Static_assert(offsetof(WNDCLASSEXW, hIconSm) == 72, "WNDCLASSEXW.hIconSm");
    _Static_assert(offsetof(MSG, hwnd) == 0, "MSG.hwnd");
    _Static_assert(offsetof(MSG, message) == 8, "MSG.message");
    _Static_assert(offsetof(MSG, wParam) == 16, "MSG.wParam");
    _Static_assert(offsetof(MSG, lParam) == 24, "MSG.lParam");
    _Static_assert(offsetof(MSG, time) == 32, "MSG.time");
    _Static_assert(offsetof(POINT, x) == 0, "POINT.x");
    _Static_assert(offsetof(POINT, y) == 4, "POINT.y");
    _Static_assert(offsetof(MSG, pt) == 36, "MSG.pt");
    _Static_assert(offsetof(RECT, left) == 0, "RECT.left");
    _Static_assert(offsetof(RECT, top) == 4, "RECT.top");
    _Static_assert(offsetof(RECT, right) == 8, "RECT.right");
    _Static_assert(offsetof(RECT, bottom) == 12, "RECT.bottom");
    _Static_assert(offsetof(PAINTSTRUCT, hdc) == 0, "PAINTSTRUCT.hdc");
    _Static_assert(offsetof(PAINTSTRUCT, fErase) == 8, "PAINTSTRUCT.fErase");
    _Static_assert(offsetof(PAINTSTRUCT, rcPaint) == 12, "PAINTSTRUCT.rcPaint");
    _Static_assert(offsetof(PAINTSTRUCT, fRestore) == 28, "PAINTSTRUCT.fRestore");
    _Static_assert(offsetof(PAINTSTRUCT, fIncUpdate) == 32, "PAINTSTRUCT.fIncUpdate");
    _Static_assert(offsetof(PAINTSTRUCT, rgbReserved) == 36, "PAINTSTRUCT.rgbReserved");
    _Static_assert(offsetof(MINMAXINFO, ptReserved) == 0, "MINMAXINFO.ptReserved");
    _Static_assert(offsetof(MINMAXINFO, ptMaxSize) == 8, "MINMAXINFO.ptMaxSize");
    _Static_assert(offsetof(MINMAXINFO, ptMaxPosition) == 16, "MINMAXINFO.ptMaxPosition");
    _Static_assert(offsetof(MINMAXINFO, ptMinTrackSize) == 24, "MINMAXINFO.ptMinTrackSize");
    _Static_assert(offsetof(MINMAXINFO, ptMaxTrackSize) == 32, "MINMAXINFO.ptMaxTrackSize");
    _Static_assert(offsetof(TRACKMOUSEEVENT, cbSize) == 0, "TRACKMOUSEEVENT.cbSize");
    _Static_assert(offsetof(TRACKMOUSEEVENT, dwFlags) == 4, "TRACKMOUSEEVENT.dwFlags");
    _Static_assert(offsetof(TRACKMOUSEEVENT, hwndTrack) == 8, "TRACKMOUSEEVENT.hwndTrack");
    _Static_assert(offsetof(TRACKMOUSEEVENT, dwHoverTime) == 16, "TRACKMOUSEEVENT.dwHoverTime");
    _Static_assert(offsetof(CREATESTRUCTW, lpCreateParams) == 0, "CREATESTRUCTW.lpCreateParams");
    _Static_assert(offsetof(CREATESTRUCTW, hInstance) == 8, "CREATESTRUCTW.hInstance");
    _Static_assert(offsetof(CREATESTRUCTW, hMenu) == 16, "CREATESTRUCTW.hMenu");
    _Static_assert(offsetof(CREATESTRUCTW, hwndParent) == 24, "CREATESTRUCTW.hwndParent");
    _Static_assert(offsetof(CREATESTRUCTW, cy) == 32, "CREATESTRUCTW.cy");
    _Static_assert(offsetof(CREATESTRUCTW, cx) == 36, "CREATESTRUCTW.cx");
    _Static_assert(offsetof(CREATESTRUCTW, y) == 40, "CREATESTRUCTW.y");
    _Static_assert(offsetof(CREATESTRUCTW, x) == 44, "CREATESTRUCTW.x");
    _Static_assert(offsetof(CREATESTRUCTW, style) == 48, "CREATESTRUCTW.style");
    _Static_assert(offsetof(CREATESTRUCTW, lpszName) == 56, "CREATESTRUCTW.lpszName");
    _Static_assert(offsetof(CREATESTRUCTW, lpszClass) == 64, "CREATESTRUCTW.lpszClass");
    _Static_assert(offsetof(CREATESTRUCTW, dwExStyle) == 72, "CREATESTRUCTW.dwExStyle");
    _Static_assert(offsetof(BITMAPINFOHEADER, biSize) == 0, "BITMAPINFOHEADER.biSize");
    _Static_assert(offsetof(BITMAPINFOHEADER, biWidth) == 4, "BITMAPINFOHEADER.biWidth");
    _Static_assert(offsetof(BITMAPINFOHEADER, biHeight) == 8, "BITMAPINFOHEADER.biHeight");
    _Static_assert(offsetof(BITMAPINFOHEADER, biPlanes) == 12, "BITMAPINFOHEADER.biPlanes");
    _Static_assert(offsetof(BITMAPINFOHEADER, biBitCount) == 14, "BITMAPINFOHEADER.biBitCount");
    _Static_assert(offsetof(BITMAPINFOHEADER, biCompression) == 16, "BITMAPINFOHEADER.biCompression");
    _Static_assert(offsetof(BITMAPINFOHEADER, biSizeImage) == 20, "BITMAPINFOHEADER.biSizeImage");
    _Static_assert(offsetof(BITMAPINFOHEADER, biXPelsPerMeter) == 24, "BITMAPINFOHEADER.biXPelsPerMeter");
    _Static_assert(offsetof(BITMAPINFOHEADER, biYPelsPerMeter) == 28, "BITMAPINFOHEADER.biYPelsPerMeter");
    _Static_assert(offsetof(BITMAPINFOHEADER, biClrUsed) == 32, "BITMAPINFOHEADER.biClrUsed");
    _Static_assert(offsetof(BITMAPINFOHEADER, biClrImportant) == 36, "BITMAPINFOHEADER.biClrImportant");
    _Static_assert(offsetof(BITMAPINFO, bmiHeader) == 0, "BITMAPINFO.bmiHeader");
    _Static_assert(offsetof(RGBQUAD, rgbBlue) == 0, "RGBQUAD.rgbBlue");
    _Static_assert(offsetof(RGBQUAD, rgbGreen) == 1, "RGBQUAD.rgbGreen");
    _Static_assert(offsetof(RGBQUAD, rgbRed) == 2, "RGBQUAD.rgbRed");
    _Static_assert(offsetof(RGBQUAD, rgbReserved) == 3, "RGBQUAD.rgbReserved");
    _Static_assert(offsetof(BITMAPINFO, bmiColors) == 40, "BITMAPINFO.bmiColors");
    _Static_assert(offsetof(BLENDFUNCTION, BlendOp) == 0, "BLENDFUNCTION.BlendOp");
    _Static_assert(offsetof(BLENDFUNCTION, BlendFlags) == 1, "BLENDFUNCTION.BlendFlags");
    _Static_assert(offsetof(BLENDFUNCTION, SourceConstantAlpha) == 2, "BLENDFUNCTION.SourceConstantAlpha");
    _Static_assert(offsetof(BLENDFUNCTION, AlphaFormat) == 3, "BLENDFUNCTION.AlphaFormat");
    _Static_assert(offsetof(WINDOWPLACEMENT, length) == 0, "WINDOWPLACEMENT.length");
    _Static_assert(offsetof(WINDOWPLACEMENT, flags) == 4, "WINDOWPLACEMENT.flags");
    _Static_assert(offsetof(WINDOWPLACEMENT, showCmd) == 8, "WINDOWPLACEMENT.showCmd");
    _Static_assert(offsetof(WINDOWPLACEMENT, ptMinPosition) == 12, "WINDOWPLACEMENT.ptMinPosition");
    _Static_assert(offsetof(WINDOWPLACEMENT, ptMaxPosition) == 20, "WINDOWPLACEMENT.ptMaxPosition");
    _Static_assert(offsetof(WINDOWPLACEMENT, rcNormalPosition) == 28, "WINDOWPLACEMENT.rcNormalPosition");
    _Static_assert(offsetof(NONCLIENTMETRICSW, cbSize) == 0, "NONCLIENTMETRICSW.cbSize");
    _Static_assert(offsetof(NONCLIENTMETRICSW, iBorderWidth) == 4, "NONCLIENTMETRICSW.iBorderWidth");
    _Static_assert(offsetof(NONCLIENTMETRICSW, iScrollWidth) == 8, "NONCLIENTMETRICSW.iScrollWidth");
    _Static_assert(offsetof(NONCLIENTMETRICSW, iScrollHeight) == 12, "NONCLIENTMETRICSW.iScrollHeight");
    _Static_assert(offsetof(NONCLIENTMETRICSW, iCaptionWidth) == 16, "NONCLIENTMETRICSW.iCaptionWidth");
    _Static_assert(offsetof(NONCLIENTMETRICSW, iCaptionHeight) == 20, "NONCLIENTMETRICSW.iCaptionHeight");
    _Static_assert(offsetof(LOGFONTW, lfHeight) == 0, "LOGFONTW.lfHeight");
    _Static_assert(offsetof(LOGFONTW, lfWidth) == 4, "LOGFONTW.lfWidth");
    _Static_assert(offsetof(LOGFONTW, lfEscapement) == 8, "LOGFONTW.lfEscapement");
    _Static_assert(offsetof(LOGFONTW, lfOrientation) == 12, "LOGFONTW.lfOrientation");
    _Static_assert(offsetof(LOGFONTW, lfWeight) == 16, "LOGFONTW.lfWeight");
    _Static_assert(offsetof(LOGFONTW, lfItalic) == 20, "LOGFONTW.lfItalic");
    _Static_assert(offsetof(LOGFONTW, lfUnderline) == 21, "LOGFONTW.lfUnderline");
    _Static_assert(offsetof(LOGFONTW, lfStrikeOut) == 22, "LOGFONTW.lfStrikeOut");
    _Static_assert(offsetof(LOGFONTW, lfCharSet) == 23, "LOGFONTW.lfCharSet");
    _Static_assert(offsetof(LOGFONTW, lfOutPrecision) == 24, "LOGFONTW.lfOutPrecision");
    _Static_assert(offsetof(LOGFONTW, lfClipPrecision) == 25, "LOGFONTW.lfClipPrecision");
    _Static_assert(offsetof(LOGFONTW, lfQuality) == 26, "LOGFONTW.lfQuality");
    _Static_assert(offsetof(LOGFONTW, lfPitchAndFamily) == 27, "LOGFONTW.lfPitchAndFamily");
    _Static_assert(offsetof(LOGFONTW, lfFaceName) == 28, "LOGFONTW.lfFaceName");
    _Static_assert(offsetof(NONCLIENTMETRICSW, lfCaptionFont) == 24, "NONCLIENTMETRICSW.lfCaptionFont");
    _Static_assert(offsetof(NONCLIENTMETRICSW, iSmCaptionWidth) == 116, "NONCLIENTMETRICSW.iSmCaptionWidth");
    _Static_assert(offsetof(NONCLIENTMETRICSW, iSmCaptionHeight) == 120, "NONCLIENTMETRICSW.iSmCaptionHeight");
    _Static_assert(offsetof(NONCLIENTMETRICSW, lfSmCaptionFont) == 124, "NONCLIENTMETRICSW.lfSmCaptionFont");
    _Static_assert(offsetof(NONCLIENTMETRICSW, iMenuWidth) == 216, "NONCLIENTMETRICSW.iMenuWidth");
    _Static_assert(offsetof(NONCLIENTMETRICSW, iMenuHeight) == 220, "NONCLIENTMETRICSW.iMenuHeight");
    _Static_assert(offsetof(NONCLIENTMETRICSW, lfMenuFont) == 224, "NONCLIENTMETRICSW.lfMenuFont");
    _Static_assert(offsetof(NONCLIENTMETRICSW, lfStatusFont) == 316, "NONCLIENTMETRICSW.lfStatusFont");
    _Static_assert(offsetof(NONCLIENTMETRICSW, lfMessageFont) == 408, "NONCLIENTMETRICSW.lfMessageFont");
    _Static_assert(offsetof(NONCLIENTMETRICSW, iPaddedBorderWidth) == 500, "NONCLIENTMETRICSW.iPaddedBorderWidth");
    _Static_assert(offsetof(HIGHCONTRASTW, cbSize) == 0, "HIGHCONTRASTW.cbSize");
    _Static_assert(offsetof(HIGHCONTRASTW, dwFlags) == 4, "HIGHCONTRASTW.dwFlags");
    _Static_assert(offsetof(HIGHCONTRASTW, lpszDefaultScheme) == 8, "HIGHCONTRASTW.lpszDefaultScheme");
    _Static_assert(offsetof(OPENFILENAMEW, lStructSize) == 0, "OPENFILENAMEW.lStructSize");
    _Static_assert(offsetof(OPENFILENAMEW, hwndOwner) == 8, "OPENFILENAMEW.hwndOwner");
    _Static_assert(offsetof(OPENFILENAMEW, hInstance) == 16, "OPENFILENAMEW.hInstance");
    _Static_assert(offsetof(OPENFILENAMEW, lpstrFilter) == 24, "OPENFILENAMEW.lpstrFilter");
    _Static_assert(offsetof(OPENFILENAMEW, lpstrCustomFilter) == 32, "OPENFILENAMEW.lpstrCustomFilter");
    _Static_assert(offsetof(OPENFILENAMEW, nMaxCustFilter) == 40, "OPENFILENAMEW.nMaxCustFilter");
    _Static_assert(offsetof(OPENFILENAMEW, nFilterIndex) == 44, "OPENFILENAMEW.nFilterIndex");
    _Static_assert(offsetof(OPENFILENAMEW, lpstrFile) == 48, "OPENFILENAMEW.lpstrFile");
    _Static_assert(offsetof(OPENFILENAMEW, nMaxFile) == 56, "OPENFILENAMEW.nMaxFile");
    _Static_assert(offsetof(OPENFILENAMEW, lpstrFileTitle) == 64, "OPENFILENAMEW.lpstrFileTitle");
    _Static_assert(offsetof(OPENFILENAMEW, nMaxFileTitle) == 72, "OPENFILENAMEW.nMaxFileTitle");
    _Static_assert(offsetof(OPENFILENAMEW, lpstrInitialDir) == 80, "OPENFILENAMEW.lpstrInitialDir");
    _Static_assert(offsetof(OPENFILENAMEW, lpstrTitle) == 88, "OPENFILENAMEW.lpstrTitle");
    _Static_assert(offsetof(OPENFILENAMEW, Flags) == 96, "OPENFILENAMEW.Flags");
    _Static_assert(offsetof(OPENFILENAMEW, nFileOffset) == 100, "OPENFILENAMEW.nFileOffset");
    _Static_assert(offsetof(OPENFILENAMEW, nFileExtension) == 102, "OPENFILENAMEW.nFileExtension");
    _Static_assert(offsetof(OPENFILENAMEW, lpstrDefExt) == 104, "OPENFILENAMEW.lpstrDefExt");
    _Static_assert(offsetof(OPENFILENAMEW, lCustData) == 112, "OPENFILENAMEW.lCustData");
    _Static_assert(offsetof(OPENFILENAMEW, lpfnHook) == 120, "OPENFILENAMEW.lpfnHook");
    _Static_assert(offsetof(OPENFILENAMEW, lpTemplateName) == 128, "OPENFILENAMEW.lpTemplateName");
    _Static_assert(offsetof(OPENFILENAMEW, pvReserved) == 136, "OPENFILENAMEW.pvReserved");
    _Static_assert(offsetof(OPENFILENAMEW, dwReserved) == 144, "OPENFILENAMEW.dwReserved");
    _Static_assert(offsetof(OPENFILENAMEW, FlagsEx) == 148, "OPENFILENAMEW.FlagsEx");
    _Static_assert(offsetof(WIN32_FIND_DATAW, dwFileAttributes) == 0, "WIN32_FIND_DATAW.dwFileAttributes");
    _Static_assert(offsetof(FILETIME, dwLowDateTime) == 0, "FILETIME.dwLowDateTime");
    _Static_assert(offsetof(FILETIME, dwHighDateTime) == 4, "FILETIME.dwHighDateTime");
    _Static_assert(offsetof(WIN32_FIND_DATAW, ftCreationTime) == 4, "WIN32_FIND_DATAW.ftCreationTime");
    _Static_assert(offsetof(WIN32_FIND_DATAW, ftLastAccessTime) == 12, "WIN32_FIND_DATAW.ftLastAccessTime");
    _Static_assert(offsetof(WIN32_FIND_DATAW, ftLastWriteTime) == 20, "WIN32_FIND_DATAW.ftLastWriteTime");
    _Static_assert(offsetof(WIN32_FIND_DATAW, nFileSizeHigh) == 28, "WIN32_FIND_DATAW.nFileSizeHigh");
    _Static_assert(offsetof(WIN32_FIND_DATAW, nFileSizeLow) == 32, "WIN32_FIND_DATAW.nFileSizeLow");
    _Static_assert(offsetof(WIN32_FIND_DATAW, dwReserved0) == 36, "WIN32_FIND_DATAW.dwReserved0");
    _Static_assert(offsetof(WIN32_FIND_DATAW, dwReserved1) == 40, "WIN32_FIND_DATAW.dwReserved1");
    _Static_assert(offsetof(WIN32_FIND_DATAW, cFileName) == 44, "WIN32_FIND_DATAW.cFileName");
    _Static_assert(offsetof(WIN32_FIND_DATAW, cAlternateFileName) == 564, "WIN32_FIND_DATAW.cAlternateFileName");
    _Static_assert(offsetof(WIN32_FILE_ATTRIBUTE_DATA, dwFileAttributes) == 0, "WIN32_FILE_ATTRIBUTE_DATA.dwFileAttributes");
    _Static_assert(offsetof(WIN32_FILE_ATTRIBUTE_DATA, ftCreationTime) == 4, "WIN32_FILE_ATTRIBUTE_DATA.ftCreationTime");
    _Static_assert(offsetof(WIN32_FILE_ATTRIBUTE_DATA, ftLastAccessTime) == 12, "WIN32_FILE_ATTRIBUTE_DATA.ftLastAccessTime");
    _Static_assert(offsetof(WIN32_FILE_ATTRIBUTE_DATA, ftLastWriteTime) == 20, "WIN32_FILE_ATTRIBUTE_DATA.ftLastWriteTime");
    _Static_assert(offsetof(WIN32_FILE_ATTRIBUTE_DATA, nFileSizeHigh) == 28, "WIN32_FILE_ATTRIBUTE_DATA.nFileSizeHigh");
    _Static_assert(offsetof(WIN32_FILE_ATTRIBUTE_DATA, nFileSizeLow) == 32, "WIN32_FILE_ATTRIBUTE_DATA.nFileSizeLow");
    _Static_assert(offsetof(BY_HANDLE_FILE_INFORMATION, dwFileAttributes) == 0, "BY_HANDLE_FILE_INFORMATION.dwFileAttributes");
    _Static_assert(offsetof(BY_HANDLE_FILE_INFORMATION, ftCreationTime) == 4, "BY_HANDLE_FILE_INFORMATION.ftCreationTime");
    _Static_assert(offsetof(BY_HANDLE_FILE_INFORMATION, ftLastAccessTime) == 12, "BY_HANDLE_FILE_INFORMATION.ftLastAccessTime");
    _Static_assert(offsetof(BY_HANDLE_FILE_INFORMATION, ftLastWriteTime) == 20, "BY_HANDLE_FILE_INFORMATION.ftLastWriteTime");
    _Static_assert(offsetof(BY_HANDLE_FILE_INFORMATION, dwVolumeSerialNumber) == 28, "BY_HANDLE_FILE_INFORMATION.dwVolumeSerialNumber");
    _Static_assert(offsetof(BY_HANDLE_FILE_INFORMATION, nFileSizeHigh) == 32, "BY_HANDLE_FILE_INFORMATION.nFileSizeHigh");
    _Static_assert(offsetof(BY_HANDLE_FILE_INFORMATION, nFileSizeLow) == 36, "BY_HANDLE_FILE_INFORMATION.nFileSizeLow");
    _Static_assert(offsetof(BY_HANDLE_FILE_INFORMATION, nNumberOfLinks) == 40, "BY_HANDLE_FILE_INFORMATION.nNumberOfLinks");
    _Static_assert(offsetof(BY_HANDLE_FILE_INFORMATION, nFileIndexHigh) == 44, "BY_HANDLE_FILE_INFORMATION.nFileIndexHigh");
    _Static_assert(offsetof(BY_HANDLE_FILE_INFORMATION, nFileIndexLow) == 48, "BY_HANDLE_FILE_INFORMATION.nFileIndexLow");
    _Static_assert(offsetof(COMPOSITIONFORM, dwStyle) == 0, "COMPOSITIONFORM.dwStyle");
    _Static_assert(offsetof(COMPOSITIONFORM, ptCurrentPos) == 4, "COMPOSITIONFORM.ptCurrentPos");
    _Static_assert(offsetof(COMPOSITIONFORM, rcArea) == 12, "COMPOSITIONFORM.rcArea");
    _Static_assert(offsetof(CANDIDATEFORM, dwIndex) == 0, "CANDIDATEFORM.dwIndex");
    _Static_assert(offsetof(CANDIDATEFORM, dwStyle) == 4, "CANDIDATEFORM.dwStyle");
    _Static_assert(offsetof(CANDIDATEFORM, ptCurrentPos) == 8, "CANDIDATEFORM.ptCurrentPos");
    _Static_assert(offsetof(CANDIDATEFORM, rcArea) == 16, "CANDIDATEFORM.rcArea");
    _Static_assert(offsetof(NOTIFYICONDATAW, cbSize) == 0, "NOTIFYICONDATAW.cbSize");
    _Static_assert(offsetof(NOTIFYICONDATAW, hWnd) == 8, "NOTIFYICONDATAW.hWnd");
    _Static_assert(offsetof(NOTIFYICONDATAW, uID) == 16, "NOTIFYICONDATAW.uID");
    _Static_assert(offsetof(NOTIFYICONDATAW, uFlags) == 20, "NOTIFYICONDATAW.uFlags");
    _Static_assert(offsetof(NOTIFYICONDATAW, uCallbackMessage) == 24, "NOTIFYICONDATAW.uCallbackMessage");
    _Static_assert(offsetof(NOTIFYICONDATAW, hIcon) == 32, "NOTIFYICONDATAW.hIcon");
    _Static_assert(offsetof(NOTIFYICONDATAW, szTip) == 40, "NOTIFYICONDATAW.szTip");
    _Static_assert(offsetof(NOTIFYICONDATAW, dwState) == 296, "NOTIFYICONDATAW.dwState");
    _Static_assert(offsetof(NOTIFYICONDATAW, dwStateMask) == 300, "NOTIFYICONDATAW.dwStateMask");
    _Static_assert(offsetof(NOTIFYICONDATAW, szInfo) == 304, "NOTIFYICONDATAW.szInfo");
    _Static_assert(offsetof(NOTIFYICONDATAW, szInfoTitle) == 820, "NOTIFYICONDATAW.szInfoTitle");
    _Static_assert(offsetof(NOTIFYICONDATAW, dwInfoFlags) == 948, "NOTIFYICONDATAW.dwInfoFlags");
    _Static_assert(offsetof(GUID, Data1) == 0, "GUID.Data1");
    _Static_assert(offsetof(GUID, Data2) == 4, "GUID.Data2");
    _Static_assert(offsetof(GUID, Data3) == 6, "GUID.Data3");
    _Static_assert(offsetof(GUID, Data4) == 8, "GUID.Data4");
    _Static_assert(offsetof(NOTIFYICONDATAW, guidItem) == 952, "NOTIFYICONDATAW.guidItem");
    _Static_assert(offsetof(NOTIFYICONDATAW, hBalloonIcon) == 968, "NOTIFYICONDATAW.hBalloonIcon");
    _Static_assert(offsetof(FORMATETC, cfFormat) == 0, "FORMATETC.cfFormat");
    _Static_assert(offsetof(FORMATETC, ptd) == 8, "FORMATETC.ptd");
    _Static_assert(offsetof(FORMATETC, dwAspect) == 16, "FORMATETC.dwAspect");
    _Static_assert(offsetof(FORMATETC, lindex) == 20, "FORMATETC.lindex");
    _Static_assert(offsetof(FORMATETC, tymed) == 24, "FORMATETC.tymed");
    _Static_assert(offsetof(STGMEDIUM, tymed) == 0, "STGMEDIUM.tymed");
    _Static_assert(offsetof(STGMEDIUM, pUnkForRelease) == 16, "STGMEDIUM.pUnkForRelease");
    _Static_assert(offsetof(DROPFILES, pFiles) == 0, "DROPFILES.pFiles");
    _Static_assert(offsetof(DROPFILES, pt) == 4, "DROPFILES.pt");
    _Static_assert(offsetof(DROPFILES, fNC) == 12, "DROPFILES.fNC");
    _Static_assert(offsetof(DROPFILES, fWide) == 16, "DROPFILES.fWide");
    _Static_assert(offsetof(SAFEARRAY, cDims) == 0, "SAFEARRAY.cDims");
    _Static_assert(offsetof(SAFEARRAY, fFeatures) == 2, "SAFEARRAY.fFeatures");
    _Static_assert(offsetof(SAFEARRAY, cbElements) == 4, "SAFEARRAY.cbElements");
    _Static_assert(offsetof(SAFEARRAY, cLocks) == 8, "SAFEARRAY.cLocks");
    _Static_assert(offsetof(SAFEARRAY, pvData) == 16, "SAFEARRAY.pvData");
    _Static_assert(offsetof(SAFEARRAYBOUND, cElements) == 0, "SAFEARRAYBOUND.cElements");
    _Static_assert(offsetof(SAFEARRAYBOUND, lLbound) == 4, "SAFEARRAYBOUND.lLbound");
    _Static_assert(offsetof(SAFEARRAY, rgsabound) == 24, "SAFEARRAY.rgsabound");
    _Static_assert(offsetof(COMDLG_FILTERSPEC, pszName) == 0, "COMDLG_FILTERSPEC.pszName");
    _Static_assert(offsetof(COMDLG_FILTERSPEC, pszSpec) == 8, "COMDLG_FILTERSPEC.pszSpec");
    _Static_assert(offsetof(DISPPARAMS, rgvarg) == 0, "DISPPARAMS.rgvarg");
    _Static_assert(offsetof(DISPPARAMS, rgdispidNamedArgs) == 8, "DISPPARAMS.rgdispidNamedArgs");
    _Static_assert(offsetof(DISPPARAMS, cArgs) == 16, "DISPPARAMS.cArgs");
    _Static_assert(offsetof(DISPPARAMS, cNamedArgs) == 20, "DISPPARAMS.cNamedArgs");
    _Static_assert(offsetof(EXCEPINFO, wCode) == 0, "EXCEPINFO.wCode");
    _Static_assert(offsetof(EXCEPINFO, wReserved) == 2, "EXCEPINFO.wReserved");
    _Static_assert(offsetof(EXCEPINFO, bstrSource) == 8, "EXCEPINFO.bstrSource");
    _Static_assert(offsetof(EXCEPINFO, bstrDescription) == 16, "EXCEPINFO.bstrDescription");
    _Static_assert(offsetof(EXCEPINFO, bstrHelpFile) == 24, "EXCEPINFO.bstrHelpFile");
    _Static_assert(offsetof(EXCEPINFO, dwHelpContext) == 32, "EXCEPINFO.dwHelpContext");
    _Static_assert(offsetof(EXCEPINFO, pvReserved) == 40, "EXCEPINFO.pvReserved");
    _Static_assert(offsetof(EXCEPINFO, pfnDeferredFillIn) == 48, "EXCEPINFO.pfnDeferredFillIn");
    _Static_assert(offsetof(EXCEPINFO, scode) == 56, "EXCEPINFO.scode");
    _Static_assert(offsetof(MOUSEINPUT, dx) == 0, "MOUSEINPUT.dx");
    _Static_assert(offsetof(MOUSEINPUT, dy) == 4, "MOUSEINPUT.dy");
    _Static_assert(offsetof(MOUSEINPUT, mouseData) == 8, "MOUSEINPUT.mouseData");
    _Static_assert(offsetof(MOUSEINPUT, dwFlags) == 12, "MOUSEINPUT.dwFlags");
    _Static_assert(offsetof(MOUSEINPUT, time) == 16, "MOUSEINPUT.time");
    _Static_assert(offsetof(MOUSEINPUT, dwExtraInfo) == 24, "MOUSEINPUT.dwExtraInfo");
    _Static_assert(offsetof(KEYBDINPUT, wVk) == 0, "KEYBDINPUT.wVk");
    _Static_assert(offsetof(KEYBDINPUT, wScan) == 2, "KEYBDINPUT.wScan");
    _Static_assert(offsetof(KEYBDINPUT, dwFlags) == 4, "KEYBDINPUT.dwFlags");
    _Static_assert(offsetof(KEYBDINPUT, time) == 8, "KEYBDINPUT.time");
    _Static_assert(offsetof(KEYBDINPUT, dwExtraInfo) == 16, "KEYBDINPUT.dwExtraInfo");
    _Static_assert(offsetof(INPUT, type) == 0, "INPUT.type");
    _Static_assert(offsetof(MARGINS, cxLeftWidth) == 0, "MARGINS.cxLeftWidth");
    _Static_assert(offsetof(MARGINS, cxRightWidth) == 4, "MARGINS.cxRightWidth");
    _Static_assert(offsetof(MARGINS, cyTopHeight) == 8, "MARGINS.cyTopHeight");
    _Static_assert(offsetof(MARGINS, cyBottomHeight) == 12, "MARGINS.cyBottomHeight");
    _Static_assert(offsetof(MONITORINFO, cbSize) == 0, "MONITORINFO.cbSize");
    _Static_assert(offsetof(MONITORINFO, rcMonitor) == 4, "MONITORINFO.rcMonitor");
    _Static_assert(offsetof(MONITORINFO, rcWork) == 20, "MONITORINFO.rcWork");
    _Static_assert(offsetof(MONITORINFO, dwFlags) == 36, "MONITORINFO.dwFlags");
    _Static_assert(offsetof(MONITORINFOEXW, szDevice) == 40, "MONITORINFOEXW.szDevice");
    _Static_assert(offsetof(DEVMODEW, dmDeviceName) == 0, "DEVMODEW.dmDeviceName");
    _Static_assert(offsetof(DEVMODEW, dmSpecVersion) == 64, "DEVMODEW.dmSpecVersion");
    _Static_assert(offsetof(DEVMODEW, dmDriverVersion) == 66, "DEVMODEW.dmDriverVersion");
    _Static_assert(offsetof(DEVMODEW, dmSize) == 68, "DEVMODEW.dmSize");
    _Static_assert(offsetof(DEVMODEW, dmDriverExtra) == 70, "DEVMODEW.dmDriverExtra");
    _Static_assert(offsetof(DEVMODEW, dmFields) == 72, "DEVMODEW.dmFields");
    _Static_assert(offsetof(DEVMODEW, dmColor) == 92, "DEVMODEW.dmColor");
    _Static_assert(offsetof(DEVMODEW, dmDuplex) == 94, "DEVMODEW.dmDuplex");
    _Static_assert(offsetof(DEVMODEW, dmYResolution) == 96, "DEVMODEW.dmYResolution");
    _Static_assert(offsetof(DEVMODEW, dmTTOption) == 98, "DEVMODEW.dmTTOption");
    _Static_assert(offsetof(DEVMODEW, dmCollate) == 100, "DEVMODEW.dmCollate");
    _Static_assert(offsetof(DEVMODEW, dmFormName) == 102, "DEVMODEW.dmFormName");
    _Static_assert(offsetof(DEVMODEW, dmLogPixels) == 166, "DEVMODEW.dmLogPixels");
    _Static_assert(offsetof(DEVMODEW, dmBitsPerPel) == 168, "DEVMODEW.dmBitsPerPel");
    _Static_assert(offsetof(DEVMODEW, dmPelsWidth) == 172, "DEVMODEW.dmPelsWidth");
    _Static_assert(offsetof(DEVMODEW, dmPelsHeight) == 176, "DEVMODEW.dmPelsHeight");
    _Static_assert(offsetof(DEVMODEW, dmDisplayFrequency) == 184, "DEVMODEW.dmDisplayFrequency");
    _Static_assert(offsetof(DEVMODEW, dmICMMethod) == 188, "DEVMODEW.dmICMMethod");
    _Static_assert(offsetof(DEVMODEW, dmICMIntent) == 192, "DEVMODEW.dmICMIntent");
    _Static_assert(offsetof(DEVMODEW, dmMediaType) == 196, "DEVMODEW.dmMediaType");
    _Static_assert(offsetof(DEVMODEW, dmDitherType) == 200, "DEVMODEW.dmDitherType");
    _Static_assert(offsetof(DEVMODEW, dmReserved1) == 204, "DEVMODEW.dmReserved1");
    _Static_assert(offsetof(DEVMODEW, dmReserved2) == 208, "DEVMODEW.dmReserved2");
    _Static_assert(offsetof(DEVMODEW, dmPanningWidth) == 212, "DEVMODEW.dmPanningWidth");
    _Static_assert(offsetof(DEVMODEW, dmPanningHeight) == 216, "DEVMODEW.dmPanningHeight");
    _Static_assert(offsetof(ICONINFO, fIcon) == 0, "ICONINFO.fIcon");
    _Static_assert(offsetof(ICONINFO, xHotspot) == 4, "ICONINFO.xHotspot");
    _Static_assert(offsetof(ICONINFO, yHotspot) == 8, "ICONINFO.yHotspot");
    _Static_assert(offsetof(ICONINFO, hbmMask) == 16, "ICONINFO.hbmMask");
    _Static_assert(offsetof(ICONINFO, hbmColor) == 24, "ICONINFO.hbmColor");
    _Static_assert(offsetof(COPYDATASTRUCT, dwData) == 0, "COPYDATASTRUCT.dwData");
    _Static_assert(offsetof(COPYDATASTRUCT, cbData) == 8, "COPYDATASTRUCT.cbData");
    _Static_assert(offsetof(COPYDATASTRUCT, lpData) == 16, "COPYDATASTRUCT.lpData");
    _Static_assert(offsetof(FLASHWINFO, cbSize) == 0, "FLASHWINFO.cbSize");
    _Static_assert(offsetof(FLASHWINFO, hwnd) == 8, "FLASHWINFO.hwnd");
    _Static_assert(offsetof(FLASHWINFO, dwFlags) == 16, "FLASHWINFO.dwFlags");
    _Static_assert(offsetof(FLASHWINFO, uCount) == 20, "FLASHWINFO.uCount");
    _Static_assert(offsetof(FLASHWINFO, dwTimeout) == 24, "FLASHWINFO.dwTimeout");
    _Static_assert(offsetof(POINTER_INFO, pointerType) == 0, "POINTER_INFO.pointerType");
    _Static_assert(offsetof(POINTER_INFO, pointerId) == 4, "POINTER_INFO.pointerId");
    _Static_assert(offsetof(POINTER_INFO, frameId) == 8, "POINTER_INFO.frameId");
    _Static_assert(offsetof(POINTER_INFO, pointerFlags) == 12, "POINTER_INFO.pointerFlags");
    _Static_assert(offsetof(POINTER_INFO, sourceDevice) == 16, "POINTER_INFO.sourceDevice");
    _Static_assert(offsetof(POINTER_INFO, hwndTarget) == 24, "POINTER_INFO.hwndTarget");
    _Static_assert(offsetof(POINTER_INFO, ptPixelLocation) == 32, "POINTER_INFO.ptPixelLocation");
    _Static_assert(offsetof(POINTER_INFO, ptHimetricLocation) == 40, "POINTER_INFO.ptHimetricLocation");
    _Static_assert(offsetof(POINTER_INFO, ptPixelLocationRaw) == 48, "POINTER_INFO.ptPixelLocationRaw");
    _Static_assert(offsetof(POINTER_INFO, ptHimetricLocationRaw) == 56, "POINTER_INFO.ptHimetricLocationRaw");
    _Static_assert(offsetof(POINTER_INFO, dwTime) == 64, "POINTER_INFO.dwTime");
    _Static_assert(offsetof(POINTER_INFO, historyCount) == 68, "POINTER_INFO.historyCount");
    _Static_assert(offsetof(POINTER_INFO, InputData) == 72, "POINTER_INFO.InputData");
    _Static_assert(offsetof(POINTER_INFO, dwKeyStates) == 76, "POINTER_INFO.dwKeyStates");
    _Static_assert(offsetof(POINTER_INFO, PerformanceCount) == 80, "POINTER_INFO.PerformanceCount");
    _Static_assert(offsetof(POINTER_INFO, ButtonChangeType) == 88, "POINTER_INFO.ButtonChangeType");
    _Static_assert(offsetof(POINTER_PEN_INFO, pointerInfo) == 0, "POINTER_PEN_INFO.pointerInfo");
    _Static_assert(offsetof(POINTER_PEN_INFO, penFlags) == 96, "POINTER_PEN_INFO.penFlags");
    _Static_assert(offsetof(POINTER_PEN_INFO, penMask) == 100, "POINTER_PEN_INFO.penMask");
    _Static_assert(offsetof(POINTER_PEN_INFO, pressure) == 104, "POINTER_PEN_INFO.pressure");
    _Static_assert(offsetof(POINTER_PEN_INFO, rotation) == 108, "POINTER_PEN_INFO.rotation");
    _Static_assert(offsetof(POINTER_PEN_INFO, tiltX) == 112, "POINTER_PEN_INFO.tiltX");
    _Static_assert(offsetof(POINTER_PEN_INFO, tiltY) == 116, "POINTER_PEN_INFO.tiltY");
    _Static_assert(offsetof(POINTER_TOUCH_INFO, pointerInfo) == 0, "POINTER_TOUCH_INFO.pointerInfo");
    _Static_assert(offsetof(POINTER_TOUCH_INFO, touchFlags) == 96, "POINTER_TOUCH_INFO.touchFlags");
    _Static_assert(offsetof(POINTER_TOUCH_INFO, touchMask) == 100, "POINTER_TOUCH_INFO.touchMask");
    _Static_assert(offsetof(POINTER_TOUCH_INFO, rcContact) == 104, "POINTER_TOUCH_INFO.rcContact");
    _Static_assert(offsetof(POINTER_TOUCH_INFO, rcContactRaw) == 120, "POINTER_TOUCH_INFO.rcContactRaw");
    _Static_assert(offsetof(POINTER_TOUCH_INFO, orientation) == 136, "POINTER_TOUCH_INFO.orientation");
    _Static_assert(offsetof(POINTER_TOUCH_INFO, pressure) == 140, "POINTER_TOUCH_INFO.pressure");
    _Static_assert(offsetof(GESTUREINFO, cbSize) == 0, "GESTUREINFO.cbSize");
    _Static_assert(offsetof(GESTUREINFO, dwFlags) == 4, "GESTUREINFO.dwFlags");
    _Static_assert(offsetof(GESTUREINFO, dwID) == 8, "GESTUREINFO.dwID");
    _Static_assert(offsetof(GESTUREINFO, hwndTarget) == 16, "GESTUREINFO.hwndTarget");
    _Static_assert(offsetof(GESTUREINFO, ptsLocation) == 24, "GESTUREINFO.ptsLocation");
    _Static_assert(offsetof(GESTUREINFO, dwInstanceID) == 28, "GESTUREINFO.dwInstanceID");
    _Static_assert(offsetof(GESTUREINFO, dwSequenceID) == 32, "GESTUREINFO.dwSequenceID");
    _Static_assert(offsetof(GESTUREINFO, ullArguments) == 40, "GESTUREINFO.ullArguments");
    _Static_assert(offsetof(GESTUREINFO, cbExtraArgs) == 48, "GESTUREINFO.cbExtraArgs");
    _Static_assert(offsetof(GESTURECONFIG, dwID) == 0, "GESTURECONFIG.dwID");
    _Static_assert(offsetof(GESTURECONFIG, dwWant) == 4, "GESTURECONFIG.dwWant");
    _Static_assert(offsetof(GESTURECONFIG, dwBlock) == 8, "GESTURECONFIG.dwBlock");
    _Static_assert(offsetof(struct UiaRect, left) == 0, "UiaRect.left");
    _Static_assert(offsetof(struct UiaRect, top) == 8, "UiaRect.top");
    _Static_assert(offsetof(struct UiaRect, width) == 16, "UiaRect.width");
    _Static_assert(offsetof(struct UiaRect, height) == 24, "UiaRect.height");
    _Static_assert(offsetof(struct UiaPoint, x) == 0, "UiaPoint.x");
    _Static_assert(offsetof(struct UiaPoint, y) == 8, "UiaPoint.y");
    _Static_assert(offsetof(IUnknownVtbl, QueryInterface) == 0, "IUnknownVtbl.QueryInterface");
    _Static_assert(offsetof(IUnknownVtbl, AddRef) == 8, "IUnknownVtbl.AddRef");
    _Static_assert(offsetof(IUnknownVtbl, Release) == 16, "IUnknownVtbl.Release");
    _Static_assert(offsetof(IUnknown, lpVtbl) == 0, "IUnknown.lpVtbl");
    _Static_assert(offsetof(IDropTargetVtbl, QueryInterface) == 0, "IDropTargetVtbl.QueryInterface");
    _Static_assert(offsetof(IDropTargetVtbl, AddRef) == 8, "IDropTargetVtbl.AddRef");
    _Static_assert(offsetof(IDropTargetVtbl, Release) == 16, "IDropTargetVtbl.Release");
    _Static_assert(offsetof(IDropTargetVtbl, DragEnter) == 24, "IDropTargetVtbl.DragEnter");
    _Static_assert(offsetof(IDropTargetVtbl, DragOver) == 32, "IDropTargetVtbl.DragOver");
    _Static_assert(offsetof(IDropTargetVtbl, DragLeave) == 40, "IDropTargetVtbl.DragLeave");
    _Static_assert(offsetof(IDropTargetVtbl, Drop) == 48, "IDropTargetVtbl.Drop");
    _Static_assert(offsetof(IDropTarget, lpVtbl) == 0, "IDropTarget.lpVtbl");
    _Static_assert(offsetof(IDataObjectVtbl, QueryInterface) == 0, "IDataObjectVtbl.QueryInterface");
    _Static_assert(offsetof(IDataObjectVtbl, AddRef) == 8, "IDataObjectVtbl.AddRef");
    _Static_assert(offsetof(IDataObjectVtbl, Release) == 16, "IDataObjectVtbl.Release");
    _Static_assert(offsetof(IDataObjectVtbl, GetData) == 24, "IDataObjectVtbl.GetData");
    _Static_assert(offsetof(IDataObjectVtbl, GetDataHere) == 32, "IDataObjectVtbl.GetDataHere");
    _Static_assert(offsetof(IDataObjectVtbl, QueryGetData) == 40, "IDataObjectVtbl.QueryGetData");
    _Static_assert(offsetof(IDataObjectVtbl, GetCanonicalFormatEtc) == 48, "IDataObjectVtbl.GetCanonicalFormatEtc");
    _Static_assert(offsetof(IDataObjectVtbl, SetData) == 56, "IDataObjectVtbl.SetData");
    _Static_assert(offsetof(IDataObjectVtbl, EnumFormatEtc) == 64, "IDataObjectVtbl.EnumFormatEtc");
    _Static_assert(offsetof(IDataObjectVtbl, DAdvise) == 72, "IDataObjectVtbl.DAdvise");
    _Static_assert(offsetof(IDataObjectVtbl, DUnadvise) == 80, "IDataObjectVtbl.DUnadvise");
    _Static_assert(offsetof(IDataObjectVtbl, EnumDAdvise) == 88, "IDataObjectVtbl.EnumDAdvise");
    _Static_assert(offsetof(IDataObject, lpVtbl) == 0, "IDataObject.lpVtbl");
    _Static_assert(offsetof(IEnumFORMATETCVtbl, QueryInterface) == 0, "IEnumFORMATETCVtbl.QueryInterface");
    _Static_assert(offsetof(IEnumFORMATETCVtbl, AddRef) == 8, "IEnumFORMATETCVtbl.AddRef");
    _Static_assert(offsetof(IEnumFORMATETCVtbl, Release) == 16, "IEnumFORMATETCVtbl.Release");
    _Static_assert(offsetof(IEnumFORMATETCVtbl, Next) == 24, "IEnumFORMATETCVtbl.Next");
    _Static_assert(offsetof(IEnumFORMATETCVtbl, Skip) == 32, "IEnumFORMATETCVtbl.Skip");
    _Static_assert(offsetof(IEnumFORMATETCVtbl, Reset) == 40, "IEnumFORMATETCVtbl.Reset");
    _Static_assert(offsetof(IEnumFORMATETCVtbl, Clone) == 48, "IEnumFORMATETCVtbl.Clone");
    _Static_assert(offsetof(IEnumFORMATETC, lpVtbl) == 0, "IEnumFORMATETC.lpVtbl");
    _Static_assert(offsetof(IDropSourceVtbl, QueryInterface) == 0, "IDropSourceVtbl.QueryInterface");
    _Static_assert(offsetof(IDropSourceVtbl, AddRef) == 8, "IDropSourceVtbl.AddRef");
    _Static_assert(offsetof(IDropSourceVtbl, Release) == 16, "IDropSourceVtbl.Release");
    _Static_assert(offsetof(IDropSourceVtbl, QueryContinueDrag) == 24, "IDropSourceVtbl.QueryContinueDrag");
    _Static_assert(offsetof(IDropSourceVtbl, GiveFeedback) == 32, "IDropSourceVtbl.GiveFeedback");
    _Static_assert(offsetof(IDropSource, lpVtbl) == 0, "IDropSource.lpVtbl");
    _Static_assert(offsetof(IFileOpenDialogVtbl, QueryInterface) == 0, "IFileOpenDialogVtbl.QueryInterface");
    _Static_assert(offsetof(IFileOpenDialogVtbl, AddRef) == 8, "IFileOpenDialogVtbl.AddRef");
    _Static_assert(offsetof(IFileOpenDialogVtbl, Release) == 16, "IFileOpenDialogVtbl.Release");
    _Static_assert(offsetof(IFileOpenDialogVtbl, Show) == 24, "IFileOpenDialogVtbl.Show");
    _Static_assert(offsetof(IFileOpenDialogVtbl, SetFileTypes) == 32, "IFileOpenDialogVtbl.SetFileTypes");
    _Static_assert(offsetof(IFileOpenDialogVtbl, SetFileTypeIndex) == 40, "IFileOpenDialogVtbl.SetFileTypeIndex");
    _Static_assert(offsetof(IFileOpenDialogVtbl, GetFileTypeIndex) == 48, "IFileOpenDialogVtbl.GetFileTypeIndex");
    _Static_assert(offsetof(IFileOpenDialogVtbl, Advise) == 56, "IFileOpenDialogVtbl.Advise");
    _Static_assert(offsetof(IFileOpenDialogVtbl, Unadvise) == 64, "IFileOpenDialogVtbl.Unadvise");
    _Static_assert(offsetof(IFileOpenDialogVtbl, SetOptions) == 72, "IFileOpenDialogVtbl.SetOptions");
    _Static_assert(offsetof(IFileOpenDialogVtbl, GetOptions) == 80, "IFileOpenDialogVtbl.GetOptions");
    _Static_assert(offsetof(IFileOpenDialogVtbl, SetDefaultFolder) == 88, "IFileOpenDialogVtbl.SetDefaultFolder");
    _Static_assert(offsetof(IFileOpenDialogVtbl, SetFolder) == 96, "IFileOpenDialogVtbl.SetFolder");
    _Static_assert(offsetof(IFileOpenDialogVtbl, GetFolder) == 104, "IFileOpenDialogVtbl.GetFolder");
    _Static_assert(offsetof(IFileOpenDialogVtbl, GetCurrentSelection) == 112, "IFileOpenDialogVtbl.GetCurrentSelection");
    _Static_assert(offsetof(IFileOpenDialogVtbl, SetFileName) == 120, "IFileOpenDialogVtbl.SetFileName");
    _Static_assert(offsetof(IFileOpenDialogVtbl, GetFileName) == 128, "IFileOpenDialogVtbl.GetFileName");
    _Static_assert(offsetof(IFileOpenDialogVtbl, SetTitle) == 136, "IFileOpenDialogVtbl.SetTitle");
    _Static_assert(offsetof(IFileOpenDialogVtbl, SetOkButtonLabel) == 144, "IFileOpenDialogVtbl.SetOkButtonLabel");
    _Static_assert(offsetof(IFileOpenDialogVtbl, SetFileNameLabel) == 152, "IFileOpenDialogVtbl.SetFileNameLabel");
    _Static_assert(offsetof(IFileOpenDialogVtbl, GetResult) == 160, "IFileOpenDialogVtbl.GetResult");
    _Static_assert(offsetof(IFileOpenDialogVtbl, AddPlace) == 168, "IFileOpenDialogVtbl.AddPlace");
    _Static_assert(offsetof(IFileOpenDialogVtbl, SetDefaultExtension) == 176, "IFileOpenDialogVtbl.SetDefaultExtension");
    _Static_assert(offsetof(IFileOpenDialogVtbl, Close) == 184, "IFileOpenDialogVtbl.Close");
    _Static_assert(offsetof(IFileOpenDialogVtbl, SetClientGuid) == 192, "IFileOpenDialogVtbl.SetClientGuid");
    _Static_assert(offsetof(IFileOpenDialogVtbl, ClearClientData) == 200, "IFileOpenDialogVtbl.ClearClientData");
    _Static_assert(offsetof(IFileOpenDialogVtbl, SetFilter) == 208, "IFileOpenDialogVtbl.SetFilter");
    _Static_assert(offsetof(IFileOpenDialogVtbl, GetResults) == 216, "IFileOpenDialogVtbl.GetResults");
    _Static_assert(offsetof(IFileOpenDialogVtbl, GetSelectedItems) == 224, "IFileOpenDialogVtbl.GetSelectedItems");
    _Static_assert(offsetof(IFileOpenDialog, lpVtbl) == 0, "IFileOpenDialog.lpVtbl");
    _Static_assert(offsetof(IFileSaveDialogVtbl, QueryInterface) == 0, "IFileSaveDialogVtbl.QueryInterface");
    _Static_assert(offsetof(IFileSaveDialogVtbl, AddRef) == 8, "IFileSaveDialogVtbl.AddRef");
    _Static_assert(offsetof(IFileSaveDialogVtbl, Release) == 16, "IFileSaveDialogVtbl.Release");
    _Static_assert(offsetof(IFileSaveDialogVtbl, Show) == 24, "IFileSaveDialogVtbl.Show");
    _Static_assert(offsetof(IFileSaveDialogVtbl, SetFileTypes) == 32, "IFileSaveDialogVtbl.SetFileTypes");
    _Static_assert(offsetof(IFileSaveDialogVtbl, SetFileTypeIndex) == 40, "IFileSaveDialogVtbl.SetFileTypeIndex");
    _Static_assert(offsetof(IFileSaveDialogVtbl, GetFileTypeIndex) == 48, "IFileSaveDialogVtbl.GetFileTypeIndex");
    _Static_assert(offsetof(IFileSaveDialogVtbl, Advise) == 56, "IFileSaveDialogVtbl.Advise");
    _Static_assert(offsetof(IFileSaveDialogVtbl, Unadvise) == 64, "IFileSaveDialogVtbl.Unadvise");
    _Static_assert(offsetof(IFileSaveDialogVtbl, SetOptions) == 72, "IFileSaveDialogVtbl.SetOptions");
    _Static_assert(offsetof(IFileSaveDialogVtbl, GetOptions) == 80, "IFileSaveDialogVtbl.GetOptions");
    _Static_assert(offsetof(IFileSaveDialogVtbl, SetDefaultFolder) == 88, "IFileSaveDialogVtbl.SetDefaultFolder");
    _Static_assert(offsetof(IFileSaveDialogVtbl, SetFolder) == 96, "IFileSaveDialogVtbl.SetFolder");
    _Static_assert(offsetof(IFileSaveDialogVtbl, GetFolder) == 104, "IFileSaveDialogVtbl.GetFolder");
    _Static_assert(offsetof(IFileSaveDialogVtbl, GetCurrentSelection) == 112, "IFileSaveDialogVtbl.GetCurrentSelection");
    _Static_assert(offsetof(IFileSaveDialogVtbl, SetFileName) == 120, "IFileSaveDialogVtbl.SetFileName");
    _Static_assert(offsetof(IFileSaveDialogVtbl, GetFileName) == 128, "IFileSaveDialogVtbl.GetFileName");
    _Static_assert(offsetof(IFileSaveDialogVtbl, SetTitle) == 136, "IFileSaveDialogVtbl.SetTitle");
    _Static_assert(offsetof(IFileSaveDialogVtbl, SetOkButtonLabel) == 144, "IFileSaveDialogVtbl.SetOkButtonLabel");
    _Static_assert(offsetof(IFileSaveDialogVtbl, SetFileNameLabel) == 152, "IFileSaveDialogVtbl.SetFileNameLabel");
    _Static_assert(offsetof(IFileSaveDialogVtbl, GetResult) == 160, "IFileSaveDialogVtbl.GetResult");
    _Static_assert(offsetof(IFileSaveDialogVtbl, AddPlace) == 168, "IFileSaveDialogVtbl.AddPlace");
    _Static_assert(offsetof(IFileSaveDialogVtbl, SetDefaultExtension) == 176, "IFileSaveDialogVtbl.SetDefaultExtension");
    _Static_assert(offsetof(IFileSaveDialogVtbl, Close) == 184, "IFileSaveDialogVtbl.Close");
    _Static_assert(offsetof(IFileSaveDialogVtbl, SetClientGuid) == 192, "IFileSaveDialogVtbl.SetClientGuid");
    _Static_assert(offsetof(IFileSaveDialogVtbl, ClearClientData) == 200, "IFileSaveDialogVtbl.ClearClientData");
    _Static_assert(offsetof(IFileSaveDialogVtbl, SetFilter) == 208, "IFileSaveDialogVtbl.SetFilter");
    _Static_assert(offsetof(IFileSaveDialogVtbl, SetSaveAsItem) == 216, "IFileSaveDialogVtbl.SetSaveAsItem");
    _Static_assert(offsetof(IFileSaveDialogVtbl, SetProperties) == 224, "IFileSaveDialogVtbl.SetProperties");
    _Static_assert(offsetof(IFileSaveDialogVtbl, SetCollectedProperties) == 232, "IFileSaveDialogVtbl.SetCollectedProperties");
    _Static_assert(offsetof(IFileSaveDialogVtbl, GetProperties) == 240, "IFileSaveDialogVtbl.GetProperties");
    _Static_assert(offsetof(IFileSaveDialogVtbl, ApplyProperties) == 248, "IFileSaveDialogVtbl.ApplyProperties");
    _Static_assert(offsetof(IFileSaveDialog, lpVtbl) == 0, "IFileSaveDialog.lpVtbl");
    _Static_assert(offsetof(IShellItemVtbl, QueryInterface) == 0, "IShellItemVtbl.QueryInterface");
    _Static_assert(offsetof(IShellItemVtbl, AddRef) == 8, "IShellItemVtbl.AddRef");
    _Static_assert(offsetof(IShellItemVtbl, Release) == 16, "IShellItemVtbl.Release");
    _Static_assert(offsetof(IShellItemVtbl, BindToHandler) == 24, "IShellItemVtbl.BindToHandler");
    _Static_assert(offsetof(IShellItemVtbl, GetParent) == 32, "IShellItemVtbl.GetParent");
    _Static_assert(offsetof(IShellItemVtbl, GetDisplayName) == 40, "IShellItemVtbl.GetDisplayName");
    _Static_assert(offsetof(IShellItemVtbl, GetAttributes) == 48, "IShellItemVtbl.GetAttributes");
    _Static_assert(offsetof(IShellItemVtbl, Compare) == 56, "IShellItemVtbl.Compare");
    _Static_assert(offsetof(IShellItem, lpVtbl) == 0, "IShellItem.lpVtbl");
    _Static_assert(offsetof(IShellItemArrayVtbl, QueryInterface) == 0, "IShellItemArrayVtbl.QueryInterface");
    _Static_assert(offsetof(IShellItemArrayVtbl, AddRef) == 8, "IShellItemArrayVtbl.AddRef");
    _Static_assert(offsetof(IShellItemArrayVtbl, Release) == 16, "IShellItemArrayVtbl.Release");
    _Static_assert(offsetof(IShellItemArrayVtbl, BindToHandler) == 24, "IShellItemArrayVtbl.BindToHandler");
    _Static_assert(offsetof(IShellItemArrayVtbl, GetPropertyStore) == 32, "IShellItemArrayVtbl.GetPropertyStore");
    _Static_assert(offsetof(IShellItemArrayVtbl, GetPropertyDescriptionList) == 40, "IShellItemArrayVtbl.GetPropertyDescriptionList");
    _Static_assert(offsetof(IShellItemArrayVtbl, GetAttributes) == 48, "IShellItemArrayVtbl.GetAttributes");
    _Static_assert(offsetof(IShellItemArrayVtbl, GetCount) == 56, "IShellItemArrayVtbl.GetCount");
    _Static_assert(offsetof(IShellItemArrayVtbl, GetItemAt) == 64, "IShellItemArrayVtbl.GetItemAt");
    _Static_assert(offsetof(IShellItemArrayVtbl, EnumItems) == 72, "IShellItemArrayVtbl.EnumItems");
    _Static_assert(offsetof(IShellItemArray, lpVtbl) == 0, "IShellItemArray.lpVtbl");
    _Static_assert(offsetof(IRawElementProviderSimpleVtbl, QueryInterface) == 0, "IRawElementProviderSimpleVtbl.QueryInterface");
    _Static_assert(offsetof(IRawElementProviderSimpleVtbl, AddRef) == 8, "IRawElementProviderSimpleVtbl.AddRef");
    _Static_assert(offsetof(IRawElementProviderSimpleVtbl, Release) == 16, "IRawElementProviderSimpleVtbl.Release");
    _Static_assert(offsetof(IRawElementProviderSimpleVtbl, get_ProviderOptions) == 24, "IRawElementProviderSimpleVtbl.get_ProviderOptions");
    _Static_assert(offsetof(IRawElementProviderSimpleVtbl, GetPatternProvider) == 32, "IRawElementProviderSimpleVtbl.GetPatternProvider");
    _Static_assert(offsetof(IRawElementProviderSimpleVtbl, GetPropertyValue) == 40, "IRawElementProviderSimpleVtbl.GetPropertyValue");
    _Static_assert(offsetof(IRawElementProviderSimpleVtbl, get_HostRawElementProvider) == 48, "IRawElementProviderSimpleVtbl.get_HostRawElementProvider");
    _Static_assert(offsetof(IRawElementProviderSimple, lpVtbl) == 0, "IRawElementProviderSimple.lpVtbl");
    _Static_assert(offsetof(IRawElementProviderFragmentVtbl, QueryInterface) == 0, "IRawElementProviderFragmentVtbl.QueryInterface");
    _Static_assert(offsetof(IRawElementProviderFragmentVtbl, AddRef) == 8, "IRawElementProviderFragmentVtbl.AddRef");
    _Static_assert(offsetof(IRawElementProviderFragmentVtbl, Release) == 16, "IRawElementProviderFragmentVtbl.Release");
    _Static_assert(offsetof(IRawElementProviderFragmentVtbl, Navigate) == 24, "IRawElementProviderFragmentVtbl.Navigate");
    _Static_assert(offsetof(IRawElementProviderFragmentVtbl, GetRuntimeId) == 32, "IRawElementProviderFragmentVtbl.GetRuntimeId");
    _Static_assert(offsetof(IRawElementProviderFragmentVtbl, get_BoundingRectangle) == 40, "IRawElementProviderFragmentVtbl.get_BoundingRectangle");
    _Static_assert(offsetof(IRawElementProviderFragmentVtbl, GetEmbeddedFragmentRoots) == 48, "IRawElementProviderFragmentVtbl.GetEmbeddedFragmentRoots");
    _Static_assert(offsetof(IRawElementProviderFragmentVtbl, SetFocus) == 56, "IRawElementProviderFragmentVtbl.SetFocus");
    _Static_assert(offsetof(IRawElementProviderFragmentVtbl, get_FragmentRoot) == 64, "IRawElementProviderFragmentVtbl.get_FragmentRoot");
    _Static_assert(offsetof(IRawElementProviderFragment, lpVtbl) == 0, "IRawElementProviderFragment.lpVtbl");
    _Static_assert(offsetof(IRawElementProviderFragmentRootVtbl, QueryInterface) == 0, "IRawElementProviderFragmentRootVtbl.QueryInterface");
    _Static_assert(offsetof(IRawElementProviderFragmentRootVtbl, AddRef) == 8, "IRawElementProviderFragmentRootVtbl.AddRef");
    _Static_assert(offsetof(IRawElementProviderFragmentRootVtbl, Release) == 16, "IRawElementProviderFragmentRootVtbl.Release");
    _Static_assert(offsetof(IRawElementProviderFragmentRootVtbl, ElementProviderFromPoint) == 24, "IRawElementProviderFragmentRootVtbl.ElementProviderFromPoint");
    _Static_assert(offsetof(IRawElementProviderFragmentRootVtbl, GetFocus) == 32, "IRawElementProviderFragmentRootVtbl.GetFocus");
    _Static_assert(offsetof(IRawElementProviderFragmentRoot, lpVtbl) == 0, "IRawElementProviderFragmentRoot.lpVtbl");
    _Static_assert(offsetof(IInvokeProviderVtbl, QueryInterface) == 0, "IInvokeProviderVtbl.QueryInterface");
    _Static_assert(offsetof(IInvokeProviderVtbl, AddRef) == 8, "IInvokeProviderVtbl.AddRef");
    _Static_assert(offsetof(IInvokeProviderVtbl, Release) == 16, "IInvokeProviderVtbl.Release");
    _Static_assert(offsetof(IInvokeProviderVtbl, Invoke) == 24, "IInvokeProviderVtbl.Invoke");
    _Static_assert(offsetof(IInvokeProvider, lpVtbl) == 0, "IInvokeProvider.lpVtbl");
    _Static_assert(offsetof(IToggleProviderVtbl, QueryInterface) == 0, "IToggleProviderVtbl.QueryInterface");
    _Static_assert(offsetof(IToggleProviderVtbl, AddRef) == 8, "IToggleProviderVtbl.AddRef");
    _Static_assert(offsetof(IToggleProviderVtbl, Release) == 16, "IToggleProviderVtbl.Release");
    _Static_assert(offsetof(IToggleProviderVtbl, Toggle) == 24, "IToggleProviderVtbl.Toggle");
    _Static_assert(offsetof(IToggleProviderVtbl, get_ToggleState) == 32, "IToggleProviderVtbl.get_ToggleState");
    _Static_assert(offsetof(IToggleProvider, lpVtbl) == 0, "IToggleProvider.lpVtbl");
    _Static_assert(offsetof(IValueProviderVtbl, QueryInterface) == 0, "IValueProviderVtbl.QueryInterface");
    _Static_assert(offsetof(IValueProviderVtbl, AddRef) == 8, "IValueProviderVtbl.AddRef");
    _Static_assert(offsetof(IValueProviderVtbl, Release) == 16, "IValueProviderVtbl.Release");
    _Static_assert(offsetof(IValueProviderVtbl, SetValue) == 24, "IValueProviderVtbl.SetValue");
    _Static_assert(offsetof(IValueProviderVtbl, get_Value) == 32, "IValueProviderVtbl.get_Value");
    _Static_assert(offsetof(IValueProviderVtbl, get_IsReadOnly) == 40, "IValueProviderVtbl.get_IsReadOnly");
    _Static_assert(offsetof(IValueProvider, lpVtbl) == 0, "IValueProvider.lpVtbl");
    _Static_assert(offsetof(IRangeValueProviderVtbl, QueryInterface) == 0, "IRangeValueProviderVtbl.QueryInterface");
    _Static_assert(offsetof(IRangeValueProviderVtbl, AddRef) == 8, "IRangeValueProviderVtbl.AddRef");
    _Static_assert(offsetof(IRangeValueProviderVtbl, Release) == 16, "IRangeValueProviderVtbl.Release");
    _Static_assert(offsetof(IRangeValueProviderVtbl, SetValue) == 24, "IRangeValueProviderVtbl.SetValue");
    _Static_assert(offsetof(IRangeValueProviderVtbl, get_Value) == 32, "IRangeValueProviderVtbl.get_Value");
    _Static_assert(offsetof(IRangeValueProviderVtbl, get_IsReadOnly) == 40, "IRangeValueProviderVtbl.get_IsReadOnly");
    _Static_assert(offsetof(IRangeValueProviderVtbl, get_Maximum) == 48, "IRangeValueProviderVtbl.get_Maximum");
    _Static_assert(offsetof(IRangeValueProviderVtbl, get_Minimum) == 56, "IRangeValueProviderVtbl.get_Minimum");
    _Static_assert(offsetof(IRangeValueProviderVtbl, get_LargeChange) == 64, "IRangeValueProviderVtbl.get_LargeChange");
    _Static_assert(offsetof(IRangeValueProviderVtbl, get_SmallChange) == 72, "IRangeValueProviderVtbl.get_SmallChange");
    _Static_assert(offsetof(IRangeValueProvider, lpVtbl) == 0, "IRangeValueProvider.lpVtbl");
    _Static_assert(offsetof(ITextProviderVtbl, QueryInterface) == 0, "ITextProviderVtbl.QueryInterface");
    _Static_assert(offsetof(ITextProviderVtbl, AddRef) == 8, "ITextProviderVtbl.AddRef");
    _Static_assert(offsetof(ITextProviderVtbl, Release) == 16, "ITextProviderVtbl.Release");
    _Static_assert(offsetof(ITextProviderVtbl, GetSelection) == 24, "ITextProviderVtbl.GetSelection");
    _Static_assert(offsetof(ITextProviderVtbl, GetVisibleRanges) == 32, "ITextProviderVtbl.GetVisibleRanges");
    _Static_assert(offsetof(ITextProviderVtbl, RangeFromChild) == 40, "ITextProviderVtbl.RangeFromChild");
    _Static_assert(offsetof(ITextProviderVtbl, RangeFromPoint) == 48, "ITextProviderVtbl.RangeFromPoint");
    _Static_assert(offsetof(ITextProviderVtbl, get_DocumentRange) == 56, "ITextProviderVtbl.get_DocumentRange");
    _Static_assert(offsetof(ITextProviderVtbl, get_SupportedTextSelection) == 64, "ITextProviderVtbl.get_SupportedTextSelection");
    _Static_assert(offsetof(ITextProvider, lpVtbl) == 0, "ITextProvider.lpVtbl");
    _Static_assert(offsetof(ITextRangeProviderVtbl, QueryInterface) == 0, "ITextRangeProviderVtbl.QueryInterface");
    _Static_assert(offsetof(ITextRangeProviderVtbl, AddRef) == 8, "ITextRangeProviderVtbl.AddRef");
    _Static_assert(offsetof(ITextRangeProviderVtbl, Release) == 16, "ITextRangeProviderVtbl.Release");
    _Static_assert(offsetof(ITextRangeProviderVtbl, Clone) == 24, "ITextRangeProviderVtbl.Clone");
    _Static_assert(offsetof(ITextRangeProviderVtbl, Compare) == 32, "ITextRangeProviderVtbl.Compare");
    _Static_assert(offsetof(ITextRangeProviderVtbl, CompareEndpoints) == 40, "ITextRangeProviderVtbl.CompareEndpoints");
    _Static_assert(offsetof(ITextRangeProviderVtbl, ExpandToEnclosingUnit) == 48, "ITextRangeProviderVtbl.ExpandToEnclosingUnit");
    _Static_assert(offsetof(ITextRangeProviderVtbl, FindAttribute) == 56, "ITextRangeProviderVtbl.FindAttribute");
    _Static_assert(offsetof(ITextRangeProviderVtbl, FindTextW) == 64, "ITextRangeProviderVtbl.FindTextW");
    _Static_assert(offsetof(ITextRangeProviderVtbl, GetAttributeValue) == 72, "ITextRangeProviderVtbl.GetAttributeValue");
    _Static_assert(offsetof(ITextRangeProviderVtbl, GetBoundingRectangles) == 80, "ITextRangeProviderVtbl.GetBoundingRectangles");
    _Static_assert(offsetof(ITextRangeProviderVtbl, GetEnclosingElement) == 88, "ITextRangeProviderVtbl.GetEnclosingElement");
    _Static_assert(offsetof(ITextRangeProviderVtbl, GetText) == 96, "ITextRangeProviderVtbl.GetText");
    _Static_assert(offsetof(ITextRangeProviderVtbl, Move) == 104, "ITextRangeProviderVtbl.Move");
    _Static_assert(offsetof(ITextRangeProviderVtbl, MoveEndpointByUnit) == 112, "ITextRangeProviderVtbl.MoveEndpointByUnit");
    _Static_assert(offsetof(ITextRangeProviderVtbl, MoveEndpointByRange) == 120, "ITextRangeProviderVtbl.MoveEndpointByRange");
    _Static_assert(offsetof(ITextRangeProviderVtbl, Select) == 128, "ITextRangeProviderVtbl.Select");
    _Static_assert(offsetof(ITextRangeProviderVtbl, AddToSelection) == 136, "ITextRangeProviderVtbl.AddToSelection");
    _Static_assert(offsetof(ITextRangeProviderVtbl, RemoveFromSelection) == 144, "ITextRangeProviderVtbl.RemoveFromSelection");
    _Static_assert(offsetof(ITextRangeProviderVtbl, ScrollIntoView) == 152, "ITextRangeProviderVtbl.ScrollIntoView");
    _Static_assert(offsetof(ITextRangeProviderVtbl, GetChildren) == 160, "ITextRangeProviderVtbl.GetChildren");
    _Static_assert(offsetof(ITextRangeProvider, lpVtbl) == 0, "ITextRangeProvider.lpVtbl");
    _Static_assert(offsetof(ISelectionProviderVtbl, QueryInterface) == 0, "ISelectionProviderVtbl.QueryInterface");
    _Static_assert(offsetof(ISelectionProviderVtbl, AddRef) == 8, "ISelectionProviderVtbl.AddRef");
    _Static_assert(offsetof(ISelectionProviderVtbl, Release) == 16, "ISelectionProviderVtbl.Release");
    _Static_assert(offsetof(ISelectionProviderVtbl, GetSelection) == 24, "ISelectionProviderVtbl.GetSelection");
    _Static_assert(offsetof(ISelectionProviderVtbl, get_CanSelectMultiple) == 32, "ISelectionProviderVtbl.get_CanSelectMultiple");
    _Static_assert(offsetof(ISelectionProviderVtbl, get_IsSelectionRequired) == 40, "ISelectionProviderVtbl.get_IsSelectionRequired");
    _Static_assert(offsetof(ISelectionProvider, lpVtbl) == 0, "ISelectionProvider.lpVtbl");
    _Static_assert(offsetof(ISelectionItemProviderVtbl, QueryInterface) == 0, "ISelectionItemProviderVtbl.QueryInterface");
    _Static_assert(offsetof(ISelectionItemProviderVtbl, AddRef) == 8, "ISelectionItemProviderVtbl.AddRef");
    _Static_assert(offsetof(ISelectionItemProviderVtbl, Release) == 16, "ISelectionItemProviderVtbl.Release");
    _Static_assert(offsetof(ISelectionItemProviderVtbl, Select) == 24, "ISelectionItemProviderVtbl.Select");
    _Static_assert(offsetof(ISelectionItemProviderVtbl, AddToSelection) == 32, "ISelectionItemProviderVtbl.AddToSelection");
    _Static_assert(offsetof(ISelectionItemProviderVtbl, RemoveFromSelection) == 40, "ISelectionItemProviderVtbl.RemoveFromSelection");
    _Static_assert(offsetof(ISelectionItemProviderVtbl, get_IsSelected) == 48, "ISelectionItemProviderVtbl.get_IsSelected");
    _Static_assert(offsetof(ISelectionItemProviderVtbl, get_SelectionContainer) == 56, "ISelectionItemProviderVtbl.get_SelectionContainer");
    _Static_assert(offsetof(ISelectionItemProvider, lpVtbl) == 0, "ISelectionItemProvider.lpVtbl");
    _Static_assert(offsetof(IExpandCollapseProviderVtbl, QueryInterface) == 0, "IExpandCollapseProviderVtbl.QueryInterface");
    _Static_assert(offsetof(IExpandCollapseProviderVtbl, AddRef) == 8, "IExpandCollapseProviderVtbl.AddRef");
    _Static_assert(offsetof(IExpandCollapseProviderVtbl, Release) == 16, "IExpandCollapseProviderVtbl.Release");
    _Static_assert(offsetof(IExpandCollapseProviderVtbl, Expand) == 24, "IExpandCollapseProviderVtbl.Expand");
    _Static_assert(offsetof(IExpandCollapseProviderVtbl, Collapse) == 32, "IExpandCollapseProviderVtbl.Collapse");
    _Static_assert(offsetof(IExpandCollapseProviderVtbl, get_ExpandCollapseState) == 40, "IExpandCollapseProviderVtbl.get_ExpandCollapseState");
    _Static_assert(offsetof(IExpandCollapseProvider, lpVtbl) == 0, "IExpandCollapseProvider.lpVtbl");
    _Static_assert(offsetof(IScrollProviderVtbl, QueryInterface) == 0, "IScrollProviderVtbl.QueryInterface");
    _Static_assert(offsetof(IScrollProviderVtbl, AddRef) == 8, "IScrollProviderVtbl.AddRef");
    _Static_assert(offsetof(IScrollProviderVtbl, Release) == 16, "IScrollProviderVtbl.Release");
    _Static_assert(offsetof(IScrollProviderVtbl, Scroll) == 24, "IScrollProviderVtbl.Scroll");
    _Static_assert(offsetof(IScrollProviderVtbl, SetScrollPercent) == 32, "IScrollProviderVtbl.SetScrollPercent");
    _Static_assert(offsetof(IScrollProviderVtbl, get_HorizontalScrollPercent) == 40, "IScrollProviderVtbl.get_HorizontalScrollPercent");
    _Static_assert(offsetof(IScrollProviderVtbl, get_VerticalScrollPercent) == 48, "IScrollProviderVtbl.get_VerticalScrollPercent");
    _Static_assert(offsetof(IScrollProviderVtbl, get_HorizontalViewSize) == 56, "IScrollProviderVtbl.get_HorizontalViewSize");
    _Static_assert(offsetof(IScrollProviderVtbl, get_VerticalViewSize) == 64, "IScrollProviderVtbl.get_VerticalViewSize");
    _Static_assert(offsetof(IScrollProviderVtbl, get_HorizontallyScrollable) == 72, "IScrollProviderVtbl.get_HorizontallyScrollable");
    _Static_assert(offsetof(IScrollProviderVtbl, get_VerticallyScrollable) == 80, "IScrollProviderVtbl.get_VerticallyScrollable");
    _Static_assert(offsetof(IScrollProvider, lpVtbl) == 0, "IScrollProvider.lpVtbl");
    _Static_assert(offsetof(IScrollItemProviderVtbl, QueryInterface) == 0, "IScrollItemProviderVtbl.QueryInterface");
    _Static_assert(offsetof(IScrollItemProviderVtbl, AddRef) == 8, "IScrollItemProviderVtbl.AddRef");
    _Static_assert(offsetof(IScrollItemProviderVtbl, Release) == 16, "IScrollItemProviderVtbl.Release");
    _Static_assert(offsetof(IScrollItemProviderVtbl, ScrollIntoView) == 24, "IScrollItemProviderVtbl.ScrollIntoView");
    _Static_assert(offsetof(IScrollItemProvider, lpVtbl) == 0, "IScrollItemProvider.lpVtbl");
    _Static_assert(offsetof(IGridProviderVtbl, QueryInterface) == 0, "IGridProviderVtbl.QueryInterface");
    _Static_assert(offsetof(IGridProviderVtbl, AddRef) == 8, "IGridProviderVtbl.AddRef");
    _Static_assert(offsetof(IGridProviderVtbl, Release) == 16, "IGridProviderVtbl.Release");
    _Static_assert(offsetof(IGridProviderVtbl, GetItem) == 24, "IGridProviderVtbl.GetItem");
    _Static_assert(offsetof(IGridProviderVtbl, get_RowCount) == 32, "IGridProviderVtbl.get_RowCount");
    _Static_assert(offsetof(IGridProviderVtbl, get_ColumnCount) == 40, "IGridProviderVtbl.get_ColumnCount");
    _Static_assert(offsetof(IGridProvider, lpVtbl) == 0, "IGridProvider.lpVtbl");
    _Static_assert(offsetof(IGridItemProviderVtbl, QueryInterface) == 0, "IGridItemProviderVtbl.QueryInterface");
    _Static_assert(offsetof(IGridItemProviderVtbl, AddRef) == 8, "IGridItemProviderVtbl.AddRef");
    _Static_assert(offsetof(IGridItemProviderVtbl, Release) == 16, "IGridItemProviderVtbl.Release");
    _Static_assert(offsetof(IGridItemProviderVtbl, get_Row) == 24, "IGridItemProviderVtbl.get_Row");
    _Static_assert(offsetof(IGridItemProviderVtbl, get_Column) == 32, "IGridItemProviderVtbl.get_Column");
    _Static_assert(offsetof(IGridItemProviderVtbl, get_RowSpan) == 40, "IGridItemProviderVtbl.get_RowSpan");
    _Static_assert(offsetof(IGridItemProviderVtbl, get_ColumnSpan) == 48, "IGridItemProviderVtbl.get_ColumnSpan");
    _Static_assert(offsetof(IGridItemProviderVtbl, get_ContainingGrid) == 56, "IGridItemProviderVtbl.get_ContainingGrid");
    _Static_assert(offsetof(IGridItemProvider, lpVtbl) == 0, "IGridItemProvider.lpVtbl");
    _Static_assert(offsetof(ITableProviderVtbl, QueryInterface) == 0, "ITableProviderVtbl.QueryInterface");
    _Static_assert(offsetof(ITableProviderVtbl, AddRef) == 8, "ITableProviderVtbl.AddRef");
    _Static_assert(offsetof(ITableProviderVtbl, Release) == 16, "ITableProviderVtbl.Release");
    _Static_assert(offsetof(ITableProviderVtbl, GetRowHeaders) == 24, "ITableProviderVtbl.GetRowHeaders");
    _Static_assert(offsetof(ITableProviderVtbl, GetColumnHeaders) == 32, "ITableProviderVtbl.GetColumnHeaders");
    _Static_assert(offsetof(ITableProviderVtbl, get_RowOrColumnMajor) == 40, "ITableProviderVtbl.get_RowOrColumnMajor");
    _Static_assert(offsetof(ITableProvider, lpVtbl) == 0, "ITableProvider.lpVtbl");
    _Static_assert(offsetof(ITableItemProviderVtbl, QueryInterface) == 0, "ITableItemProviderVtbl.QueryInterface");
    _Static_assert(offsetof(ITableItemProviderVtbl, AddRef) == 8, "ITableItemProviderVtbl.AddRef");
    _Static_assert(offsetof(ITableItemProviderVtbl, Release) == 16, "ITableItemProviderVtbl.Release");
    _Static_assert(offsetof(ITableItemProviderVtbl, GetRowHeaderItems) == 24, "ITableItemProviderVtbl.GetRowHeaderItems");
    _Static_assert(offsetof(ITableItemProviderVtbl, GetColumnHeaderItems) == 32, "ITableItemProviderVtbl.GetColumnHeaderItems");
    _Static_assert(offsetof(ITableItemProvider, lpVtbl) == 0, "ITableItemProvider.lpVtbl");
    int failures = 0;
    if ((long long)(CW_USEDEFAULT) != (-2147483648LL)) { printf("FAIL constant CW_USEDEFAULT\n"); failures++; }
    if ((long long)(INFINITE) != 4294967295LL) { printf("FAIL constant INFINITE\n"); failures++; }
    if ((long long)(SRCCOPY) != 13369376LL) { printf("FAIL constant SRCCOPY\n"); failures++; }
    if ((long long)(MAX_PATH) != 260LL) { printf("FAIL constant MAX_PATH\n"); failures++; }
    if ((long long)(INVALID_HANDLE_VALUE) != (-1LL)) { printf("FAIL constant INVALID_HANDLE_VALUE\n"); failures++; }
    if ((long long)(INVALID_FILE_ATTRIBUTES) != 4294967295LL) { printf("FAIL constant INVALID_FILE_ATTRIBUTES\n"); failures++; }
    if ((long long)(OPEN_EXISTING) != 3LL) { printf("FAIL constant OPEN_EXISTING\n"); failures++; }
    if ((long long)(OPEN_ALWAYS) != 4LL) { printf("FAIL constant OPEN_ALWAYS\n"); failures++; }
    if ((long long)(CREATE_ALWAYS) != 2LL) { printf("FAIL constant CREATE_ALWAYS\n"); failures++; }
    if ((long long)(CREATE_NEW) != 1LL) { printf("FAIL constant CREATE_NEW\n"); failures++; }
    if ((long long)(IMAGE_BITMAP) != 0LL) { printf("FAIL constant IMAGE_BITMAP\n"); failures++; }
    if ((long long)(IMAGE_ICON) != 1LL) { printf("FAIL constant IMAGE_ICON\n"); failures++; }
    if ((long long)(IMAGE_CURSOR) != 2LL) { printf("FAIL constant IMAGE_CURSOR\n"); failures++; }
    if ((long long)(WHEEL_DELTA) != 120LL) { printf("FAIL constant WHEEL_DELTA\n"); failures++; }
    if ((long long)(XBUTTON1) != 1LL) { printf("FAIL constant XBUTTON1\n"); failures++; }
    if ((long long)(XBUTTON2) != 2LL) { printf("FAIL constant XBUTTON2\n"); failures++; }
    if ((long long)(LOGPIXELSX) != 88LL) { printf("FAIL constant LOGPIXELSX\n"); failures++; }
    if ((long long)(LOGPIXELSY) != 90LL) { printf("FAIL constant LOGPIXELSY\n"); failures++; }
    if ((long long)(LOCALE_NAME_MAX_LENGTH) != 85LL) { printf("FAIL constant LOCALE_NAME_MAX_LENGTH\n"); failures++; }
    if ((long long)(S_OK) != 0LL) { printf("FAIL constant S_OK\n"); failures++; }
    if ((long long)(S_FALSE) != 1LL) { printf("FAIL constant S_FALSE\n"); failures++; }
    if ((long long)(E_FAIL) != (-2147467259LL)) { printf("FAIL constant E_FAIL\n"); failures++; }
    if ((long long)(E_ABORT) != (-2147467260LL)) { printf("FAIL constant E_ABORT\n"); failures++; }
    if ((long long)(E_POINTER) != (-2147467261LL)) { printf("FAIL constant E_POINTER\n"); failures++; }
    if ((long long)(E_NOINTERFACE) != (-2147467262LL)) { printf("FAIL constant E_NOINTERFACE\n"); failures++; }
    if ((long long)(E_NOTIMPL) != (-2147467263LL)) { printf("FAIL constant E_NOTIMPL\n"); failures++; }
    if ((long long)(E_INVALIDARG) != (-2147024809LL)) { printf("FAIL constant E_INVALIDARG\n"); failures++; }
    if ((long long)(E_OUTOFMEMORY) != (-2147024882LL)) { printf("FAIL constant E_OUTOFMEMORY\n"); failures++; }
    if ((long long)(E_UNEXPECTED) != (-2147418113LL)) { printf("FAIL constant E_UNEXPECTED\n"); failures++; }
    if ((long long)(E_ACCESSDENIED) != (-2147024891LL)) { printf("FAIL constant E_ACCESSDENIED\n"); failures++; }
    if ((long long)(RPC_E_CHANGED_MODE) != (-2147417850LL)) { printf("FAIL constant RPC_E_CHANGED_MODE\n"); failures++; }
    if ((long long)(OLE_E_ADVISENOTSUPPORTED) != (-2147221501LL)) { printf("FAIL constant OLE_E_ADVISENOTSUPPORTED\n"); failures++; }
    if ((long long)(ERROR_SUCCESS) != 0LL) { printf("FAIL constant ERROR_SUCCESS\n"); failures++; }
    if ((long long)(ERROR_FILE_NOT_FOUND) != 2LL) { printf("FAIL constant ERROR_FILE_NOT_FOUND\n"); failures++; }
    if ((long long)(ERROR_PATH_NOT_FOUND) != 3LL) { printf("FAIL constant ERROR_PATH_NOT_FOUND\n"); failures++; }
    if ((long long)(ERROR_NO_MORE_FILES) != 18LL) { printf("FAIL constant ERROR_NO_MORE_FILES\n"); failures++; }
    if ((long long)(ERROR_ALREADY_EXISTS) != 183LL) { printf("FAIL constant ERROR_ALREADY_EXISTS\n"); failures++; }
    if ((long long)(ERROR_INSUFFICIENT_BUFFER) != 122LL) { printf("FAIL constant ERROR_INSUFFICIENT_BUFFER\n"); failures++; }
    if ((long long)(ERROR_MORE_DATA) != 234LL) { printf("FAIL constant ERROR_MORE_DATA\n"); failures++; }
    if ((long long)(HKEY_CURRENT_USER) != (-2147483647LL)) { printf("FAIL constant HKEY_CURRENT_USER\n"); failures++; }
    if ((long long)(HKEY_LOCAL_MACHINE) != (-2147483646LL)) { printf("FAIL constant HKEY_LOCAL_MACHINE\n"); failures++; }
    if ((long long)(KEY_READ) != 131097LL) { printf("FAIL constant KEY_READ\n"); failures++; }
    if ((long long)(KEY_NOTIFY) != 16LL) { printf("FAIL constant KEY_NOTIFY\n"); failures++; }
    if ((long long)(REG_NOTIFY_CHANGE_LAST_SET) != 4LL) { printf("FAIL constant REG_NOTIFY_CHANGE_LAST_SET\n"); failures++; }
    if ((long long)(HWND_MESSAGE) != (-3LL)) { printf("FAIL constant HWND_MESSAGE\n"); failures++; }
    if ((long long)(HWND_TOP) != 0LL) { printf("FAIL constant HWND_TOP\n"); failures++; }
    if ((long long)(HWND_BOTTOM) != 1LL) { printf("FAIL constant HWND_BOTTOM\n"); failures++; }
    if ((long long)(HWND_TOPMOST) != (-1LL)) { printf("FAIL constant HWND_TOPMOST\n"); failures++; }
    if ((long long)(HWND_NOTOPMOST) != (-2LL)) { printf("FAIL constant HWND_NOTOPMOST\n"); failures++; }
    if ((long long)(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2) != (-4LL)) { printf("FAIL constant DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2\n"); failures++; }
    if ((long long)(IDC_ARROW) != 32512LL) { printf("FAIL constant IDC_ARROW\n"); failures++; }
    if ((long long)(IDC_IBEAM) != 32513LL) { printf("FAIL constant IDC_IBEAM\n"); failures++; }
    if ((long long)(IDC_WAIT) != 32514LL) { printf("FAIL constant IDC_WAIT\n"); failures++; }
    if ((long long)(IDC_CROSS) != 32515LL) { printf("FAIL constant IDC_CROSS\n"); failures++; }
    if ((long long)(IDC_UPARROW) != 32516LL) { printf("FAIL constant IDC_UPARROW\n"); failures++; }
    if ((long long)(IDC_SIZENWSE) != 32642LL) { printf("FAIL constant IDC_SIZENWSE\n"); failures++; }
    if ((long long)(IDC_SIZENESW) != 32643LL) { printf("FAIL constant IDC_SIZENESW\n"); failures++; }
    if ((long long)(IDC_SIZEWE) != 32644LL) { printf("FAIL constant IDC_SIZEWE\n"); failures++; }
    if ((long long)(IDC_SIZENS) != 32645LL) { printf("FAIL constant IDC_SIZENS\n"); failures++; }
    if ((long long)(IDC_SIZEALL) != 32646LL) { printf("FAIL constant IDC_SIZEALL\n"); failures++; }
    if ((long long)(IDC_NO) != 32648LL) { printf("FAIL constant IDC_NO\n"); failures++; }
    if ((long long)(IDC_HAND) != 32649LL) { printf("FAIL constant IDC_HAND\n"); failures++; }
    if ((long long)(IDC_APPSTARTING) != 32650LL) { printf("FAIL constant IDC_APPSTARTING\n"); failures++; }
    if ((long long)(IDC_HELP) != 32651LL) { printf("FAIL constant IDC_HELP\n"); failures++; }
    if ((long long)(UiaRootObjectId) != (-25LL)) { printf("FAIL constant UiaRootObjectId\n"); failures++; }
    if ((long long)(UiaAppendRuntimeId) != 3LL) { printf("FAIL constant UiaAppendRuntimeId\n"); failures++; }
    if ((long long)(ENUM_CURRENT_SETTINGS) != 4294967295LL) { printf("FAIL constant ENUM_CURRENT_SETTINGS\n"); failures++; }
    if ((long long)(MONITORINFOF_PRIMARY) != 1LL) { printf("FAIL constant MONITORINFOF_PRIMARY\n"); failures++; }
    if ((long long)(AC_LINE_BACKUP_POWER) != 2LL) { printf("FAIL constant AC_LINE_BACKUP_POWER\n"); failures++; }
    if ((long long)(AC_LINE_OFFLINE) != 0LL) { printf("FAIL constant AC_LINE_OFFLINE\n"); failures++; }
    if ((long long)(AC_LINE_ONLINE) != 1LL) { printf("FAIL constant AC_LINE_ONLINE\n"); failures++; }
    if ((long long)(AC_LINE_UNKNOWN) != 255LL) { printf("FAIL constant AC_LINE_UNKNOWN\n"); failures++; }
    if ((long long)(AC_SRC_ALPHA) != 1LL) { printf("FAIL constant AC_SRC_ALPHA\n"); failures++; }
    if ((long long)(AC_SRC_OVER) != 0LL) { printf("FAIL constant AC_SRC_OVER\n"); failures++; }
    if ((long long)(ATTR_CONVERTED) != 2LL) { printf("FAIL constant ATTR_CONVERTED\n"); failures++; }
    if ((long long)(ATTR_FIXEDCONVERTED) != 5LL) { printf("FAIL constant ATTR_FIXEDCONVERTED\n"); failures++; }
    if ((long long)(ATTR_INPUT) != 0LL) { printf("FAIL constant ATTR_INPUT\n"); failures++; }
    if ((long long)(ATTR_INPUT_ERROR) != 4LL) { printf("FAIL constant ATTR_INPUT_ERROR\n"); failures++; }
    if ((long long)(ATTR_TARGET_CONVERTED) != 1LL) { printf("FAIL constant ATTR_TARGET_CONVERTED\n"); failures++; }
    if ((long long)(ATTR_TARGET_NOTCONVERTED) != 3LL) { printf("FAIL constant ATTR_TARGET_NOTCONVERTED\n"); failures++; }
    if ((long long)(BI_BITFIELDS) != 3LL) { printf("FAIL constant BI_BITFIELDS\n"); failures++; }
    if ((long long)(BI_JPEG) != 4LL) { printf("FAIL constant BI_JPEG\n"); failures++; }
    if ((long long)(BI_PNG) != 5LL) { printf("FAIL constant BI_PNG\n"); failures++; }
    if ((long long)(BI_RGB) != 0LL) { printf("FAIL constant BI_RGB\n"); failures++; }
    if ((long long)(BI_RLE4) != 2LL) { printf("FAIL constant BI_RLE4\n"); failures++; }
    if ((long long)(BI_RLE8) != 1LL) { printf("FAIL constant BI_RLE8\n"); failures++; }
    if ((long long)(CFS_CANDIDATEPOS) != 64LL) { printf("FAIL constant CFS_CANDIDATEPOS\n"); failures++; }
    if ((long long)(CFS_DEFAULT) != 0LL) { printf("FAIL constant CFS_DEFAULT\n"); failures++; }
    if ((long long)(CFS_EXCLUDE) != 128LL) { printf("FAIL constant CFS_EXCLUDE\n"); failures++; }
    if ((long long)(CFS_FORCE_POSITION) != 32LL) { printf("FAIL constant CFS_FORCE_POSITION\n"); failures++; }
    if ((long long)(CFS_POINT) != 2LL) { printf("FAIL constant CFS_POINT\n"); failures++; }
    if ((long long)(CFS_RECT) != 1LL) { printf("FAIL constant CFS_RECT\n"); failures++; }
    if ((long long)(CF_ANSIONLY) != 1024LL) { printf("FAIL constant CF_ANSIONLY\n"); failures++; }
    if ((long long)(CF_APPLY) != 512LL) { printf("FAIL constant CF_APPLY\n"); failures++; }
    if ((long long)(CF_BITMAP) != 2LL) { printf("FAIL constant CF_BITMAP\n"); failures++; }
    if ((long long)(CF_BOTH) != 3LL) { printf("FAIL constant CF_BOTH\n"); failures++; }
    if ((long long)(CF_DIB) != 8LL) { printf("FAIL constant CF_DIB\n"); failures++; }
    if ((long long)(CF_DIBV5) != 17LL) { printf("FAIL constant CF_DIBV5\n"); failures++; }
    if ((long long)(CF_DIF) != 5LL) { printf("FAIL constant CF_DIF\n"); failures++; }
    if ((long long)(CF_DSPBITMAP) != 130LL) { printf("FAIL constant CF_DSPBITMAP\n"); failures++; }
    if ((long long)(CF_DSPENHMETAFILE) != 142LL) { printf("FAIL constant CF_DSPENHMETAFILE\n"); failures++; }
    if ((long long)(CF_DSPMETAFILEPICT) != 131LL) { printf("FAIL constant CF_DSPMETAFILEPICT\n"); failures++; }
    if ((long long)(CF_DSPTEXT) != 129LL) { printf("FAIL constant CF_DSPTEXT\n"); failures++; }
    if ((long long)(CF_EFFECTS) != 256LL) { printf("FAIL constant CF_EFFECTS\n"); failures++; }
    if ((long long)(CF_ENABLEHOOK) != 8LL) { printf("FAIL constant CF_ENABLEHOOK\n"); failures++; }
    if ((long long)(CF_ENABLETEMPLATE) != 16LL) { printf("FAIL constant CF_ENABLETEMPLATE\n"); failures++; }
    if ((long long)(CF_ENABLETEMPLATEHANDLE) != 32LL) { printf("FAIL constant CF_ENABLETEMPLATEHANDLE\n"); failures++; }
    if ((long long)(CF_ENHMETAFILE) != 14LL) { printf("FAIL constant CF_ENHMETAFILE\n"); failures++; }
    if ((long long)(CF_FIXEDPITCHONLY) != 16384LL) { printf("FAIL constant CF_FIXEDPITCHONLY\n"); failures++; }
    if ((long long)(CF_FORCEFONTEXIST) != 65536LL) { printf("FAIL constant CF_FORCEFONTEXIST\n"); failures++; }
    if ((long long)(CF_GDIOBJFIRST) != 768LL) { printf("FAIL constant CF_GDIOBJFIRST\n"); failures++; }
    if ((long long)(CF_GDIOBJLAST) != 1023LL) { printf("FAIL constant CF_GDIOBJLAST\n"); failures++; }
    if ((long long)(CF_HDROP) != 15LL) { printf("FAIL constant CF_HDROP\n"); failures++; }
    if ((long long)(CF_INACTIVEFONTS) != 33554432LL) { printf("FAIL constant CF_INACTIVEFONTS\n"); failures++; }
    if ((long long)(CF_INITTOLOGFONTSTRUCT) != 64LL) { printf("FAIL constant CF_INITTOLOGFONTSTRUCT\n"); failures++; }
    if ((long long)(CF_LIMITSIZE) != 8192LL) { printf("FAIL constant CF_LIMITSIZE\n"); failures++; }
    if ((long long)(CF_LOCALE) != 16LL) { printf("FAIL constant CF_LOCALE\n"); failures++; }
    if ((long long)(CF_MAX) != 18LL) { printf("FAIL constant CF_MAX\n"); failures++; }
    if ((long long)(CF_METAFILEPICT) != 3LL) { printf("FAIL constant CF_METAFILEPICT\n"); failures++; }
    if ((long long)(CF_NOFACESEL) != 524288LL) { printf("FAIL constant CF_NOFACESEL\n"); failures++; }
    if ((long long)(CF_NOOEMFONTS) != 2048LL) { printf("FAIL constant CF_NOOEMFONTS\n"); failures++; }
    if ((long long)(CF_NOSCRIPTSEL) != 8388608LL) { printf("FAIL constant CF_NOSCRIPTSEL\n"); failures++; }
    if ((long long)(CF_NOSIMULATIONS) != 4096LL) { printf("FAIL constant CF_NOSIMULATIONS\n"); failures++; }
    if ((long long)(CF_NOSIZESEL) != 2097152LL) { printf("FAIL constant CF_NOSIZESEL\n"); failures++; }
    if ((long long)(CF_NOSTYLESEL) != 1048576LL) { printf("FAIL constant CF_NOSTYLESEL\n"); failures++; }
    if ((long long)(CF_NOVECTORFONTS) != 2048LL) { printf("FAIL constant CF_NOVECTORFONTS\n"); failures++; }
    if ((long long)(CF_NOVERTFONTS) != 16777216LL) { printf("FAIL constant CF_NOVERTFONTS\n"); failures++; }
    if ((long long)(CF_NULL) != 0LL) { printf("FAIL constant CF_NULL\n"); failures++; }
    if ((long long)(CF_OEMTEXT) != 7LL) { printf("FAIL constant CF_OEMTEXT\n"); failures++; }
    if ((long long)(CF_OWNERDISPLAY) != 128LL) { printf("FAIL constant CF_OWNERDISPLAY\n"); failures++; }
    if ((long long)(CF_PALETTE) != 9LL) { printf("FAIL constant CF_PALETTE\n"); failures++; }
    if ((long long)(CF_PENDATA) != 10LL) { printf("FAIL constant CF_PENDATA\n"); failures++; }
    if ((long long)(CF_PRINTERFONTS) != 2LL) { printf("FAIL constant CF_PRINTERFONTS\n"); failures++; }
    if ((long long)(CF_PRIVATEFIRST) != 512LL) { printf("FAIL constant CF_PRIVATEFIRST\n"); failures++; }
    if ((long long)(CF_PRIVATELAST) != 767LL) { printf("FAIL constant CF_PRIVATELAST\n"); failures++; }
    if ((long long)(CF_RIFF) != 11LL) { printf("FAIL constant CF_RIFF\n"); failures++; }
    if ((long long)(CF_SCALABLEONLY) != 131072LL) { printf("FAIL constant CF_SCALABLEONLY\n"); failures++; }
    if ((long long)(CF_SCREENFONTS) != 1LL) { printf("FAIL constant CF_SCREENFONTS\n"); failures++; }
    if ((long long)(CF_SCRIPTSONLY) != 1024LL) { printf("FAIL constant CF_SCRIPTSONLY\n"); failures++; }
    if ((long long)(CF_SELECTSCRIPT) != 4194304LL) { printf("FAIL constant CF_SELECTSCRIPT\n"); failures++; }
    if ((long long)(CF_SHOWHELP) != 4LL) { printf("FAIL constant CF_SHOWHELP\n"); failures++; }
    if ((long long)(CF_SYLK) != 4LL) { printf("FAIL constant CF_SYLK\n"); failures++; }
    if ((long long)(CF_TEXT) != 1LL) { printf("FAIL constant CF_TEXT\n"); failures++; }
    if ((long long)(CF_TIFF) != 6LL) { printf("FAIL constant CF_TIFF\n"); failures++; }
    if ((long long)(CF_TTONLY) != 262144LL) { printf("FAIL constant CF_TTONLY\n"); failures++; }
    if ((long long)(CF_UNICODETEXT) != 13LL) { printf("FAIL constant CF_UNICODETEXT\n"); failures++; }
    if ((long long)(CF_USESTYLE) != 128LL) { printf("FAIL constant CF_USESTYLE\n"); failures++; }
    if ((long long)(CF_WAVE) != 12LL) { printf("FAIL constant CF_WAVE\n"); failures++; }
    if ((long long)(CF_WYSIWYG) != 32768LL) { printf("FAIL constant CF_WYSIWYG\n"); failures++; }
    if ((long long)(CLSCTX_ACTIVATE_32_BIT_SERVER) != 262144LL) { printf("FAIL constant CLSCTX_ACTIVATE_32_BIT_SERVER\n"); failures++; }
    if ((long long)(CLSCTX_ACTIVATE_64_BIT_SERVER) != 524288LL) { printf("FAIL constant CLSCTX_ACTIVATE_64_BIT_SERVER\n"); failures++; }
    if ((long long)(CLSCTX_ACTIVATE_AAA_AS_IU) != 8388608LL) { printf("FAIL constant CLSCTX_ACTIVATE_AAA_AS_IU\n"); failures++; }
    if ((long long)(CLSCTX_ALL) != 23LL) { printf("FAIL constant CLSCTX_ALL\n"); failures++; }
    if ((long long)(CLSCTX_APPCONTAINER) != 4194304LL) { printf("FAIL constant CLSCTX_APPCONTAINER\n"); failures++; }
    if ((long long)(CLSCTX_DISABLE_AAA) != 32768LL) { printf("FAIL constant CLSCTX_DISABLE_AAA\n"); failures++; }
    if ((long long)(CLSCTX_ENABLE_AAA) != 65536LL) { printf("FAIL constant CLSCTX_ENABLE_AAA\n"); failures++; }
    if ((long long)(CLSCTX_ENABLE_CLOAKING) != 1048576LL) { printf("FAIL constant CLSCTX_ENABLE_CLOAKING\n"); failures++; }
    if ((long long)(CLSCTX_ENABLE_CODE_DOWNLOAD) != 8192LL) { printf("FAIL constant CLSCTX_ENABLE_CODE_DOWNLOAD\n"); failures++; }
    if ((long long)(CLSCTX_FROM_DEFAULT_CONTEXT) != 131072LL) { printf("FAIL constant CLSCTX_FROM_DEFAULT_CONTEXT\n"); failures++; }
    if ((long long)(CLSCTX_INPROC) != 3LL) { printf("FAIL constant CLSCTX_INPROC\n"); failures++; }
    if ((long long)(CLSCTX_INPROC_HANDLER) != 2LL) { printf("FAIL constant CLSCTX_INPROC_HANDLER\n"); failures++; }
    if ((long long)(CLSCTX_INPROC_HANDLER16) != 32LL) { printf("FAIL constant CLSCTX_INPROC_HANDLER16\n"); failures++; }
    if ((long long)(CLSCTX_INPROC_SERVER) != 1LL) { printf("FAIL constant CLSCTX_INPROC_SERVER\n"); failures++; }
    if ((long long)(CLSCTX_INPROC_SERVER16) != 8LL) { printf("FAIL constant CLSCTX_INPROC_SERVER16\n"); failures++; }
    if ((long long)(CLSCTX_LOCAL_SERVER) != 4LL) { printf("FAIL constant CLSCTX_LOCAL_SERVER\n"); failures++; }
    if ((long long)(CLSCTX_NO_CODE_DOWNLOAD) != 1024LL) { printf("FAIL constant CLSCTX_NO_CODE_DOWNLOAD\n"); failures++; }
    if ((long long)(CLSCTX_NO_CUSTOM_MARSHAL) != 4096LL) { printf("FAIL constant CLSCTX_NO_CUSTOM_MARSHAL\n"); failures++; }
    if ((long long)(CLSCTX_NO_FAILURE_LOG) != 16384LL) { printf("FAIL constant CLSCTX_NO_FAILURE_LOG\n"); failures++; }
    if ((long long)(CLSCTX_PS_DLL) != (-2147483648LL)) { printf("FAIL constant CLSCTX_PS_DLL\n"); failures++; }
    if ((long long)(CLSCTX_REMOTE_SERVER) != 16LL) { printf("FAIL constant CLSCTX_REMOTE_SERVER\n"); failures++; }
    if ((long long)(CLSCTX_RESERVED1) != 64LL) { printf("FAIL constant CLSCTX_RESERVED1\n"); failures++; }
    if ((long long)(CLSCTX_RESERVED2) != 128LL) { printf("FAIL constant CLSCTX_RESERVED2\n"); failures++; }
    if ((long long)(CLSCTX_RESERVED3) != 256LL) { printf("FAIL constant CLSCTX_RESERVED3\n"); failures++; }
    if ((long long)(CLSCTX_RESERVED4) != 512LL) { printf("FAIL constant CLSCTX_RESERVED4\n"); failures++; }
    if ((long long)(CLSCTX_RESERVED5) != 2048LL) { printf("FAIL constant CLSCTX_RESERVED5\n"); failures++; }
    if ((long long)(CLSCTX_SERVER) != 21LL) { printf("FAIL constant CLSCTX_SERVER\n"); failures++; }
    if ((long long)(CLSCTX_VALID_MASK) != (-2132806625LL)) { printf("FAIL constant CLSCTX_VALID_MASK\n"); failures++; }
    if ((long long)(COINIT_APARTMENTTHREADED) != 2LL) { printf("FAIL constant COINIT_APARTMENTTHREADED\n"); failures++; }
    if ((long long)(COINIT_DISABLE_OLE1DDE) != 4LL) { printf("FAIL constant COINIT_DISABLE_OLE1DDE\n"); failures++; }
    if ((long long)(COINIT_MULTITHREADED) != 0LL) { printf("FAIL constant COINIT_MULTITHREADED\n"); failures++; }
    if ((long long)(COINIT_SPEED_OVER_MEMORY) != 8LL) { printf("FAIL constant COINIT_SPEED_OVER_MEMORY\n"); failures++; }
    if ((long long)(COLOR_3DDKSHADOW) != 21LL) { printf("FAIL constant COLOR_3DDKSHADOW\n"); failures++; }
    if ((long long)(COLOR_3DFACE) != 15LL) { printf("FAIL constant COLOR_3DFACE\n"); failures++; }
    if ((long long)(COLOR_3DHIGHLIGHT) != 20LL) { printf("FAIL constant COLOR_3DHIGHLIGHT\n"); failures++; }
    if ((long long)(COLOR_3DHILIGHT) != 20LL) { printf("FAIL constant COLOR_3DHILIGHT\n"); failures++; }
    if ((long long)(COLOR_3DLIGHT) != 22LL) { printf("FAIL constant COLOR_3DLIGHT\n"); failures++; }
    if ((long long)(COLOR_3DSHADOW) != 16LL) { printf("FAIL constant COLOR_3DSHADOW\n"); failures++; }
    if ((long long)(COLOR_ACTIVEBORDER) != 10LL) { printf("FAIL constant COLOR_ACTIVEBORDER\n"); failures++; }
    if ((long long)(COLOR_ACTIVECAPTION) != 2LL) { printf("FAIL constant COLOR_ACTIVECAPTION\n"); failures++; }
    if ((long long)(COLOR_ADJ_MAX) != 100LL) { printf("FAIL constant COLOR_ADJ_MAX\n"); failures++; }
    if ((long long)(COLOR_ADJ_MIN) != (-100LL)) { printf("FAIL constant COLOR_ADJ_MIN\n"); failures++; }
    if ((long long)(COLOR_APPWORKSPACE) != 12LL) { printf("FAIL constant COLOR_APPWORKSPACE\n"); failures++; }
    if ((long long)(COLOR_BACKGROUND) != 1LL) { printf("FAIL constant COLOR_BACKGROUND\n"); failures++; }
    if ((long long)(COLOR_BTNFACE) != 15LL) { printf("FAIL constant COLOR_BTNFACE\n"); failures++; }
    if ((long long)(COLOR_BTNHIGHLIGHT) != 20LL) { printf("FAIL constant COLOR_BTNHIGHLIGHT\n"); failures++; }
    if ((long long)(COLOR_BTNHILIGHT) != 20LL) { printf("FAIL constant COLOR_BTNHILIGHT\n"); failures++; }
    if ((long long)(COLOR_BTNSHADOW) != 16LL) { printf("FAIL constant COLOR_BTNSHADOW\n"); failures++; }
    if ((long long)(COLOR_BTNTEXT) != 18LL) { printf("FAIL constant COLOR_BTNTEXT\n"); failures++; }
    if ((long long)(COLOR_CAPTIONTEXT) != 9LL) { printf("FAIL constant COLOR_CAPTIONTEXT\n"); failures++; }
    if ((long long)(COLOR_DESKTOP) != 1LL) { printf("FAIL constant COLOR_DESKTOP\n"); failures++; }
    if ((long long)(COLOR_GRADIENTACTIVECAPTION) != 27LL) { printf("FAIL constant COLOR_GRADIENTACTIVECAPTION\n"); failures++; }
    if ((long long)(COLOR_GRADIENTINACTIVECAPTION) != 28LL) { printf("FAIL constant COLOR_GRADIENTINACTIVECAPTION\n"); failures++; }
    if ((long long)(COLOR_GRAYTEXT) != 17LL) { printf("FAIL constant COLOR_GRAYTEXT\n"); failures++; }
    if ((long long)(COLOR_HIGHLIGHT) != 13LL) { printf("FAIL constant COLOR_HIGHLIGHT\n"); failures++; }
    if ((long long)(COLOR_HIGHLIGHTTEXT) != 14LL) { printf("FAIL constant COLOR_HIGHLIGHTTEXT\n"); failures++; }
    if ((long long)(COLOR_HOTLIGHT) != 26LL) { printf("FAIL constant COLOR_HOTLIGHT\n"); failures++; }
    if ((long long)(COLOR_INACTIVEBORDER) != 11LL) { printf("FAIL constant COLOR_INACTIVEBORDER\n"); failures++; }
    if ((long long)(COLOR_INACTIVECAPTION) != 3LL) { printf("FAIL constant COLOR_INACTIVECAPTION\n"); failures++; }
    if ((long long)(COLOR_INACTIVECAPTIONTEXT) != 19LL) { printf("FAIL constant COLOR_INACTIVECAPTIONTEXT\n"); failures++; }
    if ((long long)(COLOR_INFOBK) != 24LL) { printf("FAIL constant COLOR_INFOBK\n"); failures++; }
    if ((long long)(COLOR_INFOTEXT) != 23LL) { printf("FAIL constant COLOR_INFOTEXT\n"); failures++; }
    if ((long long)(COLOR_MENU) != 4LL) { printf("FAIL constant COLOR_MENU\n"); failures++; }
    if ((long long)(COLOR_MENUBAR) != 30LL) { printf("FAIL constant COLOR_MENUBAR\n"); failures++; }
    if ((long long)(COLOR_MENUHILIGHT) != 29LL) { printf("FAIL constant COLOR_MENUHILIGHT\n"); failures++; }
    if ((long long)(COLOR_MENUTEXT) != 7LL) { printf("FAIL constant COLOR_MENUTEXT\n"); failures++; }
    if ((long long)(COLOR_SCROLLBAR) != 0LL) { printf("FAIL constant COLOR_SCROLLBAR\n"); failures++; }
    if ((long long)(COLOR_WINDOW) != 5LL) { printf("FAIL constant COLOR_WINDOW\n"); failures++; }
    if ((long long)(COLOR_WINDOWFRAME) != 6LL) { printf("FAIL constant COLOR_WINDOWFRAME\n"); failures++; }
    if ((long long)(COLOR_WINDOWTEXT) != 8LL) { printf("FAIL constant COLOR_WINDOWTEXT\n"); failures++; }
    if ((long long)(CPS_CANCEL) != 4LL) { printf("FAIL constant CPS_CANCEL\n"); failures++; }
    if ((long long)(CPS_COMPLETE) != 1LL) { printf("FAIL constant CPS_COMPLETE\n"); failures++; }
    if ((long long)(CPS_CONVERT) != 2LL) { printf("FAIL constant CPS_CONVERT\n"); failures++; }
    if ((long long)(CPS_REVERT) != 3LL) { printf("FAIL constant CPS_REVERT\n"); failures++; }
    if ((long long)(CP_ACP) != 0LL) { printf("FAIL constant CP_ACP\n"); failures++; }
    if ((long long)(CP_INSTALLED) != 1LL) { printf("FAIL constant CP_INSTALLED\n"); failures++; }
    if ((long long)(CP_MACCP) != 2LL) { printf("FAIL constant CP_MACCP\n"); failures++; }
    if ((long long)(CP_NONE) != 0LL) { printf("FAIL constant CP_NONE\n"); failures++; }
    if ((long long)(CP_OEMCP) != 1LL) { printf("FAIL constant CP_OEMCP\n"); failures++; }
    if ((long long)(CP_RECTANGLE) != 1LL) { printf("FAIL constant CP_RECTANGLE\n"); failures++; }
    if ((long long)(CP_REGION) != 2LL) { printf("FAIL constant CP_REGION\n"); failures++; }
    if ((long long)(CP_SUPPORTED) != 2LL) { printf("FAIL constant CP_SUPPORTED\n"); failures++; }
    if ((long long)(CP_SYMBOL) != 42LL) { printf("FAIL constant CP_SYMBOL\n"); failures++; }
    if ((long long)(CP_THREAD_ACP) != 3LL) { printf("FAIL constant CP_THREAD_ACP\n"); failures++; }
    if ((long long)(CP_UTF7) != 65000LL) { printf("FAIL constant CP_UTF7\n"); failures++; }
    if ((long long)(CP_UTF8) != 65001LL) { printf("FAIL constant CP_UTF8\n"); failures++; }
    if ((long long)(CP_WINANSI) != 1004LL) { printf("FAIL constant CP_WINANSI\n"); failures++; }
    if ((long long)(CP_WINNEUTRAL) != 1200LL) { printf("FAIL constant CP_WINNEUTRAL\n"); failures++; }
    if ((long long)(CP_WINUNICODE) != 1200LL) { printf("FAIL constant CP_WINUNICODE\n"); failures++; }
    if ((long long)(CS_BYTEALIGNCLIENT) != 4096LL) { printf("FAIL constant CS_BYTEALIGNCLIENT\n"); failures++; }
    if ((long long)(CS_BYTEALIGNWINDOW) != 8192LL) { printf("FAIL constant CS_BYTEALIGNWINDOW\n"); failures++; }
    if ((long long)(CS_CLASSDC) != 64LL) { printf("FAIL constant CS_CLASSDC\n"); failures++; }
    if ((long long)(CS_DBLCLKS) != 8LL) { printf("FAIL constant CS_DBLCLKS\n"); failures++; }
    if ((long long)(CS_DELETE_TRANSFORM) != 3LL) { printf("FAIL constant CS_DELETE_TRANSFORM\n"); failures++; }
    if ((long long)(CS_DISABLE) != 2LL) { printf("FAIL constant CS_DISABLE\n"); failures++; }
    if ((long long)(CS_DROPSHADOW) != 131072LL) { printf("FAIL constant CS_DROPSHADOW\n"); failures++; }
    if ((long long)(CS_ENABLE) != 1LL) { printf("FAIL constant CS_ENABLE\n"); failures++; }
    if ((long long)(CS_E_ADMIN_LIMIT_EXCEEDED) != (-2147221139LL)) { printf("FAIL constant CS_E_ADMIN_LIMIT_EXCEEDED\n"); failures++; }
    if ((long long)(CS_E_CLASS_NOTFOUND) != (-2147221146LL)) { printf("FAIL constant CS_E_CLASS_NOTFOUND\n"); failures++; }
    if ((long long)(CS_E_FIRST) != 2147746148LL) { printf("FAIL constant CS_E_FIRST\n"); failures++; }
    if ((long long)(CS_E_INTERNAL_ERROR) != (-2147221137LL)) { printf("FAIL constant CS_E_INTERNAL_ERROR\n"); failures++; }
    if ((long long)(CS_E_INVALID_PATH) != (-2147221141LL)) { printf("FAIL constant CS_E_INVALID_PATH\n"); failures++; }
    if ((long long)(CS_E_INVALID_VERSION) != (-2147221145LL)) { printf("FAIL constant CS_E_INVALID_VERSION\n"); failures++; }
    if ((long long)(CS_E_LAST) != 2147746159LL) { printf("FAIL constant CS_E_LAST\n"); failures++; }
    if ((long long)(CS_E_NETWORK_ERROR) != (-2147221140LL)) { printf("FAIL constant CS_E_NETWORK_ERROR\n"); failures++; }
    if ((long long)(CS_E_NOT_DELETABLE) != (-2147221147LL)) { printf("FAIL constant CS_E_NOT_DELETABLE\n"); failures++; }
    if ((long long)(CS_E_NO_CLASSSTORE) != (-2147221144LL)) { printf("FAIL constant CS_E_NO_CLASSSTORE\n"); failures++; }
    if ((long long)(CS_E_OBJECT_ALREADY_EXISTS) != (-2147221142LL)) { printf("FAIL constant CS_E_OBJECT_ALREADY_EXISTS\n"); failures++; }
    if ((long long)(CS_E_OBJECT_NOTFOUND) != (-2147221143LL)) { printf("FAIL constant CS_E_OBJECT_NOTFOUND\n"); failures++; }
    if ((long long)(CS_E_PACKAGE_NOTFOUND) != (-2147221148LL)) { printf("FAIL constant CS_E_PACKAGE_NOTFOUND\n"); failures++; }
    if ((long long)(CS_E_SCHEMA_MISMATCH) != (-2147221138LL)) { printf("FAIL constant CS_E_SCHEMA_MISMATCH\n"); failures++; }
    if ((long long)(CS_GLOBALCLASS) != 16384LL) { printf("FAIL constant CS_GLOBALCLASS\n"); failures++; }
    if ((long long)(CS_HREDRAW) != 2LL) { printf("FAIL constant CS_HREDRAW\n"); failures++; }
    if ((long long)(CS_IME) != 65536LL) { printf("FAIL constant CS_IME\n"); failures++; }
    if ((long long)(CS_INSERTCHAR) != 8192LL) { printf("FAIL constant CS_INSERTCHAR\n"); failures++; }
    if ((long long)(CS_NOCLOSE) != 512LL) { printf("FAIL constant CS_NOCLOSE\n"); failures++; }
    if ((long long)(CS_NOMOVECARET) != 16384LL) { printf("FAIL constant CS_NOMOVECARET\n"); failures++; }
    if ((long long)(CS_OWNDC) != 32LL) { printf("FAIL constant CS_OWNDC\n"); failures++; }
    if ((long long)(CS_PARENTDC) != 128LL) { printf("FAIL constant CS_PARENTDC\n"); failures++; }
    if ((long long)(CS_SAVEBITS) != 2048LL) { printf("FAIL constant CS_SAVEBITS\n"); failures++; }
    if ((long long)(CS_VREDRAW) != 1LL) { printf("FAIL constant CS_VREDRAW\n"); failures++; }
    if ((long long)(DATADIR_GET) != 1LL) { printf("FAIL constant DATADIR_GET\n"); failures++; }
    if ((long long)(DATADIR_SET) != 2LL) { printf("FAIL constant DATADIR_SET\n"); failures++; }
    if ((long long)(DIB_PAL_COLORS) != 1LL) { printf("FAIL constant DIB_PAL_COLORS\n"); failures++; }
    if ((long long)(DIB_RGB_COLORS) != 0LL) { printf("FAIL constant DIB_RGB_COLORS\n"); failures++; }
    if ((long long)(DRAGDROP_S_CANCEL) != 262401LL) { printf("FAIL constant DRAGDROP_S_CANCEL\n"); failures++; }
    if ((long long)(DRAGDROP_S_DROP) != 262400LL) { printf("FAIL constant DRAGDROP_S_DROP\n"); failures++; }
    if ((long long)(DRAGDROP_S_FIRST) != 262400LL) { printf("FAIL constant DRAGDROP_S_FIRST\n"); failures++; }
    if ((long long)(DRAGDROP_S_LAST) != 262415LL) { printf("FAIL constant DRAGDROP_S_LAST\n"); failures++; }
    if ((long long)(DRAGDROP_S_USEDEFAULTCURSORS) != 262402LL) { printf("FAIL constant DRAGDROP_S_USEDEFAULTCURSORS\n"); failures++; }
    if ((long long)(DRIVE_CDROM) != 5LL) { printf("FAIL constant DRIVE_CDROM\n"); failures++; }
    if ((long long)(DRIVE_FIXED) != 3LL) { printf("FAIL constant DRIVE_FIXED\n"); failures++; }
    if ((long long)(DRIVE_NO_ROOT_DIR) != 1LL) { printf("FAIL constant DRIVE_NO_ROOT_DIR\n"); failures++; }
    if ((long long)(DRIVE_RAMDISK) != 6LL) { printf("FAIL constant DRIVE_RAMDISK\n"); failures++; }
    if ((long long)(DRIVE_REMOTE) != 4LL) { printf("FAIL constant DRIVE_REMOTE\n"); failures++; }
    if ((long long)(DRIVE_REMOVABLE) != 2LL) { printf("FAIL constant DRIVE_REMOVABLE\n"); failures++; }
    if ((long long)(DRIVE_UNKNOWN) != 0LL) { printf("FAIL constant DRIVE_UNKNOWN\n"); failures++; }
    if ((long long)(DROPEFFECT_COPY) != 1LL) { printf("FAIL constant DROPEFFECT_COPY\n"); failures++; }
    if ((long long)(DROPEFFECT_LINK) != 4LL) { printf("FAIL constant DROPEFFECT_LINK\n"); failures++; }
    if ((long long)(DROPEFFECT_MOVE) != 2LL) { printf("FAIL constant DROPEFFECT_MOVE\n"); failures++; }
    if ((long long)(DROPEFFECT_NONE) != 0LL) { printf("FAIL constant DROPEFFECT_NONE\n"); failures++; }
    if ((long long)(DROPEFFECT_SCROLL) != 2147483648LL) { printf("FAIL constant DROPEFFECT_SCROLL\n"); failures++; }
    if ((long long)(DVASPECT_CONTENT) != 1LL) { printf("FAIL constant DVASPECT_CONTENT\n"); failures++; }
    if ((long long)(DVASPECT_COPY) != 3LL) { printf("FAIL constant DVASPECT_COPY\n"); failures++; }
    if ((long long)(DVASPECT_DOCPRINT) != 8LL) { printf("FAIL constant DVASPECT_DOCPRINT\n"); failures++; }
    if ((long long)(DVASPECT_ICON) != 4LL) { printf("FAIL constant DVASPECT_ICON\n"); failures++; }
    if ((long long)(DVASPECT_LINK) != 4LL) { printf("FAIL constant DVASPECT_LINK\n"); failures++; }
    if ((long long)(DVASPECT_OPAQUE) != 16LL) { printf("FAIL constant DVASPECT_OPAQUE\n"); failures++; }
    if ((long long)(DVASPECT_SHORTNAME) != 2LL) { printf("FAIL constant DVASPECT_SHORTNAME\n"); failures++; }
    if ((long long)(DVASPECT_THUMBNAIL) != 2LL) { printf("FAIL constant DVASPECT_THUMBNAIL\n"); failures++; }
    if ((long long)(DVASPECT_TRANSPARENT) != 32LL) { printf("FAIL constant DVASPECT_TRANSPARENT\n"); failures++; }
    if ((long long)(DV_E_CLIPFORMAT) != (-2147221398LL)) { printf("FAIL constant DV_E_CLIPFORMAT\n"); failures++; }
    if ((long long)(DV_E_DVASPECT) != (-2147221397LL)) { printf("FAIL constant DV_E_DVASPECT\n"); failures++; }
    if ((long long)(DV_E_DVTARGETDEVICE) != (-2147221403LL)) { printf("FAIL constant DV_E_DVTARGETDEVICE\n"); failures++; }
    if ((long long)(DV_E_DVTARGETDEVICE_SIZE) != (-2147221396LL)) { printf("FAIL constant DV_E_DVTARGETDEVICE_SIZE\n"); failures++; }
    if ((long long)(DV_E_FORMATETC) != (-2147221404LL)) { printf("FAIL constant DV_E_FORMATETC\n"); failures++; }
    if ((long long)(DV_E_LINDEX) != (-2147221400LL)) { printf("FAIL constant DV_E_LINDEX\n"); failures++; }
    if ((long long)(DV_E_NOIVIEWOBJECT) != (-2147221395LL)) { printf("FAIL constant DV_E_NOIVIEWOBJECT\n"); failures++; }
    if ((long long)(DV_E_STATDATA) != (-2147221401LL)) { printf("FAIL constant DV_E_STATDATA\n"); failures++; }
    if ((long long)(DV_E_STGMEDIUM) != (-2147221402LL)) { printf("FAIL constant DV_E_STGMEDIUM\n"); failures++; }
    if ((long long)(DV_E_TYMED) != (-2147221399LL)) { printf("FAIL constant DV_E_TYMED\n"); failures++; }
    if ((long long)(DWMNCRP_DISABLED) != 1LL) { printf("FAIL constant DWMNCRP_DISABLED\n"); failures++; }
    if ((long long)(DWMNCRP_ENABLED) != 2LL) { printf("FAIL constant DWMNCRP_ENABLED\n"); failures++; }
    if ((long long)(DWMNCRP_LAST) != 3LL) { printf("FAIL constant DWMNCRP_LAST\n"); failures++; }
    if ((long long)(DWMNCRP_USEWINDOWSTYLE) != 0LL) { printf("FAIL constant DWMNCRP_USEWINDOWSTYLE\n"); failures++; }
    if ((long long)(DWMWA_ALLOW_NCPAINT) != 4LL) { printf("FAIL constant DWMWA_ALLOW_NCPAINT\n"); failures++; }
    if ((long long)(DWMWA_BORDER_COLOR) != 34LL) { printf("FAIL constant DWMWA_BORDER_COLOR\n"); failures++; }
    if ((long long)(DWMWA_CAPTION_BUTTON_BOUNDS) != 5LL) { printf("FAIL constant DWMWA_CAPTION_BUTTON_BOUNDS\n"); failures++; }
    if ((long long)(DWMWA_CAPTION_COLOR) != 35LL) { printf("FAIL constant DWMWA_CAPTION_COLOR\n"); failures++; }
    if ((long long)(DWMWA_CLOAK) != 13LL) { printf("FAIL constant DWMWA_CLOAK\n"); failures++; }
    if ((long long)(DWMWA_CLOAKED) != 14LL) { printf("FAIL constant DWMWA_CLOAKED\n"); failures++; }
    if ((long long)(DWMWA_COLOR_DEFAULT) != 4294967295LL) { printf("FAIL constant DWMWA_COLOR_DEFAULT\n"); failures++; }
    if ((long long)(DWMWA_COLOR_NONE) != 4294967294LL) { printf("FAIL constant DWMWA_COLOR_NONE\n"); failures++; }
    if ((long long)(DWMWA_DISALLOW_PEEK) != 11LL) { printf("FAIL constant DWMWA_DISALLOW_PEEK\n"); failures++; }
    if ((long long)(DWMWA_EXCLUDED_FROM_PEEK) != 12LL) { printf("FAIL constant DWMWA_EXCLUDED_FROM_PEEK\n"); failures++; }
    if ((long long)(DWMWA_EXTENDED_FRAME_BOUNDS) != 9LL) { printf("FAIL constant DWMWA_EXTENDED_FRAME_BOUNDS\n"); failures++; }
    if ((long long)(DWMWA_FLIP3D_POLICY) != 8LL) { printf("FAIL constant DWMWA_FLIP3D_POLICY\n"); failures++; }
    if ((long long)(DWMWA_FORCE_ICONIC_REPRESENTATION) != 7LL) { printf("FAIL constant DWMWA_FORCE_ICONIC_REPRESENTATION\n"); failures++; }
    if ((long long)(DWMWA_FREEZE_REPRESENTATION) != 15LL) { printf("FAIL constant DWMWA_FREEZE_REPRESENTATION\n"); failures++; }
    if ((long long)(DWMWA_HAS_ICONIC_BITMAP) != 10LL) { printf("FAIL constant DWMWA_HAS_ICONIC_BITMAP\n"); failures++; }
    if ((long long)(DWMWA_LAST) != 39LL) { printf("FAIL constant DWMWA_LAST\n"); failures++; }
    if ((long long)(DWMWA_NCRENDERING_ENABLED) != 1LL) { printf("FAIL constant DWMWA_NCRENDERING_ENABLED\n"); failures++; }
    if ((long long)(DWMWA_NCRENDERING_POLICY) != 2LL) { printf("FAIL constant DWMWA_NCRENDERING_POLICY\n"); failures++; }
    if ((long long)(DWMWA_NONCLIENT_RTL_LAYOUT) != 6LL) { printf("FAIL constant DWMWA_NONCLIENT_RTL_LAYOUT\n"); failures++; }
    if ((long long)(DWMWA_PASSIVE_UPDATE_MODE) != 16LL) { printf("FAIL constant DWMWA_PASSIVE_UPDATE_MODE\n"); failures++; }
    if ((long long)(DWMWA_SYSTEMBACKDROP_TYPE) != 38LL) { printf("FAIL constant DWMWA_SYSTEMBACKDROP_TYPE\n"); failures++; }
    if ((long long)(DWMWA_TEXT_COLOR) != 36LL) { printf("FAIL constant DWMWA_TEXT_COLOR\n"); failures++; }
    if ((long long)(DWMWA_TRANSITIONS_FORCEDISABLED) != 3LL) { printf("FAIL constant DWMWA_TRANSITIONS_FORCEDISABLED\n"); failures++; }
    if ((long long)(DWMWA_USE_HOSTBACKDROPBRUSH) != 17LL) { printf("FAIL constant DWMWA_USE_HOSTBACKDROPBRUSH\n"); failures++; }
    if ((long long)(DWMWA_USE_IMMERSIVE_DARK_MODE) != 20LL) { printf("FAIL constant DWMWA_USE_IMMERSIVE_DARK_MODE\n"); failures++; }
    if ((long long)(DWMWA_VISIBLE_FRAME_BORDER_THICKNESS) != 37LL) { printf("FAIL constant DWMWA_VISIBLE_FRAME_BORDER_THICKNESS\n"); failures++; }
    if ((long long)(DWMWA_WINDOW_CORNER_PREFERENCE) != 33LL) { printf("FAIL constant DWMWA_WINDOW_CORNER_PREFERENCE\n"); failures++; }
    if ((long long)(DWMWCP_DEFAULT) != 0LL) { printf("FAIL constant DWMWCP_DEFAULT\n"); failures++; }
    if ((long long)(DWMWCP_DONOTROUND) != 1LL) { printf("FAIL constant DWMWCP_DONOTROUND\n"); failures++; }
    if ((long long)(DWMWCP_ROUND) != 2LL) { printf("FAIL constant DWMWCP_ROUND\n"); failures++; }
    if ((long long)(DWMWCP_ROUNDSMALL) != 3LL) { printf("FAIL constant DWMWCP_ROUNDSMALL\n"); failures++; }
    if ((long long)(ExpandCollapseState_Collapsed) != 0LL) { printf("FAIL constant ExpandCollapseState_Collapsed\n"); failures++; }
    if ((long long)(ExpandCollapseState_Expanded) != 1LL) { printf("FAIL constant ExpandCollapseState_Expanded\n"); failures++; }
    if ((long long)(ExpandCollapseState_LeafNode) != 3LL) { printf("FAIL constant ExpandCollapseState_LeafNode\n"); failures++; }
    if ((long long)(ExpandCollapseState_PartiallyExpanded) != 2LL) { printf("FAIL constant ExpandCollapseState_PartiallyExpanded\n"); failures++; }
    if ((long long)(FDAP_BOTTOM) != 0LL) { printf("FAIL constant FDAP_BOTTOM\n"); failures++; }
    if ((long long)(FDAP_TOP) != 1LL) { printf("FAIL constant FDAP_TOP\n"); failures++; }
    if ((long long)(FE_FONTSMOOTHINGCLEARTYPE) != 2LL) { printf("FAIL constant FE_FONTSMOOTHINGCLEARTYPE\n"); failures++; }
    if ((long long)(FE_FONTSMOOTHINGDOCKING) != 32768LL) { printf("FAIL constant FE_FONTSMOOTHINGDOCKING\n"); failures++; }
    if ((long long)(FE_FONTSMOOTHINGORIENTATIONBGR) != 0LL) { printf("FAIL constant FE_FONTSMOOTHINGORIENTATIONBGR\n"); failures++; }
    if ((long long)(FE_FONTSMOOTHINGORIENTATIONRGB) != 1LL) { printf("FAIL constant FE_FONTSMOOTHINGORIENTATIONRGB\n"); failures++; }
    if ((long long)(FE_FONTSMOOTHINGSTANDARD) != 1LL) { printf("FAIL constant FE_FONTSMOOTHINGSTANDARD\n"); failures++; }
    if ((long long)(FILE_ATTRIBUTE_ARCHIVE) != 32LL) { printf("FAIL constant FILE_ATTRIBUTE_ARCHIVE\n"); failures++; }
    if ((long long)(FILE_ATTRIBUTE_COMPRESSED) != 2048LL) { printf("FAIL constant FILE_ATTRIBUTE_COMPRESSED\n"); failures++; }
    if ((long long)(FILE_ATTRIBUTE_DEVICE) != 64LL) { printf("FAIL constant FILE_ATTRIBUTE_DEVICE\n"); failures++; }
    if ((long long)(FILE_ATTRIBUTE_DIRECTORY) != 16LL) { printf("FAIL constant FILE_ATTRIBUTE_DIRECTORY\n"); failures++; }
    if ((long long)(FILE_ATTRIBUTE_EA) != 262144LL) { printf("FAIL constant FILE_ATTRIBUTE_EA\n"); failures++; }
    if ((long long)(FILE_ATTRIBUTE_ENCRYPTED) != 16384LL) { printf("FAIL constant FILE_ATTRIBUTE_ENCRYPTED\n"); failures++; }
    if ((long long)(FILE_ATTRIBUTE_HIDDEN) != 2LL) { printf("FAIL constant FILE_ATTRIBUTE_HIDDEN\n"); failures++; }
    if ((long long)(FILE_ATTRIBUTE_INTEGRITY_STREAM) != 32768LL) { printf("FAIL constant FILE_ATTRIBUTE_INTEGRITY_STREAM\n"); failures++; }
    if ((long long)(FILE_ATTRIBUTE_NORMAL) != 128LL) { printf("FAIL constant FILE_ATTRIBUTE_NORMAL\n"); failures++; }
    if ((long long)(FILE_ATTRIBUTE_NOT_CONTENT_INDEXED) != 8192LL) { printf("FAIL constant FILE_ATTRIBUTE_NOT_CONTENT_INDEXED\n"); failures++; }
    if ((long long)(FILE_ATTRIBUTE_NO_SCRUB_DATA) != 131072LL) { printf("FAIL constant FILE_ATTRIBUTE_NO_SCRUB_DATA\n"); failures++; }
    if ((long long)(FILE_ATTRIBUTE_OFFLINE) != 4096LL) { printf("FAIL constant FILE_ATTRIBUTE_OFFLINE\n"); failures++; }
    if ((long long)(FILE_ATTRIBUTE_PINNED) != 524288LL) { printf("FAIL constant FILE_ATTRIBUTE_PINNED\n"); failures++; }
    if ((long long)(FILE_ATTRIBUTE_READONLY) != 1LL) { printf("FAIL constant FILE_ATTRIBUTE_READONLY\n"); failures++; }
    if ((long long)(FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS) != 4194304LL) { printf("FAIL constant FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS\n"); failures++; }
    if ((long long)(FILE_ATTRIBUTE_RECALL_ON_OPEN) != 262144LL) { printf("FAIL constant FILE_ATTRIBUTE_RECALL_ON_OPEN\n"); failures++; }
    if ((long long)(FILE_ATTRIBUTE_REPARSE_POINT) != 1024LL) { printf("FAIL constant FILE_ATTRIBUTE_REPARSE_POINT\n"); failures++; }
    if ((long long)(FILE_ATTRIBUTE_SPARSE_FILE) != 512LL) { printf("FAIL constant FILE_ATTRIBUTE_SPARSE_FILE\n"); failures++; }
    if ((long long)(FILE_ATTRIBUTE_STRICTLY_SEQUENTIAL) != 536870912LL) { printf("FAIL constant FILE_ATTRIBUTE_STRICTLY_SEQUENTIAL\n"); failures++; }
    if ((long long)(FILE_ATTRIBUTE_SYSTEM) != 4LL) { printf("FAIL constant FILE_ATTRIBUTE_SYSTEM\n"); failures++; }
    if ((long long)(FILE_ATTRIBUTE_TEMPORARY) != 256LL) { printf("FAIL constant FILE_ATTRIBUTE_TEMPORARY\n"); failures++; }
    if ((long long)(FILE_ATTRIBUTE_UNPINNED) != 1048576LL) { printf("FAIL constant FILE_ATTRIBUTE_UNPINNED\n"); failures++; }
    if ((long long)(FILE_ATTRIBUTE_VIRTUAL) != 65536LL) { printf("FAIL constant FILE_ATTRIBUTE_VIRTUAL\n"); failures++; }
    if ((long long)(FILE_FLAG_BACKUP_SEMANTICS) != 33554432LL) { printf("FAIL constant FILE_FLAG_BACKUP_SEMANTICS\n"); failures++; }
    if ((long long)(FILE_FLAG_DELETE_ON_CLOSE) != 67108864LL) { printf("FAIL constant FILE_FLAG_DELETE_ON_CLOSE\n"); failures++; }
    if ((long long)(FILE_FLAG_FIRST_PIPE_INSTANCE) != 524288LL) { printf("FAIL constant FILE_FLAG_FIRST_PIPE_INSTANCE\n"); failures++; }
    if ((long long)(FILE_FLAG_IGNORE_IMPERSONATED_DEVICEMAP) != 131072LL) { printf("FAIL constant FILE_FLAG_IGNORE_IMPERSONATED_DEVICEMAP\n"); failures++; }
    if ((long long)(FILE_FLAG_NO_BUFFERING) != 536870912LL) { printf("FAIL constant FILE_FLAG_NO_BUFFERING\n"); failures++; }
    if ((long long)(FILE_FLAG_OPEN_NO_RECALL) != 1048576LL) { printf("FAIL constant FILE_FLAG_OPEN_NO_RECALL\n"); failures++; }
    if ((long long)(FILE_FLAG_OPEN_REPARSE_POINT) != 2097152LL) { printf("FAIL constant FILE_FLAG_OPEN_REPARSE_POINT\n"); failures++; }
    if ((long long)(FILE_FLAG_OPEN_REQUIRING_OPLOCK) != 262144LL) { printf("FAIL constant FILE_FLAG_OPEN_REQUIRING_OPLOCK\n"); failures++; }
    if ((long long)(FILE_FLAG_OVERLAPPED) != 1073741824LL) { printf("FAIL constant FILE_FLAG_OVERLAPPED\n"); failures++; }
    if ((long long)(FILE_FLAG_POSIX_SEMANTICS) != 16777216LL) { printf("FAIL constant FILE_FLAG_POSIX_SEMANTICS\n"); failures++; }
    if ((long long)(FILE_FLAG_RANDOM_ACCESS) != 268435456LL) { printf("FAIL constant FILE_FLAG_RANDOM_ACCESS\n"); failures++; }
    if ((long long)(FILE_FLAG_SEQUENTIAL_SCAN) != 134217728LL) { printf("FAIL constant FILE_FLAG_SEQUENTIAL_SCAN\n"); failures++; }
    if ((long long)(FILE_FLAG_SESSION_AWARE) != 8388608LL) { printf("FAIL constant FILE_FLAG_SESSION_AWARE\n"); failures++; }
    if ((long long)(FILE_FLAG_WRITE_THROUGH) != 2147483648LL) { printf("FAIL constant FILE_FLAG_WRITE_THROUGH\n"); failures++; }
    if ((long long)(FILE_MAP_ALL_ACCESS) != 983071LL) { printf("FAIL constant FILE_MAP_ALL_ACCESS\n"); failures++; }
    if ((long long)(FILE_MAP_COPY) != 1LL) { printf("FAIL constant FILE_MAP_COPY\n"); failures++; }
    if ((long long)(FILE_MAP_EXECUTE) != 32LL) { printf("FAIL constant FILE_MAP_EXECUTE\n"); failures++; }
    if ((long long)(FILE_MAP_LARGE_PAGES) != 536870912LL) { printf("FAIL constant FILE_MAP_LARGE_PAGES\n"); failures++; }
    if ((long long)(FILE_MAP_READ) != 4LL) { printf("FAIL constant FILE_MAP_READ\n"); failures++; }
    if ((long long)(FILE_MAP_RESERVE) != 2147483648LL) { printf("FAIL constant FILE_MAP_RESERVE\n"); failures++; }
    if ((long long)(FILE_MAP_TARGETS_INVALID) != 1073741824LL) { printf("FAIL constant FILE_MAP_TARGETS_INVALID\n"); failures++; }
    if ((long long)(FILE_MAP_WRITE) != 2LL) { printf("FAIL constant FILE_MAP_WRITE\n"); failures++; }
    if ((long long)(FILE_SHARE_DELETE) != 4LL) { printf("FAIL constant FILE_SHARE_DELETE\n"); failures++; }
    if ((long long)(FILE_SHARE_READ) != 1LL) { printf("FAIL constant FILE_SHARE_READ\n"); failures++; }
    if ((long long)(FILE_SHARE_VALID_FLAGS) != 7LL) { printf("FAIL constant FILE_SHARE_VALID_FLAGS\n"); failures++; }
    if ((long long)(FILE_SHARE_WRITE) != 2LL) { printf("FAIL constant FILE_SHARE_WRITE\n"); failures++; }
    if ((long long)(FLASHW_ALL) != 3LL) { printf("FAIL constant FLASHW_ALL\n"); failures++; }
    if ((long long)(FLASHW_CAPTION) != 1LL) { printf("FAIL constant FLASHW_CAPTION\n"); failures++; }
    if ((long long)(FLASHW_STOP) != 0LL) { printf("FAIL constant FLASHW_STOP\n"); failures++; }
    if ((long long)(FLASHW_TIMER) != 4LL) { printf("FAIL constant FLASHW_TIMER\n"); failures++; }
    if ((long long)(FLASHW_TIMERNOFG) != 12LL) { printf("FAIL constant FLASHW_TIMERNOFG\n"); failures++; }
    if ((long long)(FLASHW_TRAY) != 2LL) { printf("FAIL constant FLASHW_TRAY\n"); failures++; }
    if ((long long)(FOS_ALLNONSTORAGEITEMS) != 128LL) { printf("FAIL constant FOS_ALLNONSTORAGEITEMS\n"); failures++; }
    if ((long long)(FOS_ALLOWMULTISELECT) != 512LL) { printf("FAIL constant FOS_ALLOWMULTISELECT\n"); failures++; }
    if ((long long)(FOS_CREATEPROMPT) != 8192LL) { printf("FAIL constant FOS_CREATEPROMPT\n"); failures++; }
    if ((long long)(FOS_DEFAULTNOMINIMODE) != 536870912LL) { printf("FAIL constant FOS_DEFAULTNOMINIMODE\n"); failures++; }
    if ((long long)(FOS_DONTADDTORECENT) != 33554432LL) { printf("FAIL constant FOS_DONTADDTORECENT\n"); failures++; }
    if ((long long)(FOS_FILEMUSTEXIST) != 4096LL) { printf("FAIL constant FOS_FILEMUSTEXIST\n"); failures++; }
    if ((long long)(FOS_FORCEFILESYSTEM) != 64LL) { printf("FAIL constant FOS_FORCEFILESYSTEM\n"); failures++; }
    if ((long long)(FOS_FORCEPREVIEWPANEON) != 1073741824LL) { printf("FAIL constant FOS_FORCEPREVIEWPANEON\n"); failures++; }
    if ((long long)(FOS_FORCESHOWHIDDEN) != 268435456LL) { printf("FAIL constant FOS_FORCESHOWHIDDEN\n"); failures++; }
    if ((long long)(FOS_HIDEMRUPLACES) != 131072LL) { printf("FAIL constant FOS_HIDEMRUPLACES\n"); failures++; }
    if ((long long)(FOS_HIDEPINNEDPLACES) != 262144LL) { printf("FAIL constant FOS_HIDEPINNEDPLACES\n"); failures++; }
    if ((long long)(FOS_NOCHANGEDIR) != 8LL) { printf("FAIL constant FOS_NOCHANGEDIR\n"); failures++; }
    if ((long long)(FOS_NODEREFERENCELINKS) != 1048576LL) { printf("FAIL constant FOS_NODEREFERENCELINKS\n"); failures++; }
    if ((long long)(FOS_NOREADONLYRETURN) != 32768LL) { printf("FAIL constant FOS_NOREADONLYRETURN\n"); failures++; }
    if ((long long)(FOS_NOTESTFILECREATE) != 65536LL) { printf("FAIL constant FOS_NOTESTFILECREATE\n"); failures++; }
    if ((long long)(FOS_NOVALIDATE) != 256LL) { printf("FAIL constant FOS_NOVALIDATE\n"); failures++; }
    if ((long long)(FOS_OVERWRITEPROMPT) != 2LL) { printf("FAIL constant FOS_OVERWRITEPROMPT\n"); failures++; }
    if ((long long)(FOS_PATHMUSTEXIST) != 2048LL) { printf("FAIL constant FOS_PATHMUSTEXIST\n"); failures++; }
    if ((long long)(FOS_PICKFOLDERS) != 32LL) { printf("FAIL constant FOS_PICKFOLDERS\n"); failures++; }
    if ((long long)(FOS_SHAREAWARE) != 16384LL) { printf("FAIL constant FOS_SHAREAWARE\n"); failures++; }
    if ((long long)(FOS_STRICTFILETYPES) != 4LL) { printf("FAIL constant FOS_STRICTFILETYPES\n"); failures++; }
    if ((long long)(FOS_SUPPORTSTREAMABLEITEMS) != 2147483648LL) { printf("FAIL constant FOS_SUPPORTSTREAMABLEITEMS\n"); failures++; }
    if ((long long)(GA_PARENT) != 1LL) { printf("FAIL constant GA_PARENT\n"); failures++; }
    if ((long long)(GA_ROOT) != 2LL) { printf("FAIL constant GA_ROOT\n"); failures++; }
    if ((long long)(GA_ROOTOWNER) != 3LL) { printf("FAIL constant GA_ROOTOWNER\n"); failures++; }
    if ((long long)(GCS_COMPATTR) != 16LL) { printf("FAIL constant GCS_COMPATTR\n"); failures++; }
    if ((long long)(GCS_COMPCLAUSE) != 32LL) { printf("FAIL constant GCS_COMPCLAUSE\n"); failures++; }
    if ((long long)(GCS_COMPREADATTR) != 2LL) { printf("FAIL constant GCS_COMPREADATTR\n"); failures++; }
    if ((long long)(GCS_COMPREADCLAUSE) != 4LL) { printf("FAIL constant GCS_COMPREADCLAUSE\n"); failures++; }
    if ((long long)(GCS_COMPREADSTR) != 1LL) { printf("FAIL constant GCS_COMPREADSTR\n"); failures++; }
    if ((long long)(GCS_COMPSTR) != 8LL) { printf("FAIL constant GCS_COMPSTR\n"); failures++; }
    if ((long long)(GCS_CURSORPOS) != 128LL) { printf("FAIL constant GCS_CURSORPOS\n"); failures++; }
    if ((long long)(GCS_DELTASTART) != 256LL) { printf("FAIL constant GCS_DELTASTART\n"); failures++; }
    if ((long long)(GCS_HELPTEXT) != 5LL) { printf("FAIL constant GCS_HELPTEXT\n"); failures++; }
    if ((long long)(GCS_HELPTEXTA) != 1LL) { printf("FAIL constant GCS_HELPTEXTA\n"); failures++; }
    if ((long long)(GCS_HELPTEXTW) != 5LL) { printf("FAIL constant GCS_HELPTEXTW\n"); failures++; }
    if ((long long)(GCS_RESULTCLAUSE) != 4096LL) { printf("FAIL constant GCS_RESULTCLAUSE\n"); failures++; }
    if ((long long)(GCS_RESULTREADCLAUSE) != 1024LL) { printf("FAIL constant GCS_RESULTREADCLAUSE\n"); failures++; }
    if ((long long)(GCS_RESULTREADSTR) != 512LL) { printf("FAIL constant GCS_RESULTREADSTR\n"); failures++; }
    if ((long long)(GCS_RESULTSTR) != 2048LL) { printf("FAIL constant GCS_RESULTSTR\n"); failures++; }
    if ((long long)(GCS_UNICODE) != 4LL) { printf("FAIL constant GCS_UNICODE\n"); failures++; }
    if ((long long)(GCS_VALIDATE) != 6LL) { printf("FAIL constant GCS_VALIDATE\n"); failures++; }
    if ((long long)(GCS_VALIDATEA) != 2LL) { printf("FAIL constant GCS_VALIDATEA\n"); failures++; }
    if ((long long)(GCS_VALIDATEW) != 6LL) { printf("FAIL constant GCS_VALIDATEW\n"); failures++; }
    if ((long long)(GCS_VERB) != 4LL) { printf("FAIL constant GCS_VERB\n"); failures++; }
    if ((long long)(GCS_VERBA) != 0LL) { printf("FAIL constant GCS_VERBA\n"); failures++; }
    if ((long long)(GCS_VERBICONW) != 20LL) { printf("FAIL constant GCS_VERBICONW\n"); failures++; }
    if ((long long)(GCS_VERBW) != 4LL) { printf("FAIL constant GCS_VERBW\n"); failures++; }
    if ((long long)(GC_ALLGESTURES) != 1LL) { printf("FAIL constant GC_ALLGESTURES\n"); failures++; }
    if ((long long)(GC_PAN) != 1LL) { printf("FAIL constant GC_PAN\n"); failures++; }
    if ((long long)(GC_PAN_WITH_GUTTER) != 8LL) { printf("FAIL constant GC_PAN_WITH_GUTTER\n"); failures++; }
    if ((long long)(GC_PAN_WITH_INERTIA) != 16LL) { printf("FAIL constant GC_PAN_WITH_INERTIA\n"); failures++; }
    if ((long long)(GC_PAN_WITH_SINGLE_FINGER_HORIZONTALLY) != 4LL) { printf("FAIL constant GC_PAN_WITH_SINGLE_FINGER_HORIZONTALLY\n"); failures++; }
    if ((long long)(GC_PAN_WITH_SINGLE_FINGER_VERTICALLY) != 2LL) { printf("FAIL constant GC_PAN_WITH_SINGLE_FINGER_VERTICALLY\n"); failures++; }
    if ((long long)(GC_PRESSANDTAP) != 1LL) { printf("FAIL constant GC_PRESSANDTAP\n"); failures++; }
    if ((long long)(GC_ROLLOVER) != 1LL) { printf("FAIL constant GC_ROLLOVER\n"); failures++; }
    if ((long long)(GC_ROTATE) != 1LL) { printf("FAIL constant GC_ROTATE\n"); failures++; }
    if ((long long)(GC_TWOFINGERTAP) != 1LL) { printf("FAIL constant GC_TWOFINGERTAP\n"); failures++; }
    if ((long long)(GC_ZOOM) != 1LL) { printf("FAIL constant GC_ZOOM\n"); failures++; }
    if ((long long)(GENERIC_ALL) != 268435456LL) { printf("FAIL constant GENERIC_ALL\n"); failures++; }
    if ((long long)(GENERIC_EXECUTE) != 536870912LL) { printf("FAIL constant GENERIC_EXECUTE\n"); failures++; }
    if ((long long)(GENERIC_READ) != 2147483648LL) { printf("FAIL constant GENERIC_READ\n"); failures++; }
    if ((long long)(GENERIC_WRITE) != 1073741824LL) { printf("FAIL constant GENERIC_WRITE\n"); failures++; }
    if ((long long)(GF_BEGIN) != 1LL) { printf("FAIL constant GF_BEGIN\n"); failures++; }
    if ((long long)(GF_END) != 4LL) { printf("FAIL constant GF_END\n"); failures++; }
    if ((long long)(GF_INERTIA) != 2LL) { printf("FAIL constant GF_INERTIA\n"); failures++; }
    if ((long long)(GID_BEGIN) != 1LL) { printf("FAIL constant GID_BEGIN\n"); failures++; }
    if ((long long)(GID_END) != 2LL) { printf("FAIL constant GID_END\n"); failures++; }
    if ((long long)(GID_PAN) != 4LL) { printf("FAIL constant GID_PAN\n"); failures++; }
    if ((long long)(GID_PRESSANDTAP) != 7LL) { printf("FAIL constant GID_PRESSANDTAP\n"); failures++; }
    if ((long long)(GID_ROLLOVER) != 7LL) { printf("FAIL constant GID_ROLLOVER\n"); failures++; }
    if ((long long)(GID_ROTATE) != 5LL) { printf("FAIL constant GID_ROTATE\n"); failures++; }
    if ((long long)(GID_TWOFINGERTAP) != 6LL) { printf("FAIL constant GID_TWOFINGERTAP\n"); failures++; }
    if ((long long)(GID_ZOOM) != 3LL) { printf("FAIL constant GID_ZOOM\n"); failures++; }
    if ((long long)(GMEM_DDESHARE) != 8192LL) { printf("FAIL constant GMEM_DDESHARE\n"); failures++; }
    if ((long long)(GMEM_DISCARDABLE) != 256LL) { printf("FAIL constant GMEM_DISCARDABLE\n"); failures++; }
    if ((long long)(GMEM_DISCARDED) != 16384LL) { printf("FAIL constant GMEM_DISCARDED\n"); failures++; }
    if ((long long)(GMEM_FIXED) != 0LL) { printf("FAIL constant GMEM_FIXED\n"); failures++; }
    if ((long long)(GMEM_INVALID_HANDLE) != 32768LL) { printf("FAIL constant GMEM_INVALID_HANDLE\n"); failures++; }
    if ((long long)(GMEM_LOCKCOUNT) != 255LL) { printf("FAIL constant GMEM_LOCKCOUNT\n"); failures++; }
    if ((long long)(GMEM_LOWER) != 4096LL) { printf("FAIL constant GMEM_LOWER\n"); failures++; }
    if ((long long)(GMEM_MODIFY) != 128LL) { printf("FAIL constant GMEM_MODIFY\n"); failures++; }
    if ((long long)(GMEM_MOVEABLE) != 2LL) { printf("FAIL constant GMEM_MOVEABLE\n"); failures++; }
    if ((long long)(GMEM_NOCOMPACT) != 16LL) { printf("FAIL constant GMEM_NOCOMPACT\n"); failures++; }
    if ((long long)(GMEM_NODISCARD) != 32LL) { printf("FAIL constant GMEM_NODISCARD\n"); failures++; }
    if ((long long)(GMEM_NOTIFY) != 16384LL) { printf("FAIL constant GMEM_NOTIFY\n"); failures++; }
    if ((long long)(GMEM_NOT_BANKED) != 4096LL) { printf("FAIL constant GMEM_NOT_BANKED\n"); failures++; }
    if ((long long)(GMEM_SHARE) != 8192LL) { printf("FAIL constant GMEM_SHARE\n"); failures++; }
    if ((long long)(GMEM_VALID_FLAGS) != 32626LL) { printf("FAIL constant GMEM_VALID_FLAGS\n"); failures++; }
    if ((long long)(GMEM_ZEROINIT) != 64LL) { printf("FAIL constant GMEM_ZEROINIT\n"); failures++; }
    if ((long long)(GWLP_HINSTANCE) != (-6LL)) { printf("FAIL constant GWLP_HINSTANCE\n"); failures++; }
    if ((long long)(GWLP_HWNDPARENT) != (-8LL)) { printf("FAIL constant GWLP_HWNDPARENT\n"); failures++; }
    if ((long long)(GWLP_ID) != (-12LL)) { printf("FAIL constant GWLP_ID\n"); failures++; }
    if ((long long)(GWLP_USERDATA) != (-21LL)) { printf("FAIL constant GWLP_USERDATA\n"); failures++; }
    if ((long long)(GWLP_WNDPROC) != (-4LL)) { printf("FAIL constant GWLP_WNDPROC\n"); failures++; }
    if ((long long)(GWL_EXSTYLE) != (-20LL)) { printf("FAIL constant GWL_EXSTYLE\n"); failures++; }
    if ((long long)(GWL_ID) != (-12LL)) { printf("FAIL constant GWL_ID\n"); failures++; }
    if ((long long)(GWL_STYLE) != (-16LL)) { printf("FAIL constant GWL_STYLE\n"); failures++; }
    if ((long long)(GW_CHILD) != 5LL) { printf("FAIL constant GW_CHILD\n"); failures++; }
    if ((long long)(GW_ENABLEDPOPUP) != 6LL) { printf("FAIL constant GW_ENABLEDPOPUP\n"); failures++; }
    if ((long long)(GW_HWNDFIRST) != 0LL) { printf("FAIL constant GW_HWNDFIRST\n"); failures++; }
    if ((long long)(GW_HWNDLAST) != 1LL) { printf("FAIL constant GW_HWNDLAST\n"); failures++; }
    if ((long long)(GW_HWNDNEXT) != 2LL) { printf("FAIL constant GW_HWNDNEXT\n"); failures++; }
    if ((long long)(GW_HWNDPREV) != 3LL) { printf("FAIL constant GW_HWNDPREV\n"); failures++; }
    if ((long long)(GW_MAX) != 6LL) { printf("FAIL constant GW_MAX\n"); failures++; }
    if ((long long)(GW_OWNER) != 4LL) { printf("FAIL constant GW_OWNER\n"); failures++; }
    if ((long long)(HCF_AVAILABLE) != 2LL) { printf("FAIL constant HCF_AVAILABLE\n"); failures++; }
    if ((long long)(HCF_CONFIRMHOTKEY) != 8LL) { printf("FAIL constant HCF_CONFIRMHOTKEY\n"); failures++; }
    if ((long long)(HCF_DEFAULTDESKTOP) != 512LL) { printf("FAIL constant HCF_DEFAULTDESKTOP\n"); failures++; }
    if ((long long)(HCF_HIGHCONTRASTON) != 1LL) { printf("FAIL constant HCF_HIGHCONTRASTON\n"); failures++; }
    if ((long long)(HCF_HOTKEYACTIVE) != 4LL) { printf("FAIL constant HCF_HOTKEYACTIVE\n"); failures++; }
    if ((long long)(HCF_HOTKEYAVAILABLE) != 64LL) { printf("FAIL constant HCF_HOTKEYAVAILABLE\n"); failures++; }
    if ((long long)(HCF_HOTKEYSOUND) != 16LL) { printf("FAIL constant HCF_HOTKEYSOUND\n"); failures++; }
    if ((long long)(HCF_INDICATOR) != 32LL) { printf("FAIL constant HCF_INDICATOR\n"); failures++; }
    if ((long long)(HCF_LOGONDESKTOP) != 256LL) { printf("FAIL constant HCF_LOGONDESKTOP\n"); failures++; }
    if ((long long)(HCF_OPTION_NOTHEMECHANGE) != 4096LL) { printf("FAIL constant HCF_OPTION_NOTHEMECHANGE\n"); failures++; }
    if ((long long)(HTBORDER) != 18LL) { printf("FAIL constant HTBORDER\n"); failures++; }
    if ((long long)(HTBOTTOM) != 15LL) { printf("FAIL constant HTBOTTOM\n"); failures++; }
    if ((long long)(HTBOTTOMLEFT) != 16LL) { printf("FAIL constant HTBOTTOMLEFT\n"); failures++; }
    if ((long long)(HTBOTTOMRIGHT) != 17LL) { printf("FAIL constant HTBOTTOMRIGHT\n"); failures++; }
    if ((long long)(HTCAPTION) != 2LL) { printf("FAIL constant HTCAPTION\n"); failures++; }
    if ((long long)(HTCLIENT) != 1LL) { printf("FAIL constant HTCLIENT\n"); failures++; }
    if ((long long)(HTCLOSE) != 20LL) { printf("FAIL constant HTCLOSE\n"); failures++; }
    if ((long long)(HTERROR) != (-2LL)) { printf("FAIL constant HTERROR\n"); failures++; }
    if ((long long)(HTGROWBOX) != 4LL) { printf("FAIL constant HTGROWBOX\n"); failures++; }
    if ((long long)(HTHELP) != 21LL) { printf("FAIL constant HTHELP\n"); failures++; }
    if ((long long)(HTHSCROLL) != 6LL) { printf("FAIL constant HTHSCROLL\n"); failures++; }
    if ((long long)(HTLEFT) != 10LL) { printf("FAIL constant HTLEFT\n"); failures++; }
    if ((long long)(HTMAXBUTTON) != 9LL) { printf("FAIL constant HTMAXBUTTON\n"); failures++; }
    if ((long long)(HTMENU) != 5LL) { printf("FAIL constant HTMENU\n"); failures++; }
    if ((long long)(HTMINBUTTON) != 8LL) { printf("FAIL constant HTMINBUTTON\n"); failures++; }
    if ((long long)(HTNOWHERE) != 0LL) { printf("FAIL constant HTNOWHERE\n"); failures++; }
    if ((long long)(HTOBJECT) != 19LL) { printf("FAIL constant HTOBJECT\n"); failures++; }
    if ((long long)(HTREDUCE) != 8LL) { printf("FAIL constant HTREDUCE\n"); failures++; }
    if ((long long)(HTRIGHT) != 11LL) { printf("FAIL constant HTRIGHT\n"); failures++; }
    if ((long long)(HTSIZE) != 4LL) { printf("FAIL constant HTSIZE\n"); failures++; }
    if ((long long)(HTSIZEFIRST) != 10LL) { printf("FAIL constant HTSIZEFIRST\n"); failures++; }
    if ((long long)(HTSIZELAST) != 17LL) { printf("FAIL constant HTSIZELAST\n"); failures++; }
    if ((long long)(HTSYSMENU) != 3LL) { printf("FAIL constant HTSYSMENU\n"); failures++; }
    if ((long long)(HTTB_BACKGROUNDSEG) != 0LL) { printf("FAIL constant HTTB_BACKGROUNDSEG\n"); failures++; }
    if ((long long)(HTTB_CAPTION) != 4LL) { printf("FAIL constant HTTB_CAPTION\n"); failures++; }
    if ((long long)(HTTB_FIXEDBORDER) != 2LL) { printf("FAIL constant HTTB_FIXEDBORDER\n"); failures++; }
    if ((long long)(HTTB_RESIZINGBORDER) != 240LL) { printf("FAIL constant HTTB_RESIZINGBORDER\n"); failures++; }
    if ((long long)(HTTB_RESIZINGBORDER_BOTTOM) != 128LL) { printf("FAIL constant HTTB_RESIZINGBORDER_BOTTOM\n"); failures++; }
    if ((long long)(HTTB_RESIZINGBORDER_LEFT) != 16LL) { printf("FAIL constant HTTB_RESIZINGBORDER_LEFT\n"); failures++; }
    if ((long long)(HTTB_RESIZINGBORDER_RIGHT) != 64LL) { printf("FAIL constant HTTB_RESIZINGBORDER_RIGHT\n"); failures++; }
    if ((long long)(HTTB_RESIZINGBORDER_TOP) != 32LL) { printf("FAIL constant HTTB_RESIZINGBORDER_TOP\n"); failures++; }
    if ((long long)(HTTB_SIZINGTEMPLATE) != 256LL) { printf("FAIL constant HTTB_SIZINGTEMPLATE\n"); failures++; }
    if ((long long)(HTTB_SYSTEMSIZINGMARGINS) != 512LL) { printf("FAIL constant HTTB_SYSTEMSIZINGMARGINS\n"); failures++; }
    if ((long long)(HTTOP) != 12LL) { printf("FAIL constant HTTOP\n"); failures++; }
    if ((long long)(HTTOPLEFT) != 13LL) { printf("FAIL constant HTTOPLEFT\n"); failures++; }
    if ((long long)(HTTOPRIGHT) != 14LL) { printf("FAIL constant HTTOPRIGHT\n"); failures++; }
    if ((long long)(HTTRANSPARENT) != (-1LL)) { printf("FAIL constant HTTRANSPARENT\n"); failures++; }
    if ((long long)(HTVSCROLL) != 7LL) { printf("FAIL constant HTVSCROLL\n"); failures++; }
    if ((long long)(HTZOOM) != 9LL) { printf("FAIL constant HTZOOM\n"); failures++; }
    if ((long long)(IACE_CHILDREN) != 1LL) { printf("FAIL constant IACE_CHILDREN\n"); failures++; }
    if ((long long)(IACE_DEFAULT) != 16LL) { printf("FAIL constant IACE_DEFAULT\n"); failures++; }
    if ((long long)(IACE_IGNORENOCONTEXT) != 32LL) { printf("FAIL constant IACE_IGNORENOCONTEXT\n"); failures++; }
    if ((long long)(ICON_BIG) != 1LL) { printf("FAIL constant ICON_BIG\n"); failures++; }
    if ((long long)(ICON_SMALL) != 0LL) { printf("FAIL constant ICON_SMALL\n"); failures++; }
    if ((long long)(ICON_SMALL2) != 2LL) { printf("FAIL constant ICON_SMALL2\n"); failures++; }
    if ((long long)(IMN_CHANGECANDIDATE) != 3LL) { printf("FAIL constant IMN_CHANGECANDIDATE\n"); failures++; }
    if ((long long)(IMN_CLOSECANDIDATE) != 4LL) { printf("FAIL constant IMN_CLOSECANDIDATE\n"); failures++; }
    if ((long long)(IMN_CLOSESTATUSWINDOW) != 1LL) { printf("FAIL constant IMN_CLOSESTATUSWINDOW\n"); failures++; }
    if ((long long)(IMN_GUIDELINE) != 13LL) { printf("FAIL constant IMN_GUIDELINE\n"); failures++; }
    if ((long long)(IMN_OPENCANDIDATE) != 5LL) { printf("FAIL constant IMN_OPENCANDIDATE\n"); failures++; }
    if ((long long)(IMN_OPENSTATUSWINDOW) != 2LL) { printf("FAIL constant IMN_OPENSTATUSWINDOW\n"); failures++; }
    if ((long long)(IMN_PRIVATE) != 14LL) { printf("FAIL constant IMN_PRIVATE\n"); failures++; }
    if ((long long)(IMN_SETCANDIDATEPOS) != 9LL) { printf("FAIL constant IMN_SETCANDIDATEPOS\n"); failures++; }
    if ((long long)(IMN_SETCOMPOSITIONFONT) != 10LL) { printf("FAIL constant IMN_SETCOMPOSITIONFONT\n"); failures++; }
    if ((long long)(IMN_SETCOMPOSITIONWINDOW) != 11LL) { printf("FAIL constant IMN_SETCOMPOSITIONWINDOW\n"); failures++; }
    if ((long long)(IMN_SETCONVERSIONMODE) != 6LL) { printf("FAIL constant IMN_SETCONVERSIONMODE\n"); failures++; }
    if ((long long)(IMN_SETOPENSTATUS) != 8LL) { printf("FAIL constant IMN_SETOPENSTATUS\n"); failures++; }
    if ((long long)(IMN_SETSENTENCEMODE) != 7LL) { printf("FAIL constant IMN_SETSENTENCEMODE\n"); failures++; }
    if ((long long)(IMN_SETSTATUSWINDOWPOS) != 12LL) { printf("FAIL constant IMN_SETSTATUSWINDOWPOS\n"); failures++; }
    if ((long long)(INPUT_HARDWARE) != 2LL) { printf("FAIL constant INPUT_HARDWARE\n"); failures++; }
    if ((long long)(INPUT_KEYBOARD) != 1LL) { printf("FAIL constant INPUT_KEYBOARD\n"); failures++; }
    if ((long long)(INPUT_MOUSE) != 0LL) { printf("FAIL constant INPUT_MOUSE\n"); failures++; }
    if ((long long)(ISC_SHOWUIALL) != 3221225487LL) { printf("FAIL constant ISC_SHOWUIALL\n"); failures++; }
    if ((long long)(ISC_SHOWUIALLCANDIDATEWINDOW) != 15LL) { printf("FAIL constant ISC_SHOWUIALLCANDIDATEWINDOW\n"); failures++; }
    if ((long long)(ISC_SHOWUICANDIDATEWINDOW) != 1LL) { printf("FAIL constant ISC_SHOWUICANDIDATEWINDOW\n"); failures++; }
    if ((long long)(ISC_SHOWUICOMPOSITIONWINDOW) != 2147483648LL) { printf("FAIL constant ISC_SHOWUICOMPOSITIONWINDOW\n"); failures++; }
    if ((long long)(ISC_SHOWUIGUIDELINE) != 1073741824LL) { printf("FAIL constant ISC_SHOWUIGUIDELINE\n"); failures++; }
    if ((long long)(KEYEVENTF_EXTENDEDKEY) != 1LL) { printf("FAIL constant KEYEVENTF_EXTENDEDKEY\n"); failures++; }
    if ((long long)(KEYEVENTF_KEYUP) != 2LL) { printf("FAIL constant KEYEVENTF_KEYUP\n"); failures++; }
    if ((long long)(KEYEVENTF_SCANCODE) != 8LL) { printf("FAIL constant KEYEVENTF_SCANCODE\n"); failures++; }
    if ((long long)(KEYEVENTF_UNICODE) != 4LL) { printf("FAIL constant KEYEVENTF_UNICODE\n"); failures++; }
    if ((long long)(KF_ALTDOWN) != 8192LL) { printf("FAIL constant KF_ALTDOWN\n"); failures++; }
    if ((long long)(KF_CATEGORY_COMMON) != 3LL) { printf("FAIL constant KF_CATEGORY_COMMON\n"); failures++; }
    if ((long long)(KF_CATEGORY_FIXED) != 2LL) { printf("FAIL constant KF_CATEGORY_FIXED\n"); failures++; }
    if ((long long)(KF_CATEGORY_PERUSER) != 4LL) { printf("FAIL constant KF_CATEGORY_PERUSER\n"); failures++; }
    if ((long long)(KF_CATEGORY_VIRTUAL) != 1LL) { printf("FAIL constant KF_CATEGORY_VIRTUAL\n"); failures++; }
    if ((long long)(KF_DLGMODE) != 2048LL) { printf("FAIL constant KF_DLGMODE\n"); failures++; }
    if ((long long)(KF_EXTENDED) != 256LL) { printf("FAIL constant KF_EXTENDED\n"); failures++; }
    if ((long long)(KF_FLAG_ALIAS_ONLY) != 2147483648LL) { printf("FAIL constant KF_FLAG_ALIAS_ONLY\n"); failures++; }
    if ((long long)(KF_FLAG_CREATE) != 32768LL) { printf("FAIL constant KF_FLAG_CREATE\n"); failures++; }
    if ((long long)(KF_FLAG_DEFAULT) != 0LL) { printf("FAIL constant KF_FLAG_DEFAULT\n"); failures++; }
    if ((long long)(KF_FLAG_DEFAULT_PATH) != 1024LL) { printf("FAIL constant KF_FLAG_DEFAULT_PATH\n"); failures++; }
    if ((long long)(KF_FLAG_DONT_UNEXPAND) != 8192LL) { printf("FAIL constant KF_FLAG_DONT_UNEXPAND\n"); failures++; }
    if ((long long)(KF_FLAG_DONT_VERIFY) != 16384LL) { printf("FAIL constant KF_FLAG_DONT_VERIFY\n"); failures++; }
    if ((long long)(KF_FLAG_FORCE_APPCONTAINER_REDIRECTION) != 131072LL) { printf("FAIL constant KF_FLAG_FORCE_APPCONTAINER_REDIRECTION\n"); failures++; }
    if ((long long)(KF_FLAG_FORCE_APP_DATA_REDIRECTION) != 524288LL) { printf("FAIL constant KF_FLAG_FORCE_APP_DATA_REDIRECTION\n"); failures++; }
    if ((long long)(KF_FLAG_FORCE_PACKAGE_REDIRECTION) != 131072LL) { printf("FAIL constant KF_FLAG_FORCE_PACKAGE_REDIRECTION\n"); failures++; }
    if ((long long)(KF_FLAG_INIT) != 2048LL) { printf("FAIL constant KF_FLAG_INIT\n"); failures++; }
    if ((long long)(KF_FLAG_NOT_PARENT_RELATIVE) != 512LL) { printf("FAIL constant KF_FLAG_NOT_PARENT_RELATIVE\n"); failures++; }
    if ((long long)(KF_FLAG_NO_ALIAS) != 4096LL) { printf("FAIL constant KF_FLAG_NO_ALIAS\n"); failures++; }
    if ((long long)(KF_FLAG_NO_APPCONTAINER_REDIRECTION) != 65536LL) { printf("FAIL constant KF_FLAG_NO_APPCONTAINER_REDIRECTION\n"); failures++; }
    if ((long long)(KF_FLAG_NO_PACKAGE_REDIRECTION) != 65536LL) { printf("FAIL constant KF_FLAG_NO_PACKAGE_REDIRECTION\n"); failures++; }
    if ((long long)(KF_FLAG_RETURN_FILTER_REDIRECTION_TARGET) != 262144LL) { printf("FAIL constant KF_FLAG_RETURN_FILTER_REDIRECTION_TARGET\n"); failures++; }
    if ((long long)(KF_FLAG_SIMPLE_IDLIST) != 256LL) { printf("FAIL constant KF_FLAG_SIMPLE_IDLIST\n"); failures++; }
    if ((long long)(KF_MENUMODE) != 4096LL) { printf("FAIL constant KF_MENUMODE\n"); failures++; }
    if ((long long)(KF_REDIRECTION_CAPABILITIES_ALLOW_ALL) != 255LL) { printf("FAIL constant KF_REDIRECTION_CAPABILITIES_ALLOW_ALL\n"); failures++; }
    if ((long long)(KF_REDIRECTION_CAPABILITIES_DENY_ALL) != 1048320LL) { printf("FAIL constant KF_REDIRECTION_CAPABILITIES_DENY_ALL\n"); failures++; }
    if ((long long)(KF_REDIRECTION_CAPABILITIES_DENY_PERMISSIONS) != 1024LL) { printf("FAIL constant KF_REDIRECTION_CAPABILITIES_DENY_PERMISSIONS\n"); failures++; }
    if ((long long)(KF_REDIRECTION_CAPABILITIES_DENY_POLICY) != 512LL) { printf("FAIL constant KF_REDIRECTION_CAPABILITIES_DENY_POLICY\n"); failures++; }
    if ((long long)(KF_REDIRECTION_CAPABILITIES_DENY_POLICY_REDIRECTED) != 256LL) { printf("FAIL constant KF_REDIRECTION_CAPABILITIES_DENY_POLICY_REDIRECTED\n"); failures++; }
    if ((long long)(KF_REDIRECTION_CAPABILITIES_REDIRECTABLE) != 1LL) { printf("FAIL constant KF_REDIRECTION_CAPABILITIES_REDIRECTABLE\n"); failures++; }
    if ((long long)(KF_REDIRECT_CHECK_ONLY) != 16LL) { printf("FAIL constant KF_REDIRECT_CHECK_ONLY\n"); failures++; }
    if ((long long)(KF_REDIRECT_COPY_CONTENTS) != 512LL) { printf("FAIL constant KF_REDIRECT_COPY_CONTENTS\n"); failures++; }
    if ((long long)(KF_REDIRECT_COPY_SOURCE_DACL) != 2LL) { printf("FAIL constant KF_REDIRECT_COPY_SOURCE_DACL\n"); failures++; }
    if ((long long)(KF_REDIRECT_DEL_SOURCE_CONTENTS) != 1024LL) { printf("FAIL constant KF_REDIRECT_DEL_SOURCE_CONTENTS\n"); failures++; }
    if ((long long)(KF_REDIRECT_EXCLUDE_ALL_KNOWN_SUBFOLDERS) != 2048LL) { printf("FAIL constant KF_REDIRECT_EXCLUDE_ALL_KNOWN_SUBFOLDERS\n"); failures++; }
    if ((long long)(KF_REDIRECT_OWNER_USER) != 4LL) { printf("FAIL constant KF_REDIRECT_OWNER_USER\n"); failures++; }
    if ((long long)(KF_REDIRECT_PIN) != 128LL) { printf("FAIL constant KF_REDIRECT_PIN\n"); failures++; }
    if ((long long)(KF_REDIRECT_SET_OWNER_EXPLICIT) != 8LL) { printf("FAIL constant KF_REDIRECT_SET_OWNER_EXPLICIT\n"); failures++; }
    if ((long long)(KF_REDIRECT_UNPIN) != 64LL) { printf("FAIL constant KF_REDIRECT_UNPIN\n"); failures++; }
    if ((long long)(KF_REDIRECT_USER_EXCLUSIVE) != 1LL) { printf("FAIL constant KF_REDIRECT_USER_EXCLUSIVE\n"); failures++; }
    if ((long long)(KF_REDIRECT_WITH_UI) != 32LL) { printf("FAIL constant KF_REDIRECT_WITH_UI\n"); failures++; }
    if ((long long)(KF_REPEAT) != 16384LL) { printf("FAIL constant KF_REPEAT\n"); failures++; }
    if ((long long)(KF_UP) != 32768LL) { printf("FAIL constant KF_UP\n"); failures++; }
    if ((long long)(LR_COLOR) != 2LL) { printf("FAIL constant LR_COLOR\n"); failures++; }
    if ((long long)(LR_COPYDELETEORG) != 8LL) { printf("FAIL constant LR_COPYDELETEORG\n"); failures++; }
    if ((long long)(LR_COPYFROMRESOURCE) != 16384LL) { printf("FAIL constant LR_COPYFROMRESOURCE\n"); failures++; }
    if ((long long)(LR_COPYRETURNORG) != 4LL) { printf("FAIL constant LR_COPYRETURNORG\n"); failures++; }
    if ((long long)(LR_CREATEDIBSECTION) != 8192LL) { printf("FAIL constant LR_CREATEDIBSECTION\n"); failures++; }
    if ((long long)(LR_DEFAULTCOLOR) != 0LL) { printf("FAIL constant LR_DEFAULTCOLOR\n"); failures++; }
    if ((long long)(LR_DEFAULTSIZE) != 64LL) { printf("FAIL constant LR_DEFAULTSIZE\n"); failures++; }
    if ((long long)(LR_LOADFROMFILE) != 16LL) { printf("FAIL constant LR_LOADFROMFILE\n"); failures++; }
    if ((long long)(LR_LOADMAP3DCOLORS) != 4096LL) { printf("FAIL constant LR_LOADMAP3DCOLORS\n"); failures++; }
    if ((long long)(LR_LOADTRANSPARENT) != 32LL) { printf("FAIL constant LR_LOADTRANSPARENT\n"); failures++; }
    if ((long long)(LR_MONOCHROME) != 1LL) { printf("FAIL constant LR_MONOCHROME\n"); failures++; }
    if ((long long)(LR_SHARED) != 32768LL) { printf("FAIL constant LR_SHARED\n"); failures++; }
    if ((long long)(LR_VGACOLOR) != 128LL) { printf("FAIL constant LR_VGACOLOR\n"); failures++; }
    if ((long long)(LWA_ALPHA) != 2LL) { printf("FAIL constant LWA_ALPHA\n"); failures++; }
    if ((long long)(LWA_COLORKEY) != 1LL) { printf("FAIL constant LWA_COLORKEY\n"); failures++; }
    if ((long long)(MAPVK_VK_TO_CHAR) != 2LL) { printf("FAIL constant MAPVK_VK_TO_CHAR\n"); failures++; }
    if ((long long)(MAPVK_VK_TO_VSC) != 0LL) { printf("FAIL constant MAPVK_VK_TO_VSC\n"); failures++; }
    if ((long long)(MAPVK_VK_TO_VSC_EX) != 4LL) { printf("FAIL constant MAPVK_VK_TO_VSC_EX\n"); failures++; }
    if ((long long)(MAPVK_VSC_TO_VK) != 1LL) { printf("FAIL constant MAPVK_VSC_TO_VK\n"); failures++; }
    if ((long long)(MAPVK_VSC_TO_VK_EX) != 3LL) { printf("FAIL constant MAPVK_VSC_TO_VK_EX\n"); failures++; }
    if ((long long)(MA_ACTIVATE) != 1LL) { printf("FAIL constant MA_ACTIVATE\n"); failures++; }
    if ((long long)(MA_ACTIVATEANDEAT) != 2LL) { printf("FAIL constant MA_ACTIVATEANDEAT\n"); failures++; }
    if ((long long)(MA_NOACTIVATE) != 3LL) { printf("FAIL constant MA_NOACTIVATE\n"); failures++; }
    if ((long long)(MA_NOACTIVATEANDEAT) != 4LL) { printf("FAIL constant MA_NOACTIVATEANDEAT\n"); failures++; }
    if ((long long)(MDT_ANGULAR_DPI) != 1LL) { printf("FAIL constant MDT_ANGULAR_DPI\n"); failures++; }
    if ((long long)(MDT_DEFAULT) != 0LL) { printf("FAIL constant MDT_DEFAULT\n"); failures++; }
    if ((long long)(MDT_EFFECTIVE_DPI) != 0LL) { printf("FAIL constant MDT_EFFECTIVE_DPI\n"); failures++; }
    if ((long long)(MDT_RAW_DPI) != 2LL) { printf("FAIL constant MDT_RAW_DPI\n"); failures++; }
    if ((long long)(MEM_4MB_PAGES) != 2147483648LL) { printf("FAIL constant MEM_4MB_PAGES\n"); failures++; }
    if ((long long)(MEM_64K_PAGES) != 541065216LL) { printf("FAIL constant MEM_64K_PAGES\n"); failures++; }
    if ((long long)(MEM_COALESCE_PLACEHOLDERS) != 1LL) { printf("FAIL constant MEM_COALESCE_PLACEHOLDERS\n"); failures++; }
    if ((long long)(MEM_COMMIT) != 4096LL) { printf("FAIL constant MEM_COMMIT\n"); failures++; }
    if ((long long)(MEM_DECOMMIT) != 16384LL) { printf("FAIL constant MEM_DECOMMIT\n"); failures++; }
    if ((long long)(MEM_DEDICATED_ATTRIBUTE_NOT_SPECIFIED) != (-1LL)) { printf("FAIL constant MEM_DEDICATED_ATTRIBUTE_NOT_SPECIFIED\n"); failures++; }
    if ((long long)(MEM_DIFFERENT_IMAGE_BASE_OK) != 8388608LL) { printf("FAIL constant MEM_DIFFERENT_IMAGE_BASE_OK\n"); failures++; }
    if ((long long)(MEM_EXTENDED_PARAMETER_EC_CODE) != 64LL) { printf("FAIL constant MEM_EXTENDED_PARAMETER_EC_CODE\n"); failures++; }
    if ((long long)(MEM_EXTENDED_PARAMETER_GRAPHICS) != 1LL) { printf("FAIL constant MEM_EXTENDED_PARAMETER_GRAPHICS\n"); failures++; }
    if ((long long)(MEM_EXTENDED_PARAMETER_IMAGE_NO_HPAT) != 128LL) { printf("FAIL constant MEM_EXTENDED_PARAMETER_IMAGE_NO_HPAT\n"); failures++; }
    if ((long long)(MEM_EXTENDED_PARAMETER_NONPAGED) != 2LL) { printf("FAIL constant MEM_EXTENDED_PARAMETER_NONPAGED\n"); failures++; }
    if ((long long)(MEM_EXTENDED_PARAMETER_NONPAGED_HUGE) != 16LL) { printf("FAIL constant MEM_EXTENDED_PARAMETER_NONPAGED_HUGE\n"); failures++; }
    if ((long long)(MEM_EXTENDED_PARAMETER_NONPAGED_LARGE) != 8LL) { printf("FAIL constant MEM_EXTENDED_PARAMETER_NONPAGED_LARGE\n"); failures++; }
    if ((long long)(MEM_EXTENDED_PARAMETER_NUMA_NODE_MANDATORY) != (-9223372036854775807LL - 1)) { printf("FAIL constant MEM_EXTENDED_PARAMETER_NUMA_NODE_MANDATORY\n"); failures++; }
    if ((long long)(MEM_EXTENDED_PARAMETER_SOFT_FAULT_PAGES) != 32LL) { printf("FAIL constant MEM_EXTENDED_PARAMETER_SOFT_FAULT_PAGES\n"); failures++; }
    if ((long long)(MEM_EXTENDED_PARAMETER_TYPE_BITS) != 8LL) { printf("FAIL constant MEM_EXTENDED_PARAMETER_TYPE_BITS\n"); failures++; }
    if ((long long)(MEM_EXTENDED_PARAMETER_ZERO_PAGES_OPTIONAL) != 4LL) { printf("FAIL constant MEM_EXTENDED_PARAMETER_ZERO_PAGES_OPTIONAL\n"); failures++; }
    if ((long long)(MEM_E_INVALID_LINK) != (-2146959344LL)) { printf("FAIL constant MEM_E_INVALID_LINK\n"); failures++; }
    if ((long long)(MEM_E_INVALID_ROOT) != (-2146959351LL)) { printf("FAIL constant MEM_E_INVALID_ROOT\n"); failures++; }
    if ((long long)(MEM_E_INVALID_SIZE) != (-2146959343LL)) { printf("FAIL constant MEM_E_INVALID_SIZE\n"); failures++; }
    if ((long long)(MEM_FREE) != 65536LL) { printf("FAIL constant MEM_FREE\n"); failures++; }
    if ((long long)(MEM_IMAGE) != 16777216LL) { printf("FAIL constant MEM_IMAGE\n"); failures++; }
    if ((long long)(MEM_LARGE_PAGES) != 536870912LL) { printf("FAIL constant MEM_LARGE_PAGES\n"); failures++; }
    if ((long long)(MEM_MAPPED) != 262144LL) { printf("FAIL constant MEM_MAPPED\n"); failures++; }
    if ((long long)(MEM_PHYSICAL) != 4194304LL) { printf("FAIL constant MEM_PHYSICAL\n"); failures++; }
    if ((long long)(MEM_PRESERVE_PLACEHOLDER) != 2LL) { printf("FAIL constant MEM_PRESERVE_PLACEHOLDER\n"); failures++; }
    if ((long long)(MEM_PRIVATE) != 131072LL) { printf("FAIL constant MEM_PRIVATE\n"); failures++; }
    if ((long long)(MEM_RELEASE) != 32768LL) { printf("FAIL constant MEM_RELEASE\n"); failures++; }
    if ((long long)(MEM_REPLACE_PLACEHOLDER) != 16384LL) { printf("FAIL constant MEM_REPLACE_PLACEHOLDER\n"); failures++; }
    if ((long long)(MEM_RESERVE) != 8192LL) { printf("FAIL constant MEM_RESERVE\n"); failures++; }
    if ((long long)(MEM_RESERVE_PLACEHOLDER) != 262144LL) { printf("FAIL constant MEM_RESERVE_PLACEHOLDER\n"); failures++; }
    if ((long long)(MEM_RESET) != 524288LL) { printf("FAIL constant MEM_RESET\n"); failures++; }
    if ((long long)(MEM_RESET_UNDO) != 16777216LL) { printf("FAIL constant MEM_RESET_UNDO\n"); failures++; }
    if ((long long)(MEM_ROTATE) != 8388608LL) { printf("FAIL constant MEM_ROTATE\n"); failures++; }
    if ((long long)(MEM_TOP_DOWN) != 1048576LL) { printf("FAIL constant MEM_TOP_DOWN\n"); failures++; }
    if ((long long)(MEM_UNMAP_WITH_TRANSIENT_BOOST) != 1LL) { printf("FAIL constant MEM_UNMAP_WITH_TRANSIENT_BOOST\n"); failures++; }
    if ((long long)(MEM_WRITE_WATCH) != 2097152LL) { printf("FAIL constant MEM_WRITE_WATCH\n"); failures++; }
    if ((long long)(MK_ALT) != 32LL) { printf("FAIL constant MK_ALT\n"); failures++; }
    if ((long long)(MK_CONTROL) != 8LL) { printf("FAIL constant MK_CONTROL\n"); failures++; }
    if ((long long)(MK_E_CANTOPENFILE) != (-2147221014LL)) { printf("FAIL constant MK_E_CANTOPENFILE\n"); failures++; }
    if ((long long)(MK_E_CONNECTMANUALLY) != (-2147221024LL)) { printf("FAIL constant MK_E_CONNECTMANUALLY\n"); failures++; }
    if ((long long)(MK_E_ENUMERATION_FAILED) != (-2147221009LL)) { printf("FAIL constant MK_E_ENUMERATION_FAILED\n"); failures++; }
    if ((long long)(MK_E_EXCEEDEDDEADLINE) != (-2147221023LL)) { printf("FAIL constant MK_E_EXCEEDEDDEADLINE\n"); failures++; }
    if ((long long)(MK_E_FIRST) != 2147746272LL) { printf("FAIL constant MK_E_FIRST\n"); failures++; }
    if ((long long)(MK_E_INTERMEDIATEINTERFACENOTSUPPORTED) != (-2147221017LL)) { printf("FAIL constant MK_E_INTERMEDIATEINTERFACENOTSUPPORTED\n"); failures++; }
    if ((long long)(MK_E_INVALIDEXTENSION) != (-2147221018LL)) { printf("FAIL constant MK_E_INVALIDEXTENSION\n"); failures++; }
    if ((long long)(MK_E_LAST) != 2147746287LL) { printf("FAIL constant MK_E_LAST\n"); failures++; }
    if ((long long)(MK_E_MUSTBOTHERUSER) != (-2147221013LL)) { printf("FAIL constant MK_E_MUSTBOTHERUSER\n"); failures++; }
    if ((long long)(MK_E_NEEDGENERIC) != (-2147221022LL)) { printf("FAIL constant MK_E_NEEDGENERIC\n"); failures++; }
    if ((long long)(MK_E_NOINVERSE) != (-2147221012LL)) { printf("FAIL constant MK_E_NOINVERSE\n"); failures++; }
    if ((long long)(MK_E_NOOBJECT) != (-2147221019LL)) { printf("FAIL constant MK_E_NOOBJECT\n"); failures++; }
    if ((long long)(MK_E_NOPREFIX) != (-2147221010LL)) { printf("FAIL constant MK_E_NOPREFIX\n"); failures++; }
    if ((long long)(MK_E_NOSTORAGE) != (-2147221011LL)) { printf("FAIL constant MK_E_NOSTORAGE\n"); failures++; }
    if ((long long)(MK_E_NOTBINDABLE) != (-2147221016LL)) { printf("FAIL constant MK_E_NOTBINDABLE\n"); failures++; }
    if ((long long)(MK_E_NOTBOUND) != (-2147221015LL)) { printf("FAIL constant MK_E_NOTBOUND\n"); failures++; }
    if ((long long)(MK_E_NO_NORMALIZED) != (-2146959353LL)) { printf("FAIL constant MK_E_NO_NORMALIZED\n"); failures++; }
    if ((long long)(MK_E_SYNTAX) != (-2147221020LL)) { printf("FAIL constant MK_E_SYNTAX\n"); failures++; }
    if ((long long)(MK_E_UNAVAILABLE) != (-2147221021LL)) { printf("FAIL constant MK_E_UNAVAILABLE\n"); failures++; }
    if ((long long)(MK_LBUTTON) != 1LL) { printf("FAIL constant MK_LBUTTON\n"); failures++; }
    if ((long long)(MK_MBUTTON) != 16LL) { printf("FAIL constant MK_MBUTTON\n"); failures++; }
    if ((long long)(MK_RBUTTON) != 2LL) { printf("FAIL constant MK_RBUTTON\n"); failures++; }
    if ((long long)(MK_SHIFT) != 4LL) { printf("FAIL constant MK_SHIFT\n"); failures++; }
    if ((long long)(MK_S_ASYNCHRONOUS) != 262632LL) { printf("FAIL constant MK_S_ASYNCHRONOUS\n"); failures++; }
    if ((long long)(MK_S_FIRST) != 262624LL) { printf("FAIL constant MK_S_FIRST\n"); failures++; }
    if ((long long)(MK_S_HIM) != 262629LL) { printf("FAIL constant MK_S_HIM\n"); failures++; }
    if ((long long)(MK_S_LAST) != 262639LL) { printf("FAIL constant MK_S_LAST\n"); failures++; }
    if ((long long)(MK_S_ME) != 262628LL) { printf("FAIL constant MK_S_ME\n"); failures++; }
    if ((long long)(MK_S_MONIKERALREADYREGISTERED) != 262631LL) { printf("FAIL constant MK_S_MONIKERALREADYREGISTERED\n"); failures++; }
    if ((long long)(MK_S_REDUCED_TO_SELF) != 262626LL) { printf("FAIL constant MK_S_REDUCED_TO_SELF\n"); failures++; }
    if ((long long)(MK_S_US) != 262630LL) { printf("FAIL constant MK_S_US\n"); failures++; }
    if ((long long)(MK_XBUTTON1) != 32LL) { printf("FAIL constant MK_XBUTTON1\n"); failures++; }
    if ((long long)(MK_XBUTTON2) != 64LL) { printf("FAIL constant MK_XBUTTON2\n"); failures++; }
    if ((long long)(MONITOR_DEFAULTTONEAREST) != 2LL) { printf("FAIL constant MONITOR_DEFAULTTONEAREST\n"); failures++; }
    if ((long long)(MONITOR_DEFAULTTONULL) != 0LL) { printf("FAIL constant MONITOR_DEFAULTTONULL\n"); failures++; }
    if ((long long)(MONITOR_DEFAULTTOPRIMARY) != 1LL) { printf("FAIL constant MONITOR_DEFAULTTOPRIMARY\n"); failures++; }
    if ((long long)(MOUSEEVENTF_ABSOLUTE) != 32768LL) { printf("FAIL constant MOUSEEVENTF_ABSOLUTE\n"); failures++; }
    if ((long long)(MOUSEEVENTF_HWHEEL) != 4096LL) { printf("FAIL constant MOUSEEVENTF_HWHEEL\n"); failures++; }
    if ((long long)(MOUSEEVENTF_LEFTDOWN) != 2LL) { printf("FAIL constant MOUSEEVENTF_LEFTDOWN\n"); failures++; }
    if ((long long)(MOUSEEVENTF_LEFTUP) != 4LL) { printf("FAIL constant MOUSEEVENTF_LEFTUP\n"); failures++; }
    if ((long long)(MOUSEEVENTF_MIDDLEDOWN) != 32LL) { printf("FAIL constant MOUSEEVENTF_MIDDLEDOWN\n"); failures++; }
    if ((long long)(MOUSEEVENTF_MIDDLEUP) != 64LL) { printf("FAIL constant MOUSEEVENTF_MIDDLEUP\n"); failures++; }
    if ((long long)(MOUSEEVENTF_MOVE) != 1LL) { printf("FAIL constant MOUSEEVENTF_MOVE\n"); failures++; }
    if ((long long)(MOUSEEVENTF_MOVE_NOCOALESCE) != 8192LL) { printf("FAIL constant MOUSEEVENTF_MOVE_NOCOALESCE\n"); failures++; }
    if ((long long)(MOUSEEVENTF_RIGHTDOWN) != 8LL) { printf("FAIL constant MOUSEEVENTF_RIGHTDOWN\n"); failures++; }
    if ((long long)(MOUSEEVENTF_RIGHTUP) != 16LL) { printf("FAIL constant MOUSEEVENTF_RIGHTUP\n"); failures++; }
    if ((long long)(MOUSEEVENTF_VIRTUALDESK) != 16384LL) { printf("FAIL constant MOUSEEVENTF_VIRTUALDESK\n"); failures++; }
    if ((long long)(MOUSEEVENTF_WHEEL) != 2048LL) { printf("FAIL constant MOUSEEVENTF_WHEEL\n"); failures++; }
    if ((long long)(MOUSEEVENTF_XDOWN) != 128LL) { printf("FAIL constant MOUSEEVENTF_XDOWN\n"); failures++; }
    if ((long long)(MOUSEEVENTF_XUP) != 256LL) { printf("FAIL constant MOUSEEVENTF_XUP\n"); failures++; }
    if ((long long)(MOVEFILE_COPY_ALLOWED) != 2LL) { printf("FAIL constant MOVEFILE_COPY_ALLOWED\n"); failures++; }
    if ((long long)(MOVEFILE_CREATE_HARDLINK) != 16LL) { printf("FAIL constant MOVEFILE_CREATE_HARDLINK\n"); failures++; }
    if ((long long)(MOVEFILE_DELAY_UNTIL_REBOOT) != 4LL) { printf("FAIL constant MOVEFILE_DELAY_UNTIL_REBOOT\n"); failures++; }
    if ((long long)(MOVEFILE_FAIL_IF_NOT_TRACKABLE) != 32LL) { printf("FAIL constant MOVEFILE_FAIL_IF_NOT_TRACKABLE\n"); failures++; }
    if ((long long)(MOVEFILE_REPLACE_EXISTING) != 1LL) { printf("FAIL constant MOVEFILE_REPLACE_EXISTING\n"); failures++; }
    if ((long long)(MOVEFILE_WRITE_THROUGH) != 8LL) { printf("FAIL constant MOVEFILE_WRITE_THROUGH\n"); failures++; }
    if ((long long)(MUI_LANGUAGE_ID) != 4LL) { printf("FAIL constant MUI_LANGUAGE_ID\n"); failures++; }
    if ((long long)(MUI_LANGUAGE_INSTALLED) != 32LL) { printf("FAIL constant MUI_LANGUAGE_INSTALLED\n"); failures++; }
    if ((long long)(MUI_LANGUAGE_LICENSED) != 64LL) { printf("FAIL constant MUI_LANGUAGE_LICENSED\n"); failures++; }
    if ((long long)(MUI_LANGUAGE_NAME) != 8LL) { printf("FAIL constant MUI_LANGUAGE_NAME\n"); failures++; }
    if ((long long)(MWMO_ALERTABLE) != 2LL) { printf("FAIL constant MWMO_ALERTABLE\n"); failures++; }
    if ((long long)(MWMO_INPUTAVAILABLE) != 4LL) { printf("FAIL constant MWMO_INPUTAVAILABLE\n"); failures++; }
    if ((long long)(MWMO_WAITALL) != 1LL) { printf("FAIL constant MWMO_WAITALL\n"); failures++; }
    if ((long long)(NIF_GUID) != 32LL) { printf("FAIL constant NIF_GUID\n"); failures++; }
    if ((long long)(NIF_ICON) != 2LL) { printf("FAIL constant NIF_ICON\n"); failures++; }
    if ((long long)(NIF_INFO) != 16LL) { printf("FAIL constant NIF_INFO\n"); failures++; }
    if ((long long)(NIF_MESSAGE) != 1LL) { printf("FAIL constant NIF_MESSAGE\n"); failures++; }
    if ((long long)(NIF_REALTIME) != 64LL) { printf("FAIL constant NIF_REALTIME\n"); failures++; }
    if ((long long)(NIF_SHOWTIP) != 128LL) { printf("FAIL constant NIF_SHOWTIP\n"); failures++; }
    if ((long long)(NIF_STATE) != 8LL) { printf("FAIL constant NIF_STATE\n"); failures++; }
    if ((long long)(NIF_TIP) != 4LL) { printf("FAIL constant NIF_TIP\n"); failures++; }
    if ((long long)(NIIF_ERROR) != 3LL) { printf("FAIL constant NIIF_ERROR\n"); failures++; }
    if ((long long)(NIIF_ICON_MASK) != 15LL) { printf("FAIL constant NIIF_ICON_MASK\n"); failures++; }
    if ((long long)(NIIF_INFO) != 1LL) { printf("FAIL constant NIIF_INFO\n"); failures++; }
    if ((long long)(NIIF_LARGE_ICON) != 32LL) { printf("FAIL constant NIIF_LARGE_ICON\n"); failures++; }
    if ((long long)(NIIF_NONE) != 0LL) { printf("FAIL constant NIIF_NONE\n"); failures++; }
    if ((long long)(NIIF_NOSOUND) != 16LL) { printf("FAIL constant NIIF_NOSOUND\n"); failures++; }
    if ((long long)(NIIF_RESPECT_QUIET_TIME) != 128LL) { printf("FAIL constant NIIF_RESPECT_QUIET_TIME\n"); failures++; }
    if ((long long)(NIIF_USER) != 4LL) { printf("FAIL constant NIIF_USER\n"); failures++; }
    if ((long long)(NIIF_WARNING) != 2LL) { printf("FAIL constant NIIF_WARNING\n"); failures++; }
    if ((long long)(NIM_ADD) != 0LL) { printf("FAIL constant NIM_ADD\n"); failures++; }
    if ((long long)(NIM_DELETE) != 2LL) { printf("FAIL constant NIM_DELETE\n"); failures++; }
    if ((long long)(NIM_MODIFY) != 1LL) { printf("FAIL constant NIM_MODIFY\n"); failures++; }
    if ((long long)(NIM_SETFOCUS) != 3LL) { printf("FAIL constant NIM_SETFOCUS\n"); failures++; }
    if ((long long)(NIM_SETVERSION) != 4LL) { printf("FAIL constant NIM_SETVERSION\n"); failures++; }
    if ((long long)(NIN_BALLOONHIDE) != 1027LL) { printf("FAIL constant NIN_BALLOONHIDE\n"); failures++; }
    if ((long long)(NIN_BALLOONSHOW) != 1026LL) { printf("FAIL constant NIN_BALLOONSHOW\n"); failures++; }
    if ((long long)(NIN_BALLOONTIMEOUT) != 1028LL) { printf("FAIL constant NIN_BALLOONTIMEOUT\n"); failures++; }
    if ((long long)(NIN_BALLOONUSERCLICK) != 1029LL) { printf("FAIL constant NIN_BALLOONUSERCLICK\n"); failures++; }
    if ((long long)(NIN_KEYSELECT) != 1025LL) { printf("FAIL constant NIN_KEYSELECT\n"); failures++; }
    if ((long long)(NIN_POPUPCLOSE) != 1031LL) { printf("FAIL constant NIN_POPUPCLOSE\n"); failures++; }
    if ((long long)(NIN_POPUPOPEN) != 1030LL) { printf("FAIL constant NIN_POPUPOPEN\n"); failures++; }
    if ((long long)(NIN_SELECT) != 1024LL) { printf("FAIL constant NIN_SELECT\n"); failures++; }
    if ((long long)(NI_CHANGECANDIDATELIST) != 19LL) { printf("FAIL constant NI_CHANGECANDIDATELIST\n"); failures++; }
    if ((long long)(NI_CLOSECANDIDATE) != 17LL) { printf("FAIL constant NI_CLOSECANDIDATE\n"); failures++; }
    if ((long long)(NI_COMPOSITIONSTR) != 21LL) { printf("FAIL constant NI_COMPOSITIONSTR\n"); failures++; }
    if ((long long)(NI_FINALIZECONVERSIONRESULT) != 20LL) { printf("FAIL constant NI_FINALIZECONVERSIONRESULT\n"); failures++; }
    if ((long long)(NI_IMEMENUSELECTED) != 24LL) { printf("FAIL constant NI_IMEMENUSELECTED\n"); failures++; }
    if ((long long)(NI_OPENCANDIDATE) != 16LL) { printf("FAIL constant NI_OPENCANDIDATE\n"); failures++; }
    if ((long long)(NI_SELECTCANDIDATESTR) != 18LL) { printf("FAIL constant NI_SELECTCANDIDATESTR\n"); failures++; }
    if ((long long)(NI_SETCANDIDATE_PAGESIZE) != 23LL) { printf("FAIL constant NI_SETCANDIDATE_PAGESIZE\n"); failures++; }
    if ((long long)(NI_SETCANDIDATE_PAGESTART) != 22LL) { printf("FAIL constant NI_SETCANDIDATE_PAGESTART\n"); failures++; }
    if ((long long)(NOTIFYICON_VERSION) != 3LL) { printf("FAIL constant NOTIFYICON_VERSION\n"); failures++; }
    if ((long long)(NOTIFYICON_VERSION_4) != 4LL) { printf("FAIL constant NOTIFYICON_VERSION_4\n"); failures++; }
    if ((long long)(NavigateDirection_FirstChild) != 3LL) { printf("FAIL constant NavigateDirection_FirstChild\n"); failures++; }
    if ((long long)(NavigateDirection_LastChild) != 4LL) { printf("FAIL constant NavigateDirection_LastChild\n"); failures++; }
    if ((long long)(NavigateDirection_NextSibling) != 1LL) { printf("FAIL constant NavigateDirection_NextSibling\n"); failures++; }
    if ((long long)(NavigateDirection_Parent) != 0LL) { printf("FAIL constant NavigateDirection_Parent\n"); failures++; }
    if ((long long)(NavigateDirection_PreviousSibling) != 2LL) { printf("FAIL constant NavigateDirection_PreviousSibling\n"); failures++; }
    if ((long long)(NotificationKind_ActionAborted) != 3LL) { printf("FAIL constant NotificationKind_ActionAborted\n"); failures++; }
    if ((long long)(NotificationKind_ActionCompleted) != 2LL) { printf("FAIL constant NotificationKind_ActionCompleted\n"); failures++; }
    if ((long long)(NotificationKind_ItemAdded) != 0LL) { printf("FAIL constant NotificationKind_ItemAdded\n"); failures++; }
    if ((long long)(NotificationKind_ItemRemoved) != 1LL) { printf("FAIL constant NotificationKind_ItemRemoved\n"); failures++; }
    if ((long long)(NotificationKind_Other) != 4LL) { printf("FAIL constant NotificationKind_Other\n"); failures++; }
    if ((long long)(NotificationProcessing_All) != 2LL) { printf("FAIL constant NotificationProcessing_All\n"); failures++; }
    if ((long long)(NotificationProcessing_CurrentThenMostRecent) != 4LL) { printf("FAIL constant NotificationProcessing_CurrentThenMostRecent\n"); failures++; }
    if ((long long)(NotificationProcessing_ImportantAll) != 0LL) { printf("FAIL constant NotificationProcessing_ImportantAll\n"); failures++; }
    if ((long long)(NotificationProcessing_ImportantMostRecent) != 1LL) { printf("FAIL constant NotificationProcessing_ImportantMostRecent\n"); failures++; }
    if ((long long)(NotificationProcessing_MostRecent) != 3LL) { printf("FAIL constant NotificationProcessing_MostRecent\n"); failures++; }
    if ((long long)(OFN_ALLOWMULTISELECT) != 512LL) { printf("FAIL constant OFN_ALLOWMULTISELECT\n"); failures++; }
    if ((long long)(OFN_CREATEPROMPT) != 8192LL) { printf("FAIL constant OFN_CREATEPROMPT\n"); failures++; }
    if ((long long)(OFN_DONTADDTORECENT) != 33554432LL) { printf("FAIL constant OFN_DONTADDTORECENT\n"); failures++; }
    if ((long long)(OFN_ENABLEHOOK) != 32LL) { printf("FAIL constant OFN_ENABLEHOOK\n"); failures++; }
    if ((long long)(OFN_ENABLEINCLUDENOTIFY) != 4194304LL) { printf("FAIL constant OFN_ENABLEINCLUDENOTIFY\n"); failures++; }
    if ((long long)(OFN_ENABLESIZING) != 8388608LL) { printf("FAIL constant OFN_ENABLESIZING\n"); failures++; }
    if ((long long)(OFN_ENABLETEMPLATE) != 64LL) { printf("FAIL constant OFN_ENABLETEMPLATE\n"); failures++; }
    if ((long long)(OFN_ENABLETEMPLATEHANDLE) != 128LL) { printf("FAIL constant OFN_ENABLETEMPLATEHANDLE\n"); failures++; }
    if ((long long)(OFN_EXPLORER) != 524288LL) { printf("FAIL constant OFN_EXPLORER\n"); failures++; }
    if ((long long)(OFN_EXTENSIONDIFFERENT) != 1024LL) { printf("FAIL constant OFN_EXTENSIONDIFFERENT\n"); failures++; }
    if ((long long)(OFN_EX_NOPLACESBAR) != 1LL) { printf("FAIL constant OFN_EX_NOPLACESBAR\n"); failures++; }
    if ((long long)(OFN_FILEMUSTEXIST) != 4096LL) { printf("FAIL constant OFN_FILEMUSTEXIST\n"); failures++; }
    if ((long long)(OFN_FORCESHOWHIDDEN) != 268435456LL) { printf("FAIL constant OFN_FORCESHOWHIDDEN\n"); failures++; }
    if ((long long)(OFN_HIDEREADONLY) != 4LL) { printf("FAIL constant OFN_HIDEREADONLY\n"); failures++; }
    if ((long long)(OFN_LONGNAMES) != 2097152LL) { printf("FAIL constant OFN_LONGNAMES\n"); failures++; }
    if ((long long)(OFN_NOCHANGEDIR) != 8LL) { printf("FAIL constant OFN_NOCHANGEDIR\n"); failures++; }
    if ((long long)(OFN_NODEREFERENCELINKS) != 1048576LL) { printf("FAIL constant OFN_NODEREFERENCELINKS\n"); failures++; }
    if ((long long)(OFN_NOLONGNAMES) != 262144LL) { printf("FAIL constant OFN_NOLONGNAMES\n"); failures++; }
    if ((long long)(OFN_NONETWORKBUTTON) != 131072LL) { printf("FAIL constant OFN_NONETWORKBUTTON\n"); failures++; }
    if ((long long)(OFN_NOREADONLYRETURN) != 32768LL) { printf("FAIL constant OFN_NOREADONLYRETURN\n"); failures++; }
    if ((long long)(OFN_NOTESTFILECREATE) != 65536LL) { printf("FAIL constant OFN_NOTESTFILECREATE\n"); failures++; }
    if ((long long)(OFN_NOVALIDATE) != 256LL) { printf("FAIL constant OFN_NOVALIDATE\n"); failures++; }
    if ((long long)(OFN_OVERWRITEPROMPT) != 2LL) { printf("FAIL constant OFN_OVERWRITEPROMPT\n"); failures++; }
    if ((long long)(OFN_PATHMUSTEXIST) != 2048LL) { printf("FAIL constant OFN_PATHMUSTEXIST\n"); failures++; }
    if ((long long)(OFN_READONLY) != 1LL) { printf("FAIL constant OFN_READONLY\n"); failures++; }
    if ((long long)(OFN_SHAREAWARE) != 16384LL) { printf("FAIL constant OFN_SHAREAWARE\n"); failures++; }
    if ((long long)(OFN_SHAREFALLTHROUGH) != 2LL) { printf("FAIL constant OFN_SHAREFALLTHROUGH\n"); failures++; }
    if ((long long)(OFN_SHARENOWARN) != 1LL) { printf("FAIL constant OFN_SHARENOWARN\n"); failures++; }
    if ((long long)(OFN_SHAREWARN) != 0LL) { printf("FAIL constant OFN_SHAREWARN\n"); failures++; }
    if ((long long)(OFN_SHOWHELP) != 16LL) { printf("FAIL constant OFN_SHOWHELP\n"); failures++; }
    if ((long long)(OrientationType_Horizontal) != 1LL) { printf("FAIL constant OrientationType_Horizontal\n"); failures++; }
    if ((long long)(OrientationType_None) != 0LL) { printf("FAIL constant OrientationType_None\n"); failures++; }
    if ((long long)(OrientationType_Vertical) != 2LL) { printf("FAIL constant OrientationType_Vertical\n"); failures++; }
    if ((long long)(PAGE_ENCLAVE_DECOMMIT) != 268435456LL) { printf("FAIL constant PAGE_ENCLAVE_DECOMMIT\n"); failures++; }
    if ((long long)(PAGE_ENCLAVE_MASK) != 268435456LL) { printf("FAIL constant PAGE_ENCLAVE_MASK\n"); failures++; }
    if ((long long)(PAGE_ENCLAVE_SS_FIRST) != 268435457LL) { printf("FAIL constant PAGE_ENCLAVE_SS_FIRST\n"); failures++; }
    if ((long long)(PAGE_ENCLAVE_SS_REST) != 268435458LL) { printf("FAIL constant PAGE_ENCLAVE_SS_REST\n"); failures++; }
    if ((long long)(PAGE_ENCLAVE_THREAD_CONTROL) != 2147483648LL) { printf("FAIL constant PAGE_ENCLAVE_THREAD_CONTROL\n"); failures++; }
    if ((long long)(PAGE_ENCLAVE_UNVALIDATED) != 536870912LL) { printf("FAIL constant PAGE_ENCLAVE_UNVALIDATED\n"); failures++; }
    if ((long long)(PAGE_EXECUTE) != 16LL) { printf("FAIL constant PAGE_EXECUTE\n"); failures++; }
    if ((long long)(PAGE_EXECUTE_READ) != 32LL) { printf("FAIL constant PAGE_EXECUTE_READ\n"); failures++; }
    if ((long long)(PAGE_EXECUTE_READWRITE) != 64LL) { printf("FAIL constant PAGE_EXECUTE_READWRITE\n"); failures++; }
    if ((long long)(PAGE_EXECUTE_WRITECOPY) != 128LL) { printf("FAIL constant PAGE_EXECUTE_WRITECOPY\n"); failures++; }
    if ((long long)(PAGE_GRAPHICS_COHERENT) != 131072LL) { printf("FAIL constant PAGE_GRAPHICS_COHERENT\n"); failures++; }
    if ((long long)(PAGE_GRAPHICS_EXECUTE) != 16384LL) { printf("FAIL constant PAGE_GRAPHICS_EXECUTE\n"); failures++; }
    if ((long long)(PAGE_GRAPHICS_EXECUTE_READ) != 32768LL) { printf("FAIL constant PAGE_GRAPHICS_EXECUTE_READ\n"); failures++; }
    if ((long long)(PAGE_GRAPHICS_EXECUTE_READWRITE) != 65536LL) { printf("FAIL constant PAGE_GRAPHICS_EXECUTE_READWRITE\n"); failures++; }
    if ((long long)(PAGE_GRAPHICS_NOACCESS) != 2048LL) { printf("FAIL constant PAGE_GRAPHICS_NOACCESS\n"); failures++; }
    if ((long long)(PAGE_GRAPHICS_NOCACHE) != 262144LL) { printf("FAIL constant PAGE_GRAPHICS_NOCACHE\n"); failures++; }
    if ((long long)(PAGE_GRAPHICS_READONLY) != 4096LL) { printf("FAIL constant PAGE_GRAPHICS_READONLY\n"); failures++; }
    if ((long long)(PAGE_GRAPHICS_READWRITE) != 8192LL) { printf("FAIL constant PAGE_GRAPHICS_READWRITE\n"); failures++; }
    if ((long long)(PAGE_GUARD) != 256LL) { printf("FAIL constant PAGE_GUARD\n"); failures++; }
    if ((long long)(PAGE_NOACCESS) != 1LL) { printf("FAIL constant PAGE_NOACCESS\n"); failures++; }
    if ((long long)(PAGE_NOCACHE) != 512LL) { printf("FAIL constant PAGE_NOCACHE\n"); failures++; }
    if ((long long)(PAGE_READONLY) != 2LL) { printf("FAIL constant PAGE_READONLY\n"); failures++; }
    if ((long long)(PAGE_READWRITE) != 4LL) { printf("FAIL constant PAGE_READWRITE\n"); failures++; }
    if ((long long)(PAGE_REVERT_TO_FILE_MAP) != 2147483648LL) { printf("FAIL constant PAGE_REVERT_TO_FILE_MAP\n"); failures++; }
    if ((long long)(PAGE_TARGETS_INVALID) != 1073741824LL) { printf("FAIL constant PAGE_TARGETS_INVALID\n"); failures++; }
    if ((long long)(PAGE_TARGETS_NO_UPDATE) != 1073741824LL) { printf("FAIL constant PAGE_TARGETS_NO_UPDATE\n"); failures++; }
    if ((long long)(PAGE_WRITECOMBINE) != 1024LL) { printf("FAIL constant PAGE_WRITECOMBINE\n"); failures++; }
    if ((long long)(PAGE_WRITECOPY) != 8LL) { printf("FAIL constant PAGE_WRITECOPY\n"); failures++; }
    if ((long long)(PEN_FLAG_BARREL) != 1LL) { printf("FAIL constant PEN_FLAG_BARREL\n"); failures++; }
    if ((long long)(PEN_FLAG_ERASER) != 4LL) { printf("FAIL constant PEN_FLAG_ERASER\n"); failures++; }
    if ((long long)(PEN_FLAG_INVERTED) != 2LL) { printf("FAIL constant PEN_FLAG_INVERTED\n"); failures++; }
    if ((long long)(PEN_FLAG_NONE) != 0LL) { printf("FAIL constant PEN_FLAG_NONE\n"); failures++; }
    if ((long long)(PEN_MASK_NONE) != 0LL) { printf("FAIL constant PEN_MASK_NONE\n"); failures++; }
    if ((long long)(PEN_MASK_PRESSURE) != 1LL) { printf("FAIL constant PEN_MASK_PRESSURE\n"); failures++; }
    if ((long long)(PEN_MASK_ROTATION) != 2LL) { printf("FAIL constant PEN_MASK_ROTATION\n"); failures++; }
    if ((long long)(PEN_MASK_TILT_X) != 4LL) { printf("FAIL constant PEN_MASK_TILT_X\n"); failures++; }
    if ((long long)(PEN_MASK_TILT_Y) != 8LL) { printf("FAIL constant PEN_MASK_TILT_Y\n"); failures++; }
    if ((long long)(PM_NOREMOVE) != 0LL) { printf("FAIL constant PM_NOREMOVE\n"); failures++; }
    if ((long long)(PM_NOYIELD) != 2LL) { printf("FAIL constant PM_NOYIELD\n"); failures++; }
    if ((long long)(PM_QS_INPUT) != 470220800LL) { printf("FAIL constant PM_QS_INPUT\n"); failures++; }
    if ((long long)(PM_QS_PAINT) != 2097152LL) { printf("FAIL constant PM_QS_PAINT\n"); failures++; }
    if ((long long)(PM_QS_POSTMESSAGE) != 9961472LL) { printf("FAIL constant PM_QS_POSTMESSAGE\n"); failures++; }
    if ((long long)(PM_QS_SENDMESSAGE) != 4194304LL) { printf("FAIL constant PM_QS_SENDMESSAGE\n"); failures++; }
    if ((long long)(PM_REMOVE) != 1LL) { printf("FAIL constant PM_REMOVE\n"); failures++; }
    if ((long long)(POINTER_FLAG_CANCELED) != 32768LL) { printf("FAIL constant POINTER_FLAG_CANCELED\n"); failures++; }
    if ((long long)(POINTER_FLAG_CAPTURECHANGED) != 2097152LL) { printf("FAIL constant POINTER_FLAG_CAPTURECHANGED\n"); failures++; }
    if ((long long)(POINTER_FLAG_CONFIDENCE) != 16384LL) { printf("FAIL constant POINTER_FLAG_CONFIDENCE\n"); failures++; }
    if ((long long)(POINTER_FLAG_DOWN) != 65536LL) { printf("FAIL constant POINTER_FLAG_DOWN\n"); failures++; }
    if ((long long)(POINTER_FLAG_FIFTHBUTTON) != 256LL) { printf("FAIL constant POINTER_FLAG_FIFTHBUTTON\n"); failures++; }
    if ((long long)(POINTER_FLAG_FIRSTBUTTON) != 16LL) { printf("FAIL constant POINTER_FLAG_FIRSTBUTTON\n"); failures++; }
    if ((long long)(POINTER_FLAG_FOURTHBUTTON) != 128LL) { printf("FAIL constant POINTER_FLAG_FOURTHBUTTON\n"); failures++; }
    if ((long long)(POINTER_FLAG_HASTRANSFORM) != 4194304LL) { printf("FAIL constant POINTER_FLAG_HASTRANSFORM\n"); failures++; }
    if ((long long)(POINTER_FLAG_HWHEEL) != 1048576LL) { printf("FAIL constant POINTER_FLAG_HWHEEL\n"); failures++; }
    if ((long long)(POINTER_FLAG_INCONTACT) != 4LL) { printf("FAIL constant POINTER_FLAG_INCONTACT\n"); failures++; }
    if ((long long)(POINTER_FLAG_INRANGE) != 2LL) { printf("FAIL constant POINTER_FLAG_INRANGE\n"); failures++; }
    if ((long long)(POINTER_FLAG_NEW) != 1LL) { printf("FAIL constant POINTER_FLAG_NEW\n"); failures++; }
    if ((long long)(POINTER_FLAG_NONE) != 0LL) { printf("FAIL constant POINTER_FLAG_NONE\n"); failures++; }
    if ((long long)(POINTER_FLAG_PRIMARY) != 8192LL) { printf("FAIL constant POINTER_FLAG_PRIMARY\n"); failures++; }
    if ((long long)(POINTER_FLAG_SECONDBUTTON) != 32LL) { printf("FAIL constant POINTER_FLAG_SECONDBUTTON\n"); failures++; }
    if ((long long)(POINTER_FLAG_THIRDBUTTON) != 64LL) { printf("FAIL constant POINTER_FLAG_THIRDBUTTON\n"); failures++; }
    if ((long long)(POINTER_FLAG_UP) != 262144LL) { printf("FAIL constant POINTER_FLAG_UP\n"); failures++; }
    if ((long long)(POINTER_FLAG_UPDATE) != 131072LL) { printf("FAIL constant POINTER_FLAG_UPDATE\n"); failures++; }
    if ((long long)(POINTER_FLAG_WHEEL) != 524288LL) { printf("FAIL constant POINTER_FLAG_WHEEL\n"); failures++; }
    if ((long long)(POINTER_MESSAGE_FLAG_CANCELED) != 32768LL) { printf("FAIL constant POINTER_MESSAGE_FLAG_CANCELED\n"); failures++; }
    if ((long long)(POINTER_MESSAGE_FLAG_CONFIDENCE) != 16384LL) { printf("FAIL constant POINTER_MESSAGE_FLAG_CONFIDENCE\n"); failures++; }
    if ((long long)(POINTER_MESSAGE_FLAG_FIFTHBUTTON) != 256LL) { printf("FAIL constant POINTER_MESSAGE_FLAG_FIFTHBUTTON\n"); failures++; }
    if ((long long)(POINTER_MESSAGE_FLAG_FIRSTBUTTON) != 16LL) { printf("FAIL constant POINTER_MESSAGE_FLAG_FIRSTBUTTON\n"); failures++; }
    if ((long long)(POINTER_MESSAGE_FLAG_FOURTHBUTTON) != 128LL) { printf("FAIL constant POINTER_MESSAGE_FLAG_FOURTHBUTTON\n"); failures++; }
    if ((long long)(POINTER_MESSAGE_FLAG_INCONTACT) != 4LL) { printf("FAIL constant POINTER_MESSAGE_FLAG_INCONTACT\n"); failures++; }
    if ((long long)(POINTER_MESSAGE_FLAG_INRANGE) != 2LL) { printf("FAIL constant POINTER_MESSAGE_FLAG_INRANGE\n"); failures++; }
    if ((long long)(POINTER_MESSAGE_FLAG_NEW) != 1LL) { printf("FAIL constant POINTER_MESSAGE_FLAG_NEW\n"); failures++; }
    if ((long long)(POINTER_MESSAGE_FLAG_PRIMARY) != 8192LL) { printf("FAIL constant POINTER_MESSAGE_FLAG_PRIMARY\n"); failures++; }
    if ((long long)(POINTER_MESSAGE_FLAG_SECONDBUTTON) != 32LL) { printf("FAIL constant POINTER_MESSAGE_FLAG_SECONDBUTTON\n"); failures++; }
    if ((long long)(POINTER_MESSAGE_FLAG_THIRDBUTTON) != 64LL) { printf("FAIL constant POINTER_MESSAGE_FLAG_THIRDBUTTON\n"); failures++; }
    if ((long long)(PT_BEZIERTO) != 4LL) { printf("FAIL constant PT_BEZIERTO\n"); failures++; }
    if ((long long)(PT_CLOSEFIGURE) != 1LL) { printf("FAIL constant PT_CLOSEFIGURE\n"); failures++; }
    if ((long long)(PT_LINETO) != 2LL) { printf("FAIL constant PT_LINETO\n"); failures++; }
    if ((long long)(PT_MOUSE) != 4LL) { printf("FAIL constant PT_MOUSE\n"); failures++; }
    if ((long long)(PT_MOVETO) != 6LL) { printf("FAIL constant PT_MOVETO\n"); failures++; }
    if ((long long)(PT_PEN) != 3LL) { printf("FAIL constant PT_PEN\n"); failures++; }
    if ((long long)(PT_POINTER) != 1LL) { printf("FAIL constant PT_POINTER\n"); failures++; }
    if ((long long)(PT_TOUCH) != 2LL) { printf("FAIL constant PT_TOUCH\n"); failures++; }
    if ((long long)(PT_TOUCHPAD) != 5LL) { printf("FAIL constant PT_TOUCHPAD\n"); failures++; }
    if ((long long)(ProviderOptions_ClientSideProvider) != 1LL) { printf("FAIL constant ProviderOptions_ClientSideProvider\n"); failures++; }
    if ((long long)(ProviderOptions_HasNativeIAccessible) != 128LL) { printf("FAIL constant ProviderOptions_HasNativeIAccessible\n"); failures++; }
    if ((long long)(ProviderOptions_NonClientAreaProvider) != 4LL) { printf("FAIL constant ProviderOptions_NonClientAreaProvider\n"); failures++; }
    if ((long long)(ProviderOptions_OverrideProvider) != 8LL) { printf("FAIL constant ProviderOptions_OverrideProvider\n"); failures++; }
    if ((long long)(ProviderOptions_ProviderOwnsSetFocus) != 16LL) { printf("FAIL constant ProviderOptions_ProviderOwnsSetFocus\n"); failures++; }
    if ((long long)(ProviderOptions_RefuseNonClientSupport) != 64LL) { printf("FAIL constant ProviderOptions_RefuseNonClientSupport\n"); failures++; }
    if ((long long)(ProviderOptions_ServerSideProvider) != 2LL) { printf("FAIL constant ProviderOptions_ServerSideProvider\n"); failures++; }
    if ((long long)(ProviderOptions_UseClientCoordinates) != 256LL) { printf("FAIL constant ProviderOptions_UseClientCoordinates\n"); failures++; }
    if ((long long)(ProviderOptions_UseComThreading) != 32LL) { printf("FAIL constant ProviderOptions_UseComThreading\n"); failures++; }
    if ((long long)(QS_ALLEVENTS) != 7359LL) { printf("FAIL constant QS_ALLEVENTS\n"); failures++; }
    if ((long long)(QS_ALLINPUT) != 7423LL) { printf("FAIL constant QS_ALLINPUT\n"); failures++; }
    if ((long long)(QS_ALLPOSTMESSAGE) != 256LL) { printf("FAIL constant QS_ALLPOSTMESSAGE\n"); failures++; }
    if ((long long)(QS_HOTKEY) != 128LL) { printf("FAIL constant QS_HOTKEY\n"); failures++; }
    if ((long long)(QS_INPUT) != 7175LL) { printf("FAIL constant QS_INPUT\n"); failures++; }
    if ((long long)(QS_KEY) != 1LL) { printf("FAIL constant QS_KEY\n"); failures++; }
    if ((long long)(QS_MOUSE) != 6LL) { printf("FAIL constant QS_MOUSE\n"); failures++; }
    if ((long long)(QS_MOUSEBUTTON) != 4LL) { printf("FAIL constant QS_MOUSEBUTTON\n"); failures++; }
    if ((long long)(QS_MOUSEMOVE) != 2LL) { printf("FAIL constant QS_MOUSEMOVE\n"); failures++; }
    if ((long long)(QS_PAINT) != 32LL) { printf("FAIL constant QS_PAINT\n"); failures++; }
    if ((long long)(QS_POINTER) != 4096LL) { printf("FAIL constant QS_POINTER\n"); failures++; }
    if ((long long)(QS_POSTMESSAGE) != 8LL) { printf("FAIL constant QS_POSTMESSAGE\n"); failures++; }
    if ((long long)(QS_RAWINPUT) != 1024LL) { printf("FAIL constant QS_RAWINPUT\n"); failures++; }
    if ((long long)(QS_SENDMESSAGE) != 64LL) { printf("FAIL constant QS_SENDMESSAGE\n"); failures++; }
    if ((long long)(QS_TIMER) != 16LL) { printf("FAIL constant QS_TIMER\n"); failures++; }
    if ((long long)(QS_TOUCH) != 2048LL) { printf("FAIL constant QS_TOUCH\n"); failures++; }
    if ((long long)(RDW_ALLCHILDREN) != 128LL) { printf("FAIL constant RDW_ALLCHILDREN\n"); failures++; }
    if ((long long)(RDW_ERASE) != 4LL) { printf("FAIL constant RDW_ERASE\n"); failures++; }
    if ((long long)(RDW_ERASENOW) != 512LL) { printf("FAIL constant RDW_ERASENOW\n"); failures++; }
    if ((long long)(RDW_FRAME) != 1024LL) { printf("FAIL constant RDW_FRAME\n"); failures++; }
    if ((long long)(RDW_INTERNALPAINT) != 2LL) { printf("FAIL constant RDW_INTERNALPAINT\n"); failures++; }
    if ((long long)(RDW_INVALIDATE) != 1LL) { printf("FAIL constant RDW_INVALIDATE\n"); failures++; }
    if ((long long)(RDW_NOCHILDREN) != 64LL) { printf("FAIL constant RDW_NOCHILDREN\n"); failures++; }
    if ((long long)(RDW_NOERASE) != 32LL) { printf("FAIL constant RDW_NOERASE\n"); failures++; }
    if ((long long)(RDW_NOFRAME) != 2048LL) { printf("FAIL constant RDW_NOFRAME\n"); failures++; }
    if ((long long)(RDW_NOINTERNALPAINT) != 16LL) { printf("FAIL constant RDW_NOINTERNALPAINT\n"); failures++; }
    if ((long long)(RDW_UPDATENOW) != 256LL) { printf("FAIL constant RDW_UPDATENOW\n"); failures++; }
    if ((long long)(RDW_VALIDATE) != 8LL) { printf("FAIL constant RDW_VALIDATE\n"); failures++; }
    if ((long long)(RRF_NOEXPAND) != 268435456LL) { printf("FAIL constant RRF_NOEXPAND\n"); failures++; }
    if ((long long)(RRF_RT_ANY) != 65535LL) { printf("FAIL constant RRF_RT_ANY\n"); failures++; }
    if ((long long)(RRF_RT_DWORD) != 24LL) { printf("FAIL constant RRF_RT_DWORD\n"); failures++; }
    if ((long long)(RRF_RT_QWORD) != 72LL) { printf("FAIL constant RRF_RT_QWORD\n"); failures++; }
    if ((long long)(RRF_RT_REG_BINARY) != 8LL) { printf("FAIL constant RRF_RT_REG_BINARY\n"); failures++; }
    if ((long long)(RRF_RT_REG_DWORD) != 16LL) { printf("FAIL constant RRF_RT_REG_DWORD\n"); failures++; }
    if ((long long)(RRF_RT_REG_EXPAND_SZ) != 4LL) { printf("FAIL constant RRF_RT_REG_EXPAND_SZ\n"); failures++; }
    if ((long long)(RRF_RT_REG_MULTI_SZ) != 32LL) { printf("FAIL constant RRF_RT_REG_MULTI_SZ\n"); failures++; }
    if ((long long)(RRF_RT_REG_NONE) != 1LL) { printf("FAIL constant RRF_RT_REG_NONE\n"); failures++; }
    if ((long long)(RRF_RT_REG_QWORD) != 64LL) { printf("FAIL constant RRF_RT_REG_QWORD\n"); failures++; }
    if ((long long)(RRF_RT_REG_SZ) != 2LL) { printf("FAIL constant RRF_RT_REG_SZ\n"); failures++; }
    if ((long long)(RRF_SUBKEY_WOW6432KEY) != 131072LL) { printf("FAIL constant RRF_SUBKEY_WOW6432KEY\n"); failures++; }
    if ((long long)(RRF_SUBKEY_WOW6464KEY) != 65536LL) { printf("FAIL constant RRF_SUBKEY_WOW6464KEY\n"); failures++; }
    if ((long long)(RRF_WOW64_MASK) != 196608LL) { printf("FAIL constant RRF_WOW64_MASK\n"); failures++; }
    if ((long long)(RRF_ZEROONFAILURE) != 536870912LL) { printf("FAIL constant RRF_ZEROONFAILURE\n"); failures++; }
    if ((long long)(RowOrColumnMajor_ColumnMajor) != 1LL) { printf("FAIL constant RowOrColumnMajor_ColumnMajor\n"); failures++; }
    if ((long long)(RowOrColumnMajor_Indeterminate) != 2LL) { printf("FAIL constant RowOrColumnMajor_Indeterminate\n"); failures++; }
    if ((long long)(RowOrColumnMajor_RowMajor) != 0LL) { printf("FAIL constant RowOrColumnMajor_RowMajor\n"); failures++; }
    if ((long long)(SC_ACTION_NONE) != 0LL) { printf("FAIL constant SC_ACTION_NONE\n"); failures++; }
    if ((long long)(SC_ACTION_REBOOT) != 2LL) { printf("FAIL constant SC_ACTION_REBOOT\n"); failures++; }
    if ((long long)(SC_ACTION_RESTART) != 1LL) { printf("FAIL constant SC_ACTION_RESTART\n"); failures++; }
    if ((long long)(SC_ACTION_RUN_COMMAND) != 3LL) { printf("FAIL constant SC_ACTION_RUN_COMMAND\n"); failures++; }
    if ((long long)(SC_ARRANGE) != 61712LL) { printf("FAIL constant SC_ARRANGE\n"); failures++; }
    if ((long long)(SC_CLOSE) != 61536LL) { printf("FAIL constant SC_CLOSE\n"); failures++; }
    if ((long long)(SC_CONTEXTHELP) != 61824LL) { printf("FAIL constant SC_CONTEXTHELP\n"); failures++; }
    if ((long long)(SC_DEFAULT) != 61792LL) { printf("FAIL constant SC_DEFAULT\n"); failures++; }
    if ((long long)(SC_DLG_FORCE_UI) != 4LL) { printf("FAIL constant SC_DLG_FORCE_UI\n"); failures++; }
    if ((long long)(SC_DLG_MINIMAL_UI) != 1LL) { printf("FAIL constant SC_DLG_MINIMAL_UI\n"); failures++; }
    if ((long long)(SC_DLG_NO_UI) != 2LL) { printf("FAIL constant SC_DLG_NO_UI\n"); failures++; }
    if ((long long)(SC_ENUM_PROCESS_INFO) != 0LL) { printf("FAIL constant SC_ENUM_PROCESS_INFO\n"); failures++; }
    if ((long long)(SC_GROUP_IDENTIFIER) != 43LL) { printf("FAIL constant SC_GROUP_IDENTIFIER\n"); failures++; }
    if ((long long)(SC_GROUP_IDENTIFIERA) != 43LL) { printf("FAIL constant SC_GROUP_IDENTIFIERA\n"); failures++; }
    if ((long long)(SC_GROUP_IDENTIFIERW) != 43LL) { printf("FAIL constant SC_GROUP_IDENTIFIERW\n"); failures++; }
    if ((long long)(SC_HOTKEY) != 61776LL) { printf("FAIL constant SC_HOTKEY\n"); failures++; }
    if ((long long)(SC_HSCROLL) != 61568LL) { printf("FAIL constant SC_HSCROLL\n"); failures++; }
    if ((long long)(SC_ICON) != 61472LL) { printf("FAIL constant SC_ICON\n"); failures++; }
    if ((long long)(SC_KEYMENU) != 61696LL) { printf("FAIL constant SC_KEYMENU\n"); failures++; }
    if ((long long)(SC_MANAGER_ALL_ACCESS) != 983103LL) { printf("FAIL constant SC_MANAGER_ALL_ACCESS\n"); failures++; }
    if ((long long)(SC_MANAGER_CONNECT) != 1LL) { printf("FAIL constant SC_MANAGER_CONNECT\n"); failures++; }
    if ((long long)(SC_MANAGER_CREATE_SERVICE) != 2LL) { printf("FAIL constant SC_MANAGER_CREATE_SERVICE\n"); failures++; }
    if ((long long)(SC_MANAGER_ENUMERATE_SERVICE) != 4LL) { printf("FAIL constant SC_MANAGER_ENUMERATE_SERVICE\n"); failures++; }
    if ((long long)(SC_MANAGER_LOCK) != 8LL) { printf("FAIL constant SC_MANAGER_LOCK\n"); failures++; }
    if ((long long)(SC_MANAGER_MODIFY_BOOT_CONFIG) != 32LL) { printf("FAIL constant SC_MANAGER_MODIFY_BOOT_CONFIG\n"); failures++; }
    if ((long long)(SC_MANAGER_QUERY_LOCK_STATUS) != 16LL) { printf("FAIL constant SC_MANAGER_QUERY_LOCK_STATUS\n"); failures++; }
    if ((long long)(SC_MAXIMIZE) != 61488LL) { printf("FAIL constant SC_MAXIMIZE\n"); failures++; }
    if ((long long)(SC_MINIMIZE) != 61472LL) { printf("FAIL constant SC_MINIMIZE\n"); failures++; }
    if ((long long)(SC_MONITORPOWER) != 61808LL) { printf("FAIL constant SC_MONITORPOWER\n"); failures++; }
    if ((long long)(SC_MOUSEMENU) != 61584LL) { printf("FAIL constant SC_MOUSEMENU\n"); failures++; }
    if ((long long)(SC_MOVE) != 61456LL) { printf("FAIL constant SC_MOVE\n"); failures++; }
    if ((long long)(SC_NEXTWINDOW) != 61504LL) { printf("FAIL constant SC_NEXTWINDOW\n"); failures++; }
    if ((long long)(SC_PREVWINDOW) != 61520LL) { printf("FAIL constant SC_PREVWINDOW\n"); failures++; }
    if ((long long)(SC_RESTORE) != 61728LL) { printf("FAIL constant SC_RESTORE\n"); failures++; }
    if ((long long)(SC_SCREENSAVE) != 61760LL) { printf("FAIL constant SC_SCREENSAVE\n"); failures++; }
    if ((long long)(SC_SEPARATOR) != 61455LL) { printf("FAIL constant SC_SEPARATOR\n"); failures++; }
    if ((long long)(SC_SIZE) != 61440LL) { printf("FAIL constant SC_SIZE\n"); failures++; }
    if ((long long)(SC_STATUS_PROCESS_INFO) != 0LL) { printf("FAIL constant SC_STATUS_PROCESS_INFO\n"); failures++; }
    if ((long long)(SC_TASKLIST) != 61744LL) { printf("FAIL constant SC_TASKLIST\n"); failures++; }
    if ((long long)(SC_VSCROLL) != 61552LL) { printf("FAIL constant SC_VSCROLL\n"); failures++; }
    if ((long long)(SC_ZOOM) != 61488LL) { printf("FAIL constant SC_ZOOM\n"); failures++; }
    if ((long long)(SIGDN_DESKTOPABSOLUTEEDITING) != (-2147172352LL)) { printf("FAIL constant SIGDN_DESKTOPABSOLUTEEDITING\n"); failures++; }
    if ((long long)(SIGDN_DESKTOPABSOLUTEPARSING) != (-2147319808LL)) { printf("FAIL constant SIGDN_DESKTOPABSOLUTEPARSING\n"); failures++; }
    if ((long long)(SIGDN_FILESYSPATH) != (-2147123200LL)) { printf("FAIL constant SIGDN_FILESYSPATH\n"); failures++; }
    if ((long long)(SIGDN_NORMALDISPLAY) != 0LL) { printf("FAIL constant SIGDN_NORMALDISPLAY\n"); failures++; }
    if ((long long)(SIGDN_PARENTRELATIVE) != (-2146959359LL)) { printf("FAIL constant SIGDN_PARENTRELATIVE\n"); failures++; }
    if ((long long)(SIGDN_PARENTRELATIVEEDITING) != (-2147282943LL)) { printf("FAIL constant SIGDN_PARENTRELATIVEEDITING\n"); failures++; }
    if ((long long)(SIGDN_PARENTRELATIVEFORADDRESSBAR) != (-2146975743LL)) { printf("FAIL constant SIGDN_PARENTRELATIVEFORADDRESSBAR\n"); failures++; }
    if ((long long)(SIGDN_PARENTRELATIVEFORUI) != (-2146877439LL)) { printf("FAIL constant SIGDN_PARENTRELATIVEFORUI\n"); failures++; }
    if ((long long)(SIGDN_PARENTRELATIVEPARSING) != (-2147385343LL)) { printf("FAIL constant SIGDN_PARENTRELATIVEPARSING\n"); failures++; }
    if ((long long)(SIGDN_URL) != (-2147057664LL)) { printf("FAIL constant SIGDN_URL\n"); failures++; }
    if ((long long)(SIZE_MAX) != (-1LL)) { printf("FAIL constant SIZE_MAX\n"); failures++; }
    if ((long long)(SIZE_MAXHIDE) != 4LL) { printf("FAIL constant SIZE_MAXHIDE\n"); failures++; }
    if ((long long)(SIZE_MAXIMIZED) != 2LL) { printf("FAIL constant SIZE_MAXIMIZED\n"); failures++; }
    if ((long long)(SIZE_MAXSHOW) != 3LL) { printf("FAIL constant SIZE_MAXSHOW\n"); failures++; }
    if ((long long)(SIZE_MINIMIZED) != 1LL) { printf("FAIL constant SIZE_MINIMIZED\n"); failures++; }
    if ((long long)(SIZE_RESTORED) != 0LL) { printf("FAIL constant SIZE_RESTORED\n"); failures++; }
    if ((long long)(SM_ARRANGE) != 56LL) { printf("FAIL constant SM_ARRANGE\n"); failures++; }
    if ((long long)(SM_CARETBLINKINGENABLED) != 8194LL) { printf("FAIL constant SM_CARETBLINKINGENABLED\n"); failures++; }
    if ((long long)(SM_CLEANBOOT) != 67LL) { printf("FAIL constant SM_CLEANBOOT\n"); failures++; }
    if ((long long)(SM_CMETRICS) != 97LL) { printf("FAIL constant SM_CMETRICS\n"); failures++; }
    if ((long long)(SM_CMONITORS) != 80LL) { printf("FAIL constant SM_CMONITORS\n"); failures++; }
    if ((long long)(SM_CMOUSEBUTTONS) != 43LL) { printf("FAIL constant SM_CMOUSEBUTTONS\n"); failures++; }
    if ((long long)(SM_CONVERTIBLESLATEMODE) != 8195LL) { printf("FAIL constant SM_CONVERTIBLESLATEMODE\n"); failures++; }
    if ((long long)(SM_CXBORDER) != 5LL) { printf("FAIL constant SM_CXBORDER\n"); failures++; }
    if ((long long)(SM_CXCURSOR) != 13LL) { printf("FAIL constant SM_CXCURSOR\n"); failures++; }
    if ((long long)(SM_CXDLGFRAME) != 7LL) { printf("FAIL constant SM_CXDLGFRAME\n"); failures++; }
    if ((long long)(SM_CXDOUBLECLK) != 36LL) { printf("FAIL constant SM_CXDOUBLECLK\n"); failures++; }
    if ((long long)(SM_CXDRAG) != 68LL) { printf("FAIL constant SM_CXDRAG\n"); failures++; }
    if ((long long)(SM_CXEDGE) != 45LL) { printf("FAIL constant SM_CXEDGE\n"); failures++; }
    if ((long long)(SM_CXFIXEDFRAME) != 7LL) { printf("FAIL constant SM_CXFIXEDFRAME\n"); failures++; }
    if ((long long)(SM_CXFOCUSBORDER) != 83LL) { printf("FAIL constant SM_CXFOCUSBORDER\n"); failures++; }
    if ((long long)(SM_CXFRAME) != 32LL) { printf("FAIL constant SM_CXFRAME\n"); failures++; }
    if ((long long)(SM_CXFULLSCREEN) != 16LL) { printf("FAIL constant SM_CXFULLSCREEN\n"); failures++; }
    if ((long long)(SM_CXHSCROLL) != 21LL) { printf("FAIL constant SM_CXHSCROLL\n"); failures++; }
    if ((long long)(SM_CXHTHUMB) != 10LL) { printf("FAIL constant SM_CXHTHUMB\n"); failures++; }
    if ((long long)(SM_CXICON) != 11LL) { printf("FAIL constant SM_CXICON\n"); failures++; }
    if ((long long)(SM_CXICONSPACING) != 38LL) { printf("FAIL constant SM_CXICONSPACING\n"); failures++; }
    if ((long long)(SM_CXMAXIMIZED) != 61LL) { printf("FAIL constant SM_CXMAXIMIZED\n"); failures++; }
    if ((long long)(SM_CXMAXTRACK) != 59LL) { printf("FAIL constant SM_CXMAXTRACK\n"); failures++; }
    if ((long long)(SM_CXMENUCHECK) != 71LL) { printf("FAIL constant SM_CXMENUCHECK\n"); failures++; }
    if ((long long)(SM_CXMENUSIZE) != 54LL) { printf("FAIL constant SM_CXMENUSIZE\n"); failures++; }
    if ((long long)(SM_CXMIN) != 28LL) { printf("FAIL constant SM_CXMIN\n"); failures++; }
    if ((long long)(SM_CXMINIMIZED) != 57LL) { printf("FAIL constant SM_CXMINIMIZED\n"); failures++; }
    if ((long long)(SM_CXMINSPACING) != 47LL) { printf("FAIL constant SM_CXMINSPACING\n"); failures++; }
    if ((long long)(SM_CXMINTRACK) != 34LL) { printf("FAIL constant SM_CXMINTRACK\n"); failures++; }
    if ((long long)(SM_CXPADDEDBORDER) != 92LL) { printf("FAIL constant SM_CXPADDEDBORDER\n"); failures++; }
    if ((long long)(SM_CXSCREEN) != 0LL) { printf("FAIL constant SM_CXSCREEN\n"); failures++; }
    if ((long long)(SM_CXSIZE) != 30LL) { printf("FAIL constant SM_CXSIZE\n"); failures++; }
    if ((long long)(SM_CXSIZEFRAME) != 32LL) { printf("FAIL constant SM_CXSIZEFRAME\n"); failures++; }
    if ((long long)(SM_CXSMICON) != 49LL) { printf("FAIL constant SM_CXSMICON\n"); failures++; }
    if ((long long)(SM_CXSMSIZE) != 52LL) { printf("FAIL constant SM_CXSMSIZE\n"); failures++; }
    if ((long long)(SM_CXVIRTUALSCREEN) != 78LL) { printf("FAIL constant SM_CXVIRTUALSCREEN\n"); failures++; }
    if ((long long)(SM_CXVSCROLL) != 2LL) { printf("FAIL constant SM_CXVSCROLL\n"); failures++; }
    if ((long long)(SM_CYBORDER) != 6LL) { printf("FAIL constant SM_CYBORDER\n"); failures++; }
    if ((long long)(SM_CYCAPTION) != 4LL) { printf("FAIL constant SM_CYCAPTION\n"); failures++; }
    if ((long long)(SM_CYCURSOR) != 14LL) { printf("FAIL constant SM_CYCURSOR\n"); failures++; }
    if ((long long)(SM_CYDLGFRAME) != 8LL) { printf("FAIL constant SM_CYDLGFRAME\n"); failures++; }
    if ((long long)(SM_CYDOUBLECLK) != 37LL) { printf("FAIL constant SM_CYDOUBLECLK\n"); failures++; }
    if ((long long)(SM_CYDRAG) != 69LL) { printf("FAIL constant SM_CYDRAG\n"); failures++; }
    if ((long long)(SM_CYEDGE) != 46LL) { printf("FAIL constant SM_CYEDGE\n"); failures++; }
    if ((long long)(SM_CYFIXEDFRAME) != 8LL) { printf("FAIL constant SM_CYFIXEDFRAME\n"); failures++; }
    if ((long long)(SM_CYFOCUSBORDER) != 84LL) { printf("FAIL constant SM_CYFOCUSBORDER\n"); failures++; }
    if ((long long)(SM_CYFRAME) != 33LL) { printf("FAIL constant SM_CYFRAME\n"); failures++; }
    if ((long long)(SM_CYFULLSCREEN) != 17LL) { printf("FAIL constant SM_CYFULLSCREEN\n"); failures++; }
    if ((long long)(SM_CYHSCROLL) != 3LL) { printf("FAIL constant SM_CYHSCROLL\n"); failures++; }
    if ((long long)(SM_CYICON) != 12LL) { printf("FAIL constant SM_CYICON\n"); failures++; }
    if ((long long)(SM_CYICONSPACING) != 39LL) { printf("FAIL constant SM_CYICONSPACING\n"); failures++; }
    if ((long long)(SM_CYKANJIWINDOW) != 18LL) { printf("FAIL constant SM_CYKANJIWINDOW\n"); failures++; }
    if ((long long)(SM_CYMAXIMIZED) != 62LL) { printf("FAIL constant SM_CYMAXIMIZED\n"); failures++; }
    if ((long long)(SM_CYMAXTRACK) != 60LL) { printf("FAIL constant SM_CYMAXTRACK\n"); failures++; }
    if ((long long)(SM_CYMENU) != 15LL) { printf("FAIL constant SM_CYMENU\n"); failures++; }
    if ((long long)(SM_CYMENUCHECK) != 72LL) { printf("FAIL constant SM_CYMENUCHECK\n"); failures++; }
    if ((long long)(SM_CYMENUSIZE) != 55LL) { printf("FAIL constant SM_CYMENUSIZE\n"); failures++; }
    if ((long long)(SM_CYMIN) != 29LL) { printf("FAIL constant SM_CYMIN\n"); failures++; }
    if ((long long)(SM_CYMINIMIZED) != 58LL) { printf("FAIL constant SM_CYMINIMIZED\n"); failures++; }
    if ((long long)(SM_CYMINSPACING) != 48LL) { printf("FAIL constant SM_CYMINSPACING\n"); failures++; }
    if ((long long)(SM_CYMINTRACK) != 35LL) { printf("FAIL constant SM_CYMINTRACK\n"); failures++; }
    if ((long long)(SM_CYSCREEN) != 1LL) { printf("FAIL constant SM_CYSCREEN\n"); failures++; }
    if ((long long)(SM_CYSIZE) != 31LL) { printf("FAIL constant SM_CYSIZE\n"); failures++; }
    if ((long long)(SM_CYSIZEFRAME) != 33LL) { printf("FAIL constant SM_CYSIZEFRAME\n"); failures++; }
    if ((long long)(SM_CYSMCAPTION) != 51LL) { printf("FAIL constant SM_CYSMCAPTION\n"); failures++; }
    if ((long long)(SM_CYSMICON) != 50LL) { printf("FAIL constant SM_CYSMICON\n"); failures++; }
    if ((long long)(SM_CYSMSIZE) != 53LL) { printf("FAIL constant SM_CYSMSIZE\n"); failures++; }
    if ((long long)(SM_CYVIRTUALSCREEN) != 79LL) { printf("FAIL constant SM_CYVIRTUALSCREEN\n"); failures++; }
    if ((long long)(SM_CYVSCROLL) != 20LL) { printf("FAIL constant SM_CYVSCROLL\n"); failures++; }
    if ((long long)(SM_CYVTHUMB) != 9LL) { printf("FAIL constant SM_CYVTHUMB\n"); failures++; }
    if ((long long)(SM_DBCSENABLED) != 42LL) { printf("FAIL constant SM_DBCSENABLED\n"); failures++; }
    if ((long long)(SM_DEBUG) != 22LL) { printf("FAIL constant SM_DEBUG\n"); failures++; }
    if ((long long)(SM_DIGITIZER) != 94LL) { printf("FAIL constant SM_DIGITIZER\n"); failures++; }
    if ((long long)(SM_IMMENABLED) != 82LL) { printf("FAIL constant SM_IMMENABLED\n"); failures++; }
    if ((long long)(SM_MAXIMUMTOUCHES) != 95LL) { printf("FAIL constant SM_MAXIMUMTOUCHES\n"); failures++; }
    if ((long long)(SM_MEDIACENTER) != 87LL) { printf("FAIL constant SM_MEDIACENTER\n"); failures++; }
    if ((long long)(SM_MENUDROPALIGNMENT) != 40LL) { printf("FAIL constant SM_MENUDROPALIGNMENT\n"); failures++; }
    if ((long long)(SM_MIDEASTENABLED) != 74LL) { printf("FAIL constant SM_MIDEASTENABLED\n"); failures++; }
    if ((long long)(SM_MOUSEHORIZONTALWHEELPRESENT) != 91LL) { printf("FAIL constant SM_MOUSEHORIZONTALWHEELPRESENT\n"); failures++; }
    if ((long long)(SM_MOUSEPRESENT) != 19LL) { printf("FAIL constant SM_MOUSEPRESENT\n"); failures++; }
    if ((long long)(SM_MOUSEWHEELPRESENT) != 75LL) { printf("FAIL constant SM_MOUSEWHEELPRESENT\n"); failures++; }
    if ((long long)(SM_NETWORK) != 63LL) { printf("FAIL constant SM_NETWORK\n"); failures++; }
    if ((long long)(SM_PENWINDOWS) != 41LL) { printf("FAIL constant SM_PENWINDOWS\n"); failures++; }
    if ((long long)(SM_REMOTECONTROL) != 8193LL) { printf("FAIL constant SM_REMOTECONTROL\n"); failures++; }
    if ((long long)(SM_REMOTESESSION) != 4096LL) { printf("FAIL constant SM_REMOTESESSION\n"); failures++; }
    if ((long long)(SM_RESERVED1) != 24LL) { printf("FAIL constant SM_RESERVED1\n"); failures++; }
    if ((long long)(SM_RESERVED2) != 25LL) { printf("FAIL constant SM_RESERVED2\n"); failures++; }
    if ((long long)(SM_RESERVED3) != 26LL) { printf("FAIL constant SM_RESERVED3\n"); failures++; }
    if ((long long)(SM_RESERVED4) != 27LL) { printf("FAIL constant SM_RESERVED4\n"); failures++; }
    if ((long long)(SM_SAMEDISPLAYFORMAT) != 81LL) { printf("FAIL constant SM_SAMEDISPLAYFORMAT\n"); failures++; }
    if ((long long)(SM_SECURE) != 44LL) { printf("FAIL constant SM_SECURE\n"); failures++; }
    if ((long long)(SM_SERVERR2) != 89LL) { printf("FAIL constant SM_SERVERR2\n"); failures++; }
    if ((long long)(SM_SHOWSOUNDS) != 70LL) { printf("FAIL constant SM_SHOWSOUNDS\n"); failures++; }
    if ((long long)(SM_SHUTTINGDOWN) != 8192LL) { printf("FAIL constant SM_SHUTTINGDOWN\n"); failures++; }
    if ((long long)(SM_SLOWMACHINE) != 73LL) { printf("FAIL constant SM_SLOWMACHINE\n"); failures++; }
    if ((long long)(SM_STARTER) != 88LL) { printf("FAIL constant SM_STARTER\n"); failures++; }
    if ((long long)(SM_SWAPBUTTON) != 23LL) { printf("FAIL constant SM_SWAPBUTTON\n"); failures++; }
    if ((long long)(SM_SYSTEMDOCKED) != 8196LL) { printf("FAIL constant SM_SYSTEMDOCKED\n"); failures++; }
    if ((long long)(SM_TABLETPC) != 86LL) { printf("FAIL constant SM_TABLETPC\n"); failures++; }
    if ((long long)(SM_XVIRTUALSCREEN) != 76LL) { printf("FAIL constant SM_XVIRTUALSCREEN\n"); failures++; }
    if ((long long)(SM_YVIRTUALSCREEN) != 77LL) { printf("FAIL constant SM_YVIRTUALSCREEN\n"); failures++; }
    if ((long long)(SPIF_SENDCHANGE) != 2LL) { printf("FAIL constant SPIF_SENDCHANGE\n"); failures++; }
    if ((long long)(SPIF_SENDWININICHANGE) != 2LL) { printf("FAIL constant SPIF_SENDWININICHANGE\n"); failures++; }
    if ((long long)(SPIF_UPDATEINIFILE) != 1LL) { printf("FAIL constant SPIF_UPDATEINIFILE\n"); failures++; }
    if ((long long)(SPI_GETACCESSTIMEOUT) != 60LL) { printf("FAIL constant SPI_GETACCESSTIMEOUT\n"); failures++; }
    if ((long long)(SPI_GETACTIVEWINDOWTRACKING) != 4096LL) { printf("FAIL constant SPI_GETACTIVEWINDOWTRACKING\n"); failures++; }
    if ((long long)(SPI_GETACTIVEWNDTRKTIMEOUT) != 8194LL) { printf("FAIL constant SPI_GETACTIVEWNDTRKTIMEOUT\n"); failures++; }
    if ((long long)(SPI_GETACTIVEWNDTRKZORDER) != 4108LL) { printf("FAIL constant SPI_GETACTIVEWNDTRKZORDER\n"); failures++; }
    if ((long long)(SPI_GETANIMATION) != 72LL) { printf("FAIL constant SPI_GETANIMATION\n"); failures++; }
    if ((long long)(SPI_GETAUDIODESCRIPTION) != 116LL) { printf("FAIL constant SPI_GETAUDIODESCRIPTION\n"); failures++; }
    if ((long long)(SPI_GETBEEP) != 1LL) { printf("FAIL constant SPI_GETBEEP\n"); failures++; }
    if ((long long)(SPI_GETBLOCKSENDINPUTRESETS) != 4134LL) { printf("FAIL constant SPI_GETBLOCKSENDINPUTRESETS\n"); failures++; }
    if ((long long)(SPI_GETBORDER) != 5LL) { printf("FAIL constant SPI_GETBORDER\n"); failures++; }
    if ((long long)(SPI_GETCARETBROWSING) != 4172LL) { printf("FAIL constant SPI_GETCARETBROWSING\n"); failures++; }
    if ((long long)(SPI_GETCARETTIMEOUT) != 8226LL) { printf("FAIL constant SPI_GETCARETTIMEOUT\n"); failures++; }
    if ((long long)(SPI_GETCARETWIDTH) != 8198LL) { printf("FAIL constant SPI_GETCARETWIDTH\n"); failures++; }
    if ((long long)(SPI_GETCLEARTYPE) != 4168LL) { printf("FAIL constant SPI_GETCLEARTYPE\n"); failures++; }
    if ((long long)(SPI_GETCLIENTAREAANIMATION) != 4162LL) { printf("FAIL constant SPI_GETCLIENTAREAANIMATION\n"); failures++; }
    if ((long long)(SPI_GETCOMBOBOXANIMATION) != 4100LL) { printf("FAIL constant SPI_GETCOMBOBOXANIMATION\n"); failures++; }
    if ((long long)(SPI_GETCONTACTVISUALIZATION) != 8216LL) { printf("FAIL constant SPI_GETCONTACTVISUALIZATION\n"); failures++; }
    if ((long long)(SPI_GETCURSORSHADOW) != 4122LL) { printf("FAIL constant SPI_GETCURSORSHADOW\n"); failures++; }
    if ((long long)(SPI_GETDEFAULTINPUTLANG) != 89LL) { printf("FAIL constant SPI_GETDEFAULTINPUTLANG\n"); failures++; }
    if ((long long)(SPI_GETDESKWALLPAPER) != 115LL) { printf("FAIL constant SPI_GETDESKWALLPAPER\n"); failures++; }
    if ((long long)(SPI_GETDISABLEOVERLAPPEDCONTENT) != 4160LL) { printf("FAIL constant SPI_GETDISABLEOVERLAPPEDCONTENT\n"); failures++; }
    if ((long long)(SPI_GETDOCKMOVING) != 144LL) { printf("FAIL constant SPI_GETDOCKMOVING\n"); failures++; }
    if ((long long)(SPI_GETDRAGFROMMAXIMIZE) != 140LL) { printf("FAIL constant SPI_GETDRAGFROMMAXIMIZE\n"); failures++; }
    if ((long long)(SPI_GETDRAGFULLWINDOWS) != 38LL) { printf("FAIL constant SPI_GETDRAGFULLWINDOWS\n"); failures++; }
    if ((long long)(SPI_GETDROPSHADOW) != 4132LL) { printf("FAIL constant SPI_GETDROPSHADOW\n"); failures++; }
    if ((long long)(SPI_GETFASTTASKSWITCH) != 35LL) { printf("FAIL constant SPI_GETFASTTASKSWITCH\n"); failures++; }
    if ((long long)(SPI_GETFILTERKEYS) != 50LL) { printf("FAIL constant SPI_GETFILTERKEYS\n"); failures++; }
    if ((long long)(SPI_GETFLATMENU) != 4130LL) { printf("FAIL constant SPI_GETFLATMENU\n"); failures++; }
    if ((long long)(SPI_GETFOCUSBORDERHEIGHT) != 8208LL) { printf("FAIL constant SPI_GETFOCUSBORDERHEIGHT\n"); failures++; }
    if ((long long)(SPI_GETFOCUSBORDERWIDTH) != 8206LL) { printf("FAIL constant SPI_GETFOCUSBORDERWIDTH\n"); failures++; }
    if ((long long)(SPI_GETFONTSMOOTHING) != 74LL) { printf("FAIL constant SPI_GETFONTSMOOTHING\n"); failures++; }
    if ((long long)(SPI_GETFONTSMOOTHINGCONTRAST) != 8204LL) { printf("FAIL constant SPI_GETFONTSMOOTHINGCONTRAST\n"); failures++; }
    if ((long long)(SPI_GETFONTSMOOTHINGORIENTATION) != 8210LL) { printf("FAIL constant SPI_GETFONTSMOOTHINGORIENTATION\n"); failures++; }
    if ((long long)(SPI_GETFONTSMOOTHINGTYPE) != 8202LL) { printf("FAIL constant SPI_GETFONTSMOOTHINGTYPE\n"); failures++; }
    if ((long long)(SPI_GETFOREGROUNDFLASHCOUNT) != 8196LL) { printf("FAIL constant SPI_GETFOREGROUNDFLASHCOUNT\n"); failures++; }
    if ((long long)(SPI_GETFOREGROUNDLOCKTIMEOUT) != 8192LL) { printf("FAIL constant SPI_GETFOREGROUNDLOCKTIMEOUT\n"); failures++; }
    if ((long long)(SPI_GETGESTUREVISUALIZATION) != 8218LL) { printf("FAIL constant SPI_GETGESTUREVISUALIZATION\n"); failures++; }
    if ((long long)(SPI_GETGRADIENTCAPTIONS) != 4104LL) { printf("FAIL constant SPI_GETGRADIENTCAPTIONS\n"); failures++; }
    if ((long long)(SPI_GETGRIDGRANULARITY) != 18LL) { printf("FAIL constant SPI_GETGRIDGRANULARITY\n"); failures++; }
    if ((long long)(SPI_GETHANDEDNESS) != 8228LL) { printf("FAIL constant SPI_GETHANDEDNESS\n"); failures++; }
    if ((long long)(SPI_GETHIGHCONTRAST) != 66LL) { printf("FAIL constant SPI_GETHIGHCONTRAST\n"); failures++; }
    if ((long long)(SPI_GETHOTTRACKING) != 4110LL) { printf("FAIL constant SPI_GETHOTTRACKING\n"); failures++; }
    if ((long long)(SPI_GETHUNGAPPTIMEOUT) != 120LL) { printf("FAIL constant SPI_GETHUNGAPPTIMEOUT\n"); failures++; }
    if ((long long)(SPI_GETICONMETRICS) != 45LL) { printf("FAIL constant SPI_GETICONMETRICS\n"); failures++; }
    if ((long long)(SPI_GETICONTITLELOGFONT) != 31LL) { printf("FAIL constant SPI_GETICONTITLELOGFONT\n"); failures++; }
    if ((long long)(SPI_GETICONTITLEWRAP) != 25LL) { printf("FAIL constant SPI_GETICONTITLEWRAP\n"); failures++; }
    if ((long long)(SPI_GETKEYBOARDCUES) != 4106LL) { printf("FAIL constant SPI_GETKEYBOARDCUES\n"); failures++; }
    if ((long long)(SPI_GETKEYBOARDDELAY) != 22LL) { printf("FAIL constant SPI_GETKEYBOARDDELAY\n"); failures++; }
    if ((long long)(SPI_GETKEYBOARDPREF) != 68LL) { printf("FAIL constant SPI_GETKEYBOARDPREF\n"); failures++; }
    if ((long long)(SPI_GETKEYBOARDSPEED) != 10LL) { printf("FAIL constant SPI_GETKEYBOARDSPEED\n"); failures++; }
    if ((long long)(SPI_GETLISTBOXSMOOTHSCROLLING) != 4102LL) { printf("FAIL constant SPI_GETLISTBOXSMOOTHSCROLLING\n"); failures++; }
    if ((long long)(SPI_GETLOGICALDPIOVERRIDE) != 158LL) { printf("FAIL constant SPI_GETLOGICALDPIOVERRIDE\n"); failures++; }
    if ((long long)(SPI_GETLOWPOWERACTIVE) != 83LL) { printf("FAIL constant SPI_GETLOWPOWERACTIVE\n"); failures++; }
    if ((long long)(SPI_GETLOWPOWERTIMEOUT) != 79LL) { printf("FAIL constant SPI_GETLOWPOWERTIMEOUT\n"); failures++; }
    if ((long long)(SPI_GETMENUANIMATION) != 4098LL) { printf("FAIL constant SPI_GETMENUANIMATION\n"); failures++; }
    if ((long long)(SPI_GETMENUDROPALIGNMENT) != 27LL) { printf("FAIL constant SPI_GETMENUDROPALIGNMENT\n"); failures++; }
    if ((long long)(SPI_GETMENUFADE) != 4114LL) { printf("FAIL constant SPI_GETMENUFADE\n"); failures++; }
    if ((long long)(SPI_GETMENURECT) != 162LL) { printf("FAIL constant SPI_GETMENURECT\n"); failures++; }
    if ((long long)(SPI_GETMENUSHOWDELAY) != 106LL) { printf("FAIL constant SPI_GETMENUSHOWDELAY\n"); failures++; }
    if ((long long)(SPI_GETMENUUNDERLINES) != 4106LL) { printf("FAIL constant SPI_GETMENUUNDERLINES\n"); failures++; }
    if ((long long)(SPI_GETMESSAGEDURATION) != 8214LL) { printf("FAIL constant SPI_GETMESSAGEDURATION\n"); failures++; }
    if ((long long)(SPI_GETMINIMIZEDMETRICS) != 43LL) { printf("FAIL constant SPI_GETMINIMIZEDMETRICS\n"); failures++; }
    if ((long long)(SPI_GETMINIMUMHITRADIUS) != 8212LL) { printf("FAIL constant SPI_GETMINIMUMHITRADIUS\n"); failures++; }
    if ((long long)(SPI_GETMOUSE) != 3LL) { printf("FAIL constant SPI_GETMOUSE\n"); failures++; }
    if ((long long)(SPI_GETMOUSECLICKLOCK) != 4126LL) { printf("FAIL constant SPI_GETMOUSECLICKLOCK\n"); failures++; }
    if ((long long)(SPI_GETMOUSECLICKLOCKTIME) != 8200LL) { printf("FAIL constant SPI_GETMOUSECLICKLOCKTIME\n"); failures++; }
    if ((long long)(SPI_GETMOUSECORNERCLIPLENGTH) != 160LL) { printf("FAIL constant SPI_GETMOUSECORNERCLIPLENGTH\n"); failures++; }
    if ((long long)(SPI_GETMOUSEDOCKTHRESHOLD) != 126LL) { printf("FAIL constant SPI_GETMOUSEDOCKTHRESHOLD\n"); failures++; }
    if ((long long)(SPI_GETMOUSEDRAGOUTTHRESHOLD) != 132LL) { printf("FAIL constant SPI_GETMOUSEDRAGOUTTHRESHOLD\n"); failures++; }
    if ((long long)(SPI_GETMOUSEHOVERHEIGHT) != 100LL) { printf("FAIL constant SPI_GETMOUSEHOVERHEIGHT\n"); failures++; }
    if ((long long)(SPI_GETMOUSEHOVERTIME) != 102LL) { printf("FAIL constant SPI_GETMOUSEHOVERTIME\n"); failures++; }
    if ((long long)(SPI_GETMOUSEHOVERWIDTH) != 98LL) { printf("FAIL constant SPI_GETMOUSEHOVERWIDTH\n"); failures++; }
    if ((long long)(SPI_GETMOUSEKEYS) != 54LL) { printf("FAIL constant SPI_GETMOUSEKEYS\n"); failures++; }
    if ((long long)(SPI_GETMOUSESIDEMOVETHRESHOLD) != 136LL) { printf("FAIL constant SPI_GETMOUSESIDEMOVETHRESHOLD\n"); failures++; }
    if ((long long)(SPI_GETMOUSESONAR) != 4124LL) { printf("FAIL constant SPI_GETMOUSESONAR\n"); failures++; }
    if ((long long)(SPI_GETMOUSESPEED) != 112LL) { printf("FAIL constant SPI_GETMOUSESPEED\n"); failures++; }
    if ((long long)(SPI_GETMOUSETRAILS) != 94LL) { printf("FAIL constant SPI_GETMOUSETRAILS\n"); failures++; }
    if ((long long)(SPI_GETMOUSEVANISH) != 4128LL) { printf("FAIL constant SPI_GETMOUSEVANISH\n"); failures++; }
    if ((long long)(SPI_GETMOUSEWHEELROUTING) != 8220LL) { printf("FAIL constant SPI_GETMOUSEWHEELROUTING\n"); failures++; }
    if ((long long)(SPI_GETNONCLIENTMETRICS) != 41LL) { printf("FAIL constant SPI_GETNONCLIENTMETRICS\n"); failures++; }
    if ((long long)(SPI_GETPENARBITRATIONTYPE) != 8224LL) { printf("FAIL constant SPI_GETPENARBITRATIONTYPE\n"); failures++; }
    if ((long long)(SPI_GETPENDOCKTHRESHOLD) != 128LL) { printf("FAIL constant SPI_GETPENDOCKTHRESHOLD\n"); failures++; }
    if ((long long)(SPI_GETPENDRAGOUTTHRESHOLD) != 134LL) { printf("FAIL constant SPI_GETPENDRAGOUTTHRESHOLD\n"); failures++; }
    if ((long long)(SPI_GETPENSIDEMOVETHRESHOLD) != 138LL) { printf("FAIL constant SPI_GETPENSIDEMOVETHRESHOLD\n"); failures++; }
    if ((long long)(SPI_GETPENVISUALIZATION) != 8222LL) { printf("FAIL constant SPI_GETPENVISUALIZATION\n"); failures++; }
    if ((long long)(SPI_GETPOWEROFFACTIVE) != 84LL) { printf("FAIL constant SPI_GETPOWEROFFACTIVE\n"); failures++; }
    if ((long long)(SPI_GETPOWEROFFTIMEOUT) != 80LL) { printf("FAIL constant SPI_GETPOWEROFFTIMEOUT\n"); failures++; }
    if ((long long)(SPI_GETSCREENREADER) != 70LL) { printf("FAIL constant SPI_GETSCREENREADER\n"); failures++; }
    if ((long long)(SPI_GETSCREENSAVEACTIVE) != 16LL) { printf("FAIL constant SPI_GETSCREENSAVEACTIVE\n"); failures++; }
    if ((long long)(SPI_GETSCREENSAVERRUNNING) != 114LL) { printf("FAIL constant SPI_GETSCREENSAVERRUNNING\n"); failures++; }
    if ((long long)(SPI_GETSCREENSAVESECURE) != 118LL) { printf("FAIL constant SPI_GETSCREENSAVESECURE\n"); failures++; }
    if ((long long)(SPI_GETSCREENSAVETIMEOUT) != 14LL) { printf("FAIL constant SPI_GETSCREENSAVETIMEOUT\n"); failures++; }
    if ((long long)(SPI_GETSELECTIONFADE) != 4116LL) { printf("FAIL constant SPI_GETSELECTIONFADE\n"); failures++; }
    if ((long long)(SPI_GETSERIALKEYS) != 62LL) { printf("FAIL constant SPI_GETSERIALKEYS\n"); failures++; }
    if ((long long)(SPI_GETSHOWIMEUI) != 110LL) { printf("FAIL constant SPI_GETSHOWIMEUI\n"); failures++; }
    if ((long long)(SPI_GETSHOWSOUNDS) != 56LL) { printf("FAIL constant SPI_GETSHOWSOUNDS\n"); failures++; }
    if ((long long)(SPI_GETSNAPSIZING) != 142LL) { printf("FAIL constant SPI_GETSNAPSIZING\n"); failures++; }
    if ((long long)(SPI_GETSNAPTODEFBUTTON) != 95LL) { printf("FAIL constant SPI_GETSNAPTODEFBUTTON\n"); failures++; }
    if ((long long)(SPI_GETSOUNDSENTRY) != 64LL) { printf("FAIL constant SPI_GETSOUNDSENTRY\n"); failures++; }
    if ((long long)(SPI_GETSPEECHRECOGNITION) != 4170LL) { printf("FAIL constant SPI_GETSPEECHRECOGNITION\n"); failures++; }
    if ((long long)(SPI_GETSTICKYKEYS) != 58LL) { printf("FAIL constant SPI_GETSTICKYKEYS\n"); failures++; }
    if ((long long)(SPI_GETSYSTEMLANGUAGEBAR) != 4176LL) { printf("FAIL constant SPI_GETSYSTEMLANGUAGEBAR\n"); failures++; }
    if ((long long)(SPI_GETTHREADLOCALINPUTSETTINGS) != 4174LL) { printf("FAIL constant SPI_GETTHREADLOCALINPUTSETTINGS\n"); failures++; }
    if ((long long)(SPI_GETTOGGLEKEYS) != 52LL) { printf("FAIL constant SPI_GETTOGGLEKEYS\n"); failures++; }
    if ((long long)(SPI_GETTOOLTIPANIMATION) != 4118LL) { printf("FAIL constant SPI_GETTOOLTIPANIMATION\n"); failures++; }
    if ((long long)(SPI_GETTOOLTIPFADE) != 4120LL) { printf("FAIL constant SPI_GETTOOLTIPFADE\n"); failures++; }
    if ((long long)(SPI_GETTOUCHPREDICTIONPARAMETERS) != 156LL) { printf("FAIL constant SPI_GETTOUCHPREDICTIONPARAMETERS\n"); failures++; }
    if ((long long)(SPI_GETUIEFFECTS) != 4158LL) { printf("FAIL constant SPI_GETUIEFFECTS\n"); failures++; }
    if ((long long)(SPI_GETWAITTOKILLSERVICETIMEOUT) != 124LL) { printf("FAIL constant SPI_GETWAITTOKILLSERVICETIMEOUT\n"); failures++; }
    if ((long long)(SPI_GETWAITTOKILLTIMEOUT) != 122LL) { printf("FAIL constant SPI_GETWAITTOKILLTIMEOUT\n"); failures++; }
    if ((long long)(SPI_GETWHEELSCROLLCHARS) != 108LL) { printf("FAIL constant SPI_GETWHEELSCROLLCHARS\n"); failures++; }
    if ((long long)(SPI_GETWHEELSCROLLLINES) != 104LL) { printf("FAIL constant SPI_GETWHEELSCROLLLINES\n"); failures++; }
    if ((long long)(SPI_GETWINARRANGING) != 130LL) { printf("FAIL constant SPI_GETWINARRANGING\n"); failures++; }
    if ((long long)(SPI_GETWINDOWSEXTENSION) != 92LL) { printf("FAIL constant SPI_GETWINDOWSEXTENSION\n"); failures++; }
    if ((long long)(SPI_GETWORKAREA) != 48LL) { printf("FAIL constant SPI_GETWORKAREA\n"); failures++; }
    if ((long long)(SPI_ICONHORIZONTALSPACING) != 13LL) { printf("FAIL constant SPI_ICONHORIZONTALSPACING\n"); failures++; }
    if ((long long)(SPI_ICONVERTICALSPACING) != 24LL) { printf("FAIL constant SPI_ICONVERTICALSPACING\n"); failures++; }
    if ((long long)(SPI_LANGDRIVER) != 12LL) { printf("FAIL constant SPI_LANGDRIVER\n"); failures++; }
    if ((long long)(SPI_SCREENSAVERRUNNING) != 97LL) { printf("FAIL constant SPI_SCREENSAVERRUNNING\n"); failures++; }
    if ((long long)(SPI_SETACCESSTIMEOUT) != 61LL) { printf("FAIL constant SPI_SETACCESSTIMEOUT\n"); failures++; }
    if ((long long)(SPI_SETACTIVEWINDOWTRACKING) != 4097LL) { printf("FAIL constant SPI_SETACTIVEWINDOWTRACKING\n"); failures++; }
    if ((long long)(SPI_SETACTIVEWNDTRKTIMEOUT) != 8195LL) { printf("FAIL constant SPI_SETACTIVEWNDTRKTIMEOUT\n"); failures++; }
    if ((long long)(SPI_SETACTIVEWNDTRKZORDER) != 4109LL) { printf("FAIL constant SPI_SETACTIVEWNDTRKZORDER\n"); failures++; }
    if ((long long)(SPI_SETANIMATION) != 73LL) { printf("FAIL constant SPI_SETANIMATION\n"); failures++; }
    if ((long long)(SPI_SETAUDIODESCRIPTION) != 117LL) { printf("FAIL constant SPI_SETAUDIODESCRIPTION\n"); failures++; }
    if ((long long)(SPI_SETBEEP) != 2LL) { printf("FAIL constant SPI_SETBEEP\n"); failures++; }
    if ((long long)(SPI_SETBLOCKSENDINPUTRESETS) != 4135LL) { printf("FAIL constant SPI_SETBLOCKSENDINPUTRESETS\n"); failures++; }
    if ((long long)(SPI_SETBORDER) != 6LL) { printf("FAIL constant SPI_SETBORDER\n"); failures++; }
    if ((long long)(SPI_SETCARETBROWSING) != 4173LL) { printf("FAIL constant SPI_SETCARETBROWSING\n"); failures++; }
    if ((long long)(SPI_SETCARETTIMEOUT) != 8227LL) { printf("FAIL constant SPI_SETCARETTIMEOUT\n"); failures++; }
    if ((long long)(SPI_SETCARETWIDTH) != 8199LL) { printf("FAIL constant SPI_SETCARETWIDTH\n"); failures++; }
    if ((long long)(SPI_SETCLEARTYPE) != 4169LL) { printf("FAIL constant SPI_SETCLEARTYPE\n"); failures++; }
    if ((long long)(SPI_SETCLIENTAREAANIMATION) != 4163LL) { printf("FAIL constant SPI_SETCLIENTAREAANIMATION\n"); failures++; }
    if ((long long)(SPI_SETCOMBOBOXANIMATION) != 4101LL) { printf("FAIL constant SPI_SETCOMBOBOXANIMATION\n"); failures++; }
    if ((long long)(SPI_SETCONTACTVISUALIZATION) != 8217LL) { printf("FAIL constant SPI_SETCONTACTVISUALIZATION\n"); failures++; }
    if ((long long)(SPI_SETCURSORS) != 87LL) { printf("FAIL constant SPI_SETCURSORS\n"); failures++; }
    if ((long long)(SPI_SETCURSORSHADOW) != 4123LL) { printf("FAIL constant SPI_SETCURSORSHADOW\n"); failures++; }
    if ((long long)(SPI_SETDEFAULTINPUTLANG) != 90LL) { printf("FAIL constant SPI_SETDEFAULTINPUTLANG\n"); failures++; }
    if ((long long)(SPI_SETDESKPATTERN) != 21LL) { printf("FAIL constant SPI_SETDESKPATTERN\n"); failures++; }
    if ((long long)(SPI_SETDESKWALLPAPER) != 20LL) { printf("FAIL constant SPI_SETDESKWALLPAPER\n"); failures++; }
    if ((long long)(SPI_SETDISABLEOVERLAPPEDCONTENT) != 4161LL) { printf("FAIL constant SPI_SETDISABLEOVERLAPPEDCONTENT\n"); failures++; }
    if ((long long)(SPI_SETDOCKMOVING) != 145LL) { printf("FAIL constant SPI_SETDOCKMOVING\n"); failures++; }
    if ((long long)(SPI_SETDOUBLECLICKTIME) != 32LL) { printf("FAIL constant SPI_SETDOUBLECLICKTIME\n"); failures++; }
    if ((long long)(SPI_SETDOUBLECLKHEIGHT) != 30LL) { printf("FAIL constant SPI_SETDOUBLECLKHEIGHT\n"); failures++; }
    if ((long long)(SPI_SETDOUBLECLKWIDTH) != 29LL) { printf("FAIL constant SPI_SETDOUBLECLKWIDTH\n"); failures++; }
    if ((long long)(SPI_SETDRAGFROMMAXIMIZE) != 141LL) { printf("FAIL constant SPI_SETDRAGFROMMAXIMIZE\n"); failures++; }
    if ((long long)(SPI_SETDRAGFULLWINDOWS) != 37LL) { printf("FAIL constant SPI_SETDRAGFULLWINDOWS\n"); failures++; }
    if ((long long)(SPI_SETDRAGHEIGHT) != 77LL) { printf("FAIL constant SPI_SETDRAGHEIGHT\n"); failures++; }
    if ((long long)(SPI_SETDRAGWIDTH) != 76LL) { printf("FAIL constant SPI_SETDRAGWIDTH\n"); failures++; }
    if ((long long)(SPI_SETDROPSHADOW) != 4133LL) { printf("FAIL constant SPI_SETDROPSHADOW\n"); failures++; }
    if ((long long)(SPI_SETFASTTASKSWITCH) != 36LL) { printf("FAIL constant SPI_SETFASTTASKSWITCH\n"); failures++; }
    if ((long long)(SPI_SETFILTERKEYS) != 51LL) { printf("FAIL constant SPI_SETFILTERKEYS\n"); failures++; }
    if ((long long)(SPI_SETFLATMENU) != 4131LL) { printf("FAIL constant SPI_SETFLATMENU\n"); failures++; }
    if ((long long)(SPI_SETFOCUSBORDERHEIGHT) != 8209LL) { printf("FAIL constant SPI_SETFOCUSBORDERHEIGHT\n"); failures++; }
    if ((long long)(SPI_SETFOCUSBORDERWIDTH) != 8207LL) { printf("FAIL constant SPI_SETFOCUSBORDERWIDTH\n"); failures++; }
    if ((long long)(SPI_SETFONTSMOOTHING) != 75LL) { printf("FAIL constant SPI_SETFONTSMOOTHING\n"); failures++; }
    if ((long long)(SPI_SETFONTSMOOTHINGCONTRAST) != 8205LL) { printf("FAIL constant SPI_SETFONTSMOOTHINGCONTRAST\n"); failures++; }
    if ((long long)(SPI_SETFONTSMOOTHINGORIENTATION) != 8211LL) { printf("FAIL constant SPI_SETFONTSMOOTHINGORIENTATION\n"); failures++; }
    if ((long long)(SPI_SETFONTSMOOTHINGTYPE) != 8203LL) { printf("FAIL constant SPI_SETFONTSMOOTHINGTYPE\n"); failures++; }
    if ((long long)(SPI_SETFOREGROUNDFLASHCOUNT) != 8197LL) { printf("FAIL constant SPI_SETFOREGROUNDFLASHCOUNT\n"); failures++; }
    if ((long long)(SPI_SETFOREGROUNDLOCKTIMEOUT) != 8193LL) { printf("FAIL constant SPI_SETFOREGROUNDLOCKTIMEOUT\n"); failures++; }
    if ((long long)(SPI_SETGESTUREVISUALIZATION) != 8219LL) { printf("FAIL constant SPI_SETGESTUREVISUALIZATION\n"); failures++; }
    if ((long long)(SPI_SETGRADIENTCAPTIONS) != 4105LL) { printf("FAIL constant SPI_SETGRADIENTCAPTIONS\n"); failures++; }
    if ((long long)(SPI_SETGRIDGRANULARITY) != 19LL) { printf("FAIL constant SPI_SETGRIDGRANULARITY\n"); failures++; }
    if ((long long)(SPI_SETHANDEDNESS) != 8229LL) { printf("FAIL constant SPI_SETHANDEDNESS\n"); failures++; }
    if ((long long)(SPI_SETHANDHELD) != 78LL) { printf("FAIL constant SPI_SETHANDHELD\n"); failures++; }
    if ((long long)(SPI_SETHIGHCONTRAST) != 67LL) { printf("FAIL constant SPI_SETHIGHCONTRAST\n"); failures++; }
    if ((long long)(SPI_SETHOTTRACKING) != 4111LL) { printf("FAIL constant SPI_SETHOTTRACKING\n"); failures++; }
    if ((long long)(SPI_SETHUNGAPPTIMEOUT) != 121LL) { printf("FAIL constant SPI_SETHUNGAPPTIMEOUT\n"); failures++; }
    if ((long long)(SPI_SETICONMETRICS) != 46LL) { printf("FAIL constant SPI_SETICONMETRICS\n"); failures++; }
    if ((long long)(SPI_SETICONS) != 88LL) { printf("FAIL constant SPI_SETICONS\n"); failures++; }
    if ((long long)(SPI_SETICONTITLELOGFONT) != 34LL) { printf("FAIL constant SPI_SETICONTITLELOGFONT\n"); failures++; }
    if ((long long)(SPI_SETICONTITLEWRAP) != 26LL) { printf("FAIL constant SPI_SETICONTITLEWRAP\n"); failures++; }
    if ((long long)(SPI_SETKEYBOARDCUES) != 4107LL) { printf("FAIL constant SPI_SETKEYBOARDCUES\n"); failures++; }
    if ((long long)(SPI_SETKEYBOARDDELAY) != 23LL) { printf("FAIL constant SPI_SETKEYBOARDDELAY\n"); failures++; }
    if ((long long)(SPI_SETKEYBOARDPREF) != 69LL) { printf("FAIL constant SPI_SETKEYBOARDPREF\n"); failures++; }
    if ((long long)(SPI_SETKEYBOARDSPEED) != 11LL) { printf("FAIL constant SPI_SETKEYBOARDSPEED\n"); failures++; }
    if ((long long)(SPI_SETLANGTOGGLE) != 91LL) { printf("FAIL constant SPI_SETLANGTOGGLE\n"); failures++; }
    if ((long long)(SPI_SETLISTBOXSMOOTHSCROLLING) != 4103LL) { printf("FAIL constant SPI_SETLISTBOXSMOOTHSCROLLING\n"); failures++; }
    if ((long long)(SPI_SETLOGICALDPIOVERRIDE) != 159LL) { printf("FAIL constant SPI_SETLOGICALDPIOVERRIDE\n"); failures++; }
    if ((long long)(SPI_SETLOWPOWERACTIVE) != 85LL) { printf("FAIL constant SPI_SETLOWPOWERACTIVE\n"); failures++; }
    if ((long long)(SPI_SETLOWPOWERTIMEOUT) != 81LL) { printf("FAIL constant SPI_SETLOWPOWERTIMEOUT\n"); failures++; }
    if ((long long)(SPI_SETMENUANIMATION) != 4099LL) { printf("FAIL constant SPI_SETMENUANIMATION\n"); failures++; }
    if ((long long)(SPI_SETMENUDROPALIGNMENT) != 28LL) { printf("FAIL constant SPI_SETMENUDROPALIGNMENT\n"); failures++; }
    if ((long long)(SPI_SETMENUFADE) != 4115LL) { printf("FAIL constant SPI_SETMENUFADE\n"); failures++; }
    if ((long long)(SPI_SETMENURECT) != 163LL) { printf("FAIL constant SPI_SETMENURECT\n"); failures++; }
    if ((long long)(SPI_SETMENUSHOWDELAY) != 107LL) { printf("FAIL constant SPI_SETMENUSHOWDELAY\n"); failures++; }
    if ((long long)(SPI_SETMENUUNDERLINES) != 4107LL) { printf("FAIL constant SPI_SETMENUUNDERLINES\n"); failures++; }
    if ((long long)(SPI_SETMESSAGEDURATION) != 8215LL) { printf("FAIL constant SPI_SETMESSAGEDURATION\n"); failures++; }
    if ((long long)(SPI_SETMINIMIZEDMETRICS) != 44LL) { printf("FAIL constant SPI_SETMINIMIZEDMETRICS\n"); failures++; }
    if ((long long)(SPI_SETMINIMUMHITRADIUS) != 8213LL) { printf("FAIL constant SPI_SETMINIMUMHITRADIUS\n"); failures++; }
    if ((long long)(SPI_SETMOUSE) != 4LL) { printf("FAIL constant SPI_SETMOUSE\n"); failures++; }
    if ((long long)(SPI_SETMOUSEBUTTONSWAP) != 33LL) { printf("FAIL constant SPI_SETMOUSEBUTTONSWAP\n"); failures++; }
    if ((long long)(SPI_SETMOUSECLICKLOCK) != 4127LL) { printf("FAIL constant SPI_SETMOUSECLICKLOCK\n"); failures++; }
    if ((long long)(SPI_SETMOUSECLICKLOCKTIME) != 8201LL) { printf("FAIL constant SPI_SETMOUSECLICKLOCKTIME\n"); failures++; }
    if ((long long)(SPI_SETMOUSECORNERCLIPLENGTH) != 161LL) { printf("FAIL constant SPI_SETMOUSECORNERCLIPLENGTH\n"); failures++; }
    if ((long long)(SPI_SETMOUSEDOCKTHRESHOLD) != 127LL) { printf("FAIL constant SPI_SETMOUSEDOCKTHRESHOLD\n"); failures++; }
    if ((long long)(SPI_SETMOUSEDRAGOUTTHRESHOLD) != 133LL) { printf("FAIL constant SPI_SETMOUSEDRAGOUTTHRESHOLD\n"); failures++; }
    if ((long long)(SPI_SETMOUSEHOVERHEIGHT) != 101LL) { printf("FAIL constant SPI_SETMOUSEHOVERHEIGHT\n"); failures++; }
    if ((long long)(SPI_SETMOUSEHOVERTIME) != 103LL) { printf("FAIL constant SPI_SETMOUSEHOVERTIME\n"); failures++; }
    if ((long long)(SPI_SETMOUSEHOVERWIDTH) != 99LL) { printf("FAIL constant SPI_SETMOUSEHOVERWIDTH\n"); failures++; }
    if ((long long)(SPI_SETMOUSEKEYS) != 55LL) { printf("FAIL constant SPI_SETMOUSEKEYS\n"); failures++; }
    if ((long long)(SPI_SETMOUSESIDEMOVETHRESHOLD) != 137LL) { printf("FAIL constant SPI_SETMOUSESIDEMOVETHRESHOLD\n"); failures++; }
    if ((long long)(SPI_SETMOUSESONAR) != 4125LL) { printf("FAIL constant SPI_SETMOUSESONAR\n"); failures++; }
    if ((long long)(SPI_SETMOUSESPEED) != 113LL) { printf("FAIL constant SPI_SETMOUSESPEED\n"); failures++; }
    if ((long long)(SPI_SETMOUSETRAILS) != 93LL) { printf("FAIL constant SPI_SETMOUSETRAILS\n"); failures++; }
    if ((long long)(SPI_SETMOUSEVANISH) != 4129LL) { printf("FAIL constant SPI_SETMOUSEVANISH\n"); failures++; }
    if ((long long)(SPI_SETMOUSEWHEELROUTING) != 8221LL) { printf("FAIL constant SPI_SETMOUSEWHEELROUTING\n"); failures++; }
    if ((long long)(SPI_SETNONCLIENTMETRICS) != 42LL) { printf("FAIL constant SPI_SETNONCLIENTMETRICS\n"); failures++; }
    if ((long long)(SPI_SETPENARBITRATIONTYPE) != 8225LL) { printf("FAIL constant SPI_SETPENARBITRATIONTYPE\n"); failures++; }
    if ((long long)(SPI_SETPENDOCKTHRESHOLD) != 129LL) { printf("FAIL constant SPI_SETPENDOCKTHRESHOLD\n"); failures++; }
    if ((long long)(SPI_SETPENDRAGOUTTHRESHOLD) != 135LL) { printf("FAIL constant SPI_SETPENDRAGOUTTHRESHOLD\n"); failures++; }
    if ((long long)(SPI_SETPENSIDEMOVETHRESHOLD) != 139LL) { printf("FAIL constant SPI_SETPENSIDEMOVETHRESHOLD\n"); failures++; }
    if ((long long)(SPI_SETPENVISUALIZATION) != 8223LL) { printf("FAIL constant SPI_SETPENVISUALIZATION\n"); failures++; }
    if ((long long)(SPI_SETPENWINDOWS) != 49LL) { printf("FAIL constant SPI_SETPENWINDOWS\n"); failures++; }
    if ((long long)(SPI_SETPOWEROFFACTIVE) != 86LL) { printf("FAIL constant SPI_SETPOWEROFFACTIVE\n"); failures++; }
    if ((long long)(SPI_SETPOWEROFFTIMEOUT) != 82LL) { printf("FAIL constant SPI_SETPOWEROFFTIMEOUT\n"); failures++; }
    if ((long long)(SPI_SETSCREENREADER) != 71LL) { printf("FAIL constant SPI_SETSCREENREADER\n"); failures++; }
    if ((long long)(SPI_SETSCREENSAVEACTIVE) != 17LL) { printf("FAIL constant SPI_SETSCREENSAVEACTIVE\n"); failures++; }
    if ((long long)(SPI_SETSCREENSAVERRUNNING) != 97LL) { printf("FAIL constant SPI_SETSCREENSAVERRUNNING\n"); failures++; }
    if ((long long)(SPI_SETSCREENSAVESECURE) != 119LL) { printf("FAIL constant SPI_SETSCREENSAVESECURE\n"); failures++; }
    if ((long long)(SPI_SETSCREENSAVETIMEOUT) != 15LL) { printf("FAIL constant SPI_SETSCREENSAVETIMEOUT\n"); failures++; }
    if ((long long)(SPI_SETSELECTIONFADE) != 4117LL) { printf("FAIL constant SPI_SETSELECTIONFADE\n"); failures++; }
    if ((long long)(SPI_SETSERIALKEYS) != 63LL) { printf("FAIL constant SPI_SETSERIALKEYS\n"); failures++; }
    if ((long long)(SPI_SETSHOWIMEUI) != 111LL) { printf("FAIL constant SPI_SETSHOWIMEUI\n"); failures++; }
    if ((long long)(SPI_SETSHOWSOUNDS) != 57LL) { printf("FAIL constant SPI_SETSHOWSOUNDS\n"); failures++; }
    if ((long long)(SPI_SETSNAPSIZING) != 143LL) { printf("FAIL constant SPI_SETSNAPSIZING\n"); failures++; }
    if ((long long)(SPI_SETSNAPTODEFBUTTON) != 96LL) { printf("FAIL constant SPI_SETSNAPTODEFBUTTON\n"); failures++; }
    if ((long long)(SPI_SETSOUNDSENTRY) != 65LL) { printf("FAIL constant SPI_SETSOUNDSENTRY\n"); failures++; }
    if ((long long)(SPI_SETSPEECHRECOGNITION) != 4171LL) { printf("FAIL constant SPI_SETSPEECHRECOGNITION\n"); failures++; }
    if ((long long)(SPI_SETSTICKYKEYS) != 59LL) { printf("FAIL constant SPI_SETSTICKYKEYS\n"); failures++; }
    if ((long long)(SPI_SETSYSTEMLANGUAGEBAR) != 4177LL) { printf("FAIL constant SPI_SETSYSTEMLANGUAGEBAR\n"); failures++; }
    if ((long long)(SPI_SETTHREADLOCALINPUTSETTINGS) != 4175LL) { printf("FAIL constant SPI_SETTHREADLOCALINPUTSETTINGS\n"); failures++; }
    if ((long long)(SPI_SETTOGGLEKEYS) != 53LL) { printf("FAIL constant SPI_SETTOGGLEKEYS\n"); failures++; }
    if ((long long)(SPI_SETTOOLTIPANIMATION) != 4119LL) { printf("FAIL constant SPI_SETTOOLTIPANIMATION\n"); failures++; }
    if ((long long)(SPI_SETTOOLTIPFADE) != 4121LL) { printf("FAIL constant SPI_SETTOOLTIPFADE\n"); failures++; }
    if ((long long)(SPI_SETTOUCHPREDICTIONPARAMETERS) != 157LL) { printf("FAIL constant SPI_SETTOUCHPREDICTIONPARAMETERS\n"); failures++; }
    if ((long long)(SPI_SETUIEFFECTS) != 4159LL) { printf("FAIL constant SPI_SETUIEFFECTS\n"); failures++; }
    if ((long long)(SPI_SETWAITTOKILLSERVICETIMEOUT) != 125LL) { printf("FAIL constant SPI_SETWAITTOKILLSERVICETIMEOUT\n"); failures++; }
    if ((long long)(SPI_SETWAITTOKILLTIMEOUT) != 123LL) { printf("FAIL constant SPI_SETWAITTOKILLTIMEOUT\n"); failures++; }
    if ((long long)(SPI_SETWHEELSCROLLCHARS) != 109LL) { printf("FAIL constant SPI_SETWHEELSCROLLCHARS\n"); failures++; }
    if ((long long)(SPI_SETWHEELSCROLLLINES) != 105LL) { printf("FAIL constant SPI_SETWHEELSCROLLLINES\n"); failures++; }
    if ((long long)(SPI_SETWINARRANGING) != 131LL) { printf("FAIL constant SPI_SETWINARRANGING\n"); failures++; }
    if ((long long)(SPI_SETWORKAREA) != 47LL) { printf("FAIL constant SPI_SETWORKAREA\n"); failures++; }
    if ((long long)(SWP_ASYNCWINDOWPOS) != 16384LL) { printf("FAIL constant SWP_ASYNCWINDOWPOS\n"); failures++; }
    if ((long long)(SWP_DEFERERASE) != 8192LL) { printf("FAIL constant SWP_DEFERERASE\n"); failures++; }
    if ((long long)(SWP_DRAWFRAME) != 32LL) { printf("FAIL constant SWP_DRAWFRAME\n"); failures++; }
    if ((long long)(SWP_FRAMECHANGED) != 32LL) { printf("FAIL constant SWP_FRAMECHANGED\n"); failures++; }
    if ((long long)(SWP_HIDEWINDOW) != 128LL) { printf("FAIL constant SWP_HIDEWINDOW\n"); failures++; }
    if ((long long)(SWP_NOACTIVATE) != 16LL) { printf("FAIL constant SWP_NOACTIVATE\n"); failures++; }
    if ((long long)(SWP_NOCOPYBITS) != 256LL) { printf("FAIL constant SWP_NOCOPYBITS\n"); failures++; }
    if ((long long)(SWP_NOMOVE) != 2LL) { printf("FAIL constant SWP_NOMOVE\n"); failures++; }
    if ((long long)(SWP_NOOWNERZORDER) != 512LL) { printf("FAIL constant SWP_NOOWNERZORDER\n"); failures++; }
    if ((long long)(SWP_NOREDRAW) != 8LL) { printf("FAIL constant SWP_NOREDRAW\n"); failures++; }
    if ((long long)(SWP_NOREPOSITION) != 512LL) { printf("FAIL constant SWP_NOREPOSITION\n"); failures++; }
    if ((long long)(SWP_NOSENDCHANGING) != 1024LL) { printf("FAIL constant SWP_NOSENDCHANGING\n"); failures++; }
    if ((long long)(SWP_NOSIZE) != 1LL) { printf("FAIL constant SWP_NOSIZE\n"); failures++; }
    if ((long long)(SWP_NOZORDER) != 4LL) { printf("FAIL constant SWP_NOZORDER\n"); failures++; }
    if ((long long)(SWP_SHOWWINDOW) != 64LL) { printf("FAIL constant SWP_SHOWWINDOW\n"); failures++; }
    if ((long long)(SW_ERASE) != 4LL) { printf("FAIL constant SW_ERASE\n"); failures++; }
    if ((long long)(SW_FORCEMINIMIZE) != 11LL) { printf("FAIL constant SW_FORCEMINIMIZE\n"); failures++; }
    if ((long long)(SW_HIDE) != 0LL) { printf("FAIL constant SW_HIDE\n"); failures++; }
    if ((long long)(SW_INVALIDATE) != 2LL) { printf("FAIL constant SW_INVALIDATE\n"); failures++; }
    if ((long long)(SW_MAX) != 11LL) { printf("FAIL constant SW_MAX\n"); failures++; }
    if ((long long)(SW_MAXIMIZE) != 3LL) { printf("FAIL constant SW_MAXIMIZE\n"); failures++; }
    if ((long long)(SW_MINIMIZE) != 6LL) { printf("FAIL constant SW_MINIMIZE\n"); failures++; }
    if ((long long)(SW_NORMAL) != 1LL) { printf("FAIL constant SW_NORMAL\n"); failures++; }
    if ((long long)(SW_OTHERUNZOOM) != 4LL) { printf("FAIL constant SW_OTHERUNZOOM\n"); failures++; }
    if ((long long)(SW_OTHERZOOM) != 2LL) { printf("FAIL constant SW_OTHERZOOM\n"); failures++; }
    if ((long long)(SW_PARENTCLOSING) != 1LL) { printf("FAIL constant SW_PARENTCLOSING\n"); failures++; }
    if ((long long)(SW_PARENTOPENING) != 3LL) { printf("FAIL constant SW_PARENTOPENING\n"); failures++; }
    if ((long long)(SW_RESTORE) != 9LL) { printf("FAIL constant SW_RESTORE\n"); failures++; }
    if ((long long)(SW_SCROLLCHILDREN) != 1LL) { printf("FAIL constant SW_SCROLLCHILDREN\n"); failures++; }
    if ((long long)(SW_SHOW) != 5LL) { printf("FAIL constant SW_SHOW\n"); failures++; }
    if ((long long)(SW_SHOWDEFAULT) != 10LL) { printf("FAIL constant SW_SHOWDEFAULT\n"); failures++; }
    if ((long long)(SW_SHOWMAXIMIZED) != 3LL) { printf("FAIL constant SW_SHOWMAXIMIZED\n"); failures++; }
    if ((long long)(SW_SHOWMINIMIZED) != 2LL) { printf("FAIL constant SW_SHOWMINIMIZED\n"); failures++; }
    if ((long long)(SW_SHOWMINNOACTIVE) != 7LL) { printf("FAIL constant SW_SHOWMINNOACTIVE\n"); failures++; }
    if ((long long)(SW_SHOWNA) != 8LL) { printf("FAIL constant SW_SHOWNA\n"); failures++; }
    if ((long long)(SW_SHOWNOACTIVATE) != 4LL) { printf("FAIL constant SW_SHOWNOACTIVATE\n"); failures++; }
    if ((long long)(SW_SHOWNORMAL) != 1LL) { printf("FAIL constant SW_SHOWNORMAL\n"); failures++; }
    if ((long long)(SW_SMOOTHSCROLL) != 16LL) { printf("FAIL constant SW_SMOOTHSCROLL\n"); failures++; }
    if ((long long)(ScrollAmount_LargeDecrement) != 0LL) { printf("FAIL constant ScrollAmount_LargeDecrement\n"); failures++; }
    if ((long long)(ScrollAmount_LargeIncrement) != 3LL) { printf("FAIL constant ScrollAmount_LargeIncrement\n"); failures++; }
    if ((long long)(ScrollAmount_NoAmount) != 2LL) { printf("FAIL constant ScrollAmount_NoAmount\n"); failures++; }
    if ((long long)(ScrollAmount_SmallDecrement) != 1LL) { printf("FAIL constant ScrollAmount_SmallDecrement\n"); failures++; }
    if ((long long)(ScrollAmount_SmallIncrement) != 4LL) { printf("FAIL constant ScrollAmount_SmallIncrement\n"); failures++; }
    if ((long long)(StructureChangeType_ChildAdded) != 0LL) { printf("FAIL constant StructureChangeType_ChildAdded\n"); failures++; }
    if ((long long)(StructureChangeType_ChildRemoved) != 1LL) { printf("FAIL constant StructureChangeType_ChildRemoved\n"); failures++; }
    if ((long long)(StructureChangeType_ChildrenBulkAdded) != 3LL) { printf("FAIL constant StructureChangeType_ChildrenBulkAdded\n"); failures++; }
    if ((long long)(StructureChangeType_ChildrenBulkRemoved) != 4LL) { printf("FAIL constant StructureChangeType_ChildrenBulkRemoved\n"); failures++; }
    if ((long long)(StructureChangeType_ChildrenInvalidated) != 2LL) { printf("FAIL constant StructureChangeType_ChildrenInvalidated\n"); failures++; }
    if ((long long)(StructureChangeType_ChildrenReordered) != 5LL) { printf("FAIL constant StructureChangeType_ChildrenReordered\n"); failures++; }
    if ((long long)(SupportedTextSelection_Multiple) != 2LL) { printf("FAIL constant SupportedTextSelection_Multiple\n"); failures++; }
    if ((long long)(SupportedTextSelection_None) != 0LL) { printf("FAIL constant SupportedTextSelection_None\n"); failures++; }
    if ((long long)(SupportedTextSelection_Single) != 1LL) { printf("FAIL constant SupportedTextSelection_Single\n"); failures++; }
    if ((long long)(TME_CANCEL) != 2147483648LL) { printf("FAIL constant TME_CANCEL\n"); failures++; }
    if ((long long)(TME_HOVER) != 1LL) { printf("FAIL constant TME_HOVER\n"); failures++; }
    if ((long long)(TME_LEAVE) != 2LL) { printf("FAIL constant TME_LEAVE\n"); failures++; }
    if ((long long)(TME_NONCLIENT) != 16LL) { printf("FAIL constant TME_NONCLIENT\n"); failures++; }
    if ((long long)(TME_QUERY) != 1073741824LL) { printf("FAIL constant TME_QUERY\n"); failures++; }
    if ((long long)(TOUCH_FLAG_NONE) != 0LL) { printf("FAIL constant TOUCH_FLAG_NONE\n"); failures++; }
    if ((long long)(TOUCH_MASK_CONTACTAREA) != 1LL) { printf("FAIL constant TOUCH_MASK_CONTACTAREA\n"); failures++; }
    if ((long long)(TOUCH_MASK_NONE) != 0LL) { printf("FAIL constant TOUCH_MASK_NONE\n"); failures++; }
    if ((long long)(TOUCH_MASK_ORIENTATION) != 2LL) { printf("FAIL constant TOUCH_MASK_ORIENTATION\n"); failures++; }
    if ((long long)(TOUCH_MASK_PRESSURE) != 4LL) { printf("FAIL constant TOUCH_MASK_PRESSURE\n"); failures++; }
    if ((long long)(TPM_BOTTOMALIGN) != 32LL) { printf("FAIL constant TPM_BOTTOMALIGN\n"); failures++; }
    if ((long long)(TPM_CENTERALIGN) != 4LL) { printf("FAIL constant TPM_CENTERALIGN\n"); failures++; }
    if ((long long)(TPM_HORIZONTAL) != 0LL) { printf("FAIL constant TPM_HORIZONTAL\n"); failures++; }
    if ((long long)(TPM_HORNEGANIMATION) != 2048LL) { printf("FAIL constant TPM_HORNEGANIMATION\n"); failures++; }
    if ((long long)(TPM_HORPOSANIMATION) != 1024LL) { printf("FAIL constant TPM_HORPOSANIMATION\n"); failures++; }
    if ((long long)(TPM_LAYOUTRTL) != 32768LL) { printf("FAIL constant TPM_LAYOUTRTL\n"); failures++; }
    if ((long long)(TPM_LEFTALIGN) != 0LL) { printf("FAIL constant TPM_LEFTALIGN\n"); failures++; }
    if ((long long)(TPM_LEFTBUTTON) != 0LL) { printf("FAIL constant TPM_LEFTBUTTON\n"); failures++; }
    if ((long long)(TPM_NOANIMATION) != 16384LL) { printf("FAIL constant TPM_NOANIMATION\n"); failures++; }
    if ((long long)(TPM_NONOTIFY) != 128LL) { printf("FAIL constant TPM_NONOTIFY\n"); failures++; }
    if ((long long)(TPM_RECURSE) != 1LL) { printf("FAIL constant TPM_RECURSE\n"); failures++; }
    if ((long long)(TPM_RETURNCMD) != 256LL) { printf("FAIL constant TPM_RETURNCMD\n"); failures++; }
    if ((long long)(TPM_RIGHTALIGN) != 8LL) { printf("FAIL constant TPM_RIGHTALIGN\n"); failures++; }
    if ((long long)(TPM_RIGHTBUTTON) != 2LL) { printf("FAIL constant TPM_RIGHTBUTTON\n"); failures++; }
    if ((long long)(TPM_TOPALIGN) != 0LL) { printf("FAIL constant TPM_TOPALIGN\n"); failures++; }
    if ((long long)(TPM_VCENTERALIGN) != 16LL) { printf("FAIL constant TPM_VCENTERALIGN\n"); failures++; }
    if ((long long)(TPM_VERNEGANIMATION) != 8192LL) { printf("FAIL constant TPM_VERNEGANIMATION\n"); failures++; }
    if ((long long)(TPM_VERPOSANIMATION) != 4096LL) { printf("FAIL constant TPM_VERPOSANIMATION\n"); failures++; }
    if ((long long)(TPM_VERTICAL) != 64LL) { printf("FAIL constant TPM_VERTICAL\n"); failures++; }
    if ((long long)(TPM_WORKAREA) != 65536LL) { printf("FAIL constant TPM_WORKAREA\n"); failures++; }
    if ((long long)(TYMED_ENHMF) != 64LL) { printf("FAIL constant TYMED_ENHMF\n"); failures++; }
    if ((long long)(TYMED_FILE) != 2LL) { printf("FAIL constant TYMED_FILE\n"); failures++; }
    if ((long long)(TYMED_GDI) != 16LL) { printf("FAIL constant TYMED_GDI\n"); failures++; }
    if ((long long)(TYMED_HGLOBAL) != 1LL) { printf("FAIL constant TYMED_HGLOBAL\n"); failures++; }
    if ((long long)(TYMED_ISTORAGE) != 8LL) { printf("FAIL constant TYMED_ISTORAGE\n"); failures++; }
    if ((long long)(TYMED_ISTREAM) != 4LL) { printf("FAIL constant TYMED_ISTREAM\n"); failures++; }
    if ((long long)(TYMED_MFPICT) != 32LL) { printf("FAIL constant TYMED_MFPICT\n"); failures++; }
    if ((long long)(TYMED_NULL) != 0LL) { printf("FAIL constant TYMED_NULL\n"); failures++; }
    if ((long long)(TextPatternRangeEndpoint_End) != 1LL) { printf("FAIL constant TextPatternRangeEndpoint_End\n"); failures++; }
    if ((long long)(TextPatternRangeEndpoint_Start) != 0LL) { printf("FAIL constant TextPatternRangeEndpoint_Start\n"); failures++; }
    if ((long long)(TextUnit_Character) != 0LL) { printf("FAIL constant TextUnit_Character\n"); failures++; }
    if ((long long)(TextUnit_Document) != 6LL) { printf("FAIL constant TextUnit_Document\n"); failures++; }
    if ((long long)(TextUnit_Format) != 1LL) { printf("FAIL constant TextUnit_Format\n"); failures++; }
    if ((long long)(TextUnit_Line) != 3LL) { printf("FAIL constant TextUnit_Line\n"); failures++; }
    if ((long long)(TextUnit_Page) != 5LL) { printf("FAIL constant TextUnit_Page\n"); failures++; }
    if ((long long)(TextUnit_Paragraph) != 4LL) { printf("FAIL constant TextUnit_Paragraph\n"); failures++; }
    if ((long long)(TextUnit_Word) != 2LL) { printf("FAIL constant TextUnit_Word\n"); failures++; }
    if ((long long)(ToggleState_Indeterminate) != 2LL) { printf("FAIL constant ToggleState_Indeterminate\n"); failures++; }
    if ((long long)(ToggleState_Off) != 0LL) { printf("FAIL constant ToggleState_Off\n"); failures++; }
    if ((long long)(ToggleState_On) != 1LL) { printf("FAIL constant ToggleState_On\n"); failures++; }
    if ((long long)(UIA_AcceleratorKeyPropertyId) != 30006LL) { printf("FAIL constant UIA_AcceleratorKeyPropertyId\n"); failures++; }
    if ((long long)(UIA_AccessKeyPropertyId) != 30007LL) { printf("FAIL constant UIA_AccessKeyPropertyId\n"); failures++; }
    if ((long long)(UIA_AfterParagraphSpacingAttributeId) != 40042LL) { printf("FAIL constant UIA_AfterParagraphSpacingAttributeId\n"); failures++; }
    if ((long long)(UIA_AnimationStyleAttributeId) != 40000LL) { printf("FAIL constant UIA_AnimationStyleAttributeId\n"); failures++; }
    if ((long long)(UIA_AnnotationAnnotationTypeIdPropertyId) != 30113LL) { printf("FAIL constant UIA_AnnotationAnnotationTypeIdPropertyId\n"); failures++; }
    if ((long long)(UIA_AnnotationAnnotationTypeNamePropertyId) != 30114LL) { printf("FAIL constant UIA_AnnotationAnnotationTypeNamePropertyId\n"); failures++; }
    if ((long long)(UIA_AnnotationAuthorPropertyId) != 30115LL) { printf("FAIL constant UIA_AnnotationAuthorPropertyId\n"); failures++; }
    if ((long long)(UIA_AnnotationDateTimePropertyId) != 30116LL) { printf("FAIL constant UIA_AnnotationDateTimePropertyId\n"); failures++; }
    if ((long long)(UIA_AnnotationObjectsAttributeId) != 40032LL) { printf("FAIL constant UIA_AnnotationObjectsAttributeId\n"); failures++; }
    if ((long long)(UIA_AnnotationObjectsPropertyId) != 30156LL) { printf("FAIL constant UIA_AnnotationObjectsPropertyId\n"); failures++; }
    if ((long long)(UIA_AnnotationPatternId) != 10023LL) { printf("FAIL constant UIA_AnnotationPatternId\n"); failures++; }
    if ((long long)(UIA_AnnotationTargetPropertyId) != 30117LL) { printf("FAIL constant UIA_AnnotationTargetPropertyId\n"); failures++; }
    if ((long long)(UIA_AnnotationTypesAttributeId) != 40031LL) { printf("FAIL constant UIA_AnnotationTypesAttributeId\n"); failures++; }
    if ((long long)(UIA_AnnotationTypesPropertyId) != 30155LL) { printf("FAIL constant UIA_AnnotationTypesPropertyId\n"); failures++; }
    if ((long long)(UIA_AppBarControlTypeId) != 50040LL) { printf("FAIL constant UIA_AppBarControlTypeId\n"); failures++; }
    if ((long long)(UIA_AriaPropertiesPropertyId) != 30102LL) { printf("FAIL constant UIA_AriaPropertiesPropertyId\n"); failures++; }
    if ((long long)(UIA_AriaRolePropertyId) != 30101LL) { printf("FAIL constant UIA_AriaRolePropertyId\n"); failures++; }
    if ((long long)(UIA_AsyncContentLoadedEventId) != 20006LL) { printf("FAIL constant UIA_AsyncContentLoadedEventId\n"); failures++; }
    if ((long long)(UIA_AutomationFocusChangedEventId) != 20005LL) { printf("FAIL constant UIA_AutomationFocusChangedEventId\n"); failures++; }
    if ((long long)(UIA_AutomationIdPropertyId) != 30011LL) { printf("FAIL constant UIA_AutomationIdPropertyId\n"); failures++; }
    if ((long long)(UIA_AutomationPropertyChangedEventId) != 20004LL) { printf("FAIL constant UIA_AutomationPropertyChangedEventId\n"); failures++; }
    if ((long long)(UIA_BackgroundColorAttributeId) != 40001LL) { printf("FAIL constant UIA_BackgroundColorAttributeId\n"); failures++; }
    if ((long long)(UIA_BeforeParagraphSpacingAttributeId) != 40041LL) { printf("FAIL constant UIA_BeforeParagraphSpacingAttributeId\n"); failures++; }
    if ((long long)(UIA_BoundingRectanglePropertyId) != 30001LL) { printf("FAIL constant UIA_BoundingRectanglePropertyId\n"); failures++; }
    if ((long long)(UIA_BulletStyleAttributeId) != 40002LL) { printf("FAIL constant UIA_BulletStyleAttributeId\n"); failures++; }
    if ((long long)(UIA_ButtonControlTypeId) != 50000LL) { printf("FAIL constant UIA_ButtonControlTypeId\n"); failures++; }
    if ((long long)(UIA_CalendarControlTypeId) != 50001LL) { printf("FAIL constant UIA_CalendarControlTypeId\n"); failures++; }
    if ((long long)(UIA_CapStyleAttributeId) != 40003LL) { printf("FAIL constant UIA_CapStyleAttributeId\n"); failures++; }
    if ((long long)(UIA_CaretBidiModeAttributeId) != 40039LL) { printf("FAIL constant UIA_CaretBidiModeAttributeId\n"); failures++; }
    if ((long long)(UIA_CaretPositionAttributeId) != 40038LL) { printf("FAIL constant UIA_CaretPositionAttributeId\n"); failures++; }
    if ((long long)(UIA_CenterPointPropertyId) != 30165LL) { printf("FAIL constant UIA_CenterPointPropertyId\n"); failures++; }
    if ((long long)(UIA_ChangesEventId) != 20034LL) { printf("FAIL constant UIA_ChangesEventId\n"); failures++; }
    if ((long long)(UIA_CheckBoxControlTypeId) != 50002LL) { printf("FAIL constant UIA_CheckBoxControlTypeId\n"); failures++; }
    if ((long long)(UIA_ClassNamePropertyId) != 30012LL) { printf("FAIL constant UIA_ClassNamePropertyId\n"); failures++; }
    if ((long long)(UIA_ClickablePointPropertyId) != 30014LL) { printf("FAIL constant UIA_ClickablePointPropertyId\n"); failures++; }
    if ((long long)(UIA_ComboBoxControlTypeId) != 50003LL) { printf("FAIL constant UIA_ComboBoxControlTypeId\n"); failures++; }
    if ((long long)(UIA_ControlTypePropertyId) != 30003LL) { printf("FAIL constant UIA_ControlTypePropertyId\n"); failures++; }
    if ((long long)(UIA_ControllerForPropertyId) != 30104LL) { printf("FAIL constant UIA_ControllerForPropertyId\n"); failures++; }
    if ((long long)(UIA_CultureAttributeId) != 40004LL) { printf("FAIL constant UIA_CultureAttributeId\n"); failures++; }
    if ((long long)(UIA_CulturePropertyId) != 30015LL) { printf("FAIL constant UIA_CulturePropertyId\n"); failures++; }
    if ((long long)(UIA_CustomControlTypeId) != 50025LL) { printf("FAIL constant UIA_CustomControlTypeId\n"); failures++; }
    if ((long long)(UIA_CustomLandmarkTypeId) != 80000LL) { printf("FAIL constant UIA_CustomLandmarkTypeId\n"); failures++; }
    if ((long long)(UIA_CustomNavigationPatternId) != 10033LL) { printf("FAIL constant UIA_CustomNavigationPatternId\n"); failures++; }
    if ((long long)(UIA_DataGridControlTypeId) != 50028LL) { printf("FAIL constant UIA_DataGridControlTypeId\n"); failures++; }
    if ((long long)(UIA_DataItemControlTypeId) != 50029LL) { printf("FAIL constant UIA_DataItemControlTypeId\n"); failures++; }
    if ((long long)(UIA_DescribedByPropertyId) != 30105LL) { printf("FAIL constant UIA_DescribedByPropertyId\n"); failures++; }
    if ((long long)(UIA_DockDockPositionPropertyId) != 30069LL) { printf("FAIL constant UIA_DockDockPositionPropertyId\n"); failures++; }
    if ((long long)(UIA_DockPatternId) != 10011LL) { printf("FAIL constant UIA_DockPatternId\n"); failures++; }
    if ((long long)(UIA_DocumentControlTypeId) != 50030LL) { printf("FAIL constant UIA_DocumentControlTypeId\n"); failures++; }
    if ((long long)(UIA_DragDropEffectPropertyId) != 30139LL) { printf("FAIL constant UIA_DragDropEffectPropertyId\n"); failures++; }
    if ((long long)(UIA_DragDropEffectsPropertyId) != 30140LL) { printf("FAIL constant UIA_DragDropEffectsPropertyId\n"); failures++; }
    if ((long long)(UIA_DragGrabbedItemsPropertyId) != 30144LL) { printf("FAIL constant UIA_DragGrabbedItemsPropertyId\n"); failures++; }
    if ((long long)(UIA_DragIsGrabbedPropertyId) != 30138LL) { printf("FAIL constant UIA_DragIsGrabbedPropertyId\n"); failures++; }
    if ((long long)(UIA_DragPatternId) != 10030LL) { printf("FAIL constant UIA_DragPatternId\n"); failures++; }
    if ((long long)(UIA_Drag_DragCancelEventId) != 20027LL) { printf("FAIL constant UIA_Drag_DragCancelEventId\n"); failures++; }
    if ((long long)(UIA_Drag_DragCompleteEventId) != 20028LL) { printf("FAIL constant UIA_Drag_DragCompleteEventId\n"); failures++; }
    if ((long long)(UIA_Drag_DragStartEventId) != 20026LL) { printf("FAIL constant UIA_Drag_DragStartEventId\n"); failures++; }
    if ((long long)(UIA_DropTargetDropTargetEffectPropertyId) != 30142LL) { printf("FAIL constant UIA_DropTargetDropTargetEffectPropertyId\n"); failures++; }
    if ((long long)(UIA_DropTargetDropTargetEffectsPropertyId) != 30143LL) { printf("FAIL constant UIA_DropTargetDropTargetEffectsPropertyId\n"); failures++; }
    if ((long long)(UIA_DropTargetPatternId) != 10031LL) { printf("FAIL constant UIA_DropTargetPatternId\n"); failures++; }
    if ((long long)(UIA_DropTarget_DragEnterEventId) != 20029LL) { printf("FAIL constant UIA_DropTarget_DragEnterEventId\n"); failures++; }
    if ((long long)(UIA_DropTarget_DragLeaveEventId) != 20030LL) { printf("FAIL constant UIA_DropTarget_DragLeaveEventId\n"); failures++; }
    if ((long long)(UIA_DropTarget_DroppedEventId) != 20031LL) { printf("FAIL constant UIA_DropTarget_DroppedEventId\n"); failures++; }
    if ((long long)(UIA_E_ELEMENTNOTAVAILABLE) != 2147746305LL) { printf("FAIL constant UIA_E_ELEMENTNOTAVAILABLE\n"); failures++; }
    if ((long long)(UIA_E_ELEMENTNOTENABLED) != 2147746304LL) { printf("FAIL constant UIA_E_ELEMENTNOTENABLED\n"); failures++; }
    if ((long long)(UIA_E_INVALIDOPERATION) != 2148734217LL) { printf("FAIL constant UIA_E_INVALIDOPERATION\n"); failures++; }
    if ((long long)(UIA_E_NOCLICKABLEPOINT) != 2147746306LL) { printf("FAIL constant UIA_E_NOCLICKABLEPOINT\n"); failures++; }
    if ((long long)(UIA_E_NOTSUPPORTED) != 2147746308LL) { printf("FAIL constant UIA_E_NOTSUPPORTED\n"); failures++; }
    if ((long long)(UIA_E_PROXYASSEMBLYNOTLOADED) != 2147746307LL) { printf("FAIL constant UIA_E_PROXYASSEMBLYNOTLOADED\n"); failures++; }
    if ((long long)(UIA_E_TIMEOUT) != 2148734213LL) { printf("FAIL constant UIA_E_TIMEOUT\n"); failures++; }
    if ((long long)(UIA_EditControlTypeId) != 50004LL) { printf("FAIL constant UIA_EditControlTypeId\n"); failures++; }
    if ((long long)(UIA_ExpandCollapseExpandCollapseStatePropertyId) != 30070LL) { printf("FAIL constant UIA_ExpandCollapseExpandCollapseStatePropertyId\n"); failures++; }
    if ((long long)(UIA_ExpandCollapsePatternId) != 10005LL) { printf("FAIL constant UIA_ExpandCollapsePatternId\n"); failures++; }
    if ((long long)(UIA_FillColorPropertyId) != 30160LL) { printf("FAIL constant UIA_FillColorPropertyId\n"); failures++; }
    if ((long long)(UIA_FillTypePropertyId) != 30162LL) { printf("FAIL constant UIA_FillTypePropertyId\n"); failures++; }
    if ((long long)(UIA_FlowsFromPropertyId) != 30148LL) { printf("FAIL constant UIA_FlowsFromPropertyId\n"); failures++; }
    if ((long long)(UIA_FlowsToPropertyId) != 30106LL) { printf("FAIL constant UIA_FlowsToPropertyId\n"); failures++; }
    if ((long long)(UIA_FontNameAttributeId) != 40005LL) { printf("FAIL constant UIA_FontNameAttributeId\n"); failures++; }
    if ((long long)(UIA_FontSizeAttributeId) != 40006LL) { printf("FAIL constant UIA_FontSizeAttributeId\n"); failures++; }
    if ((long long)(UIA_FontWeightAttributeId) != 40007LL) { printf("FAIL constant UIA_FontWeightAttributeId\n"); failures++; }
    if ((long long)(UIA_ForegroundColorAttributeId) != 40008LL) { printf("FAIL constant UIA_ForegroundColorAttributeId\n"); failures++; }
    if ((long long)(UIA_FormLandmarkTypeId) != 80001LL) { printf("FAIL constant UIA_FormLandmarkTypeId\n"); failures++; }
    if ((long long)(UIA_FrameworkIdPropertyId) != 30024LL) { printf("FAIL constant UIA_FrameworkIdPropertyId\n"); failures++; }
    if ((long long)(UIA_FullDescriptionPropertyId) != 30159LL) { printf("FAIL constant UIA_FullDescriptionPropertyId\n"); failures++; }
    if ((long long)(UIA_GridColumnCountPropertyId) != 30063LL) { printf("FAIL constant UIA_GridColumnCountPropertyId\n"); failures++; }
    if ((long long)(UIA_GridItemColumnPropertyId) != 30065LL) { printf("FAIL constant UIA_GridItemColumnPropertyId\n"); failures++; }
    if ((long long)(UIA_GridItemColumnSpanPropertyId) != 30067LL) { printf("FAIL constant UIA_GridItemColumnSpanPropertyId\n"); failures++; }
    if ((long long)(UIA_GridItemContainingGridPropertyId) != 30068LL) { printf("FAIL constant UIA_GridItemContainingGridPropertyId\n"); failures++; }
    if ((long long)(UIA_GridItemPatternId) != 10007LL) { printf("FAIL constant UIA_GridItemPatternId\n"); failures++; }
    if ((long long)(UIA_GridItemRowPropertyId) != 30064LL) { printf("FAIL constant UIA_GridItemRowPropertyId\n"); failures++; }
    if ((long long)(UIA_GridItemRowSpanPropertyId) != 30066LL) { printf("FAIL constant UIA_GridItemRowSpanPropertyId\n"); failures++; }
    if ((long long)(UIA_GridPatternId) != 10006LL) { printf("FAIL constant UIA_GridPatternId\n"); failures++; }
    if ((long long)(UIA_GridRowCountPropertyId) != 30062LL) { printf("FAIL constant UIA_GridRowCountPropertyId\n"); failures++; }
    if ((long long)(UIA_GroupControlTypeId) != 50026LL) { printf("FAIL constant UIA_GroupControlTypeId\n"); failures++; }
    if ((long long)(UIA_HasKeyboardFocusPropertyId) != 30008LL) { printf("FAIL constant UIA_HasKeyboardFocusPropertyId\n"); failures++; }
    if ((long long)(UIA_HeaderControlTypeId) != 50034LL) { printf("FAIL constant UIA_HeaderControlTypeId\n"); failures++; }
    if ((long long)(UIA_HeaderItemControlTypeId) != 50035LL) { printf("FAIL constant UIA_HeaderItemControlTypeId\n"); failures++; }
    if ((long long)(UIA_HeadingLevelPropertyId) != 30173LL) { printf("FAIL constant UIA_HeadingLevelPropertyId\n"); failures++; }
    if ((long long)(UIA_HelpTextPropertyId) != 30013LL) { printf("FAIL constant UIA_HelpTextPropertyId\n"); failures++; }
    if ((long long)(UIA_HorizontalTextAlignmentAttributeId) != 40009LL) { printf("FAIL constant UIA_HorizontalTextAlignmentAttributeId\n"); failures++; }
    if ((long long)(UIA_HostedFragmentRootsInvalidatedEventId) != 20025LL) { printf("FAIL constant UIA_HostedFragmentRootsInvalidatedEventId\n"); failures++; }
    if ((long long)(UIA_HyperlinkControlTypeId) != 50005LL) { printf("FAIL constant UIA_HyperlinkControlTypeId\n"); failures++; }
    if ((long long)(UIA_ImageControlTypeId) != 50006LL) { printf("FAIL constant UIA_ImageControlTypeId\n"); failures++; }
    if ((long long)(UIA_IndentationFirstLineAttributeId) != 40010LL) { printf("FAIL constant UIA_IndentationFirstLineAttributeId\n"); failures++; }
    if ((long long)(UIA_IndentationLeadingAttributeId) != 40011LL) { printf("FAIL constant UIA_IndentationLeadingAttributeId\n"); failures++; }
    if ((long long)(UIA_IndentationTrailingAttributeId) != 40012LL) { printf("FAIL constant UIA_IndentationTrailingAttributeId\n"); failures++; }
    if ((long long)(UIA_InputDiscardedEventId) != 20022LL) { printf("FAIL constant UIA_InputDiscardedEventId\n"); failures++; }
    if ((long long)(UIA_InputReachedOtherElementEventId) != 20021LL) { printf("FAIL constant UIA_InputReachedOtherElementEventId\n"); failures++; }
    if ((long long)(UIA_InputReachedTargetEventId) != 20020LL) { printf("FAIL constant UIA_InputReachedTargetEventId\n"); failures++; }
    if ((long long)(UIA_InvokePatternId) != 10000LL) { printf("FAIL constant UIA_InvokePatternId\n"); failures++; }
    if ((long long)(UIA_Invoke_InvokedEventId) != 20009LL) { printf("FAIL constant UIA_Invoke_InvokedEventId\n"); failures++; }
    if ((long long)(UIA_IsActiveAttributeId) != 40036LL) { printf("FAIL constant UIA_IsActiveAttributeId\n"); failures++; }
    if ((long long)(UIA_IsAnnotationPatternAvailablePropertyId) != 30118LL) { printf("FAIL constant UIA_IsAnnotationPatternAvailablePropertyId\n"); failures++; }
    if ((long long)(UIA_IsContentElementPropertyId) != 30017LL) { printf("FAIL constant UIA_IsContentElementPropertyId\n"); failures++; }
    if ((long long)(UIA_IsControlElementPropertyId) != 30016LL) { printf("FAIL constant UIA_IsControlElementPropertyId\n"); failures++; }
    if ((long long)(UIA_IsCustomNavigationPatternAvailablePropertyId) != 30151LL) { printf("FAIL constant UIA_IsCustomNavigationPatternAvailablePropertyId\n"); failures++; }
    if ((long long)(UIA_IsDataValidForFormPropertyId) != 30103LL) { printf("FAIL constant UIA_IsDataValidForFormPropertyId\n"); failures++; }
    if ((long long)(UIA_IsDialogPropertyId) != 30174LL) { printf("FAIL constant UIA_IsDialogPropertyId\n"); failures++; }
    if ((long long)(UIA_IsDockPatternAvailablePropertyId) != 30027LL) { printf("FAIL constant UIA_IsDockPatternAvailablePropertyId\n"); failures++; }
    if ((long long)(UIA_IsDragPatternAvailablePropertyId) != 30137LL) { printf("FAIL constant UIA_IsDragPatternAvailablePropertyId\n"); failures++; }
    if ((long long)(UIA_IsDropTargetPatternAvailablePropertyId) != 30141LL) { printf("FAIL constant UIA_IsDropTargetPatternAvailablePropertyId\n"); failures++; }
    if ((long long)(UIA_IsEnabledPropertyId) != 30010LL) { printf("FAIL constant UIA_IsEnabledPropertyId\n"); failures++; }
    if ((long long)(UIA_IsExpandCollapsePatternAvailablePropertyId) != 30028LL) { printf("FAIL constant UIA_IsExpandCollapsePatternAvailablePropertyId\n"); failures++; }
    if ((long long)(UIA_IsGridItemPatternAvailablePropertyId) != 30029LL) { printf("FAIL constant UIA_IsGridItemPatternAvailablePropertyId\n"); failures++; }
    if ((long long)(UIA_IsGridPatternAvailablePropertyId) != 30030LL) { printf("FAIL constant UIA_IsGridPatternAvailablePropertyId\n"); failures++; }
    if ((long long)(UIA_IsHiddenAttributeId) != 40013LL) { printf("FAIL constant UIA_IsHiddenAttributeId\n"); failures++; }
    if ((long long)(UIA_IsInvokePatternAvailablePropertyId) != 30031LL) { printf("FAIL constant UIA_IsInvokePatternAvailablePropertyId\n"); failures++; }
    if ((long long)(UIA_IsItalicAttributeId) != 40014LL) { printf("FAIL constant UIA_IsItalicAttributeId\n"); failures++; }
    if ((long long)(UIA_IsItemContainerPatternAvailablePropertyId) != 30108LL) { printf("FAIL constant UIA_IsItemContainerPatternAvailablePropertyId\n"); failures++; }
    if ((long long)(UIA_IsKeyboardFocusablePropertyId) != 30009LL) { printf("FAIL constant UIA_IsKeyboardFocusablePropertyId\n"); failures++; }
    if ((long long)(UIA_IsLegacyIAccessiblePatternAvailablePropertyId) != 30090LL) { printf("FAIL constant UIA_IsLegacyIAccessiblePatternAvailablePropertyId\n"); failures++; }
    if ((long long)(UIA_IsMultipleViewPatternAvailablePropertyId) != 30032LL) { printf("FAIL constant UIA_IsMultipleViewPatternAvailablePropertyId\n"); failures++; }
    if ((long long)(UIA_IsObjectModelPatternAvailablePropertyId) != 30112LL) { printf("FAIL constant UIA_IsObjectModelPatternAvailablePropertyId\n"); failures++; }
    if ((long long)(UIA_IsOffscreenPropertyId) != 30022LL) { printf("FAIL constant UIA_IsOffscreenPropertyId\n"); failures++; }
    if ((long long)(UIA_IsPasswordPropertyId) != 30019LL) { printf("FAIL constant UIA_IsPasswordPropertyId\n"); failures++; }
    if ((long long)(UIA_IsPeripheralPropertyId) != 30150LL) { printf("FAIL constant UIA_IsPeripheralPropertyId\n"); failures++; }
    if ((long long)(UIA_IsRangeValuePatternAvailablePropertyId) != 30033LL) { printf("FAIL constant UIA_IsRangeValuePatternAvailablePropertyId\n"); failures++; }
    if ((long long)(UIA_IsReadOnlyAttributeId) != 40015LL) { printf("FAIL constant UIA_IsReadOnlyAttributeId\n"); failures++; }
    if ((long long)(UIA_IsRequiredForFormPropertyId) != 30025LL) { printf("FAIL constant UIA_IsRequiredForFormPropertyId\n"); failures++; }
    if ((long long)(UIA_IsScrollItemPatternAvailablePropertyId) != 30035LL) { printf("FAIL constant UIA_IsScrollItemPatternAvailablePropertyId\n"); failures++; }
    if ((long long)(UIA_IsScrollPatternAvailablePropertyId) != 30034LL) { printf("FAIL constant UIA_IsScrollPatternAvailablePropertyId\n"); failures++; }
    if ((long long)(UIA_IsSelectionItemPatternAvailablePropertyId) != 30036LL) { printf("FAIL constant UIA_IsSelectionItemPatternAvailablePropertyId\n"); failures++; }
    if ((long long)(UIA_IsSelectionPattern2AvailablePropertyId) != 30168LL) { printf("FAIL constant UIA_IsSelectionPattern2AvailablePropertyId\n"); failures++; }
    if ((long long)(UIA_IsSelectionPatternAvailablePropertyId) != 30037LL) { printf("FAIL constant UIA_IsSelectionPatternAvailablePropertyId\n"); failures++; }
    if ((long long)(UIA_IsSpreadsheetItemPatternAvailablePropertyId) != 30132LL) { printf("FAIL constant UIA_IsSpreadsheetItemPatternAvailablePropertyId\n"); failures++; }
    if ((long long)(UIA_IsSpreadsheetPatternAvailablePropertyId) != 30128LL) { printf("FAIL constant UIA_IsSpreadsheetPatternAvailablePropertyId\n"); failures++; }
    if ((long long)(UIA_IsStylesPatternAvailablePropertyId) != 30127LL) { printf("FAIL constant UIA_IsStylesPatternAvailablePropertyId\n"); failures++; }
    if ((long long)(UIA_IsSubscriptAttributeId) != 40016LL) { printf("FAIL constant UIA_IsSubscriptAttributeId\n"); failures++; }
    if ((long long)(UIA_IsSuperscriptAttributeId) != 40017LL) { printf("FAIL constant UIA_IsSuperscriptAttributeId\n"); failures++; }
    if ((long long)(UIA_IsSynchronizedInputPatternAvailablePropertyId) != 30110LL) { printf("FAIL constant UIA_IsSynchronizedInputPatternAvailablePropertyId\n"); failures++; }
    if ((long long)(UIA_IsTableItemPatternAvailablePropertyId) != 30039LL) { printf("FAIL constant UIA_IsTableItemPatternAvailablePropertyId\n"); failures++; }
    if ((long long)(UIA_IsTablePatternAvailablePropertyId) != 30038LL) { printf("FAIL constant UIA_IsTablePatternAvailablePropertyId\n"); failures++; }
    if ((long long)(UIA_IsTextChildPatternAvailablePropertyId) != 30136LL) { printf("FAIL constant UIA_IsTextChildPatternAvailablePropertyId\n"); failures++; }
    if ((long long)(UIA_IsTextEditPatternAvailablePropertyId) != 30149LL) { printf("FAIL constant UIA_IsTextEditPatternAvailablePropertyId\n"); failures++; }
    if ((long long)(UIA_IsTextPattern2AvailablePropertyId) != 30119LL) { printf("FAIL constant UIA_IsTextPattern2AvailablePropertyId\n"); failures++; }
    if ((long long)(UIA_IsTextPatternAvailablePropertyId) != 30040LL) { printf("FAIL constant UIA_IsTextPatternAvailablePropertyId\n"); failures++; }
    if ((long long)(UIA_IsTogglePatternAvailablePropertyId) != 30041LL) { printf("FAIL constant UIA_IsTogglePatternAvailablePropertyId\n"); failures++; }
    if ((long long)(UIA_IsTransformPattern2AvailablePropertyId) != 30134LL) { printf("FAIL constant UIA_IsTransformPattern2AvailablePropertyId\n"); failures++; }
    if ((long long)(UIA_IsTransformPatternAvailablePropertyId) != 30042LL) { printf("FAIL constant UIA_IsTransformPatternAvailablePropertyId\n"); failures++; }
    if ((long long)(UIA_IsValuePatternAvailablePropertyId) != 30043LL) { printf("FAIL constant UIA_IsValuePatternAvailablePropertyId\n"); failures++; }
    if ((long long)(UIA_IsVirtualizedItemPatternAvailablePropertyId) != 30109LL) { printf("FAIL constant UIA_IsVirtualizedItemPatternAvailablePropertyId\n"); failures++; }
    if ((long long)(UIA_IsWindowPatternAvailablePropertyId) != 30044LL) { printf("FAIL constant UIA_IsWindowPatternAvailablePropertyId\n"); failures++; }
    if ((long long)(UIA_ItemContainerPatternId) != 10019LL) { printf("FAIL constant UIA_ItemContainerPatternId\n"); failures++; }
    if ((long long)(UIA_ItemStatusPropertyId) != 30026LL) { printf("FAIL constant UIA_ItemStatusPropertyId\n"); failures++; }
    if ((long long)(UIA_ItemTypePropertyId) != 30021LL) { printf("FAIL constant UIA_ItemTypePropertyId\n"); failures++; }
    if ((long long)(UIA_LabeledByPropertyId) != 30018LL) { printf("FAIL constant UIA_LabeledByPropertyId\n"); failures++; }
    if ((long long)(UIA_LandmarkTypePropertyId) != 30157LL) { printf("FAIL constant UIA_LandmarkTypePropertyId\n"); failures++; }
    if ((long long)(UIA_LayoutInvalidatedEventId) != 20008LL) { printf("FAIL constant UIA_LayoutInvalidatedEventId\n"); failures++; }
    if ((long long)(UIA_LegacyIAccessibleChildIdPropertyId) != 30091LL) { printf("FAIL constant UIA_LegacyIAccessibleChildIdPropertyId\n"); failures++; }
    if ((long long)(UIA_LegacyIAccessibleDefaultActionPropertyId) != 30100LL) { printf("FAIL constant UIA_LegacyIAccessibleDefaultActionPropertyId\n"); failures++; }
    if ((long long)(UIA_LegacyIAccessibleDescriptionPropertyId) != 30094LL) { printf("FAIL constant UIA_LegacyIAccessibleDescriptionPropertyId\n"); failures++; }
    if ((long long)(UIA_LegacyIAccessibleHelpPropertyId) != 30097LL) { printf("FAIL constant UIA_LegacyIAccessibleHelpPropertyId\n"); failures++; }
    if ((long long)(UIA_LegacyIAccessibleKeyboardShortcutPropertyId) != 30098LL) { printf("FAIL constant UIA_LegacyIAccessibleKeyboardShortcutPropertyId\n"); failures++; }
    if ((long long)(UIA_LegacyIAccessibleNamePropertyId) != 30092LL) { printf("FAIL constant UIA_LegacyIAccessibleNamePropertyId\n"); failures++; }
    if ((long long)(UIA_LegacyIAccessiblePatternId) != 10018LL) { printf("FAIL constant UIA_LegacyIAccessiblePatternId\n"); failures++; }
    if ((long long)(UIA_LegacyIAccessibleRolePropertyId) != 30095LL) { printf("FAIL constant UIA_LegacyIAccessibleRolePropertyId\n"); failures++; }
    if ((long long)(UIA_LegacyIAccessibleSelectionPropertyId) != 30099LL) { printf("FAIL constant UIA_LegacyIAccessibleSelectionPropertyId\n"); failures++; }
    if ((long long)(UIA_LegacyIAccessibleStatePropertyId) != 30096LL) { printf("FAIL constant UIA_LegacyIAccessibleStatePropertyId\n"); failures++; }
    if ((long long)(UIA_LegacyIAccessibleValuePropertyId) != 30093LL) { printf("FAIL constant UIA_LegacyIAccessibleValuePropertyId\n"); failures++; }
    if ((long long)(UIA_LevelPropertyId) != 30154LL) { printf("FAIL constant UIA_LevelPropertyId\n"); failures++; }
    if ((long long)(UIA_LineSpacingAttributeId) != 40040LL) { printf("FAIL constant UIA_LineSpacingAttributeId\n"); failures++; }
    if ((long long)(UIA_LinkAttributeId) != 40035LL) { printf("FAIL constant UIA_LinkAttributeId\n"); failures++; }
    if ((long long)(UIA_ListControlTypeId) != 50008LL) { printf("FAIL constant UIA_ListControlTypeId\n"); failures++; }
    if ((long long)(UIA_ListItemControlTypeId) != 50007LL) { printf("FAIL constant UIA_ListItemControlTypeId\n"); failures++; }
    if ((long long)(UIA_LiveRegionChangedEventId) != 20024LL) { printf("FAIL constant UIA_LiveRegionChangedEventId\n"); failures++; }
    if ((long long)(UIA_LiveSettingPropertyId) != 30135LL) { printf("FAIL constant UIA_LiveSettingPropertyId\n"); failures++; }
    if ((long long)(UIA_LocalizedControlTypePropertyId) != 30004LL) { printf("FAIL constant UIA_LocalizedControlTypePropertyId\n"); failures++; }
    if ((long long)(UIA_LocalizedLandmarkTypePropertyId) != 30158LL) { printf("FAIL constant UIA_LocalizedLandmarkTypePropertyId\n"); failures++; }
    if ((long long)(UIA_MainLandmarkTypeId) != 80002LL) { printf("FAIL constant UIA_MainLandmarkTypeId\n"); failures++; }
    if ((long long)(UIA_MarginBottomAttributeId) != 40018LL) { printf("FAIL constant UIA_MarginBottomAttributeId\n"); failures++; }
    if ((long long)(UIA_MarginLeadingAttributeId) != 40019LL) { printf("FAIL constant UIA_MarginLeadingAttributeId\n"); failures++; }
    if ((long long)(UIA_MarginTopAttributeId) != 40020LL) { printf("FAIL constant UIA_MarginTopAttributeId\n"); failures++; }
    if ((long long)(UIA_MarginTrailingAttributeId) != 40021LL) { printf("FAIL constant UIA_MarginTrailingAttributeId\n"); failures++; }
    if ((long long)(UIA_MenuBarControlTypeId) != 50010LL) { printf("FAIL constant UIA_MenuBarControlTypeId\n"); failures++; }
    if ((long long)(UIA_MenuClosedEventId) != 20007LL) { printf("FAIL constant UIA_MenuClosedEventId\n"); failures++; }
    if ((long long)(UIA_MenuControlTypeId) != 50009LL) { printf("FAIL constant UIA_MenuControlTypeId\n"); failures++; }
    if ((long long)(UIA_MenuItemControlTypeId) != 50011LL) { printf("FAIL constant UIA_MenuItemControlTypeId\n"); failures++; }
    if ((long long)(UIA_MenuModeEndEventId) != 20019LL) { printf("FAIL constant UIA_MenuModeEndEventId\n"); failures++; }
    if ((long long)(UIA_MenuModeStartEventId) != 20018LL) { printf("FAIL constant UIA_MenuModeStartEventId\n"); failures++; }
    if ((long long)(UIA_MenuOpenedEventId) != 20003LL) { printf("FAIL constant UIA_MenuOpenedEventId\n"); failures++; }
    if ((long long)(UIA_MultipleViewCurrentViewPropertyId) != 30071LL) { printf("FAIL constant UIA_MultipleViewCurrentViewPropertyId\n"); failures++; }
    if ((long long)(UIA_MultipleViewPatternId) != 10008LL) { printf("FAIL constant UIA_MultipleViewPatternId\n"); failures++; }
    if ((long long)(UIA_MultipleViewSupportedViewsPropertyId) != 30072LL) { printf("FAIL constant UIA_MultipleViewSupportedViewsPropertyId\n"); failures++; }
    if ((long long)(UIA_NamePropertyId) != 30005LL) { printf("FAIL constant UIA_NamePropertyId\n"); failures++; }
    if ((long long)(UIA_NativeWindowHandlePropertyId) != 30020LL) { printf("FAIL constant UIA_NativeWindowHandlePropertyId\n"); failures++; }
    if ((long long)(UIA_NavigationLandmarkTypeId) != 80003LL) { printf("FAIL constant UIA_NavigationLandmarkTypeId\n"); failures++; }
    if ((long long)(UIA_NotificationEventId) != 20035LL) { printf("FAIL constant UIA_NotificationEventId\n"); failures++; }
    if ((long long)(UIA_ObjectModelPatternId) != 10022LL) { printf("FAIL constant UIA_ObjectModelPatternId\n"); failures++; }
    if ((long long)(UIA_OptimizeForVisualContentPropertyId) != 30111LL) { printf("FAIL constant UIA_OptimizeForVisualContentPropertyId\n"); failures++; }
    if ((long long)(UIA_OrientationPropertyId) != 30023LL) { printf("FAIL constant UIA_OrientationPropertyId\n"); failures++; }
    if ((long long)(UIA_OutlineColorPropertyId) != 30161LL) { printf("FAIL constant UIA_OutlineColorPropertyId\n"); failures++; }
    if ((long long)(UIA_OutlineStylesAttributeId) != 40022LL) { printf("FAIL constant UIA_OutlineStylesAttributeId\n"); failures++; }
    if ((long long)(UIA_OutlineThicknessPropertyId) != 30164LL) { printf("FAIL constant UIA_OutlineThicknessPropertyId\n"); failures++; }
    if ((long long)(UIA_OverlineColorAttributeId) != 40023LL) { printf("FAIL constant UIA_OverlineColorAttributeId\n"); failures++; }
    if ((long long)(UIA_OverlineStyleAttributeId) != 40024LL) { printf("FAIL constant UIA_OverlineStyleAttributeId\n"); failures++; }
    if ((long long)(UIA_PFIA_DEFAULT) != 0LL) { printf("FAIL constant UIA_PFIA_DEFAULT\n"); failures++; }
    if ((long long)(UIA_PFIA_UNWRAP_BRIDGE) != 1LL) { printf("FAIL constant UIA_PFIA_UNWRAP_BRIDGE\n"); failures++; }
    if ((long long)(UIA_PaneControlTypeId) != 50033LL) { printf("FAIL constant UIA_PaneControlTypeId\n"); failures++; }
    if ((long long)(UIA_PositionInSetPropertyId) != 30152LL) { printf("FAIL constant UIA_PositionInSetPropertyId\n"); failures++; }
    if ((long long)(UIA_ProcessIdPropertyId) != 30002LL) { printf("FAIL constant UIA_ProcessIdPropertyId\n"); failures++; }
    if ((long long)(UIA_ProgressBarControlTypeId) != 50012LL) { printf("FAIL constant UIA_ProgressBarControlTypeId\n"); failures++; }
    if ((long long)(UIA_ProviderDescriptionPropertyId) != 30107LL) { printf("FAIL constant UIA_ProviderDescriptionPropertyId\n"); failures++; }
    if ((long long)(UIA_RadioButtonControlTypeId) != 50013LL) { printf("FAIL constant UIA_RadioButtonControlTypeId\n"); failures++; }
    if ((long long)(UIA_RangeValueIsReadOnlyPropertyId) != 30048LL) { printf("FAIL constant UIA_RangeValueIsReadOnlyPropertyId\n"); failures++; }
    if ((long long)(UIA_RangeValueLargeChangePropertyId) != 30051LL) { printf("FAIL constant UIA_RangeValueLargeChangePropertyId\n"); failures++; }
    if ((long long)(UIA_RangeValueMaximumPropertyId) != 30050LL) { printf("FAIL constant UIA_RangeValueMaximumPropertyId\n"); failures++; }
    if ((long long)(UIA_RangeValueMinimumPropertyId) != 30049LL) { printf("FAIL constant UIA_RangeValueMinimumPropertyId\n"); failures++; }
    if ((long long)(UIA_RangeValuePatternId) != 10003LL) { printf("FAIL constant UIA_RangeValuePatternId\n"); failures++; }
    if ((long long)(UIA_RangeValueSmallChangePropertyId) != 30052LL) { printf("FAIL constant UIA_RangeValueSmallChangePropertyId\n"); failures++; }
    if ((long long)(UIA_RangeValueValuePropertyId) != 30047LL) { printf("FAIL constant UIA_RangeValueValuePropertyId\n"); failures++; }
    if ((long long)(UIA_RotationPropertyId) != 30166LL) { printf("FAIL constant UIA_RotationPropertyId\n"); failures++; }
    if ((long long)(UIA_RuntimeIdPropertyId) != 30000LL) { printf("FAIL constant UIA_RuntimeIdPropertyId\n"); failures++; }
    if ((long long)(UIA_SayAsInterpretAsAttributeId) != 40043LL) { printf("FAIL constant UIA_SayAsInterpretAsAttributeId\n"); failures++; }
    if ((long long)(UIA_ScrollBarControlTypeId) != 50014LL) { printf("FAIL constant UIA_ScrollBarControlTypeId\n"); failures++; }
    if ((long long)(UIA_ScrollHorizontalScrollPercentPropertyId) != 30053LL) { printf("FAIL constant UIA_ScrollHorizontalScrollPercentPropertyId\n"); failures++; }
    if ((long long)(UIA_ScrollHorizontalViewSizePropertyId) != 30054LL) { printf("FAIL constant UIA_ScrollHorizontalViewSizePropertyId\n"); failures++; }
    if ((long long)(UIA_ScrollHorizontallyScrollablePropertyId) != 30057LL) { printf("FAIL constant UIA_ScrollHorizontallyScrollablePropertyId\n"); failures++; }
    if ((long long)(UIA_ScrollItemPatternId) != 10017LL) { printf("FAIL constant UIA_ScrollItemPatternId\n"); failures++; }
    if ((long long)(UIA_ScrollPatternId) != 10004LL) { printf("FAIL constant UIA_ScrollPatternId\n"); failures++; }
    if ((long long)(UIA_ScrollVerticalScrollPercentPropertyId) != 30055LL) { printf("FAIL constant UIA_ScrollVerticalScrollPercentPropertyId\n"); failures++; }
    if ((long long)(UIA_ScrollVerticalViewSizePropertyId) != 30056LL) { printf("FAIL constant UIA_ScrollVerticalViewSizePropertyId\n"); failures++; }
    if ((long long)(UIA_ScrollVerticallyScrollablePropertyId) != 30058LL) { printf("FAIL constant UIA_ScrollVerticallyScrollablePropertyId\n"); failures++; }
    if ((long long)(UIA_SearchLandmarkTypeId) != 80004LL) { printf("FAIL constant UIA_SearchLandmarkTypeId\n"); failures++; }
    if ((long long)(UIA_Selection2CurrentSelectedItemPropertyId) != 30171LL) { printf("FAIL constant UIA_Selection2CurrentSelectedItemPropertyId\n"); failures++; }
    if ((long long)(UIA_Selection2FirstSelectedItemPropertyId) != 30169LL) { printf("FAIL constant UIA_Selection2FirstSelectedItemPropertyId\n"); failures++; }
    if ((long long)(UIA_Selection2ItemCountPropertyId) != 30172LL) { printf("FAIL constant UIA_Selection2ItemCountPropertyId\n"); failures++; }
    if ((long long)(UIA_Selection2LastSelectedItemPropertyId) != 30170LL) { printf("FAIL constant UIA_Selection2LastSelectedItemPropertyId\n"); failures++; }
    if ((long long)(UIA_SelectionActiveEndAttributeId) != 40037LL) { printf("FAIL constant UIA_SelectionActiveEndAttributeId\n"); failures++; }
    if ((long long)(UIA_SelectionCanSelectMultiplePropertyId) != 30060LL) { printf("FAIL constant UIA_SelectionCanSelectMultiplePropertyId\n"); failures++; }
    if ((long long)(UIA_SelectionIsSelectionRequiredPropertyId) != 30061LL) { printf("FAIL constant UIA_SelectionIsSelectionRequiredPropertyId\n"); failures++; }
    if ((long long)(UIA_SelectionItemIsSelectedPropertyId) != 30079LL) { printf("FAIL constant UIA_SelectionItemIsSelectedPropertyId\n"); failures++; }
    if ((long long)(UIA_SelectionItemPatternId) != 10010LL) { printf("FAIL constant UIA_SelectionItemPatternId\n"); failures++; }
    if ((long long)(UIA_SelectionItemSelectionContainerPropertyId) != 30080LL) { printf("FAIL constant UIA_SelectionItemSelectionContainerPropertyId\n"); failures++; }
    if ((long long)(UIA_SelectionItem_ElementAddedToSelectionEventId) != 20010LL) { printf("FAIL constant UIA_SelectionItem_ElementAddedToSelectionEventId\n"); failures++; }
    if ((long long)(UIA_SelectionItem_ElementRemovedFromSelectionEventId) != 20011LL) { printf("FAIL constant UIA_SelectionItem_ElementRemovedFromSelectionEventId\n"); failures++; }
    if ((long long)(UIA_SelectionItem_ElementSelectedEventId) != 20012LL) { printf("FAIL constant UIA_SelectionItem_ElementSelectedEventId\n"); failures++; }
    if ((long long)(UIA_SelectionPatternId) != 10001LL) { printf("FAIL constant UIA_SelectionPatternId\n"); failures++; }
    if ((long long)(UIA_SelectionSelectionPropertyId) != 30059LL) { printf("FAIL constant UIA_SelectionSelectionPropertyId\n"); failures++; }
    if ((long long)(UIA_Selection_InvalidatedEventId) != 20013LL) { printf("FAIL constant UIA_Selection_InvalidatedEventId\n"); failures++; }
    if ((long long)(UIA_SemanticZoomControlTypeId) != 50039LL) { printf("FAIL constant UIA_SemanticZoomControlTypeId\n"); failures++; }
    if ((long long)(UIA_SeparatorControlTypeId) != 50038LL) { printf("FAIL constant UIA_SeparatorControlTypeId\n"); failures++; }
    if ((long long)(UIA_SizeOfSetPropertyId) != 30153LL) { printf("FAIL constant UIA_SizeOfSetPropertyId\n"); failures++; }
    if ((long long)(UIA_SizePropertyId) != 30167LL) { printf("FAIL constant UIA_SizePropertyId\n"); failures++; }
    if ((long long)(UIA_SliderControlTypeId) != 50015LL) { printf("FAIL constant UIA_SliderControlTypeId\n"); failures++; }
    if ((long long)(UIA_SpinnerControlTypeId) != 50016LL) { printf("FAIL constant UIA_SpinnerControlTypeId\n"); failures++; }
    if ((long long)(UIA_SplitButtonControlTypeId) != 50031LL) { printf("FAIL constant UIA_SplitButtonControlTypeId\n"); failures++; }
    if ((long long)(UIA_SpreadsheetItemAnnotationObjectsPropertyId) != 30130LL) { printf("FAIL constant UIA_SpreadsheetItemAnnotationObjectsPropertyId\n"); failures++; }
    if ((long long)(UIA_SpreadsheetItemAnnotationTypesPropertyId) != 30131LL) { printf("FAIL constant UIA_SpreadsheetItemAnnotationTypesPropertyId\n"); failures++; }
    if ((long long)(UIA_SpreadsheetItemFormulaPropertyId) != 30129LL) { printf("FAIL constant UIA_SpreadsheetItemFormulaPropertyId\n"); failures++; }
    if ((long long)(UIA_SpreadsheetItemPatternId) != 10027LL) { printf("FAIL constant UIA_SpreadsheetItemPatternId\n"); failures++; }
    if ((long long)(UIA_SpreadsheetPatternId) != 10026LL) { printf("FAIL constant UIA_SpreadsheetPatternId\n"); failures++; }
    if ((long long)(UIA_StatusBarControlTypeId) != 50017LL) { printf("FAIL constant UIA_StatusBarControlTypeId\n"); failures++; }
    if ((long long)(UIA_StrikethroughColorAttributeId) != 40025LL) { printf("FAIL constant UIA_StrikethroughColorAttributeId\n"); failures++; }
    if ((long long)(UIA_StrikethroughStyleAttributeId) != 40026LL) { printf("FAIL constant UIA_StrikethroughStyleAttributeId\n"); failures++; }
    if ((long long)(UIA_StructureChangedEventId) != 20002LL) { printf("FAIL constant UIA_StructureChangedEventId\n"); failures++; }
    if ((long long)(UIA_StyleIdAttributeId) != 40034LL) { printf("FAIL constant UIA_StyleIdAttributeId\n"); failures++; }
    if ((long long)(UIA_StyleNameAttributeId) != 40033LL) { printf("FAIL constant UIA_StyleNameAttributeId\n"); failures++; }
    if ((long long)(UIA_StylesExtendedPropertiesPropertyId) != 30126LL) { printf("FAIL constant UIA_StylesExtendedPropertiesPropertyId\n"); failures++; }
    if ((long long)(UIA_StylesFillColorPropertyId) != 30122LL) { printf("FAIL constant UIA_StylesFillColorPropertyId\n"); failures++; }
    if ((long long)(UIA_StylesFillPatternColorPropertyId) != 30125LL) { printf("FAIL constant UIA_StylesFillPatternColorPropertyId\n"); failures++; }
    if ((long long)(UIA_StylesFillPatternStylePropertyId) != 30123LL) { printf("FAIL constant UIA_StylesFillPatternStylePropertyId\n"); failures++; }
    if ((long long)(UIA_StylesPatternId) != 10025LL) { printf("FAIL constant UIA_StylesPatternId\n"); failures++; }
    if ((long long)(UIA_StylesShapePropertyId) != 30124LL) { printf("FAIL constant UIA_StylesShapePropertyId\n"); failures++; }
    if ((long long)(UIA_StylesStyleIdPropertyId) != 30120LL) { printf("FAIL constant UIA_StylesStyleIdPropertyId\n"); failures++; }
    if ((long long)(UIA_StylesStyleNamePropertyId) != 30121LL) { printf("FAIL constant UIA_StylesStyleNamePropertyId\n"); failures++; }
    if ((long long)(UIA_SynchronizedInputPatternId) != 10021LL) { printf("FAIL constant UIA_SynchronizedInputPatternId\n"); failures++; }
    if ((long long)(UIA_SystemAlertEventId) != 20023LL) { printf("FAIL constant UIA_SystemAlertEventId\n"); failures++; }
    if ((long long)(UIA_TabControlTypeId) != 50018LL) { printf("FAIL constant UIA_TabControlTypeId\n"); failures++; }
    if ((long long)(UIA_TabItemControlTypeId) != 50019LL) { printf("FAIL constant UIA_TabItemControlTypeId\n"); failures++; }
    if ((long long)(UIA_TableColumnHeadersPropertyId) != 30082LL) { printf("FAIL constant UIA_TableColumnHeadersPropertyId\n"); failures++; }
    if ((long long)(UIA_TableControlTypeId) != 50036LL) { printf("FAIL constant UIA_TableControlTypeId\n"); failures++; }
    if ((long long)(UIA_TableItemColumnHeaderItemsPropertyId) != 30085LL) { printf("FAIL constant UIA_TableItemColumnHeaderItemsPropertyId\n"); failures++; }
    if ((long long)(UIA_TableItemPatternId) != 10013LL) { printf("FAIL constant UIA_TableItemPatternId\n"); failures++; }
    if ((long long)(UIA_TableItemRowHeaderItemsPropertyId) != 30084LL) { printf("FAIL constant UIA_TableItemRowHeaderItemsPropertyId\n"); failures++; }
    if ((long long)(UIA_TablePatternId) != 10012LL) { printf("FAIL constant UIA_TablePatternId\n"); failures++; }
    if ((long long)(UIA_TableRowHeadersPropertyId) != 30081LL) { printf("FAIL constant UIA_TableRowHeadersPropertyId\n"); failures++; }
    if ((long long)(UIA_TableRowOrColumnMajorPropertyId) != 30083LL) { printf("FAIL constant UIA_TableRowOrColumnMajorPropertyId\n"); failures++; }
    if ((long long)(UIA_TabsAttributeId) != 40027LL) { printf("FAIL constant UIA_TabsAttributeId\n"); failures++; }
    if ((long long)(UIA_TextChildPatternId) != 10029LL) { printf("FAIL constant UIA_TextChildPatternId\n"); failures++; }
    if ((long long)(UIA_TextControlTypeId) != 50020LL) { printf("FAIL constant UIA_TextControlTypeId\n"); failures++; }
    if ((long long)(UIA_TextEditPatternId) != 10032LL) { printf("FAIL constant UIA_TextEditPatternId\n"); failures++; }
    if ((long long)(UIA_TextEdit_ConversionTargetChangedEventId) != 20033LL) { printf("FAIL constant UIA_TextEdit_ConversionTargetChangedEventId\n"); failures++; }
    if ((long long)(UIA_TextEdit_TextChangedEventId) != 20032LL) { printf("FAIL constant UIA_TextEdit_TextChangedEventId\n"); failures++; }
    if ((long long)(UIA_TextFlowDirectionsAttributeId) != 40028LL) { printf("FAIL constant UIA_TextFlowDirectionsAttributeId\n"); failures++; }
    if ((long long)(UIA_TextPattern2Id) != 10024LL) { printf("FAIL constant UIA_TextPattern2Id\n"); failures++; }
    if ((long long)(UIA_TextPatternId) != 10014LL) { printf("FAIL constant UIA_TextPatternId\n"); failures++; }
    if ((long long)(UIA_Text_TextChangedEventId) != 20015LL) { printf("FAIL constant UIA_Text_TextChangedEventId\n"); failures++; }
    if ((long long)(UIA_Text_TextSelectionChangedEventId) != 20014LL) { printf("FAIL constant UIA_Text_TextSelectionChangedEventId\n"); failures++; }
    if ((long long)(UIA_ThumbControlTypeId) != 50027LL) { printf("FAIL constant UIA_ThumbControlTypeId\n"); failures++; }
    if ((long long)(UIA_TitleBarControlTypeId) != 50037LL) { printf("FAIL constant UIA_TitleBarControlTypeId\n"); failures++; }
    if ((long long)(UIA_TogglePatternId) != 10015LL) { printf("FAIL constant UIA_TogglePatternId\n"); failures++; }
    if ((long long)(UIA_ToggleToggleStatePropertyId) != 30086LL) { printf("FAIL constant UIA_ToggleToggleStatePropertyId\n"); failures++; }
    if ((long long)(UIA_ToolBarControlTypeId) != 50021LL) { printf("FAIL constant UIA_ToolBarControlTypeId\n"); failures++; }
    if ((long long)(UIA_ToolTipClosedEventId) != 20001LL) { printf("FAIL constant UIA_ToolTipClosedEventId\n"); failures++; }
    if ((long long)(UIA_ToolTipControlTypeId) != 50022LL) { printf("FAIL constant UIA_ToolTipControlTypeId\n"); failures++; }
    if ((long long)(UIA_ToolTipOpenedEventId) != 20000LL) { printf("FAIL constant UIA_ToolTipOpenedEventId\n"); failures++; }
    if ((long long)(UIA_Transform2CanZoomPropertyId) != 30133LL) { printf("FAIL constant UIA_Transform2CanZoomPropertyId\n"); failures++; }
    if ((long long)(UIA_Transform2ZoomLevelPropertyId) != 30145LL) { printf("FAIL constant UIA_Transform2ZoomLevelPropertyId\n"); failures++; }
    if ((long long)(UIA_Transform2ZoomMaximumPropertyId) != 30147LL) { printf("FAIL constant UIA_Transform2ZoomMaximumPropertyId\n"); failures++; }
    if ((long long)(UIA_Transform2ZoomMinimumPropertyId) != 30146LL) { printf("FAIL constant UIA_Transform2ZoomMinimumPropertyId\n"); failures++; }
    if ((long long)(UIA_TransformCanMovePropertyId) != 30087LL) { printf("FAIL constant UIA_TransformCanMovePropertyId\n"); failures++; }
    if ((long long)(UIA_TransformCanResizePropertyId) != 30088LL) { printf("FAIL constant UIA_TransformCanResizePropertyId\n"); failures++; }
    if ((long long)(UIA_TransformCanRotatePropertyId) != 30089LL) { printf("FAIL constant UIA_TransformCanRotatePropertyId\n"); failures++; }
    if ((long long)(UIA_TransformPattern2Id) != 10028LL) { printf("FAIL constant UIA_TransformPattern2Id\n"); failures++; }
    if ((long long)(UIA_TransformPatternId) != 10016LL) { printf("FAIL constant UIA_TransformPatternId\n"); failures++; }
    if ((long long)(UIA_TreeControlTypeId) != 50023LL) { printf("FAIL constant UIA_TreeControlTypeId\n"); failures++; }
    if ((long long)(UIA_TreeItemControlTypeId) != 50024LL) { printf("FAIL constant UIA_TreeItemControlTypeId\n"); failures++; }
    if ((long long)(UIA_UnderlineColorAttributeId) != 40029LL) { printf("FAIL constant UIA_UnderlineColorAttributeId\n"); failures++; }
    if ((long long)(UIA_UnderlineStyleAttributeId) != 40030LL) { printf("FAIL constant UIA_UnderlineStyleAttributeId\n"); failures++; }
    if ((long long)(UIA_ValueIsReadOnlyPropertyId) != 30046LL) { printf("FAIL constant UIA_ValueIsReadOnlyPropertyId\n"); failures++; }
    if ((long long)(UIA_ValuePatternId) != 10002LL) { printf("FAIL constant UIA_ValuePatternId\n"); failures++; }
    if ((long long)(UIA_ValueValuePropertyId) != 30045LL) { printf("FAIL constant UIA_ValueValuePropertyId\n"); failures++; }
    if ((long long)(UIA_VirtualizedItemPatternId) != 10020LL) { printf("FAIL constant UIA_VirtualizedItemPatternId\n"); failures++; }
    if ((long long)(UIA_VisualEffectsPropertyId) != 30163LL) { printf("FAIL constant UIA_VisualEffectsPropertyId\n"); failures++; }
    if ((long long)(UIA_WindowCanMaximizePropertyId) != 30073LL) { printf("FAIL constant UIA_WindowCanMaximizePropertyId\n"); failures++; }
    if ((long long)(UIA_WindowCanMinimizePropertyId) != 30074LL) { printf("FAIL constant UIA_WindowCanMinimizePropertyId\n"); failures++; }
    if ((long long)(UIA_WindowControlTypeId) != 50032LL) { printf("FAIL constant UIA_WindowControlTypeId\n"); failures++; }
    if ((long long)(UIA_WindowIsModalPropertyId) != 30077LL) { printf("FAIL constant UIA_WindowIsModalPropertyId\n"); failures++; }
    if ((long long)(UIA_WindowIsTopmostPropertyId) != 30078LL) { printf("FAIL constant UIA_WindowIsTopmostPropertyId\n"); failures++; }
    if ((long long)(UIA_WindowPatternId) != 10009LL) { printf("FAIL constant UIA_WindowPatternId\n"); failures++; }
    if ((long long)(UIA_WindowWindowInteractionStatePropertyId) != 30076LL) { printf("FAIL constant UIA_WindowWindowInteractionStatePropertyId\n"); failures++; }
    if ((long long)(UIA_WindowWindowVisualStatePropertyId) != 30075LL) { printf("FAIL constant UIA_WindowWindowVisualStatePropertyId\n"); failures++; }
    if ((long long)(UIA_Window_WindowClosedEventId) != 20017LL) { printf("FAIL constant UIA_Window_WindowClosedEventId\n"); failures++; }
    if ((long long)(UIA_Window_WindowOpenedEventId) != 20016LL) { printf("FAIL constant UIA_Window_WindowOpenedEventId\n"); failures++; }
    if ((long long)(ULW_ALPHA) != 2LL) { printf("FAIL constant ULW_ALPHA\n"); failures++; }
    if ((long long)(ULW_COLORKEY) != 1LL) { printf("FAIL constant ULW_COLORKEY\n"); failures++; }
    if ((long long)(ULW_EX_NORESIZE) != 8LL) { printf("FAIL constant ULW_EX_NORESIZE\n"); failures++; }
    if ((long long)(ULW_OPAQUE) != 4LL) { printf("FAIL constant ULW_OPAQUE\n"); failures++; }
    if ((long long)(VK_ACCEPT) != 30LL) { printf("FAIL constant VK_ACCEPT\n"); failures++; }
    if ((long long)(VK_ADD) != 107LL) { printf("FAIL constant VK_ADD\n"); failures++; }
    if ((long long)(VK_APPS) != 93LL) { printf("FAIL constant VK_APPS\n"); failures++; }
    if ((long long)(VK_ATTN) != 246LL) { printf("FAIL constant VK_ATTN\n"); failures++; }
    if ((long long)(VK_BACK) != 8LL) { printf("FAIL constant VK_BACK\n"); failures++; }
    if ((long long)(VK_BROWSER_BACK) != 166LL) { printf("FAIL constant VK_BROWSER_BACK\n"); failures++; }
    if ((long long)(VK_BROWSER_FAVORITES) != 171LL) { printf("FAIL constant VK_BROWSER_FAVORITES\n"); failures++; }
    if ((long long)(VK_BROWSER_FORWARD) != 167LL) { printf("FAIL constant VK_BROWSER_FORWARD\n"); failures++; }
    if ((long long)(VK_BROWSER_HOME) != 172LL) { printf("FAIL constant VK_BROWSER_HOME\n"); failures++; }
    if ((long long)(VK_BROWSER_REFRESH) != 168LL) { printf("FAIL constant VK_BROWSER_REFRESH\n"); failures++; }
    if ((long long)(VK_BROWSER_SEARCH) != 170LL) { printf("FAIL constant VK_BROWSER_SEARCH\n"); failures++; }
    if ((long long)(VK_BROWSER_STOP) != 169LL) { printf("FAIL constant VK_BROWSER_STOP\n"); failures++; }
    if ((long long)(VK_CANCEL) != 3LL) { printf("FAIL constant VK_CANCEL\n"); failures++; }
    if ((long long)(VK_CAPITAL) != 20LL) { printf("FAIL constant VK_CAPITAL\n"); failures++; }
    if ((long long)(VK_CLEAR) != 12LL) { printf("FAIL constant VK_CLEAR\n"); failures++; }
    if ((long long)(VK_CONTROL) != 17LL) { printf("FAIL constant VK_CONTROL\n"); failures++; }
    if ((long long)(VK_CONVERT) != 28LL) { printf("FAIL constant VK_CONVERT\n"); failures++; }
    if ((long long)(VK_CRSEL) != 247LL) { printf("FAIL constant VK_CRSEL\n"); failures++; }
    if ((long long)(VK_DECIMAL) != 110LL) { printf("FAIL constant VK_DECIMAL\n"); failures++; }
    if ((long long)(VK_DELETE) != 46LL) { printf("FAIL constant VK_DELETE\n"); failures++; }
    if ((long long)(VK_DIVIDE) != 111LL) { printf("FAIL constant VK_DIVIDE\n"); failures++; }
    if ((long long)(VK_DOWN) != 40LL) { printf("FAIL constant VK_DOWN\n"); failures++; }
    if ((long long)(VK_END) != 35LL) { printf("FAIL constant VK_END\n"); failures++; }
    if ((long long)(VK_EREOF) != 249LL) { printf("FAIL constant VK_EREOF\n"); failures++; }
    if ((long long)(VK_ESCAPE) != 27LL) { printf("FAIL constant VK_ESCAPE\n"); failures++; }
    if ((long long)(VK_EXECUTE) != 43LL) { printf("FAIL constant VK_EXECUTE\n"); failures++; }
    if ((long long)(VK_EXSEL) != 248LL) { printf("FAIL constant VK_EXSEL\n"); failures++; }
    if ((long long)(VK_F1) != 112LL) { printf("FAIL constant VK_F1\n"); failures++; }
    if ((long long)(VK_F10) != 121LL) { printf("FAIL constant VK_F10\n"); failures++; }
    if ((long long)(VK_F11) != 122LL) { printf("FAIL constant VK_F11\n"); failures++; }
    if ((long long)(VK_F12) != 123LL) { printf("FAIL constant VK_F12\n"); failures++; }
    if ((long long)(VK_F13) != 124LL) { printf("FAIL constant VK_F13\n"); failures++; }
    if ((long long)(VK_F14) != 125LL) { printf("FAIL constant VK_F14\n"); failures++; }
    if ((long long)(VK_F15) != 126LL) { printf("FAIL constant VK_F15\n"); failures++; }
    if ((long long)(VK_F16) != 127LL) { printf("FAIL constant VK_F16\n"); failures++; }
    if ((long long)(VK_F17) != 128LL) { printf("FAIL constant VK_F17\n"); failures++; }
    if ((long long)(VK_F18) != 129LL) { printf("FAIL constant VK_F18\n"); failures++; }
    if ((long long)(VK_F19) != 130LL) { printf("FAIL constant VK_F19\n"); failures++; }
    if ((long long)(VK_F2) != 113LL) { printf("FAIL constant VK_F2\n"); failures++; }
    if ((long long)(VK_F20) != 131LL) { printf("FAIL constant VK_F20\n"); failures++; }
    if ((long long)(VK_F21) != 132LL) { printf("FAIL constant VK_F21\n"); failures++; }
    if ((long long)(VK_F22) != 133LL) { printf("FAIL constant VK_F22\n"); failures++; }
    if ((long long)(VK_F23) != 134LL) { printf("FAIL constant VK_F23\n"); failures++; }
    if ((long long)(VK_F24) != 135LL) { printf("FAIL constant VK_F24\n"); failures++; }
    if ((long long)(VK_F3) != 114LL) { printf("FAIL constant VK_F3\n"); failures++; }
    if ((long long)(VK_F4) != 115LL) { printf("FAIL constant VK_F4\n"); failures++; }
    if ((long long)(VK_F5) != 116LL) { printf("FAIL constant VK_F5\n"); failures++; }
    if ((long long)(VK_F6) != 117LL) { printf("FAIL constant VK_F6\n"); failures++; }
    if ((long long)(VK_F7) != 118LL) { printf("FAIL constant VK_F7\n"); failures++; }
    if ((long long)(VK_F8) != 119LL) { printf("FAIL constant VK_F8\n"); failures++; }
    if ((long long)(VK_F9) != 120LL) { printf("FAIL constant VK_F9\n"); failures++; }
    if ((long long)(VK_FINAL) != 24LL) { printf("FAIL constant VK_FINAL\n"); failures++; }
    if ((long long)(VK_GAMEPAD_A) != 195LL) { printf("FAIL constant VK_GAMEPAD_A\n"); failures++; }
    if ((long long)(VK_GAMEPAD_B) != 196LL) { printf("FAIL constant VK_GAMEPAD_B\n"); failures++; }
    if ((long long)(VK_GAMEPAD_DPAD_DOWN) != 204LL) { printf("FAIL constant VK_GAMEPAD_DPAD_DOWN\n"); failures++; }
    if ((long long)(VK_GAMEPAD_DPAD_LEFT) != 205LL) { printf("FAIL constant VK_GAMEPAD_DPAD_LEFT\n"); failures++; }
    if ((long long)(VK_GAMEPAD_DPAD_RIGHT) != 206LL) { printf("FAIL constant VK_GAMEPAD_DPAD_RIGHT\n"); failures++; }
    if ((long long)(VK_GAMEPAD_DPAD_UP) != 203LL) { printf("FAIL constant VK_GAMEPAD_DPAD_UP\n"); failures++; }
    if ((long long)(VK_GAMEPAD_LEFT_SHOULDER) != 200LL) { printf("FAIL constant VK_GAMEPAD_LEFT_SHOULDER\n"); failures++; }
    if ((long long)(VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON) != 209LL) { printf("FAIL constant VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON\n"); failures++; }
    if ((long long)(VK_GAMEPAD_LEFT_THUMBSTICK_DOWN) != 212LL) { printf("FAIL constant VK_GAMEPAD_LEFT_THUMBSTICK_DOWN\n"); failures++; }
    if ((long long)(VK_GAMEPAD_LEFT_THUMBSTICK_LEFT) != 214LL) { printf("FAIL constant VK_GAMEPAD_LEFT_THUMBSTICK_LEFT\n"); failures++; }
    if ((long long)(VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT) != 213LL) { printf("FAIL constant VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT\n"); failures++; }
    if ((long long)(VK_GAMEPAD_LEFT_THUMBSTICK_UP) != 211LL) { printf("FAIL constant VK_GAMEPAD_LEFT_THUMBSTICK_UP\n"); failures++; }
    if ((long long)(VK_GAMEPAD_LEFT_TRIGGER) != 201LL) { printf("FAIL constant VK_GAMEPAD_LEFT_TRIGGER\n"); failures++; }
    if ((long long)(VK_GAMEPAD_MENU) != 207LL) { printf("FAIL constant VK_GAMEPAD_MENU\n"); failures++; }
    if ((long long)(VK_GAMEPAD_RIGHT_SHOULDER) != 199LL) { printf("FAIL constant VK_GAMEPAD_RIGHT_SHOULDER\n"); failures++; }
    if ((long long)(VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON) != 210LL) { printf("FAIL constant VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON\n"); failures++; }
    if ((long long)(VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN) != 216LL) { printf("FAIL constant VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN\n"); failures++; }
    if ((long long)(VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT) != 218LL) { printf("FAIL constant VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT\n"); failures++; }
    if ((long long)(VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT) != 217LL) { printf("FAIL constant VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT\n"); failures++; }
    if ((long long)(VK_GAMEPAD_RIGHT_THUMBSTICK_UP) != 215LL) { printf("FAIL constant VK_GAMEPAD_RIGHT_THUMBSTICK_UP\n"); failures++; }
    if ((long long)(VK_GAMEPAD_RIGHT_TRIGGER) != 202LL) { printf("FAIL constant VK_GAMEPAD_RIGHT_TRIGGER\n"); failures++; }
    if ((long long)(VK_GAMEPAD_VIEW) != 208LL) { printf("FAIL constant VK_GAMEPAD_VIEW\n"); failures++; }
    if ((long long)(VK_GAMEPAD_X) != 197LL) { printf("FAIL constant VK_GAMEPAD_X\n"); failures++; }
    if ((long long)(VK_GAMEPAD_Y) != 198LL) { printf("FAIL constant VK_GAMEPAD_Y\n"); failures++; }
    if ((long long)(VK_HANGEUL) != 21LL) { printf("FAIL constant VK_HANGEUL\n"); failures++; }
    if ((long long)(VK_HANGUL) != 21LL) { printf("FAIL constant VK_HANGUL\n"); failures++; }
    if ((long long)(VK_HANJA) != 25LL) { printf("FAIL constant VK_HANJA\n"); failures++; }
    if ((long long)(VK_HELP) != 47LL) { printf("FAIL constant VK_HELP\n"); failures++; }
    if ((long long)(VK_HOME) != 36LL) { printf("FAIL constant VK_HOME\n"); failures++; }
    if ((long long)(VK_ICO_00) != 228LL) { printf("FAIL constant VK_ICO_00\n"); failures++; }
    if ((long long)(VK_ICO_CLEAR) != 230LL) { printf("FAIL constant VK_ICO_CLEAR\n"); failures++; }
    if ((long long)(VK_ICO_HELP) != 227LL) { printf("FAIL constant VK_ICO_HELP\n"); failures++; }
    if ((long long)(VK_IME_OFF) != 26LL) { printf("FAIL constant VK_IME_OFF\n"); failures++; }
    if ((long long)(VK_IME_ON) != 22LL) { printf("FAIL constant VK_IME_ON\n"); failures++; }
    if ((long long)(VK_INSERT) != 45LL) { printf("FAIL constant VK_INSERT\n"); failures++; }
    if ((long long)(VK_JUNJA) != 23LL) { printf("FAIL constant VK_JUNJA\n"); failures++; }
    if ((long long)(VK_KANA) != 21LL) { printf("FAIL constant VK_KANA\n"); failures++; }
    if ((long long)(VK_KANJI) != 25LL) { printf("FAIL constant VK_KANJI\n"); failures++; }
    if ((long long)(VK_LAUNCH_APP1) != 182LL) { printf("FAIL constant VK_LAUNCH_APP1\n"); failures++; }
    if ((long long)(VK_LAUNCH_APP2) != 183LL) { printf("FAIL constant VK_LAUNCH_APP2\n"); failures++; }
    if ((long long)(VK_LAUNCH_MAIL) != 180LL) { printf("FAIL constant VK_LAUNCH_MAIL\n"); failures++; }
    if ((long long)(VK_LAUNCH_MEDIA_SELECT) != 181LL) { printf("FAIL constant VK_LAUNCH_MEDIA_SELECT\n"); failures++; }
    if ((long long)(VK_LBUTTON) != 1LL) { printf("FAIL constant VK_LBUTTON\n"); failures++; }
    if ((long long)(VK_LCONTROL) != 162LL) { printf("FAIL constant VK_LCONTROL\n"); failures++; }
    if ((long long)(VK_LEFT) != 37LL) { printf("FAIL constant VK_LEFT\n"); failures++; }
    if ((long long)(VK_LMENU) != 164LL) { printf("FAIL constant VK_LMENU\n"); failures++; }
    if ((long long)(VK_LSHIFT) != 160LL) { printf("FAIL constant VK_LSHIFT\n"); failures++; }
    if ((long long)(VK_LWIN) != 91LL) { printf("FAIL constant VK_LWIN\n"); failures++; }
    if ((long long)(VK_MBUTTON) != 4LL) { printf("FAIL constant VK_MBUTTON\n"); failures++; }
    if ((long long)(VK_MEDIA_NEXT_TRACK) != 176LL) { printf("FAIL constant VK_MEDIA_NEXT_TRACK\n"); failures++; }
    if ((long long)(VK_MEDIA_PLAY_PAUSE) != 179LL) { printf("FAIL constant VK_MEDIA_PLAY_PAUSE\n"); failures++; }
    if ((long long)(VK_MEDIA_PREV_TRACK) != 177LL) { printf("FAIL constant VK_MEDIA_PREV_TRACK\n"); failures++; }
    if ((long long)(VK_MEDIA_STOP) != 178LL) { printf("FAIL constant VK_MEDIA_STOP\n"); failures++; }
    if ((long long)(VK_MENU) != 18LL) { printf("FAIL constant VK_MENU\n"); failures++; }
    if ((long long)(VK_MODECHANGE) != 31LL) { printf("FAIL constant VK_MODECHANGE\n"); failures++; }
    if ((long long)(VK_MULTIPLY) != 106LL) { printf("FAIL constant VK_MULTIPLY\n"); failures++; }
    if ((long long)(VK_NAVIGATION_ACCEPT) != 142LL) { printf("FAIL constant VK_NAVIGATION_ACCEPT\n"); failures++; }
    if ((long long)(VK_NAVIGATION_CANCEL) != 143LL) { printf("FAIL constant VK_NAVIGATION_CANCEL\n"); failures++; }
    if ((long long)(VK_NAVIGATION_DOWN) != 139LL) { printf("FAIL constant VK_NAVIGATION_DOWN\n"); failures++; }
    if ((long long)(VK_NAVIGATION_LEFT) != 140LL) { printf("FAIL constant VK_NAVIGATION_LEFT\n"); failures++; }
    if ((long long)(VK_NAVIGATION_MENU) != 137LL) { printf("FAIL constant VK_NAVIGATION_MENU\n"); failures++; }
    if ((long long)(VK_NAVIGATION_RIGHT) != 141LL) { printf("FAIL constant VK_NAVIGATION_RIGHT\n"); failures++; }
    if ((long long)(VK_NAVIGATION_UP) != 138LL) { printf("FAIL constant VK_NAVIGATION_UP\n"); failures++; }
    if ((long long)(VK_NAVIGATION_VIEW) != 136LL) { printf("FAIL constant VK_NAVIGATION_VIEW\n"); failures++; }
    if ((long long)(VK_NEXT) != 34LL) { printf("FAIL constant VK_NEXT\n"); failures++; }
    if ((long long)(VK_NONAME) != 252LL) { printf("FAIL constant VK_NONAME\n"); failures++; }
    if ((long long)(VK_NONCONVERT) != 29LL) { printf("FAIL constant VK_NONCONVERT\n"); failures++; }
    if ((long long)(VK_NUMLOCK) != 144LL) { printf("FAIL constant VK_NUMLOCK\n"); failures++; }
    if ((long long)(VK_NUMPAD0) != 96LL) { printf("FAIL constant VK_NUMPAD0\n"); failures++; }
    if ((long long)(VK_NUMPAD1) != 97LL) { printf("FAIL constant VK_NUMPAD1\n"); failures++; }
    if ((long long)(VK_NUMPAD2) != 98LL) { printf("FAIL constant VK_NUMPAD2\n"); failures++; }
    if ((long long)(VK_NUMPAD3) != 99LL) { printf("FAIL constant VK_NUMPAD3\n"); failures++; }
    if ((long long)(VK_NUMPAD4) != 100LL) { printf("FAIL constant VK_NUMPAD4\n"); failures++; }
    if ((long long)(VK_NUMPAD5) != 101LL) { printf("FAIL constant VK_NUMPAD5\n"); failures++; }
    if ((long long)(VK_NUMPAD6) != 102LL) { printf("FAIL constant VK_NUMPAD6\n"); failures++; }
    if ((long long)(VK_NUMPAD7) != 103LL) { printf("FAIL constant VK_NUMPAD7\n"); failures++; }
    if ((long long)(VK_NUMPAD8) != 104LL) { printf("FAIL constant VK_NUMPAD8\n"); failures++; }
    if ((long long)(VK_NUMPAD9) != 105LL) { printf("FAIL constant VK_NUMPAD9\n"); failures++; }
    if ((long long)(VK_OEM_1) != 186LL) { printf("FAIL constant VK_OEM_1\n"); failures++; }
    if ((long long)(VK_OEM_102) != 226LL) { printf("FAIL constant VK_OEM_102\n"); failures++; }
    if ((long long)(VK_OEM_2) != 191LL) { printf("FAIL constant VK_OEM_2\n"); failures++; }
    if ((long long)(VK_OEM_3) != 192LL) { printf("FAIL constant VK_OEM_3\n"); failures++; }
    if ((long long)(VK_OEM_4) != 219LL) { printf("FAIL constant VK_OEM_4\n"); failures++; }
    if ((long long)(VK_OEM_5) != 220LL) { printf("FAIL constant VK_OEM_5\n"); failures++; }
    if ((long long)(VK_OEM_6) != 221LL) { printf("FAIL constant VK_OEM_6\n"); failures++; }
    if ((long long)(VK_OEM_7) != 222LL) { printf("FAIL constant VK_OEM_7\n"); failures++; }
    if ((long long)(VK_OEM_8) != 223LL) { printf("FAIL constant VK_OEM_8\n"); failures++; }
    if ((long long)(VK_OEM_ATTN) != 240LL) { printf("FAIL constant VK_OEM_ATTN\n"); failures++; }
    if ((long long)(VK_OEM_AUTO) != 243LL) { printf("FAIL constant VK_OEM_AUTO\n"); failures++; }
    if ((long long)(VK_OEM_AX) != 225LL) { printf("FAIL constant VK_OEM_AX\n"); failures++; }
    if ((long long)(VK_OEM_BACKTAB) != 245LL) { printf("FAIL constant VK_OEM_BACKTAB\n"); failures++; }
    if ((long long)(VK_OEM_CLEAR) != 254LL) { printf("FAIL constant VK_OEM_CLEAR\n"); failures++; }
    if ((long long)(VK_OEM_COMMA) != 188LL) { printf("FAIL constant VK_OEM_COMMA\n"); failures++; }
    if ((long long)(VK_OEM_COPY) != 242LL) { printf("FAIL constant VK_OEM_COPY\n"); failures++; }
    if ((long long)(VK_OEM_CUSEL) != 239LL) { printf("FAIL constant VK_OEM_CUSEL\n"); failures++; }
    if ((long long)(VK_OEM_ENLW) != 244LL) { printf("FAIL constant VK_OEM_ENLW\n"); failures++; }
    if ((long long)(VK_OEM_FINISH) != 241LL) { printf("FAIL constant VK_OEM_FINISH\n"); failures++; }
    if ((long long)(VK_OEM_FJ_JISHO) != 146LL) { printf("FAIL constant VK_OEM_FJ_JISHO\n"); failures++; }
    if ((long long)(VK_OEM_FJ_LOYA) != 149LL) { printf("FAIL constant VK_OEM_FJ_LOYA\n"); failures++; }
    if ((long long)(VK_OEM_FJ_MASSHOU) != 147LL) { printf("FAIL constant VK_OEM_FJ_MASSHOU\n"); failures++; }
    if ((long long)(VK_OEM_FJ_ROYA) != 150LL) { printf("FAIL constant VK_OEM_FJ_ROYA\n"); failures++; }
    if ((long long)(VK_OEM_FJ_TOUROKU) != 148LL) { printf("FAIL constant VK_OEM_FJ_TOUROKU\n"); failures++; }
    if ((long long)(VK_OEM_JUMP) != 234LL) { printf("FAIL constant VK_OEM_JUMP\n"); failures++; }
    if ((long long)(VK_OEM_MINUS) != 189LL) { printf("FAIL constant VK_OEM_MINUS\n"); failures++; }
    if ((long long)(VK_OEM_NEC_EQUAL) != 146LL) { printf("FAIL constant VK_OEM_NEC_EQUAL\n"); failures++; }
    if ((long long)(VK_OEM_PA1) != 235LL) { printf("FAIL constant VK_OEM_PA1\n"); failures++; }
    if ((long long)(VK_OEM_PA2) != 236LL) { printf("FAIL constant VK_OEM_PA2\n"); failures++; }
    if ((long long)(VK_OEM_PA3) != 237LL) { printf("FAIL constant VK_OEM_PA3\n"); failures++; }
    if ((long long)(VK_OEM_PERIOD) != 190LL) { printf("FAIL constant VK_OEM_PERIOD\n"); failures++; }
    if ((long long)(VK_OEM_PLUS) != 187LL) { printf("FAIL constant VK_OEM_PLUS\n"); failures++; }
    if ((long long)(VK_OEM_RESET) != 233LL) { printf("FAIL constant VK_OEM_RESET\n"); failures++; }
    if ((long long)(VK_OEM_WSCTRL) != 238LL) { printf("FAIL constant VK_OEM_WSCTRL\n"); failures++; }
    if ((long long)(VK_PA1) != 253LL) { printf("FAIL constant VK_PA1\n"); failures++; }
    if ((long long)(VK_PACKET) != 231LL) { printf("FAIL constant VK_PACKET\n"); failures++; }
    if ((long long)(VK_PAUSE) != 19LL) { printf("FAIL constant VK_PAUSE\n"); failures++; }
    if ((long long)(VK_PLAY) != 250LL) { printf("FAIL constant VK_PLAY\n"); failures++; }
    if ((long long)(VK_PRINT) != 42LL) { printf("FAIL constant VK_PRINT\n"); failures++; }
    if ((long long)(VK_PRIOR) != 33LL) { printf("FAIL constant VK_PRIOR\n"); failures++; }
    if ((long long)(VK_PROCESSKEY) != 229LL) { printf("FAIL constant VK_PROCESSKEY\n"); failures++; }
    if ((long long)(VK_RBUTTON) != 2LL) { printf("FAIL constant VK_RBUTTON\n"); failures++; }
    if ((long long)(VK_RCONTROL) != 163LL) { printf("FAIL constant VK_RCONTROL\n"); failures++; }
    if ((long long)(VK_RETURN) != 13LL) { printf("FAIL constant VK_RETURN\n"); failures++; }
    if ((long long)(VK_RIGHT) != 39LL) { printf("FAIL constant VK_RIGHT\n"); failures++; }
    if ((long long)(VK_RMENU) != 165LL) { printf("FAIL constant VK_RMENU\n"); failures++; }
    if ((long long)(VK_RSHIFT) != 161LL) { printf("FAIL constant VK_RSHIFT\n"); failures++; }
    if ((long long)(VK_RWIN) != 92LL) { printf("FAIL constant VK_RWIN\n"); failures++; }
    if ((long long)(VK_SCROLL) != 145LL) { printf("FAIL constant VK_SCROLL\n"); failures++; }
    if ((long long)(VK_SELECT) != 41LL) { printf("FAIL constant VK_SELECT\n"); failures++; }
    if ((long long)(VK_SEPARATOR) != 108LL) { printf("FAIL constant VK_SEPARATOR\n"); failures++; }
    if ((long long)(VK_SHIFT) != 16LL) { printf("FAIL constant VK_SHIFT\n"); failures++; }
    if ((long long)(VK_SLEEP) != 95LL) { printf("FAIL constant VK_SLEEP\n"); failures++; }
    if ((long long)(VK_SNAPSHOT) != 44LL) { printf("FAIL constant VK_SNAPSHOT\n"); failures++; }
    if ((long long)(VK_SPACE) != 32LL) { printf("FAIL constant VK_SPACE\n"); failures++; }
    if ((long long)(VK_SUBTRACT) != 109LL) { printf("FAIL constant VK_SUBTRACT\n"); failures++; }
    if ((long long)(VK_TAB) != 9LL) { printf("FAIL constant VK_TAB\n"); failures++; }
    if ((long long)(VK_UP) != 38LL) { printf("FAIL constant VK_UP\n"); failures++; }
    if ((long long)(VK_VOLUME_DOWN) != 174LL) { printf("FAIL constant VK_VOLUME_DOWN\n"); failures++; }
    if ((long long)(VK_VOLUME_MUTE) != 173LL) { printf("FAIL constant VK_VOLUME_MUTE\n"); failures++; }
    if ((long long)(VK_VOLUME_UP) != 175LL) { printf("FAIL constant VK_VOLUME_UP\n"); failures++; }
    if ((long long)(VK_XBUTTON1) != 5LL) { printf("FAIL constant VK_XBUTTON1\n"); failures++; }
    if ((long long)(VK_XBUTTON2) != 6LL) { printf("FAIL constant VK_XBUTTON2\n"); failures++; }
    if ((long long)(VK_ZOOM) != 251LL) { printf("FAIL constant VK_ZOOM\n"); failures++; }
    if ((long long)(VT_ARRAY) != 8192LL) { printf("FAIL constant VT_ARRAY\n"); failures++; }
    if ((long long)(VT_BLOB) != 65LL) { printf("FAIL constant VT_BLOB\n"); failures++; }
    if ((long long)(VT_BLOB_OBJECT) != 70LL) { printf("FAIL constant VT_BLOB_OBJECT\n"); failures++; }
    if ((long long)(VT_BOOL) != 11LL) { printf("FAIL constant VT_BOOL\n"); failures++; }
    if ((long long)(VT_BSTR) != 8LL) { printf("FAIL constant VT_BSTR\n"); failures++; }
    if ((long long)(VT_BSTR_BLOB) != 4095LL) { printf("FAIL constant VT_BSTR_BLOB\n"); failures++; }
    if ((long long)(VT_BYREF) != 16384LL) { printf("FAIL constant VT_BYREF\n"); failures++; }
    if ((long long)(VT_CARRAY) != 28LL) { printf("FAIL constant VT_CARRAY\n"); failures++; }
    if ((long long)(VT_CF) != 71LL) { printf("FAIL constant VT_CF\n"); failures++; }
    if ((long long)(VT_CLSID) != 72LL) { printf("FAIL constant VT_CLSID\n"); failures++; }
    if ((long long)(VT_CY) != 6LL) { printf("FAIL constant VT_CY\n"); failures++; }
    if ((long long)(VT_DATE) != 7LL) { printf("FAIL constant VT_DATE\n"); failures++; }
    if ((long long)(VT_DECIMAL) != 14LL) { printf("FAIL constant VT_DECIMAL\n"); failures++; }
    if ((long long)(VT_DISPATCH) != 9LL) { printf("FAIL constant VT_DISPATCH\n"); failures++; }
    if ((long long)(VT_EMPTY) != 0LL) { printf("FAIL constant VT_EMPTY\n"); failures++; }
    if ((long long)(VT_ERROR) != 10LL) { printf("FAIL constant VT_ERROR\n"); failures++; }
    if ((long long)(VT_FILETIME) != 64LL) { printf("FAIL constant VT_FILETIME\n"); failures++; }
    if ((long long)(VT_HARDTYPE) != 32768LL) { printf("FAIL constant VT_HARDTYPE\n"); failures++; }
    if ((long long)(VT_HRESULT) != 25LL) { printf("FAIL constant VT_HRESULT\n"); failures++; }
    if ((long long)(VT_I1) != 16LL) { printf("FAIL constant VT_I1\n"); failures++; }
    if ((long long)(VT_I2) != 2LL) { printf("FAIL constant VT_I2\n"); failures++; }
    if ((long long)(VT_I4) != 3LL) { printf("FAIL constant VT_I4\n"); failures++; }
    if ((long long)(VT_I8) != 20LL) { printf("FAIL constant VT_I8\n"); failures++; }
    if ((long long)(VT_ILLEGAL) != 65535LL) { printf("FAIL constant VT_ILLEGAL\n"); failures++; }
    if ((long long)(VT_ILLEGALMASKED) != 4095LL) { printf("FAIL constant VT_ILLEGALMASKED\n"); failures++; }
    if ((long long)(VT_INT) != 22LL) { printf("FAIL constant VT_INT\n"); failures++; }
    if ((long long)(VT_INT_PTR) != 37LL) { printf("FAIL constant VT_INT_PTR\n"); failures++; }
    if ((long long)(VT_LPSTR) != 30LL) { printf("FAIL constant VT_LPSTR\n"); failures++; }
    if ((long long)(VT_LPWSTR) != 31LL) { printf("FAIL constant VT_LPWSTR\n"); failures++; }
    if ((long long)(VT_NULL) != 1LL) { printf("FAIL constant VT_NULL\n"); failures++; }
    if ((long long)(VT_PTR) != 26LL) { printf("FAIL constant VT_PTR\n"); failures++; }
    if ((long long)(VT_R4) != 4LL) { printf("FAIL constant VT_R4\n"); failures++; }
    if ((long long)(VT_R8) != 5LL) { printf("FAIL constant VT_R8\n"); failures++; }
    if ((long long)(VT_RECORD) != 36LL) { printf("FAIL constant VT_RECORD\n"); failures++; }
    if ((long long)(VT_RESERVED) != 32768LL) { printf("FAIL constant VT_RESERVED\n"); failures++; }
    if ((long long)(VT_SAFEARRAY) != 27LL) { printf("FAIL constant VT_SAFEARRAY\n"); failures++; }
    if ((long long)(VT_STORAGE) != 67LL) { printf("FAIL constant VT_STORAGE\n"); failures++; }
    if ((long long)(VT_STORED_OBJECT) != 69LL) { printf("FAIL constant VT_STORED_OBJECT\n"); failures++; }
    if ((long long)(VT_STREAM) != 66LL) { printf("FAIL constant VT_STREAM\n"); failures++; }
    if ((long long)(VT_STREAMED_OBJECT) != 68LL) { printf("FAIL constant VT_STREAMED_OBJECT\n"); failures++; }
    if ((long long)(VT_TYPEMASK) != 4095LL) { printf("FAIL constant VT_TYPEMASK\n"); failures++; }
    if ((long long)(VT_UI1) != 17LL) { printf("FAIL constant VT_UI1\n"); failures++; }
    if ((long long)(VT_UI2) != 18LL) { printf("FAIL constant VT_UI2\n"); failures++; }
    if ((long long)(VT_UI4) != 19LL) { printf("FAIL constant VT_UI4\n"); failures++; }
    if ((long long)(VT_UI8) != 21LL) { printf("FAIL constant VT_UI8\n"); failures++; }
    if ((long long)(VT_UINT) != 23LL) { printf("FAIL constant VT_UINT\n"); failures++; }
    if ((long long)(VT_UINT_PTR) != 38LL) { printf("FAIL constant VT_UINT_PTR\n"); failures++; }
    if ((long long)(VT_UNKNOWN) != 13LL) { printf("FAIL constant VT_UNKNOWN\n"); failures++; }
    if ((long long)(VT_USERDEFINED) != 29LL) { printf("FAIL constant VT_USERDEFINED\n"); failures++; }
    if ((long long)(VT_VARIANT) != 12LL) { printf("FAIL constant VT_VARIANT\n"); failures++; }
    if ((long long)(VT_VECTOR) != 4096LL) { printf("FAIL constant VT_VECTOR\n"); failures++; }
    if ((long long)(VT_VERSIONED_STREAM) != 73LL) { printf("FAIL constant VT_VERSIONED_STREAM\n"); failures++; }
    if ((long long)(VT_VOID) != 24LL) { printf("FAIL constant VT_VOID\n"); failures++; }
    if ((long long)(WAIT_ABANDONED) != 128LL) { printf("FAIL constant WAIT_ABANDONED\n"); failures++; }
    if ((long long)(WAIT_ABANDONED_0) != 128LL) { printf("FAIL constant WAIT_ABANDONED_0\n"); failures++; }
    if ((long long)(WAIT_FAILED) != 4294967295LL) { printf("FAIL constant WAIT_FAILED\n"); failures++; }
    if ((long long)(WAIT_IO_COMPLETION) != 192LL) { printf("FAIL constant WAIT_IO_COMPLETION\n"); failures++; }
    if ((long long)(WAIT_OBJECT_0) != 0LL) { printf("FAIL constant WAIT_OBJECT_0\n"); failures++; }
    if ((long long)(WAIT_TIMEOUT) != 258LL) { printf("FAIL constant WAIT_TIMEOUT\n"); failures++; }
    if ((long long)(WA_ACTIVE) != 1LL) { printf("FAIL constant WA_ACTIVE\n"); failures++; }
    if ((long long)(WA_CLICKACTIVE) != 2LL) { printf("FAIL constant WA_CLICKACTIVE\n"); failures++; }
    if ((long long)(WA_INACTIVE) != 0LL) { printf("FAIL constant WA_INACTIVE\n"); failures++; }
    if ((long long)(WMSZ_BOTTOM) != 6LL) { printf("FAIL constant WMSZ_BOTTOM\n"); failures++; }
    if ((long long)(WMSZ_BOTTOMLEFT) != 7LL) { printf("FAIL constant WMSZ_BOTTOMLEFT\n"); failures++; }
    if ((long long)(WMSZ_BOTTOMRIGHT) != 8LL) { printf("FAIL constant WMSZ_BOTTOMRIGHT\n"); failures++; }
    if ((long long)(WMSZ_LEFT) != 1LL) { printf("FAIL constant WMSZ_LEFT\n"); failures++; }
    if ((long long)(WMSZ_RIGHT) != 2LL) { printf("FAIL constant WMSZ_RIGHT\n"); failures++; }
    if ((long long)(WMSZ_TOP) != 3LL) { printf("FAIL constant WMSZ_TOP\n"); failures++; }
    if ((long long)(WMSZ_TOPLEFT) != 4LL) { printf("FAIL constant WMSZ_TOPLEFT\n"); failures++; }
    if ((long long)(WMSZ_TOPRIGHT) != 5LL) { printf("FAIL constant WMSZ_TOPRIGHT\n"); failures++; }
    if ((long long)(WM_ACTIVATE) != 6LL) { printf("FAIL constant WM_ACTIVATE\n"); failures++; }
    if ((long long)(WM_ACTIVATEAPP) != 28LL) { printf("FAIL constant WM_ACTIVATEAPP\n"); failures++; }
    if ((long long)(WM_AFXFIRST) != 864LL) { printf("FAIL constant WM_AFXFIRST\n"); failures++; }
    if ((long long)(WM_AFXLAST) != 895LL) { printf("FAIL constant WM_AFXLAST\n"); failures++; }
    if ((long long)(WM_APP) != 32768LL) { printf("FAIL constant WM_APP\n"); failures++; }
    if ((long long)(WM_APPCOMMAND) != 793LL) { printf("FAIL constant WM_APPCOMMAND\n"); failures++; }
    if ((long long)(WM_ASKCBFORMATNAME) != 780LL) { printf("FAIL constant WM_ASKCBFORMATNAME\n"); failures++; }
    if ((long long)(WM_CANCELJOURNAL) != 75LL) { printf("FAIL constant WM_CANCELJOURNAL\n"); failures++; }
    if ((long long)(WM_CANCELMODE) != 31LL) { printf("FAIL constant WM_CANCELMODE\n"); failures++; }
    if ((long long)(WM_CAPTURECHANGED) != 533LL) { printf("FAIL constant WM_CAPTURECHANGED\n"); failures++; }
    if ((long long)(WM_CHANGECBCHAIN) != 781LL) { printf("FAIL constant WM_CHANGECBCHAIN\n"); failures++; }
    if ((long long)(WM_CHANGEUISTATE) != 295LL) { printf("FAIL constant WM_CHANGEUISTATE\n"); failures++; }
    if ((long long)(WM_CHAR) != 258LL) { printf("FAIL constant WM_CHAR\n"); failures++; }
    if ((long long)(WM_CHARTOITEM) != 47LL) { printf("FAIL constant WM_CHARTOITEM\n"); failures++; }
    if ((long long)(WM_CHILDACTIVATE) != 34LL) { printf("FAIL constant WM_CHILDACTIVATE\n"); failures++; }
    if ((long long)(WM_CHOOSEFONT_GETLOGFONT) != 1025LL) { printf("FAIL constant WM_CHOOSEFONT_GETLOGFONT\n"); failures++; }
    if ((long long)(WM_CHOOSEFONT_SETFLAGS) != 1126LL) { printf("FAIL constant WM_CHOOSEFONT_SETFLAGS\n"); failures++; }
    if ((long long)(WM_CHOOSEFONT_SETLOGFONT) != 1125LL) { printf("FAIL constant WM_CHOOSEFONT_SETLOGFONT\n"); failures++; }
    if ((long long)(WM_CLEAR) != 771LL) { printf("FAIL constant WM_CLEAR\n"); failures++; }
    if ((long long)(WM_CLIPBOARDUPDATE) != 797LL) { printf("FAIL constant WM_CLIPBOARDUPDATE\n"); failures++; }
    if ((long long)(WM_CLOSE) != 16LL) { printf("FAIL constant WM_CLOSE\n"); failures++; }
    if ((long long)(WM_COMMAND) != 273LL) { printf("FAIL constant WM_COMMAND\n"); failures++; }
    if ((long long)(WM_COMMNOTIFY) != 68LL) { printf("FAIL constant WM_COMMNOTIFY\n"); failures++; }
    if ((long long)(WM_COMPACTING) != 65LL) { printf("FAIL constant WM_COMPACTING\n"); failures++; }
    if ((long long)(WM_COMPAREITEM) != 57LL) { printf("FAIL constant WM_COMPAREITEM\n"); failures++; }
    if ((long long)(WM_CONTEXTMENU) != 123LL) { printf("FAIL constant WM_CONTEXTMENU\n"); failures++; }
    if ((long long)(WM_COPY) != 769LL) { printf("FAIL constant WM_COPY\n"); failures++; }
    if ((long long)(WM_COPYDATA) != 74LL) { printf("FAIL constant WM_COPYDATA\n"); failures++; }
    if ((long long)(WM_CREATE) != 1LL) { printf("FAIL constant WM_CREATE\n"); failures++; }
    if ((long long)(WM_CTLCOLORBTN) != 309LL) { printf("FAIL constant WM_CTLCOLORBTN\n"); failures++; }
    if ((long long)(WM_CTLCOLORDLG) != 310LL) { printf("FAIL constant WM_CTLCOLORDLG\n"); failures++; }
    if ((long long)(WM_CTLCOLOREDIT) != 307LL) { printf("FAIL constant WM_CTLCOLOREDIT\n"); failures++; }
    if ((long long)(WM_CTLCOLORLISTBOX) != 308LL) { printf("FAIL constant WM_CTLCOLORLISTBOX\n"); failures++; }
    if ((long long)(WM_CTLCOLORMSGBOX) != 306LL) { printf("FAIL constant WM_CTLCOLORMSGBOX\n"); failures++; }
    if ((long long)(WM_CTLCOLORSCROLLBAR) != 311LL) { printf("FAIL constant WM_CTLCOLORSCROLLBAR\n"); failures++; }
    if ((long long)(WM_CTLCOLORSTATIC) != 312LL) { printf("FAIL constant WM_CTLCOLORSTATIC\n"); failures++; }
    if ((long long)(WM_CUT) != 768LL) { printf("FAIL constant WM_CUT\n"); failures++; }
    if ((long long)(WM_DDE_ACK) != 996LL) { printf("FAIL constant WM_DDE_ACK\n"); failures++; }
    if ((long long)(WM_DDE_ADVISE) != 994LL) { printf("FAIL constant WM_DDE_ADVISE\n"); failures++; }
    if ((long long)(WM_DDE_DATA) != 997LL) { printf("FAIL constant WM_DDE_DATA\n"); failures++; }
    if ((long long)(WM_DDE_EXECUTE) != 1000LL) { printf("FAIL constant WM_DDE_EXECUTE\n"); failures++; }
    if ((long long)(WM_DDE_FIRST) != 992LL) { printf("FAIL constant WM_DDE_FIRST\n"); failures++; }
    if ((long long)(WM_DDE_INITIATE) != 992LL) { printf("FAIL constant WM_DDE_INITIATE\n"); failures++; }
    if ((long long)(WM_DDE_LAST) != 1000LL) { printf("FAIL constant WM_DDE_LAST\n"); failures++; }
    if ((long long)(WM_DDE_POKE) != 999LL) { printf("FAIL constant WM_DDE_POKE\n"); failures++; }
    if ((long long)(WM_DDE_REQUEST) != 998LL) { printf("FAIL constant WM_DDE_REQUEST\n"); failures++; }
    if ((long long)(WM_DDE_TERMINATE) != 993LL) { printf("FAIL constant WM_DDE_TERMINATE\n"); failures++; }
    if ((long long)(WM_DDE_UNADVISE) != 995LL) { printf("FAIL constant WM_DDE_UNADVISE\n"); failures++; }
    if ((long long)(WM_DEADCHAR) != 259LL) { printf("FAIL constant WM_DEADCHAR\n"); failures++; }
    if ((long long)(WM_DELETEITEM) != 45LL) { printf("FAIL constant WM_DELETEITEM\n"); failures++; }
    if ((long long)(WM_DESTROY) != 2LL) { printf("FAIL constant WM_DESTROY\n"); failures++; }
    if ((long long)(WM_DESTROYCLIPBOARD) != 775LL) { printf("FAIL constant WM_DESTROYCLIPBOARD\n"); failures++; }
    if ((long long)(WM_DEVICECHANGE) != 537LL) { printf("FAIL constant WM_DEVICECHANGE\n"); failures++; }
    if ((long long)(WM_DEVMODECHANGE) != 27LL) { printf("FAIL constant WM_DEVMODECHANGE\n"); failures++; }
    if ((long long)(WM_DISPLAYCHANGE) != 126LL) { printf("FAIL constant WM_DISPLAYCHANGE\n"); failures++; }
    if ((long long)(WM_DPICHANGED) != 736LL) { printf("FAIL constant WM_DPICHANGED\n"); failures++; }
    if ((long long)(WM_DPICHANGED_AFTERPARENT) != 739LL) { printf("FAIL constant WM_DPICHANGED_AFTERPARENT\n"); failures++; }
    if ((long long)(WM_DPICHANGED_BEFOREPARENT) != 738LL) { printf("FAIL constant WM_DPICHANGED_BEFOREPARENT\n"); failures++; }
    if ((long long)(WM_DRAWCLIPBOARD) != 776LL) { printf("FAIL constant WM_DRAWCLIPBOARD\n"); failures++; }
    if ((long long)(WM_DRAWITEM) != 43LL) { printf("FAIL constant WM_DRAWITEM\n"); failures++; }
    if ((long long)(WM_DROPFILES) != 563LL) { printf("FAIL constant WM_DROPFILES\n"); failures++; }
    if ((long long)(WM_DWMCOLORIZATIONCOLORCHANGED) != 800LL) { printf("FAIL constant WM_DWMCOLORIZATIONCOLORCHANGED\n"); failures++; }
    if ((long long)(WM_DWMCOMPOSITIONCHANGED) != 798LL) { printf("FAIL constant WM_DWMCOMPOSITIONCHANGED\n"); failures++; }
    if ((long long)(WM_DWMNCRENDERINGCHANGED) != 799LL) { printf("FAIL constant WM_DWMNCRENDERINGCHANGED\n"); failures++; }
    if ((long long)(WM_DWMSENDICONICLIVEPREVIEWBITMAP) != 806LL) { printf("FAIL constant WM_DWMSENDICONICLIVEPREVIEWBITMAP\n"); failures++; }
    if ((long long)(WM_DWMSENDICONICTHUMBNAIL) != 803LL) { printf("FAIL constant WM_DWMSENDICONICTHUMBNAIL\n"); failures++; }
    if ((long long)(WM_DWMWINDOWMAXIMIZEDCHANGE) != 801LL) { printf("FAIL constant WM_DWMWINDOWMAXIMIZEDCHANGE\n"); failures++; }
    if ((long long)(WM_ENABLE) != 10LL) { printf("FAIL constant WM_ENABLE\n"); failures++; }
    if ((long long)(WM_ENDSESSION) != 22LL) { printf("FAIL constant WM_ENDSESSION\n"); failures++; }
    if ((long long)(WM_ENTERIDLE) != 289LL) { printf("FAIL constant WM_ENTERIDLE\n"); failures++; }
    if ((long long)(WM_ENTERMENULOOP) != 529LL) { printf("FAIL constant WM_ENTERMENULOOP\n"); failures++; }
    if ((long long)(WM_ENTERSIZEMOVE) != 561LL) { printf("FAIL constant WM_ENTERSIZEMOVE\n"); failures++; }
    if ((long long)(WM_ERASEBKGND) != 20LL) { printf("FAIL constant WM_ERASEBKGND\n"); failures++; }
    if ((long long)(WM_EXITMENULOOP) != 530LL) { printf("FAIL constant WM_EXITMENULOOP\n"); failures++; }
    if ((long long)(WM_EXITSIZEMOVE) != 562LL) { printf("FAIL constant WM_EXITSIZEMOVE\n"); failures++; }
    if ((long long)(WM_FONTCHANGE) != 29LL) { printf("FAIL constant WM_FONTCHANGE\n"); failures++; }
    if ((long long)(WM_GESTURE) != 281LL) { printf("FAIL constant WM_GESTURE\n"); failures++; }
    if ((long long)(WM_GESTURENOTIFY) != 282LL) { printf("FAIL constant WM_GESTURENOTIFY\n"); failures++; }
    if ((long long)(WM_GETDLGCODE) != 135LL) { printf("FAIL constant WM_GETDLGCODE\n"); failures++; }
    if ((long long)(WM_GETDPISCALEDSIZE) != 740LL) { printf("FAIL constant WM_GETDPISCALEDSIZE\n"); failures++; }
    if ((long long)(WM_GETFONT) != 49LL) { printf("FAIL constant WM_GETFONT\n"); failures++; }
    if ((long long)(WM_GETHOTKEY) != 51LL) { printf("FAIL constant WM_GETHOTKEY\n"); failures++; }
    if ((long long)(WM_GETICON) != 127LL) { printf("FAIL constant WM_GETICON\n"); failures++; }
    if ((long long)(WM_GETMINMAXINFO) != 36LL) { printf("FAIL constant WM_GETMINMAXINFO\n"); failures++; }
    if ((long long)(WM_GETOBJECT) != 61LL) { printf("FAIL constant WM_GETOBJECT\n"); failures++; }
    if ((long long)(WM_GETTEXT) != 13LL) { printf("FAIL constant WM_GETTEXT\n"); failures++; }
    if ((long long)(WM_GETTEXTLENGTH) != 14LL) { printf("FAIL constant WM_GETTEXTLENGTH\n"); failures++; }
    if ((long long)(WM_GETTITLEBARINFOEX) != 831LL) { printf("FAIL constant WM_GETTITLEBARINFOEX\n"); failures++; }
    if ((long long)(WM_HANDHELDFIRST) != 856LL) { printf("FAIL constant WM_HANDHELDFIRST\n"); failures++; }
    if ((long long)(WM_HANDHELDLAST) != 863LL) { printf("FAIL constant WM_HANDHELDLAST\n"); failures++; }
    if ((long long)(WM_HELP) != 83LL) { printf("FAIL constant WM_HELP\n"); failures++; }
    if ((long long)(WM_HOTKEY) != 786LL) { printf("FAIL constant WM_HOTKEY\n"); failures++; }
    if ((long long)(WM_HSCROLL) != 276LL) { printf("FAIL constant WM_HSCROLL\n"); failures++; }
    if ((long long)(WM_HSCROLLCLIPBOARD) != 782LL) { printf("FAIL constant WM_HSCROLLCLIPBOARD\n"); failures++; }
    if ((long long)(WM_ICONERASEBKGND) != 39LL) { printf("FAIL constant WM_ICONERASEBKGND\n"); failures++; }
    if ((long long)(WM_IME_CHAR) != 646LL) { printf("FAIL constant WM_IME_CHAR\n"); failures++; }
    if ((long long)(WM_IME_COMPOSITION) != 271LL) { printf("FAIL constant WM_IME_COMPOSITION\n"); failures++; }
    if ((long long)(WM_IME_COMPOSITIONFULL) != 644LL) { printf("FAIL constant WM_IME_COMPOSITIONFULL\n"); failures++; }
    if ((long long)(WM_IME_CONTROL) != 643LL) { printf("FAIL constant WM_IME_CONTROL\n"); failures++; }
    if ((long long)(WM_IME_ENDCOMPOSITION) != 270LL) { printf("FAIL constant WM_IME_ENDCOMPOSITION\n"); failures++; }
    if ((long long)(WM_IME_KEYDOWN) != 656LL) { printf("FAIL constant WM_IME_KEYDOWN\n"); failures++; }
    if ((long long)(WM_IME_KEYLAST) != 271LL) { printf("FAIL constant WM_IME_KEYLAST\n"); failures++; }
    if ((long long)(WM_IME_KEYUP) != 657LL) { printf("FAIL constant WM_IME_KEYUP\n"); failures++; }
    if ((long long)(WM_IME_NOTIFY) != 642LL) { printf("FAIL constant WM_IME_NOTIFY\n"); failures++; }
    if ((long long)(WM_IME_REQUEST) != 648LL) { printf("FAIL constant WM_IME_REQUEST\n"); failures++; }
    if ((long long)(WM_IME_SELECT) != 645LL) { printf("FAIL constant WM_IME_SELECT\n"); failures++; }
    if ((long long)(WM_IME_SETCONTEXT) != 641LL) { printf("FAIL constant WM_IME_SETCONTEXT\n"); failures++; }
    if ((long long)(WM_IME_STARTCOMPOSITION) != 269LL) { printf("FAIL constant WM_IME_STARTCOMPOSITION\n"); failures++; }
    if ((long long)(WM_INITDIALOG) != 272LL) { printf("FAIL constant WM_INITDIALOG\n"); failures++; }
    if ((long long)(WM_INITMENU) != 278LL) { printf("FAIL constant WM_INITMENU\n"); failures++; }
    if ((long long)(WM_INITMENUPOPUP) != 279LL) { printf("FAIL constant WM_INITMENUPOPUP\n"); failures++; }
    if ((long long)(WM_INPUT) != 255LL) { printf("FAIL constant WM_INPUT\n"); failures++; }
    if ((long long)(WM_INPUTLANGCHANGE) != 81LL) { printf("FAIL constant WM_INPUTLANGCHANGE\n"); failures++; }
    if ((long long)(WM_INPUTLANGCHANGEREQUEST) != 80LL) { printf("FAIL constant WM_INPUTLANGCHANGEREQUEST\n"); failures++; }
    if ((long long)(WM_INPUT_DEVICE_CHANGE) != 254LL) { printf("FAIL constant WM_INPUT_DEVICE_CHANGE\n"); failures++; }
    if ((long long)(WM_KEYDOWN) != 256LL) { printf("FAIL constant WM_KEYDOWN\n"); failures++; }
    if ((long long)(WM_KEYFIRST) != 256LL) { printf("FAIL constant WM_KEYFIRST\n"); failures++; }
    if ((long long)(WM_KEYLAST) != 265LL) { printf("FAIL constant WM_KEYLAST\n"); failures++; }
    if ((long long)(WM_KEYUP) != 257LL) { printf("FAIL constant WM_KEYUP\n"); failures++; }
    if ((long long)(WM_KILLFOCUS) != 8LL) { printf("FAIL constant WM_KILLFOCUS\n"); failures++; }
    if ((long long)(WM_LBUTTONDBLCLK) != 515LL) { printf("FAIL constant WM_LBUTTONDBLCLK\n"); failures++; }
    if ((long long)(WM_LBUTTONDOWN) != 513LL) { printf("FAIL constant WM_LBUTTONDOWN\n"); failures++; }
    if ((long long)(WM_LBUTTONUP) != 514LL) { printf("FAIL constant WM_LBUTTONUP\n"); failures++; }
    if ((long long)(WM_MBUTTONDBLCLK) != 521LL) { printf("FAIL constant WM_MBUTTONDBLCLK\n"); failures++; }
    if ((long long)(WM_MBUTTONDOWN) != 519LL) { printf("FAIL constant WM_MBUTTONDOWN\n"); failures++; }
    if ((long long)(WM_MBUTTONUP) != 520LL) { printf("FAIL constant WM_MBUTTONUP\n"); failures++; }
    if ((long long)(WM_MDIACTIVATE) != 546LL) { printf("FAIL constant WM_MDIACTIVATE\n"); failures++; }
    if ((long long)(WM_MDICASCADE) != 551LL) { printf("FAIL constant WM_MDICASCADE\n"); failures++; }
    if ((long long)(WM_MDICREATE) != 544LL) { printf("FAIL constant WM_MDICREATE\n"); failures++; }
    if ((long long)(WM_MDIDESTROY) != 545LL) { printf("FAIL constant WM_MDIDESTROY\n"); failures++; }
    if ((long long)(WM_MDIGETACTIVE) != 553LL) { printf("FAIL constant WM_MDIGETACTIVE\n"); failures++; }
    if ((long long)(WM_MDIICONARRANGE) != 552LL) { printf("FAIL constant WM_MDIICONARRANGE\n"); failures++; }
    if ((long long)(WM_MDIMAXIMIZE) != 549LL) { printf("FAIL constant WM_MDIMAXIMIZE\n"); failures++; }
    if ((long long)(WM_MDINEXT) != 548LL) { printf("FAIL constant WM_MDINEXT\n"); failures++; }
    if ((long long)(WM_MDIREFRESHMENU) != 564LL) { printf("FAIL constant WM_MDIREFRESHMENU\n"); failures++; }
    if ((long long)(WM_MDIRESTORE) != 547LL) { printf("FAIL constant WM_MDIRESTORE\n"); failures++; }
    if ((long long)(WM_MDISETMENU) != 560LL) { printf("FAIL constant WM_MDISETMENU\n"); failures++; }
    if ((long long)(WM_MDITILE) != 550LL) { printf("FAIL constant WM_MDITILE\n"); failures++; }
    if ((long long)(WM_MEASUREITEM) != 44LL) { printf("FAIL constant WM_MEASUREITEM\n"); failures++; }
    if ((long long)(WM_MENUCHAR) != 288LL) { printf("FAIL constant WM_MENUCHAR\n"); failures++; }
    if ((long long)(WM_MENUCOMMAND) != 294LL) { printf("FAIL constant WM_MENUCOMMAND\n"); failures++; }
    if ((long long)(WM_MENUDRAG) != 291LL) { printf("FAIL constant WM_MENUDRAG\n"); failures++; }
    if ((long long)(WM_MENUGETOBJECT) != 292LL) { printf("FAIL constant WM_MENUGETOBJECT\n"); failures++; }
    if ((long long)(WM_MENURBUTTONUP) != 290LL) { printf("FAIL constant WM_MENURBUTTONUP\n"); failures++; }
    if ((long long)(WM_MENUSELECT) != 287LL) { printf("FAIL constant WM_MENUSELECT\n"); failures++; }
    if ((long long)(WM_MOUSEACTIVATE) != 33LL) { printf("FAIL constant WM_MOUSEACTIVATE\n"); failures++; }
    if ((long long)(WM_MOUSEFIRST) != 512LL) { printf("FAIL constant WM_MOUSEFIRST\n"); failures++; }
    if ((long long)(WM_MOUSEHOVER) != 673LL) { printf("FAIL constant WM_MOUSEHOVER\n"); failures++; }
    if ((long long)(WM_MOUSEHWHEEL) != 526LL) { printf("FAIL constant WM_MOUSEHWHEEL\n"); failures++; }
    if ((long long)(WM_MOUSELAST) != 526LL) { printf("FAIL constant WM_MOUSELAST\n"); failures++; }
    if ((long long)(WM_MOUSELEAVE) != 675LL) { printf("FAIL constant WM_MOUSELEAVE\n"); failures++; }
    if ((long long)(WM_MOUSEMOVE) != 512LL) { printf("FAIL constant WM_MOUSEMOVE\n"); failures++; }
    if ((long long)(WM_MOUSEWHEEL) != 522LL) { printf("FAIL constant WM_MOUSEWHEEL\n"); failures++; }
    if ((long long)(WM_MOVE) != 3LL) { printf("FAIL constant WM_MOVE\n"); failures++; }
    if ((long long)(WM_MOVING) != 534LL) { printf("FAIL constant WM_MOVING\n"); failures++; }
    if ((long long)(WM_NCACTIVATE) != 134LL) { printf("FAIL constant WM_NCACTIVATE\n"); failures++; }
    if ((long long)(WM_NCCALCSIZE) != 131LL) { printf("FAIL constant WM_NCCALCSIZE\n"); failures++; }
    if ((long long)(WM_NCCREATE) != 129LL) { printf("FAIL constant WM_NCCREATE\n"); failures++; }
    if ((long long)(WM_NCDESTROY) != 130LL) { printf("FAIL constant WM_NCDESTROY\n"); failures++; }
    if ((long long)(WM_NCHITTEST) != 132LL) { printf("FAIL constant WM_NCHITTEST\n"); failures++; }
    if ((long long)(WM_NCLBUTTONDBLCLK) != 163LL) { printf("FAIL constant WM_NCLBUTTONDBLCLK\n"); failures++; }
    if ((long long)(WM_NCLBUTTONDOWN) != 161LL) { printf("FAIL constant WM_NCLBUTTONDOWN\n"); failures++; }
    if ((long long)(WM_NCLBUTTONUP) != 162LL) { printf("FAIL constant WM_NCLBUTTONUP\n"); failures++; }
    if ((long long)(WM_NCMBUTTONDBLCLK) != 169LL) { printf("FAIL constant WM_NCMBUTTONDBLCLK\n"); failures++; }
    if ((long long)(WM_NCMBUTTONDOWN) != 167LL) { printf("FAIL constant WM_NCMBUTTONDOWN\n"); failures++; }
    if ((long long)(WM_NCMBUTTONUP) != 168LL) { printf("FAIL constant WM_NCMBUTTONUP\n"); failures++; }
    if ((long long)(WM_NCMOUSEHOVER) != 672LL) { printf("FAIL constant WM_NCMOUSEHOVER\n"); failures++; }
    if ((long long)(WM_NCMOUSELEAVE) != 674LL) { printf("FAIL constant WM_NCMOUSELEAVE\n"); failures++; }
    if ((long long)(WM_NCMOUSEMOVE) != 160LL) { printf("FAIL constant WM_NCMOUSEMOVE\n"); failures++; }
    if ((long long)(WM_NCPAINT) != 133LL) { printf("FAIL constant WM_NCPAINT\n"); failures++; }
    if ((long long)(WM_NCPOINTERDOWN) != 578LL) { printf("FAIL constant WM_NCPOINTERDOWN\n"); failures++; }
    if ((long long)(WM_NCPOINTERUP) != 579LL) { printf("FAIL constant WM_NCPOINTERUP\n"); failures++; }
    if ((long long)(WM_NCPOINTERUPDATE) != 577LL) { printf("FAIL constant WM_NCPOINTERUPDATE\n"); failures++; }
    if ((long long)(WM_NCRBUTTONDBLCLK) != 166LL) { printf("FAIL constant WM_NCRBUTTONDBLCLK\n"); failures++; }
    if ((long long)(WM_NCRBUTTONDOWN) != 164LL) { printf("FAIL constant WM_NCRBUTTONDOWN\n"); failures++; }
    if ((long long)(WM_NCRBUTTONUP) != 165LL) { printf("FAIL constant WM_NCRBUTTONUP\n"); failures++; }
    if ((long long)(WM_NCXBUTTONDBLCLK) != 173LL) { printf("FAIL constant WM_NCXBUTTONDBLCLK\n"); failures++; }
    if ((long long)(WM_NCXBUTTONDOWN) != 171LL) { printf("FAIL constant WM_NCXBUTTONDOWN\n"); failures++; }
    if ((long long)(WM_NCXBUTTONUP) != 172LL) { printf("FAIL constant WM_NCXBUTTONUP\n"); failures++; }
    if ((long long)(WM_NEXTDLGCTL) != 40LL) { printf("FAIL constant WM_NEXTDLGCTL\n"); failures++; }
    if ((long long)(WM_NEXTMENU) != 531LL) { printf("FAIL constant WM_NEXTMENU\n"); failures++; }
    if ((long long)(WM_NOTIFY) != 78LL) { printf("FAIL constant WM_NOTIFY\n"); failures++; }
    if ((long long)(WM_NOTIFYFORMAT) != 85LL) { printf("FAIL constant WM_NOTIFYFORMAT\n"); failures++; }
    if ((long long)(WM_NULL) != 0LL) { printf("FAIL constant WM_NULL\n"); failures++; }
    if ((long long)(WM_PAINT) != 15LL) { printf("FAIL constant WM_PAINT\n"); failures++; }
    if ((long long)(WM_PAINTCLIPBOARD) != 777LL) { printf("FAIL constant WM_PAINTCLIPBOARD\n"); failures++; }
    if ((long long)(WM_PAINTICON) != 38LL) { printf("FAIL constant WM_PAINTICON\n"); failures++; }
    if ((long long)(WM_PALETTECHANGED) != 785LL) { printf("FAIL constant WM_PALETTECHANGED\n"); failures++; }
    if ((long long)(WM_PALETTEISCHANGING) != 784LL) { printf("FAIL constant WM_PALETTEISCHANGING\n"); failures++; }
    if ((long long)(WM_PARENTNOTIFY) != 528LL) { printf("FAIL constant WM_PARENTNOTIFY\n"); failures++; }
    if ((long long)(WM_PASTE) != 770LL) { printf("FAIL constant WM_PASTE\n"); failures++; }
    if ((long long)(WM_PENWINFIRST) != 896LL) { printf("FAIL constant WM_PENWINFIRST\n"); failures++; }
    if ((long long)(WM_PENWINLAST) != 911LL) { printf("FAIL constant WM_PENWINLAST\n"); failures++; }
    if ((long long)(WM_POINTERACTIVATE) != 587LL) { printf("FAIL constant WM_POINTERACTIVATE\n"); failures++; }
    if ((long long)(WM_POINTERCAPTURECHANGED) != 588LL) { printf("FAIL constant WM_POINTERCAPTURECHANGED\n"); failures++; }
    if ((long long)(WM_POINTERDEVICECHANGE) != 568LL) { printf("FAIL constant WM_POINTERDEVICECHANGE\n"); failures++; }
    if ((long long)(WM_POINTERDEVICEINRANGE) != 569LL) { printf("FAIL constant WM_POINTERDEVICEINRANGE\n"); failures++; }
    if ((long long)(WM_POINTERDEVICEOUTOFRANGE) != 570LL) { printf("FAIL constant WM_POINTERDEVICEOUTOFRANGE\n"); failures++; }
    if ((long long)(WM_POINTERDOWN) != 582LL) { printf("FAIL constant WM_POINTERDOWN\n"); failures++; }
    if ((long long)(WM_POINTERENTER) != 585LL) { printf("FAIL constant WM_POINTERENTER\n"); failures++; }
    if ((long long)(WM_POINTERHWHEEL) != 591LL) { printf("FAIL constant WM_POINTERHWHEEL\n"); failures++; }
    if ((long long)(WM_POINTERLEAVE) != 586LL) { printf("FAIL constant WM_POINTERLEAVE\n"); failures++; }
    if ((long long)(WM_POINTERROUTEDAWAY) != 594LL) { printf("FAIL constant WM_POINTERROUTEDAWAY\n"); failures++; }
    if ((long long)(WM_POINTERROUTEDRELEASED) != 595LL) { printf("FAIL constant WM_POINTERROUTEDRELEASED\n"); failures++; }
    if ((long long)(WM_POINTERROUTEDTO) != 593LL) { printf("FAIL constant WM_POINTERROUTEDTO\n"); failures++; }
    if ((long long)(WM_POINTERUP) != 583LL) { printf("FAIL constant WM_POINTERUP\n"); failures++; }
    if ((long long)(WM_POINTERUPDATE) != 581LL) { printf("FAIL constant WM_POINTERUPDATE\n"); failures++; }
    if ((long long)(WM_POINTERWHEEL) != 590LL) { printf("FAIL constant WM_POINTERWHEEL\n"); failures++; }
    if ((long long)(WM_POWER) != 72LL) { printf("FAIL constant WM_POWER\n"); failures++; }
    if ((long long)(WM_POWERBROADCAST) != 536LL) { printf("FAIL constant WM_POWERBROADCAST\n"); failures++; }
    if ((long long)(WM_PRINT) != 791LL) { printf("FAIL constant WM_PRINT\n"); failures++; }
    if ((long long)(WM_PRINTCLIENT) != 792LL) { printf("FAIL constant WM_PRINTCLIENT\n"); failures++; }
    if ((long long)(WM_PSD_ENVSTAMPRECT) != 1029LL) { printf("FAIL constant WM_PSD_ENVSTAMPRECT\n"); failures++; }
    if ((long long)(WM_PSD_FULLPAGERECT) != 1025LL) { printf("FAIL constant WM_PSD_FULLPAGERECT\n"); failures++; }
    if ((long long)(WM_PSD_GREEKTEXTRECT) != 1028LL) { printf("FAIL constant WM_PSD_GREEKTEXTRECT\n"); failures++; }
    if ((long long)(WM_PSD_MARGINRECT) != 1027LL) { printf("FAIL constant WM_PSD_MARGINRECT\n"); failures++; }
    if ((long long)(WM_PSD_MINMARGINRECT) != 1026LL) { printf("FAIL constant WM_PSD_MINMARGINRECT\n"); failures++; }
    if ((long long)(WM_PSD_PAGESETUPDLG) != 1024LL) { printf("FAIL constant WM_PSD_PAGESETUPDLG\n"); failures++; }
    if ((long long)(WM_PSD_YAFULLPAGERECT) != 1030LL) { printf("FAIL constant WM_PSD_YAFULLPAGERECT\n"); failures++; }
    if ((long long)(WM_QUERYDRAGICON) != 55LL) { printf("FAIL constant WM_QUERYDRAGICON\n"); failures++; }
    if ((long long)(WM_QUERYENDSESSION) != 17LL) { printf("FAIL constant WM_QUERYENDSESSION\n"); failures++; }
    if ((long long)(WM_QUERYNEWPALETTE) != 783LL) { printf("FAIL constant WM_QUERYNEWPALETTE\n"); failures++; }
    if ((long long)(WM_QUERYOPEN) != 19LL) { printf("FAIL constant WM_QUERYOPEN\n"); failures++; }
    if ((long long)(WM_QUERYUISTATE) != 297LL) { printf("FAIL constant WM_QUERYUISTATE\n"); failures++; }
    if ((long long)(WM_QUEUESYNC) != 35LL) { printf("FAIL constant WM_QUEUESYNC\n"); failures++; }
    if ((long long)(WM_QUIT) != 18LL) { printf("FAIL constant WM_QUIT\n"); failures++; }
    if ((long long)(WM_RBUTTONDBLCLK) != 518LL) { printf("FAIL constant WM_RBUTTONDBLCLK\n"); failures++; }
    if ((long long)(WM_RBUTTONDOWN) != 516LL) { printf("FAIL constant WM_RBUTTONDOWN\n"); failures++; }
    if ((long long)(WM_RBUTTONUP) != 517LL) { printf("FAIL constant WM_RBUTTONUP\n"); failures++; }
    if ((long long)(WM_RENDERALLFORMATS) != 774LL) { printf("FAIL constant WM_RENDERALLFORMATS\n"); failures++; }
    if ((long long)(WM_RENDERFORMAT) != 773LL) { printf("FAIL constant WM_RENDERFORMAT\n"); failures++; }
    if ((long long)(WM_SETCURSOR) != 32LL) { printf("FAIL constant WM_SETCURSOR\n"); failures++; }
    if ((long long)(WM_SETFOCUS) != 7LL) { printf("FAIL constant WM_SETFOCUS\n"); failures++; }
    if ((long long)(WM_SETFONT) != 48LL) { printf("FAIL constant WM_SETFONT\n"); failures++; }
    if ((long long)(WM_SETHOTKEY) != 50LL) { printf("FAIL constant WM_SETHOTKEY\n"); failures++; }
    if ((long long)(WM_SETICON) != 128LL) { printf("FAIL constant WM_SETICON\n"); failures++; }
    if ((long long)(WM_SETREDRAW) != 11LL) { printf("FAIL constant WM_SETREDRAW\n"); failures++; }
    if ((long long)(WM_SETTEXT) != 12LL) { printf("FAIL constant WM_SETTEXT\n"); failures++; }
    if ((long long)(WM_SETTINGCHANGE) != 26LL) { printf("FAIL constant WM_SETTINGCHANGE\n"); failures++; }
    if ((long long)(WM_SHOWWINDOW) != 24LL) { printf("FAIL constant WM_SHOWWINDOW\n"); failures++; }
    if ((long long)(WM_SIZE) != 5LL) { printf("FAIL constant WM_SIZE\n"); failures++; }
    if ((long long)(WM_SIZECLIPBOARD) != 779LL) { printf("FAIL constant WM_SIZECLIPBOARD\n"); failures++; }
    if ((long long)(WM_SIZING) != 532LL) { printf("FAIL constant WM_SIZING\n"); failures++; }
    if ((long long)(WM_SPOOLERSTATUS) != 42LL) { printf("FAIL constant WM_SPOOLERSTATUS\n"); failures++; }
    if ((long long)(WM_STYLECHANGED) != 125LL) { printf("FAIL constant WM_STYLECHANGED\n"); failures++; }
    if ((long long)(WM_STYLECHANGING) != 124LL) { printf("FAIL constant WM_STYLECHANGING\n"); failures++; }
    if ((long long)(WM_SYNCPAINT) != 136LL) { printf("FAIL constant WM_SYNCPAINT\n"); failures++; }
    if ((long long)(WM_SYSCHAR) != 262LL) { printf("FAIL constant WM_SYSCHAR\n"); failures++; }
    if ((long long)(WM_SYSCOLORCHANGE) != 21LL) { printf("FAIL constant WM_SYSCOLORCHANGE\n"); failures++; }
    if ((long long)(WM_SYSCOMMAND) != 274LL) { printf("FAIL constant WM_SYSCOMMAND\n"); failures++; }
    if ((long long)(WM_SYSDEADCHAR) != 263LL) { printf("FAIL constant WM_SYSDEADCHAR\n"); failures++; }
    if ((long long)(WM_SYSKEYDOWN) != 260LL) { printf("FAIL constant WM_SYSKEYDOWN\n"); failures++; }
    if ((long long)(WM_SYSKEYUP) != 261LL) { printf("FAIL constant WM_SYSKEYUP\n"); failures++; }
    if ((long long)(WM_TABLET_FIRST) != 704LL) { printf("FAIL constant WM_TABLET_FIRST\n"); failures++; }
    if ((long long)(WM_TABLET_LAST) != 735LL) { printf("FAIL constant WM_TABLET_LAST\n"); failures++; }
    if ((long long)(WM_TCARD) != 82LL) { printf("FAIL constant WM_TCARD\n"); failures++; }
    if ((long long)(WM_THEMECHANGED) != 794LL) { printf("FAIL constant WM_THEMECHANGED\n"); failures++; }
    if ((long long)(WM_TIMECHANGE) != 30LL) { printf("FAIL constant WM_TIMECHANGE\n"); failures++; }
    if ((long long)(WM_TIMER) != 275LL) { printf("FAIL constant WM_TIMER\n"); failures++; }
    if ((long long)(WM_TOUCH) != 576LL) { printf("FAIL constant WM_TOUCH\n"); failures++; }
    if ((long long)(WM_TOUCHHITTESTING) != 589LL) { printf("FAIL constant WM_TOUCHHITTESTING\n"); failures++; }
    if ((long long)(WM_UNDO) != 772LL) { printf("FAIL constant WM_UNDO\n"); failures++; }
    if ((long long)(WM_UNICHAR) != 265LL) { printf("FAIL constant WM_UNICHAR\n"); failures++; }
    if ((long long)(WM_UNINITMENUPOPUP) != 293LL) { printf("FAIL constant WM_UNINITMENUPOPUP\n"); failures++; }
    if ((long long)(WM_UPDATEUISTATE) != 296LL) { printf("FAIL constant WM_UPDATEUISTATE\n"); failures++; }
    if ((long long)(WM_USER) != 1024LL) { printf("FAIL constant WM_USER\n"); failures++; }
    if ((long long)(WM_USERCHANGED) != 84LL) { printf("FAIL constant WM_USERCHANGED\n"); failures++; }
    if ((long long)(WM_VKEYTOITEM) != 46LL) { printf("FAIL constant WM_VKEYTOITEM\n"); failures++; }
    if ((long long)(WM_VSCROLL) != 277LL) { printf("FAIL constant WM_VSCROLL\n"); failures++; }
    if ((long long)(WM_VSCROLLCLIPBOARD) != 778LL) { printf("FAIL constant WM_VSCROLLCLIPBOARD\n"); failures++; }
    if ((long long)(WM_WINDOWPOSCHANGED) != 71LL) { printf("FAIL constant WM_WINDOWPOSCHANGED\n"); failures++; }
    if ((long long)(WM_WINDOWPOSCHANGING) != 70LL) { printf("FAIL constant WM_WINDOWPOSCHANGING\n"); failures++; }
    if ((long long)(WM_WININICHANGE) != 26LL) { printf("FAIL constant WM_WININICHANGE\n"); failures++; }
    if ((long long)(WM_WTSSESSION_CHANGE) != 689LL) { printf("FAIL constant WM_WTSSESSION_CHANGE\n"); failures++; }
    if ((long long)(WM_XBUTTONDBLCLK) != 525LL) { printf("FAIL constant WM_XBUTTONDBLCLK\n"); failures++; }
    if ((long long)(WM_XBUTTONDOWN) != 523LL) { printf("FAIL constant WM_XBUTTONDOWN\n"); failures++; }
    if ((long long)(WM_XBUTTONUP) != 524LL) { printf("FAIL constant WM_XBUTTONUP\n"); failures++; }
    if ((long long)(WS_ACTIVECAPTION) != 1LL) { printf("FAIL constant WS_ACTIVECAPTION\n"); failures++; }
    if ((long long)(WS_BORDER) != 8388608LL) { printf("FAIL constant WS_BORDER\n"); failures++; }
    if ((long long)(WS_CAPTION) != 12582912LL) { printf("FAIL constant WS_CAPTION\n"); failures++; }
    if ((long long)(WS_CHILD) != 1073741824LL) { printf("FAIL constant WS_CHILD\n"); failures++; }
    if ((long long)(WS_CHILDWINDOW) != 1073741824LL) { printf("FAIL constant WS_CHILDWINDOW\n"); failures++; }
    if ((long long)(WS_CLIPCHILDREN) != 33554432LL) { printf("FAIL constant WS_CLIPCHILDREN\n"); failures++; }
    if ((long long)(WS_CLIPSIBLINGS) != 67108864LL) { printf("FAIL constant WS_CLIPSIBLINGS\n"); failures++; }
    if ((long long)(WS_DISABLED) != 134217728LL) { printf("FAIL constant WS_DISABLED\n"); failures++; }
    if ((long long)(WS_DLGFRAME) != 4194304LL) { printf("FAIL constant WS_DLGFRAME\n"); failures++; }
    if ((long long)(WS_EX_ACCEPTFILES) != 16LL) { printf("FAIL constant WS_EX_ACCEPTFILES\n"); failures++; }
    if ((long long)(WS_EX_APPWINDOW) != 262144LL) { printf("FAIL constant WS_EX_APPWINDOW\n"); failures++; }
    if ((long long)(WS_EX_CLIENTEDGE) != 512LL) { printf("FAIL constant WS_EX_CLIENTEDGE\n"); failures++; }
    if ((long long)(WS_EX_COMPOSITED) != 33554432LL) { printf("FAIL constant WS_EX_COMPOSITED\n"); failures++; }
    if ((long long)(WS_EX_CONTEXTHELP) != 1024LL) { printf("FAIL constant WS_EX_CONTEXTHELP\n"); failures++; }
    if ((long long)(WS_EX_CONTROLPARENT) != 65536LL) { printf("FAIL constant WS_EX_CONTROLPARENT\n"); failures++; }
    if ((long long)(WS_EX_DLGMODALFRAME) != 1LL) { printf("FAIL constant WS_EX_DLGMODALFRAME\n"); failures++; }
    if ((long long)(WS_EX_LAYERED) != 524288LL) { printf("FAIL constant WS_EX_LAYERED\n"); failures++; }
    if ((long long)(WS_EX_LAYOUTRTL) != 4194304LL) { printf("FAIL constant WS_EX_LAYOUTRTL\n"); failures++; }
    if ((long long)(WS_EX_LEFT) != 0LL) { printf("FAIL constant WS_EX_LEFT\n"); failures++; }
    if ((long long)(WS_EX_LEFTSCROLLBAR) != 16384LL) { printf("FAIL constant WS_EX_LEFTSCROLLBAR\n"); failures++; }
    if ((long long)(WS_EX_LTRREADING) != 0LL) { printf("FAIL constant WS_EX_LTRREADING\n"); failures++; }
    if ((long long)(WS_EX_MDICHILD) != 64LL) { printf("FAIL constant WS_EX_MDICHILD\n"); failures++; }
    if ((long long)(WS_EX_NOACTIVATE) != 134217728LL) { printf("FAIL constant WS_EX_NOACTIVATE\n"); failures++; }
    if ((long long)(WS_EX_NOINHERITLAYOUT) != 1048576LL) { printf("FAIL constant WS_EX_NOINHERITLAYOUT\n"); failures++; }
    if ((long long)(WS_EX_NOPARENTNOTIFY) != 4LL) { printf("FAIL constant WS_EX_NOPARENTNOTIFY\n"); failures++; }
    if ((long long)(WS_EX_NOREDIRECTIONBITMAP) != 2097152LL) { printf("FAIL constant WS_EX_NOREDIRECTIONBITMAP\n"); failures++; }
    if ((long long)(WS_EX_OVERLAPPEDWINDOW) != 768LL) { printf("FAIL constant WS_EX_OVERLAPPEDWINDOW\n"); failures++; }
    if ((long long)(WS_EX_PALETTEWINDOW) != 392LL) { printf("FAIL constant WS_EX_PALETTEWINDOW\n"); failures++; }
    if ((long long)(WS_EX_RIGHT) != 4096LL) { printf("FAIL constant WS_EX_RIGHT\n"); failures++; }
    if ((long long)(WS_EX_RIGHTSCROLLBAR) != 0LL) { printf("FAIL constant WS_EX_RIGHTSCROLLBAR\n"); failures++; }
    if ((long long)(WS_EX_RTLREADING) != 8192LL) { printf("FAIL constant WS_EX_RTLREADING\n"); failures++; }
    if ((long long)(WS_EX_STATICEDGE) != 131072LL) { printf("FAIL constant WS_EX_STATICEDGE\n"); failures++; }
    if ((long long)(WS_EX_TOOLWINDOW) != 128LL) { printf("FAIL constant WS_EX_TOOLWINDOW\n"); failures++; }
    if ((long long)(WS_EX_TOPMOST) != 8LL) { printf("FAIL constant WS_EX_TOPMOST\n"); failures++; }
    if ((long long)(WS_EX_TRANSPARENT) != 32LL) { printf("FAIL constant WS_EX_TRANSPARENT\n"); failures++; }
    if ((long long)(WS_EX_WINDOWEDGE) != 256LL) { printf("FAIL constant WS_EX_WINDOWEDGE\n"); failures++; }
    if ((long long)(WS_GROUP) != 131072LL) { printf("FAIL constant WS_GROUP\n"); failures++; }
    if ((long long)(WS_HSCROLL) != 1048576LL) { printf("FAIL constant WS_HSCROLL\n"); failures++; }
    if ((long long)(WS_ICONIC) != 536870912LL) { printf("FAIL constant WS_ICONIC\n"); failures++; }
    if ((long long)(WS_MAXIMIZE) != 16777216LL) { printf("FAIL constant WS_MAXIMIZE\n"); failures++; }
    if ((long long)(WS_MAXIMIZEBOX) != 65536LL) { printf("FAIL constant WS_MAXIMIZEBOX\n"); failures++; }
    if ((long long)(WS_MINIMIZE) != 536870912LL) { printf("FAIL constant WS_MINIMIZE\n"); failures++; }
    if ((long long)(WS_MINIMIZEBOX) != 131072LL) { printf("FAIL constant WS_MINIMIZEBOX\n"); failures++; }
    if ((long long)(WS_OVERLAPPED) != 0LL) { printf("FAIL constant WS_OVERLAPPED\n"); failures++; }
    if ((long long)(WS_OVERLAPPEDWINDOW) != 13565952LL) { printf("FAIL constant WS_OVERLAPPEDWINDOW\n"); failures++; }
    if ((long long)(WS_POPUP) != 2147483648LL) { printf("FAIL constant WS_POPUP\n"); failures++; }
    if ((long long)(WS_POPUPWINDOW) != 2156396544LL) { printf("FAIL constant WS_POPUPWINDOW\n"); failures++; }
    if ((long long)(WS_SIZEBOX) != 262144LL) { printf("FAIL constant WS_SIZEBOX\n"); failures++; }
    if ((long long)(WS_SYSMENU) != 524288LL) { printf("FAIL constant WS_SYSMENU\n"); failures++; }
    if ((long long)(WS_TABSTOP) != 65536LL) { printf("FAIL constant WS_TABSTOP\n"); failures++; }
    if ((long long)(WS_THICKFRAME) != 262144LL) { printf("FAIL constant WS_THICKFRAME\n"); failures++; }
    if ((long long)(WS_TILED) != 0LL) { printf("FAIL constant WS_TILED\n"); failures++; }
    if ((long long)(WS_TILEDWINDOW) != 13565952LL) { printf("FAIL constant WS_TILEDWINDOW\n"); failures++; }
    if ((long long)(WS_VISIBLE) != 268435456LL) { printf("FAIL constant WS_VISIBLE\n"); failures++; }
    if ((long long)(WS_VSCROLL) != 2097152LL) { printf("FAIL constant WS_VSCROLL\n"); failures++; }
    if ((long long)(WVR_ALIGNBOTTOM) != 64LL) { printf("FAIL constant WVR_ALIGNBOTTOM\n"); failures++; }
    if ((long long)(WVR_ALIGNLEFT) != 32LL) { printf("FAIL constant WVR_ALIGNLEFT\n"); failures++; }
    if ((long long)(WVR_ALIGNRIGHT) != 128LL) { printf("FAIL constant WVR_ALIGNRIGHT\n"); failures++; }
    if ((long long)(WVR_ALIGNTOP) != 16LL) { printf("FAIL constant WVR_ALIGNTOP\n"); failures++; }
    if ((long long)(WVR_HREDRAW) != 256LL) { printf("FAIL constant WVR_HREDRAW\n"); failures++; }
    if ((long long)(WVR_REDRAW) != 768LL) { printf("FAIL constant WVR_REDRAW\n"); failures++; }
    if ((long long)(WVR_VALIDRECTS) != 1024LL) { printf("FAIL constant WVR_VALIDRECTS\n"); failures++; }
    if ((long long)(WVR_VREDRAW) != 512LL) { printf("FAIL constant WVR_VREDRAW\n"); failures++; }
    if (failures != 0) return 1;
    printf("win32 constants: 2456 checked\n");
    return 0;
}
