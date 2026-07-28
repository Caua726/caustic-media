/* window/x11/tools/x11_layout.c — where the offsets come from.
 *
 * Caustic structs are packed and C structs are not, so every struct bound in
 * window/x11/bind/ carries explicit _padN fields wherever the C ABI would have
 * inserted padding. Those pads are load-bearing: drop the one after
 * XConfigureEvent.border_width and `above` lands at 68 instead of 72, a resize
 * reads the wrong Window, and nothing says a word.
 *
 * So the numbers are not counted by hand. This program asks the C compiler
 * that built the libX11 on this machine, and its output is the reference the
 * bindings are written from and x11_layout_test.cst asserts against.
 *
 * It is committed rather than run by the build for two reasons: CI then needs
 * no C compiler, and re-deriving the layout becomes a deliberate act with a
 * visible diff instead of something that changes silently under a system
 * update. That is window.md's "every system update is a manual audit" reduced
 * to "a diff in one file".
 *
 *   cc -o x11_layout x11_layout.c && ./x11_layout > layout.txt
 *
 * The output is x86_64 Linux/glibc. Another architecture needs another table;
 * nothing here detects that, and the test it feeds would fail loudly rather
 * than quietly if it were ever run somewhere else.
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XShm.h>
#include <X11/extensions/scrnsaver.h>
#include <stddef.h>
#include <stdio.h>

/* `size` lines pin sizeof, which is what lets Caustic walk an array the server
 * allocated. `off` lines pin one field each. The test file mirrors both. */
#define S(t)    printf("size %-24s %zu\n", #t, sizeof(t))
#define F(t, f) printf("off  %-24s %-18s %zu\n", #t, #f, offsetof(t, f))
/* Constants are checked by tools/check_constants.sh diffing bind/x.cst against
 * these lines, not by assertions in the test: an assertion comparing a number
 * this repo typed against a number this repo typed proves nothing. The value
 * has to come from the header to mean anything. */
#define C(n)    printf("const %-28s %lld\n", #n, (long long)(n))

