/*
 * Created by ckalo on 2/3/26.
 */

#include "win_x11.h"
// #include "window.h"
// #include "window.h" // TODO: implement a window INTERFACE
#include "sys_utils.h"
// #include <string.h>
#include <X11/Xlib.h>

#include "logger.h"
// #include "glad.h"
// #include "opengl.h"
#include "GL/glx.h"

#include "opengl.h"

struct temp_win_params {
    uint32 border_width;
    unsigned long border_pixel_val;
    unsigned long bk_fill_;

};

struct temp_win_params win_params = {4, };

static struct os_window {
    Display *display;
    XVisualInfo* visual;
    Window window;
    int32 screen_id;
} os_window;

static struct os_window os_win;


XVisualInfo* init_glx();

void init_x11_events();

XSetWindowAttributes win_x11_create_attributes();


// XCreateWindow(Display *display, Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, int depth, unsigned int class, Visual *visual, unsigned long valuemask, XSetWindowAttributes *attributes);
struct os_window* win_x11_create(const uint32 xpos, const uint32 ypos, const uint32 w, const uint32 h)
{ // x,y,width,height,border_width,

    os_win.display = XOpenDisplay(NULL);
    if (os_win.display == NULL)
        my_exit("X11 window display FAILED to initialize", __FILE__, __LINE__);


    os_win.visual = init_glx();
    
    
    // create window attributes
    XSetWindowAttributes win_attribs = win_x11_create_attributes();
    
    os_win.window = XCreateWindow( os_win.display, RootWindow(os_win.display, os_win.screen_id),
                            xpos, ypos, w, h, 10,
                            os_win.visual->depth, InputOutput, os_win.visual->visual,
                            CWBackPixel | CWColormap | CWBorderPixel | CWEventMask,
                            &win_attribs);


    // GLXContext context = glXCreateContext(os_win.display, os_win.visual, NULL, GL_TRUE);
    // glXMakeCurrent(os_win.display, os_win.window, context);
    // void* proc = glXGetProcAddress((const GLubyte*)"glGetString");
    // print("glXGetProcAddress test: %p\n", proc);  // Should NOT be NULL
    // print("GL Vendor: %s\n", glGetString(GL_VENDOR));
    // print("GL Renderer: %s\n",glGetString(GL_RENDERER));
    // print("GL Version: %s\n", glGetString(GL_VERSION));
    // print("GL Shading Language: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    opengl_create_context(os_win.window, os_win.display, os_win.visual);

    // todo: glad load openGl functions
    // if(!gladLoadGLLoader((GLADloadproc) glXGetProcAddress)) {
    //     print("Failed to initialize GLAD!\n");
    //     return NULL;
    // }
    // Optional: Verify OpenGL version
    print("OpenGL %s\n", glGetString(GL_VERSION));

    init_x11_events();

    XClearWindow(os_win.display, os_win.window);
    XMapRaised(os_win.display, os_win.window);


    glViewport(xpos, ypos, w, h);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);


    XSelectInput(os_win.display, os_win.window, ExposureMask | KeyPressMask);
    XMapWindow(os_win.display, os_win.window);
    XEvent e;
    const char* msg = "Hello there! This is a sample text";


    while (1) {

        XNextEvent(os_win.display, &e);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (e.type == Expose) {

            print("%d\n", e.type);
            // XFillRectangle(os_win.display, os_win.window, DefaultGC(os_win.display, os_win.screen_id), 20, 20, 10, 10);
            // XDrawString(os_win.display, os_win.window, DefaultGC(os_win.display, os_win.screen_id), 10, 50, msg, strlen(msg));
            XWindowAttributes attribs;
            XGetWindowAttributes(os_win.display, os_win.window, &attribs);


            glXSwapBuffers(os_win.display, os_win.window);


        }
        if (e.type == KeyPress) {
            print("%d\n", e.type);
            // XFillRectangle(os_win.display, os_win.window, DefaultGC(os_win.display, os_win.screen_id), 20, 20, 10, 10);
            // XDrawString(os_win.display, os_win.window, DefaultGC(os_win.display, os_win.screen_id), 10, 50, msg, strlen(msg));
            XWindowAttributes attribs;
            XGetWindowAttributes(os_win.display, os_win.window, &attribs);
        }

        glXSwapBuffers(os_win.display, os_win.window);

    }



    return &os_win;

}

XSetWindowAttributes win_x11_create_attributes()
{
    XSetWindowAttributes win_attribs;
    win_attribs.border_pixel = BlackPixel(os_win.display, os_win.screen_id);
    win_attribs.background_pixel = WhitePixel(os_win.display, os_win.screen_id);
    win_attribs.override_redirect = True;
    win_attribs.colormap =
        XCreateColormap(os_win.display, RootWindow(os_win.display, os_win.screen_id),
                        os_win.visual->visual, AllocNone);
    win_attribs.event_mask = ExposureMask;

    return win_attribs;
}

void init_x11_events()
{

}

XVisualInfo* init_glx(XVisualInfo* visual)
{
    GLint glxAttribs[] = {
        GLX_RGBA,
        GLX_DOUBLEBUFFER,
        GLX_DEPTH_SIZE,     24,
        GLX_STENCIL_SIZE,   8,
        GLX_RED_SIZE,       8,
        GLX_GREEN_SIZE,     8,
        GLX_BLUE_SIZE,      8,
        GLX_SAMPLE_BUFFERS, 0,
        GLX_SAMPLES,        0,
        None
    };

    visual = glXChooseVisual(os_win.display, os_win.screen_id, glxAttribs);

    if (visual == 0) {
        XCloseDisplay(os_win.display);
        my_exit("glXChooseVisual FAILED to initialize", __FILE__, __LINE__);
    }
    
    return visual; 
}

// todo: implement
void win_x11_destroy(){}
