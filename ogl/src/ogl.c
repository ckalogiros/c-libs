/*
 * Created by ckalo on 2/3/26.
 */

#include "ogl.h"
#include "glad.h"
#include "glfw3.h"
#include "logger.h"
#include "sys_utils.h"

static struct gfx_context gfx_ctx;

struct gfx_context*
ogl_create_context(const uint32 win_width, const uint32 win_height)
{
        gfx_ctx.window = NULL;
        gfx_ctx.win_width = win_width;
        gfx_ctx.win_height = win_height;

    return &gfx_ctx;
}

void
ogl_init(struct gfx_context* const ctx)
{
    GLFWwindow* window;

    /* Initialize the library */
    if(!glfwInit())
        return;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(ctx->win_width, ctx->win_height, "glfw renderer", NULL, NULL);
    if(!window) {
        glfwTerminate();
        return;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);


    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        print("Failed to initialize GLAD!\n");
        return;
    }
    // Optional: Verify OpenGL version
    print("OpenGL: %s\n", glGetString(GL_VERSION));


    // store to graphics context
    ctx->window = window;
}


void
ogl_run(struct gfx_context* ctx)
{
    if(!ctx)
        my_exit("GFX CONTEXT IS NULL", __FILE__, __LINE__);


    glViewport(0, 0, ctx->win_width, ctx->win_height);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);

    // const uint32 shader_program = shader_build();
    //
    // const uint32 VAO = init();


    while(!glfwWindowShouldClose(ctx->window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glUseProgram(shader_program);
        // glBindVertexArray(VAO);
        //
        //
        // // draw 100 instanced quads
        // glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100); // 100 triangles of 6 vertices each
        // glBindVertexArray(0);


        glfwSwapBuffers(ctx->window);
        glfwPollEvents();
    }
}

void
ogl_destroy()
{
    glfwTerminate();
}
