/*
 * Created by ckalo on 3/20/26.
 */

#include "test_window.h"
#include "renderer_structs.h"
#include "renderer.h"
#include "win_x11.h"
#include "GL/glx.h"
#include "opengl.h"

void test_window_create()
{
    struct os_window* os_win = NULL;

    os_win = win_x11_init_display(); // choose/init display

    os_win->visual = init_glx(os_win); // create visual: needs display

    os_win = win_x11_create(600, 100, 800, 600); // create window: needs display and visual


    XClearWindow(os_win->display, os_win->window);
    XMapRaised(os_win->display, os_win->window);

    XSelectInput(os_win->display, os_win->window, ExposureMask | KeyPressMask);
    XMapWindow(os_win->display, os_win->window);

    XEvent e;

    while (1) {

        XNextEvent(os_win->display, &e);

        // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // glClear(GL_COLOR_BUFFER_BIT);

        // if (e.type == Expose) {
        //     XWindowAttributes attribs;
        //     XGetWindowAttributes(os_win->display, os_win->window, &attribs);
        //     glXSwapBuffers(os_win->display, os_win->window);
        // }
        if (e.type == KeyPress) {
            XWindowAttributes attribs;
            XGetWindowAttributes(os_win->display, os_win->window, &attribs);
        }


        // glXSwapBuffers(os_win->display, os_win->window);

    }
}
