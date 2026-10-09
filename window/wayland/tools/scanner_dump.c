/* Print wayland-scanner's own interface tables in protocol.txt's normalized
 * form. check_protocols.sh links this with the scanner's private code, so the
 * generated Caustic metadata is compared against an independent marshaller's
 * names, versions, opcodes, since versions, signatures and argument types. */
#include <stdio.h>
#include "wayland-util.h"
#include "interfaces.h" /* generated: the scanner's interfaces, in `all` */

static void messages(const char *iface, const char *dir, const struct wl_message *m, int n) {
    for (int i = 0; i < n; i++) {
        const char *sig = m[i].signature;
        int since = 0;
        while (*sig >= '0' && *sig <= '9') since = since * 10 + (*sig++ - '0');
        if (since == 0) since = 1;
        printf("M %s %s %d %d %s %s ", iface, dir, i, since, m[i].name, sig);
        int arg = 0;
        for (const char *c = sig; *c; c++) {
            if (*c == '?') continue;
            if (arg > 0) putchar(',');
            const struct wl_interface *type = m[i].types[arg];
            if (type) fputs(type->name, stdout);
            arg++;
        }
        putchar('\n');
    }
}

int main(void) {
    for (size_t i = 0; i < sizeof(all) / sizeof(all[0]); i++) {
        const struct wl_interface *f = all[i];
        printf("I %s %d %d %d\n", f->name, f->version, f->method_count, f->event_count);
        messages(f->name, "request", f->methods, f->method_count);
        messages(f->name, "event", f->events, f->event_count);
    }
    return 0;
}
