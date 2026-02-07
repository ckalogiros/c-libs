/*
 * Created by ckalo on 2/3/26.
 */

#include "win_wayland.h"
#include <stdlib.h>
#include "window.h"
#include <wayland-client.h>
#include "sys_utils.h"

static struct os_window {
    struct wl_display* display;
    struct wl_surface* surface;
    // ... Wayland specific data
};

static struct os_window os_win;


/*
 * See also
 * XChangeWindowAttributes(), XCreateWindow(), XDestroyWindow(),
 * XMapWindow(), XMoveWindow(), XMoveResizeWindow(), XRaiseWindow(),
 * XResizeWindow(), XSetWindowBorderWidth(), XUnmapWindow(),
 * : https://tronche.com/gui/x/xlib/window/XConfigureWindow.html
 */

struct os_window* window_create(int w, int h, const char* title) {
    // Wayland implementation
    // Window* win = malloc(sizeof(Window));

    os_win.display = wl_display_connect(NULL);
    if (os_win.display == NULL)
        my_exit("WAYLAND window display FAILED to initialize", __FILE__, __LINE__);

    // ... Wayland setup

    return &os_win;
}