static void events(void)
{
    printf("# --- events: the 33 members of the XEvent union ---\n");

    /* The union itself. Xlib declares `long pad[24]`, so every XEvent-taking
     * function expects 192 bytes whichever member is live. */
    S(XEvent);

    S(XAnyEvent);
    F(XAnyEvent, type);
    F(XAnyEvent, serial);
    F(XAnyEvent, send_event);
    F(XAnyEvent, display);
    F(XAnyEvent, window);

    S(XKeyEvent);
    F(XKeyEvent, type);
    F(XKeyEvent, serial);
    F(XKeyEvent, send_event);
    F(XKeyEvent, display);
    F(XKeyEvent, window);
    F(XKeyEvent, root);
    F(XKeyEvent, subwindow);
    F(XKeyEvent, time);
    F(XKeyEvent, x);
    F(XKeyEvent, y);
    F(XKeyEvent, x_root);
    F(XKeyEvent, y_root);
    F(XKeyEvent, state);
    F(XKeyEvent, keycode);
    F(XKeyEvent, same_screen);

    S(XButtonEvent);
    F(XButtonEvent, type);
    F(XButtonEvent, serial);
    F(XButtonEvent, send_event);
    F(XButtonEvent, display);
    F(XButtonEvent, window);
    F(XButtonEvent, root);
    F(XButtonEvent, subwindow);
    F(XButtonEvent, time);
    F(XButtonEvent, x);
    F(XButtonEvent, y);
    F(XButtonEvent, x_root);
    F(XButtonEvent, y_root);
    F(XButtonEvent, state);
    F(XButtonEvent, button);
    F(XButtonEvent, same_screen);

    /* is_hint is a `char`, not an int — the one place a member of this family
     * breaks the pattern, and the reason same_screen is not where the button
     * and key events put it. */
    S(XMotionEvent);
    F(XMotionEvent, type);
    F(XMotionEvent, serial);
    F(XMotionEvent, send_event);
    F(XMotionEvent, display);
    F(XMotionEvent, window);
    F(XMotionEvent, root);
    F(XMotionEvent, subwindow);
    F(XMotionEvent, time);
    F(XMotionEvent, x);
    F(XMotionEvent, y);
    F(XMotionEvent, x_root);
    F(XMotionEvent, y_root);
    F(XMotionEvent, state);
    F(XMotionEvent, is_hint);
    F(XMotionEvent, same_screen);

    S(XCrossingEvent);
    F(XCrossingEvent, type);
    F(XCrossingEvent, serial);
    F(XCrossingEvent, send_event);
    F(XCrossingEvent, display);
    F(XCrossingEvent, window);
    F(XCrossingEvent, root);
    F(XCrossingEvent, subwindow);
    F(XCrossingEvent, time);
    F(XCrossingEvent, x);
    F(XCrossingEvent, y);
    F(XCrossingEvent, x_root);
    F(XCrossingEvent, y_root);
    F(XCrossingEvent, mode);
    F(XCrossingEvent, detail);
    F(XCrossingEvent, same_screen);
    F(XCrossingEvent, focus);
    F(XCrossingEvent, state);

    S(XFocusChangeEvent);
    F(XFocusChangeEvent, type);
    F(XFocusChangeEvent, serial);
    F(XFocusChangeEvent, send_event);
    F(XFocusChangeEvent, display);
    F(XFocusChangeEvent, window);
    F(XFocusChangeEvent, mode);
    F(XFocusChangeEvent, detail);

    /* key_vector is a 32-byte bitmap, one bit per keycode. */
    S(XKeymapEvent);
    F(XKeymapEvent, type);
    F(XKeymapEvent, serial);
    F(XKeymapEvent, send_event);
    F(XKeymapEvent, display);
    F(XKeymapEvent, window);
    F(XKeymapEvent, key_vector);

    S(XExposeEvent);
    F(XExposeEvent, type);
    F(XExposeEvent, serial);
    F(XExposeEvent, send_event);
    F(XExposeEvent, display);
    F(XExposeEvent, window);
    F(XExposeEvent, x);
    F(XExposeEvent, y);
    F(XExposeEvent, width);
    F(XExposeEvent, height);
    F(XExposeEvent, count);

    S(XGraphicsExposeEvent);
    F(XGraphicsExposeEvent, type);
    F(XGraphicsExposeEvent, serial);
    F(XGraphicsExposeEvent, send_event);
    F(XGraphicsExposeEvent, display);
    F(XGraphicsExposeEvent, drawable);
    F(XGraphicsExposeEvent, x);
    F(XGraphicsExposeEvent, y);
    F(XGraphicsExposeEvent, width);
    F(XGraphicsExposeEvent, height);
    F(XGraphicsExposeEvent, count);
    F(XGraphicsExposeEvent, major_code);
    F(XGraphicsExposeEvent, minor_code);

    S(XNoExposeEvent);
    F(XNoExposeEvent, type);
    F(XNoExposeEvent, serial);
    F(XNoExposeEvent, send_event);
    F(XNoExposeEvent, display);
    F(XNoExposeEvent, drawable);
    F(XNoExposeEvent, major_code);
    F(XNoExposeEvent, minor_code);

    S(XVisibilityEvent);
    F(XVisibilityEvent, type);
    F(XVisibilityEvent, serial);
    F(XVisibilityEvent, send_event);
    F(XVisibilityEvent, display);
    F(XVisibilityEvent, window);
    F(XVisibilityEvent, state);

    S(XCreateWindowEvent);
    F(XCreateWindowEvent, type);
    F(XCreateWindowEvent, serial);
    F(XCreateWindowEvent, send_event);
    F(XCreateWindowEvent, display);
    F(XCreateWindowEvent, parent);
    F(XCreateWindowEvent, window);
    F(XCreateWindowEvent, x);
    F(XCreateWindowEvent, y);
    F(XCreateWindowEvent, width);
    F(XCreateWindowEvent, height);
    F(XCreateWindowEvent, border_width);
    F(XCreateWindowEvent, override_redirect);

    S(XDestroyWindowEvent);
    F(XDestroyWindowEvent, type);
    F(XDestroyWindowEvent, serial);
    F(XDestroyWindowEvent, send_event);
    F(XDestroyWindowEvent, display);
    F(XDestroyWindowEvent, event);
    F(XDestroyWindowEvent, window);

    S(XUnmapEvent);
    F(XUnmapEvent, type);
    F(XUnmapEvent, serial);
    F(XUnmapEvent, send_event);
    F(XUnmapEvent, display);
    F(XUnmapEvent, event);
    F(XUnmapEvent, window);
    F(XUnmapEvent, from_configure);

    S(XMapEvent);
    F(XMapEvent, type);
    F(XMapEvent, serial);
    F(XMapEvent, send_event);
    F(XMapEvent, display);
    F(XMapEvent, event);
    F(XMapEvent, window);
    F(XMapEvent, override_redirect);

    S(XMapRequestEvent);
    F(XMapRequestEvent, type);
    F(XMapRequestEvent, serial);
    F(XMapRequestEvent, send_event);
    F(XMapRequestEvent, display);
    F(XMapRequestEvent, parent);
    F(XMapRequestEvent, window);

    S(XReparentEvent);
    F(XReparentEvent, type);
    F(XReparentEvent, serial);
    F(XReparentEvent, send_event);
    F(XReparentEvent, display);
    F(XReparentEvent, event);
    F(XReparentEvent, window);
    F(XReparentEvent, parent);
    F(XReparentEvent, x);
    F(XReparentEvent, y);
    F(XReparentEvent, override_redirect);

    /* The one the banding bug went through: width/height here are what a
     * resize reads, and `above` is what a missing pad would corrupt. */
    S(XConfigureEvent);
    F(XConfigureEvent, type);
    F(XConfigureEvent, serial);
    F(XConfigureEvent, send_event);
    F(XConfigureEvent, display);
    F(XConfigureEvent, event);
    F(XConfigureEvent, window);
    F(XConfigureEvent, x);
    F(XConfigureEvent, y);
    F(XConfigureEvent, width);
    F(XConfigureEvent, height);
    F(XConfigureEvent, border_width);
    F(XConfigureEvent, above);
    F(XConfigureEvent, override_redirect);

    S(XGravityEvent);
    F(XGravityEvent, type);
    F(XGravityEvent, serial);
    F(XGravityEvent, send_event);
    F(XGravityEvent, display);
    F(XGravityEvent, event);
    F(XGravityEvent, window);
    F(XGravityEvent, x);
    F(XGravityEvent, y);

    S(XResizeRequestEvent);
    F(XResizeRequestEvent, type);
    F(XResizeRequestEvent, serial);
    F(XResizeRequestEvent, send_event);
    F(XResizeRequestEvent, display);
    F(XResizeRequestEvent, window);
    F(XResizeRequestEvent, width);
    F(XResizeRequestEvent, height);

    S(XConfigureRequestEvent);
    F(XConfigureRequestEvent, type);
    F(XConfigureRequestEvent, serial);
    F(XConfigureRequestEvent, send_event);
    F(XConfigureRequestEvent, display);
    F(XConfigureRequestEvent, parent);
    F(XConfigureRequestEvent, window);
    F(XConfigureRequestEvent, x);
    F(XConfigureRequestEvent, y);
    F(XConfigureRequestEvent, width);
    F(XConfigureRequestEvent, height);
    F(XConfigureRequestEvent, border_width);
    F(XConfigureRequestEvent, above);
    F(XConfigureRequestEvent, detail);
    F(XConfigureRequestEvent, value_mask);

    S(XCirculateEvent);
    F(XCirculateEvent, type);
    F(XCirculateEvent, serial);
    F(XCirculateEvent, send_event);
    F(XCirculateEvent, display);
    F(XCirculateEvent, event);
    F(XCirculateEvent, window);
    F(XCirculateEvent, place);

    S(XCirculateRequestEvent);
    F(XCirculateRequestEvent, type);
    F(XCirculateRequestEvent, serial);
    F(XCirculateRequestEvent, send_event);
    F(XCirculateRequestEvent, display);
    F(XCirculateRequestEvent, parent);
    F(XCirculateRequestEvent, window);
    F(XCirculateRequestEvent, place);

    S(XPropertyEvent);
    F(XPropertyEvent, type);
    F(XPropertyEvent, serial);
    F(XPropertyEvent, send_event);
    F(XPropertyEvent, display);
    F(XPropertyEvent, window);
    F(XPropertyEvent, atom);
    F(XPropertyEvent, time);
    F(XPropertyEvent, state);

    S(XSelectionClearEvent);
    F(XSelectionClearEvent, type);
    F(XSelectionClearEvent, serial);
    F(XSelectionClearEvent, send_event);
    F(XSelectionClearEvent, display);
    F(XSelectionClearEvent, window);
    F(XSelectionClearEvent, selection);
    F(XSelectionClearEvent, time);

    /* The clipboard depends on every field of these two being right: an
     * unanswered SelectionRequest hangs the other client, not us. */
    S(XSelectionRequestEvent);
    F(XSelectionRequestEvent, type);
    F(XSelectionRequestEvent, serial);
    F(XSelectionRequestEvent, send_event);
    F(XSelectionRequestEvent, display);
    F(XSelectionRequestEvent, owner);
    F(XSelectionRequestEvent, requestor);
    F(XSelectionRequestEvent, selection);
    F(XSelectionRequestEvent, target);
    F(XSelectionRequestEvent, property);
    F(XSelectionRequestEvent, time);

    S(XSelectionEvent);
    F(XSelectionEvent, type);
    F(XSelectionEvent, serial);
    F(XSelectionEvent, send_event);
    F(XSelectionEvent, display);
    F(XSelectionEvent, requestor);
    F(XSelectionEvent, selection);
    F(XSelectionEvent, target);
    F(XSelectionEvent, property);
    F(XSelectionEvent, time);

    /* `new` in C, `is_new` in Caustic — C++ made Xlib rename it to c_new under
     * __cplusplus, so there is no single spelling to be faithful to. */
    S(XColormapEvent);
    F(XColormapEvent, type);
    F(XColormapEvent, serial);
    F(XColormapEvent, send_event);
    F(XColormapEvent, display);
    F(XColormapEvent, window);
    F(XColormapEvent, colormap);
    F(XColormapEvent, new);
    F(XColormapEvent, state);

    /* data is a union of char[20] / short[10] / long[5], 8-aligned because of
     * the long — so 40 bytes, not 20. WM_DELETE_WINDOW arrives in data.l[0]. */
    S(XClientMessageEvent);
    F(XClientMessageEvent, type);
    F(XClientMessageEvent, serial);
    F(XClientMessageEvent, send_event);
    F(XClientMessageEvent, display);
    F(XClientMessageEvent, window);
    F(XClientMessageEvent, message_type);
    F(XClientMessageEvent, format);
    F(XClientMessageEvent, data);

    S(XMappingEvent);
    F(XMappingEvent, type);
    F(XMappingEvent, serial);
    F(XMappingEvent, send_event);
    F(XMappingEvent, display);
    F(XMappingEvent, window);
    F(XMappingEvent, request);
    F(XMappingEvent, first_keycode);
    F(XMappingEvent, count);

    /* The odd one out: no send_event, and display comes second. Three
     * unsigned chars at the tail rather than ints. */
    S(XErrorEvent);
    F(XErrorEvent, type);
    F(XErrorEvent, display);
    F(XErrorEvent, resourceid);
    F(XErrorEvent, serial);
    F(XErrorEvent, error_code);
    F(XErrorEvent, request_code);
    F(XErrorEvent, minor_code);

    S(XGenericEvent);
    F(XGenericEvent, type);
    F(XGenericEvent, serial);
    F(XGenericEvent, send_event);
    F(XGenericEvent, display);
    F(XGenericEvent, extension);
    F(XGenericEvent, evtype);

    /* `data` points into libX11's own memory and XFreeEventData invalidates
     * it — which is why the pump copies the payload before the ring. */
    S(XGenericEventCookie);
    F(XGenericEventCookie, type);
    F(XGenericEventCookie, serial);
    F(XGenericEventCookie, send_event);
    F(XGenericEventCookie, display);
    F(XGenericEventCookie, extension);
    F(XGenericEventCookie, evtype);
    F(XGenericEventCookie, cookie);
    F(XGenericEventCookie, data);
}


