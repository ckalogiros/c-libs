#include "win_x11.h"
#include "window.h"
#include "src/win_wayland.h"




int
main(void)
{

    win_x11_create(600, 100, 800, 600);


    // todo:
    // Window* window = window_create(800, 600, "HELLO!");

    return 0;
}