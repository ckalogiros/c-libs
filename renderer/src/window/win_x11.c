/*
 * Created by ckalo on 2/3/26.
 */

#include "win_x11.h"
// #include "window.h" // TODO: implement a window INTERFACE
#include "renderer_structs.h"
#include "sys_utils.h"
// #include <X11/Xlib.h>
// #include "renderer_internal.h"

struct os_window os_win;

XSetWindowAttributes win_x11_create_attributes(struct os_window* win);

struct os_window* win_x11_init_display(void)
{
    os_win.display = XOpenDisplay(NULL);
    if (os_win.display == NULL)
        my_exit("X11 window display FAILED to initialize", __FILE__, __LINE__);

    return &os_win;
}


// XCreateWindow(Display *display, Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, int depth, unsigned int class, Visual *visual, unsigned long valuemask, XSetWindowAttributes *attributes);
// struct os_window* win_x11_create(const uint32 xpos, const uint32 ypos, const uint32 w, const uint32 h)
struct os_window* win_x11_create(const uint32 xpos, const uint32 ypos, const uint32 w, const uint32 h)
{
    os_win.display = XOpenDisplay(NULL);
    if (os_win.display == NULL)
        my_exit("X11 window display FAILED to initialize", __FILE__, __LINE__);

    // screen = DefaultScreenOfDisplay(os_win.display);
    os_win.screen_id = DefaultScreen(os_win.display);

    // create window attributes
    XSetWindowAttributes win_attribs = win_x11_create_attributes(&os_win);

    os_win.window = XCreateWindow( os_win.display, RootWindow(os_win.display, os_win.screen_id),
                            xpos, ypos, w, h, 10,
                            os_win.visual->depth, InputOutput, os_win.visual->visual,
                            CWBackPixel | CWColormap | CWBorderPixel | CWEventMask,
                            &win_attribs);

    return &os_win;
}


XSetWindowAttributes win_x11_create_attributes(struct os_window* win)
{
    XSetWindowAttributes win_attribs;
    win_attribs.border_pixel = BlackPixel(win->display, win->screen_id);
    win_attribs.background_pixel = WhitePixel(win->display, win->screen_id);
    win_attribs.override_redirect = True;
    win_attribs.colormap =
        XCreateColormap(win->display, RootWindow(win->display, win->screen_id), win->visual->visual, AllocNone);
    win_attribs.event_mask = ExposureMask;

    return win_attribs;
}


// todo: implement
void win_x11_destroy(){}