static void structs(void)
{
    printf("# --- structs: Xlib.h, everything that is not an event ---\n");
    S(Depth);
    F(Depth, depth);
    F(Depth, nvisuals);
    F(Depth, visuals);
    S(Screen);
    F(Screen, ext_data);
    F(Screen, display);
    F(Screen, root);
    F(Screen, width);
    F(Screen, height);
    F(Screen, mwidth);
    F(Screen, mheight);
    F(Screen, ndepths);
    F(Screen, depths);
    F(Screen, root_depth);
    F(Screen, root_visual);
    F(Screen, default_gc);
    F(Screen, cmap);
    F(Screen, white_pixel);
    F(Screen, black_pixel);
    F(Screen, max_maps);
    F(Screen, min_maps);
    F(Screen, backing_store);
    F(Screen, save_unders);
    F(Screen, root_input_mask);
    S(ScreenFormat);
    F(ScreenFormat, ext_data);
    F(ScreenFormat, depth);
    F(ScreenFormat, bits_per_pixel);
    F(ScreenFormat, scanline_pad);
    S(Visual);
    F(Visual, ext_data);
    F(Visual, visualid);
    F(Visual, class);
    F(Visual, red_mask);
    F(Visual, green_mask);
    F(Visual, blue_mask);
    F(Visual, bits_per_rgb);
    F(Visual, map_entries);
    S(XArc);
    F(XArc, x);
    F(XArc, y);
    F(XArc, width);
    F(XArc, height);
    F(XArc, angle1);
    F(XArc, angle2);
    S(XChar2b);
    F(XChar2b, byte1);
    F(XChar2b, byte2);
    S(XCharStruct);
    F(XCharStruct, lbearing);
    F(XCharStruct, rbearing);
    F(XCharStruct, width);
    F(XCharStruct, ascent);
    F(XCharStruct, descent);
    F(XCharStruct, attributes);
    S(XColor);
    F(XColor, pixel);
    F(XColor, red);
    F(XColor, green);
    F(XColor, blue);
    F(XColor, flags);
    F(XColor, pad);
    S(XExtCodes);
    F(XExtCodes, extension);
    F(XExtCodes, major_opcode);
    F(XExtCodes, first_event);
    F(XExtCodes, first_error);
    S(XExtData);
    F(XExtData, number);
    F(XExtData, next);
    F(XExtData, free_private);
    F(XExtData, private_data);
    S(XFontProp);
    F(XFontProp, name);
    F(XFontProp, card32);
    S(XFontSetExtents);
    F(XFontSetExtents, max_ink_extent);
    F(XFontSetExtents, max_logical_extent);
    S(XFontStruct);
    F(XFontStruct, ext_data);
    F(XFontStruct, fid);
    F(XFontStruct, direction);
    F(XFontStruct, min_char_or_byte2);
    F(XFontStruct, max_char_or_byte2);
    F(XFontStruct, min_byte1);
    F(XFontStruct, max_byte1);
    F(XFontStruct, all_chars_exist);
    F(XFontStruct, default_char);
    F(XFontStruct, n_properties);
    F(XFontStruct, properties);
    F(XFontStruct, min_bounds);
    F(XFontStruct, max_bounds);
    F(XFontStruct, per_char);
    F(XFontStruct, ascent);
    F(XFontStruct, descent);
    S(XGCValues);
    F(XGCValues, function);
    F(XGCValues, plane_mask);
    F(XGCValues, foreground);
    F(XGCValues, background);
    F(XGCValues, line_width);
    F(XGCValues, line_style);
    F(XGCValues, cap_style);
    F(XGCValues, join_style);
    F(XGCValues, fill_style);
    F(XGCValues, fill_rule);
    F(XGCValues, arc_mode);
    F(XGCValues, tile);
    F(XGCValues, stipple);
    F(XGCValues, ts_x_origin);
    F(XGCValues, ts_y_origin);
    F(XGCValues, font);
    F(XGCValues, subwindow_mode);
    F(XGCValues, graphics_exposures);
    F(XGCValues, clip_x_origin);
    F(XGCValues, clip_y_origin);
    F(XGCValues, clip_mask);
    F(XGCValues, dash_offset);
    F(XGCValues, dashes);
    S(XHostAddress);
    F(XHostAddress, family);
    F(XHostAddress, length);
    F(XHostAddress, address);
    S(XICCallback);
    F(XICCallback, client_data);
    F(XICCallback, callback);
    S(XIMCallback);
    F(XIMCallback, client_data);
    F(XIMCallback, callback);
    S(XIMStyles);
    F(XIMStyles, count_styles);
    F(XIMStyles, supported_styles);
    S(XIMValuesList);
    F(XIMValuesList, count_values);
    F(XIMValuesList, supported_values);
    S(XImage);
    F(XImage, width);
    F(XImage, height);
    F(XImage, xoffset);
    F(XImage, format);
    F(XImage, data);
    F(XImage, byte_order);
    F(XImage, bitmap_unit);
    F(XImage, bitmap_bit_order);
    F(XImage, bitmap_pad);
    F(XImage, depth);
    F(XImage, bytes_per_line);
    F(XImage, bits_per_pixel);
    F(XImage, red_mask);
    F(XImage, green_mask);
    F(XImage, blue_mask);
    F(XImage, obdata);
    F(XImage, f);
    S(XKeyboardControl);
    F(XKeyboardControl, key_click_percent);
    F(XKeyboardControl, bell_percent);
    F(XKeyboardControl, bell_pitch);
    F(XKeyboardControl, bell_duration);
    F(XKeyboardControl, led);
    F(XKeyboardControl, led_mode);
    F(XKeyboardControl, key);
    F(XKeyboardControl, auto_repeat_mode);
    S(XKeyboardState);
    F(XKeyboardState, key_click_percent);
    F(XKeyboardState, bell_percent);
    F(XKeyboardState, bell_pitch);
    F(XKeyboardState, bell_duration);
    F(XKeyboardState, led_mask);
    F(XKeyboardState, global_auto_repeat);
    F(XKeyboardState, auto_repeats);
    S(XModifierKeymap);
    F(XModifierKeymap, max_keypermod);
    F(XModifierKeymap, modifiermap);
    S(XOMCharSetList);
    F(XOMCharSetList, charset_count);
    F(XOMCharSetList, charset_list);
    S(XOMFontInfo);
    F(XOMFontInfo, num_font);
    F(XOMFontInfo, font_struct_list);
    F(XOMFontInfo, font_name_list);
    S(XOMOrientation);
    F(XOMOrientation, num_orientation);
    F(XOMOrientation, orientation);
    S(XPixmapFormatValues);
    F(XPixmapFormatValues, depth);
    F(XPixmapFormatValues, bits_per_pixel);
    F(XPixmapFormatValues, scanline_pad);
    S(XPoint);
    F(XPoint, x);
    F(XPoint, y);
    S(XRectangle);
    F(XRectangle, x);
    F(XRectangle, y);
    F(XRectangle, width);
    F(XRectangle, height);
    S(XSegment);
    F(XSegment, x1);
    F(XSegment, y1);
    F(XSegment, x2);
    F(XSegment, y2);
    S(XServerInterpretedAddress);
    F(XServerInterpretedAddress, typelength);
    F(XServerInterpretedAddress, valuelength);
    F(XServerInterpretedAddress, type);
    F(XServerInterpretedAddress, value);
    S(XSetWindowAttributes);
    F(XSetWindowAttributes, background_pixmap);
    F(XSetWindowAttributes, background_pixel);
    F(XSetWindowAttributes, border_pixmap);
    F(XSetWindowAttributes, border_pixel);
    F(XSetWindowAttributes, bit_gravity);
    F(XSetWindowAttributes, win_gravity);
    F(XSetWindowAttributes, backing_store);
    F(XSetWindowAttributes, backing_planes);
    F(XSetWindowAttributes, backing_pixel);
    F(XSetWindowAttributes, save_under);
    F(XSetWindowAttributes, event_mask);
    F(XSetWindowAttributes, do_not_propagate_mask);
    F(XSetWindowAttributes, override_redirect);
    F(XSetWindowAttributes, colormap);
    F(XSetWindowAttributes, cursor);
    S(XTextItem);
    F(XTextItem, chars);
    F(XTextItem, nchars);
    F(XTextItem, delta);
    F(XTextItem, font);
    S(XTextItem16);
    F(XTextItem16, chars);
    F(XTextItem16, nchars);
    F(XTextItem16, delta);
    F(XTextItem16, font);
    S(XTimeCoord);
    F(XTimeCoord, time);
    F(XTimeCoord, x);
    F(XTimeCoord, y);
    S(XWindowAttributes);
    F(XWindowAttributes, x);
    F(XWindowAttributes, y);
    F(XWindowAttributes, width);
    F(XWindowAttributes, height);
    F(XWindowAttributes, border_width);
    F(XWindowAttributes, depth);
    F(XWindowAttributes, visual);
    F(XWindowAttributes, root);
    F(XWindowAttributes, class);
    F(XWindowAttributes, bit_gravity);
    F(XWindowAttributes, win_gravity);
    F(XWindowAttributes, backing_store);
    F(XWindowAttributes, backing_planes);
    F(XWindowAttributes, backing_pixel);
    F(XWindowAttributes, save_under);
    F(XWindowAttributes, colormap);
    F(XWindowAttributes, map_installed);
    F(XWindowAttributes, map_state);
    F(XWindowAttributes, all_event_masks);
    F(XWindowAttributes, your_event_mask);
    F(XWindowAttributes, do_not_propagate_mask);
    F(XWindowAttributes, override_redirect);
    F(XWindowAttributes, screen);
    S(XWindowChanges);
    F(XWindowChanges, x);
    F(XWindowChanges, y);
    F(XWindowChanges, width);
    F(XWindowChanges, height);
    F(XWindowChanges, border_width);
    F(XWindowChanges, sibling);
    F(XWindowChanges, stack_mode);
    S(XmbTextItem);
    F(XmbTextItem, chars);
    F(XmbTextItem, nchars);
    F(XmbTextItem, delta);
    F(XmbTextItem, font_set);
    S(XwcTextItem);
    F(XwcTextItem, chars);
    F(XwcTextItem, nchars);
    F(XwcTextItem, delta);
    F(XwcTextItem, font_set);

    /* XSizeHints is Xutil.h rather than Xlib.h, and it is here because it is
     * the only way to tell a window manager a window has a fixed size. That
     * matters more than it sounds on a tiling compositor, where a window
     * without it is simply tiled to whatever the layout says. The two nested
     * aspect structs are the part worth pinning: they are the only place in
     * the bindings where a field is a struct rather than a scalar. */
    /* XShmSegmentInfo is the one struct in the bindings whose address escapes:
     * XShmCreateImage stores a POINTER to it in the XImage and every
     * XShmPutImage reads the segment id back out through that pointer. So its
     * layout has to be right and its storage has to outlive the call — the
     * second half is surface.cst's problem, this pins the first. */
    S(XShmSegmentInfo);
    F(XShmSegmentInfo, shmseg);
    F(XShmSegmentInfo, shmid);
    F(XShmSegmentInfo, shmaddr);
    F(XShmSegmentInfo, readOnly);

    /* MIT-SCREEN-SAVER's one struct. Bound because a program that suspends the
     * screensaver usually also wants to know how long the user has been idle,
     * and that answer arrives in here. */
    S(XScreenSaverInfo);
    F(XScreenSaverInfo, window);
    F(XScreenSaverInfo, state);
    F(XScreenSaverInfo, kind);
    F(XScreenSaverInfo, til_or_since);
    F(XScreenSaverInfo, idle);
    F(XScreenSaverInfo, eventMask);

    S(XSizeHints);
    F(XSizeHints, flags);
    F(XSizeHints, x);
    F(XSizeHints, y);
    F(XSizeHints, width);
    F(XSizeHints, height);
    F(XSizeHints, min_width);
    F(XSizeHints, min_height);
    F(XSizeHints, max_width);
    F(XSizeHints, max_height);
    F(XSizeHints, width_inc);
    F(XSizeHints, height_inc);
    F(XSizeHints, min_aspect);
    F(XSizeHints, max_aspect);
    F(XSizeHints, base_width);
    F(XSizeHints, base_height);
    F(XSizeHints, win_gravity);
}

