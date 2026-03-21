#include <stdio.h>

#include "renderer.h"
#include "renderer_internal.h"
#include "test_window.h"

// void init_dprint ()
// {
//     dprint = fopen("dprint.txt", "w");
//     if ( == NULL)
//     {
//         printf("Error opening file!\n");
//         exit(1);
//     }
// }



int
main(void)
{

    {
        // create file for debug-print


        // glutDisplayFunc(MainLoopStep);
        //
        // // Setup Dear ImGui context
        // IMGUI_CHECKVERSION();
        // ImGui::CreateContext();
        // ImGuiIO& io = ImGui::GetIO(); (void)io;
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        //
        // ImGui_ImplGLUT_Init();
        // ImGui_ImplOpenGL2_Init();
        //
        // ImGui_ImplGLUT_InstallFuncs();

        // create renderer
        // create window
        struct os_window *os_win = renderer_create_x11_window(500, 100, 800, 600);

        render(os_win);
    }

    // test_window_create();
    return 0;
}
