/*
 * Created by ckalo on 2/3/26.
 */
#ifndef WINDOW_WINDOW_H
#define WINDOW_WINDOW_H

// window.h - Common interface
struct os_window;

struct os_window* window_create(int width, int height, const char* title);
void window_destroy(struct os_window* win);
void window_poll_events(struct os_window* win);
int window_should_close(struct os_window* win);
void* window_get_native_handle(struct os_window* win);

#endif //WINDOW_WINDOW_H