static void im_structs(void)
{
    printf("# --- structs: the input-method types, named tags in Xlib.h ---\n");
    S(XIMHotKeyTrigger);
    F(XIMHotKeyTrigger, keysym);
    F(XIMHotKeyTrigger, modifier);
    F(XIMHotKeyTrigger, modifier_mask);
    S(XIMHotKeyTriggers);
    F(XIMHotKeyTriggers, num_hot_key);
    F(XIMHotKeyTriggers, key);
    S(XIMPreeditCaretCallbackStruct);
    F(XIMPreeditCaretCallbackStruct, position);
    F(XIMPreeditCaretCallbackStruct, direction);
    F(XIMPreeditCaretCallbackStruct, style);
    S(XIMPreeditDrawCallbackStruct);
    F(XIMPreeditDrawCallbackStruct, caret);
    F(XIMPreeditDrawCallbackStruct, chg_first);
    F(XIMPreeditDrawCallbackStruct, chg_length);
    F(XIMPreeditDrawCallbackStruct, text);
    S(XIMPreeditStateNotifyCallbackStruct);
    F(XIMPreeditStateNotifyCallbackStruct, state);
    S(XIMStatusDrawCallbackStruct);
    F(XIMStatusDrawCallbackStruct, type);
    F(XIMStatusDrawCallbackStruct, data);
    S(XIMStringConversionCallbackStruct);
    F(XIMStringConversionCallbackStruct, position);
    F(XIMStringConversionCallbackStruct, direction);
    F(XIMStringConversionCallbackStruct, operation);
    F(XIMStringConversionCallbackStruct, factor);
    F(XIMStringConversionCallbackStruct, text);
    S(XIMStringConversionText);
    F(XIMStringConversionText, length);
    F(XIMStringConversionText, feedback);
    F(XIMStringConversionText, encoding_is_wchar);
    F(XIMStringConversionText, string);
    S(XIMText);
    F(XIMText, length);
    F(XIMText, feedback);
    F(XIMText, encoding_is_wchar);
    F(XIMText, string);
}

static void constants(void)
{
    printf("# --- constants: every #define in X.h ---\n");
    C(X_PROTOCOL);
    C(X_PROTOCOL_REVISION);
    C(None);
    C(ParentRelative);
    C(CopyFromParent);
    C(PointerWindow);
    C(InputFocus);
    C(PointerRoot);
    C(AnyPropertyType);
    C(AnyKey);
    C(AnyButton);
    C(AllTemporary);
    C(CurrentTime);
    C(NoSymbol);
    C(NoEventMask);
    C(KeyPressMask);
    C(KeyReleaseMask);
    C(ButtonPressMask);
    C(ButtonReleaseMask);
    C(EnterWindowMask);
    C(LeaveWindowMask);
    C(PointerMotionMask);
    C(PointerMotionHintMask);
    C(Button1MotionMask);
    C(Button2MotionMask);
    C(Button3MotionMask);
    C(Button4MotionMask);
    C(Button5MotionMask);
    C(ButtonMotionMask);
    C(KeymapStateMask);
    C(ExposureMask);
    C(VisibilityChangeMask);
    C(StructureNotifyMask);
    C(ResizeRedirectMask);
    C(SubstructureNotifyMask);
    C(SubstructureRedirectMask);
    C(FocusChangeMask);
    C(PropertyChangeMask);
    C(ColormapChangeMask);
    C(OwnerGrabButtonMask);
    C(KeyPress);
    C(KeyRelease);
    C(ButtonPress);
    C(ButtonRelease);
    C(MotionNotify);
    C(EnterNotify);
    C(LeaveNotify);
    C(FocusIn);
    C(FocusOut);
    C(KeymapNotify);
    C(Expose);
    C(GraphicsExpose);
    C(NoExpose);
    C(VisibilityNotify);
    C(CreateNotify);
    C(DestroyNotify);
    C(UnmapNotify);
    C(MapNotify);
    C(MapRequest);
    C(ReparentNotify);
    C(ConfigureNotify);
    C(ConfigureRequest);
    C(GravityNotify);
    C(ResizeRequest);
    C(CirculateNotify);
    C(CirculateRequest);
    C(PropertyNotify);
    C(SelectionClear);
    C(SelectionRequest);
    C(SelectionNotify);
    C(ColormapNotify);
    C(ClientMessage);
    C(MappingNotify);
    C(GenericEvent);
    C(LASTEvent);
    C(ShiftMask);
    C(LockMask);
    C(ControlMask);
    C(Mod1Mask);
    C(Mod2Mask);
    C(Mod3Mask);
    C(Mod4Mask);
    C(Mod5Mask);
    C(ShiftMapIndex);
    C(LockMapIndex);
    C(ControlMapIndex);
    C(Mod1MapIndex);
    C(Mod2MapIndex);
    C(Mod3MapIndex);
    C(Mod4MapIndex);
    C(Mod5MapIndex);
    C(Button1Mask);
    C(Button2Mask);
    C(Button3Mask);
    C(Button4Mask);
    C(Button5Mask);
    C(AnyModifier);
    C(Button1);
    C(Button2);
    C(Button3);
    C(Button4);
    C(Button5);
    C(NotifyNormal);
    C(NotifyGrab);
    C(NotifyUngrab);
    C(NotifyWhileGrabbed);
    C(NotifyHint);
    C(NotifyAncestor);
    C(NotifyVirtual);
    C(NotifyInferior);
    C(NotifyNonlinear);
    C(NotifyNonlinearVirtual);
    C(NotifyPointer);
    C(NotifyPointerRoot);
    C(NotifyDetailNone);
    C(VisibilityUnobscured);
    C(VisibilityPartiallyObscured);
    C(VisibilityFullyObscured);
    C(PlaceOnTop);
    C(PlaceOnBottom);
    C(FamilyInternet);
    C(FamilyDECnet);
    C(FamilyChaos);
    C(FamilyServerInterpreted);
    C(FamilyInternet6);
    C(PropertyNewValue);
    C(PropertyDelete);
    C(ColormapUninstalled);
    C(ColormapInstalled);
    C(GrabModeSync);
    C(GrabModeAsync);
    C(GrabSuccess);
    C(AlreadyGrabbed);
    C(GrabInvalidTime);
    C(GrabNotViewable);
    C(GrabFrozen);
    C(AsyncPointer);
    C(SyncPointer);
    C(ReplayPointer);
    C(AsyncKeyboard);
    C(SyncKeyboard);
    C(ReplayKeyboard);
    C(AsyncBoth);
    C(SyncBoth);
    C(RevertToNone);
    C(RevertToPointerRoot);
    C(RevertToParent);
    C(Success);
    C(BadRequest);
    C(BadValue);
    C(BadWindow);
    C(BadPixmap);
    C(BadAtom);
    C(BadCursor);
    C(BadFont);
    C(BadMatch);
    C(BadDrawable);
    C(BadAccess);
    C(BadAlloc);
    C(BadColor);
    C(BadGC);
    C(BadIDChoice);
    C(BadName);
    C(BadLength);
    C(BadImplementation);
    C(FirstExtensionError);
    C(LastExtensionError);
    C(InputOutput);
    C(InputOnly);
    C(CWBackPixmap);
    C(CWBackPixel);
    C(CWBorderPixmap);
    C(CWBorderPixel);
    C(CWBitGravity);
    C(CWWinGravity);
    C(CWBackingStore);
    C(CWBackingPlanes);
    C(CWBackingPixel);
    C(CWOverrideRedirect);
    C(CWSaveUnder);
    C(CWEventMask);
    C(CWDontPropagate);
    C(CWColormap);
    C(CWCursor);
    C(CWX);
    C(CWY);
    C(CWWidth);
    C(CWHeight);
    C(CWBorderWidth);
    C(CWSibling);
    C(CWStackMode);
    C(ForgetGravity);
    C(NorthWestGravity);
    C(NorthGravity);
    C(NorthEastGravity);
    C(WestGravity);
    C(CenterGravity);
    C(EastGravity);
    C(SouthWestGravity);
    C(SouthGravity);
    C(SouthEastGravity);
    C(StaticGravity);
    C(UnmapGravity);
    C(NotUseful);
    C(WhenMapped);
    C(Always);
    C(IsUnmapped);
    C(IsUnviewable);
    C(IsViewable);
    C(SetModeInsert);
    C(SetModeDelete);
    C(DestroyAll);
    C(RetainPermanent);
    C(RetainTemporary);
    C(Above);
    C(Below);
    C(TopIf);
    C(BottomIf);
    C(Opposite);
    C(RaiseLowest);
    C(LowerHighest);
    C(PropModeReplace);
    C(PropModePrepend);
    C(PropModeAppend);
    C(GXclear);
    C(GXand);
    C(GXandReverse);
    C(GXcopy);
    C(GXandInverted);
    C(GXnoop);
    C(GXxor);
    C(GXor);
    C(GXnor);
    C(GXequiv);
    C(GXinvert);
    C(GXorReverse);
    C(GXcopyInverted);
    C(GXorInverted);
    C(GXnand);
    C(GXset);
    C(LineSolid);
    C(LineOnOffDash);
    C(LineDoubleDash);
    C(CapNotLast);
    C(CapButt);
    C(CapRound);
    C(CapProjecting);
    C(JoinMiter);
    C(JoinRound);
    C(JoinBevel);
    C(FillSolid);
    C(FillTiled);
    C(FillStippled);
    C(FillOpaqueStippled);
    C(EvenOddRule);
    C(WindingRule);
    C(ClipByChildren);
    C(IncludeInferiors);
    C(Unsorted);
    C(YSorted);
    C(YXSorted);
    C(YXBanded);
    C(CoordModeOrigin);
    C(CoordModePrevious);
    C(Complex);
    C(Nonconvex);
    C(Convex);
    C(ArcChord);
    C(ArcPieSlice);
    C(GCFunction);
    C(GCPlaneMask);
    C(GCForeground);
    C(GCBackground);
    C(GCLineWidth);
    C(GCLineStyle);
    C(GCCapStyle);
    C(GCJoinStyle);
    C(GCFillStyle);
    C(GCFillRule);
    C(GCTile);
    C(GCStipple);
    C(GCTileStipXOrigin);
    C(GCTileStipYOrigin);
    C(GCFont);
    C(GCSubwindowMode);
    C(GCGraphicsExposures);
    C(GCClipXOrigin);
    C(GCClipYOrigin);
    C(GCClipMask);
    C(GCDashOffset);
    C(GCDashList);
    C(GCArcMode);
    C(GCLastBit);
    C(FontLeftToRight);
    C(FontRightToLeft);
    C(FontChange);
    C(XYBitmap);
    C(XYPixmap);
    C(ZPixmap);
    C(AllocNone);
    C(AllocAll);
    C(DoRed);
    C(DoGreen);
    C(DoBlue);
    C(CursorShape);
    C(TileShape);
    C(StippleShape);
    C(AutoRepeatModeOff);
    C(AutoRepeatModeOn);
    C(AutoRepeatModeDefault);
    C(LedModeOff);
    C(LedModeOn);
    C(KBKeyClickPercent);
    C(KBBellPercent);
    C(KBBellPitch);
    C(KBBellDuration);
    C(KBLed);
    C(KBLedMode);
    C(KBKey);
    C(KBAutoRepeatMode);
    C(MappingSuccess);
    C(MappingBusy);
    C(MappingFailed);
    C(MappingModifier);
    C(MappingKeyboard);
    C(MappingPointer);
    C(DontPreferBlanking);
    C(PreferBlanking);
    C(DefaultBlanking);
    C(DisableScreenSaver);
    C(DisableScreenInterval);
    C(DontAllowExposures);
    C(AllowExposures);
    C(DefaultExposures);
    C(ScreenSaverReset);
    C(ScreenSaverActive);
    C(HostInsert);
    C(HostDelete);
    C(EnableAccess);
    C(DisableAccess);
    C(StaticGray);
    C(GrayScale);
    C(StaticColor);
    C(PseudoColor);
    C(TrueColor);
    C(DirectColor);
    C(LSBFirst);
    C(MSBFirst);

    /* Xutil.h rather than X.h: the flags word of XSizeHints. They are here
     * because they decide something visible — a window that does not declare
     * PMinSize and PMaxSize equal is not fixed-size, and a tiling compositor
     * tiles it to whatever its layout says instead of leaving it a window.
     * Getting one wrong produces no error, just a window the size of the
     * screen. */
    C(USPosition);
    C(USSize);
    C(PPosition);
    C(PSize);
    C(PMinSize);
    C(PMaxSize);
    C(PResizeInc);
    C(PAspect);
    C(PBaseSize);
    C(PWinGravity);
}

int main(void)
{
    printf("# window/x11/tools/x11_layout.c output — x86_64 linux, glibc\n");
    printf("# regenerate: cc -o x11_layout x11_layout.c && ./x11_layout > layout.txt\n");
    events();
    structs();
    im_structs();
    constants();
    return 0;
}
