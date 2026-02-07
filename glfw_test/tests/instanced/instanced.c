/*
 * Created by ckalo on 1/31/26.
 */

#include "instanced.h"
#include <string.h>
#include "glad.h"
#include "glfw3.h"
#include "gl_perf_timer.h"
#include "logger.h"


#define RED_GL 1.0, 0.0, 0.0, 1.0
#define GREEN_GL 0.0, 1.0, 0.0, 1.0
#define BLUE_GL 0.0, 0.5, 1.0, 1.0
#define CYAN_GL 0.0, 1.0, 1.0, 1.0
#define MAGENDA_GL 0.0, 1.0, 1.0, 1.0
#define YELLOW_GL 1.0, 1.0, 0.0, 1.0

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


static const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec2 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "layout (location = 2) in vec2 aOffset;\n"
        "out vec3 fColor;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos + aOffset, 0.0, 1.0);\n"
        "   fColor = aColor;\n"
        "}\0";


static const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec3 fColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(fColor, 1.0);\n"
        "}\n\0";

static void check_shader_errors(unsigned int shader, const char* type) {
    int success;
    char infoLog[1024];  // Increased buffer size

    if (strcmp(type, "PROGRAM") == 0) {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, sizeof(infoLog), NULL, infoLog);
            print("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
        }
    } else {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
            print("ERROR::SHADER::%s::COMPILATION_FAILED\n%s\n", type, infoLog);
        }
    }
}




static uint32
shader_build();
static uint32
init();

void
instanced_render()
{
    GLFWwindow* window;

    /* Initialize the library */
    if(!glfwInit())
        return;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello World", NULL, NULL);
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
    print("OpenGL %s\n", glGetString(GL_VERSION));



    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);

    const uint32 shader_program = shader_build();

    const uint32 VAO = init();



    while(!glfwWindowShouldClose(window)) {

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(VAO);


        gl_perf_start();

        // draw 100 instanced quads
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100); // 100 triangles of 6 vertices each
        glBindVertexArray(0);

        gl_perf_stop();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
}


/**************************************************************************************************
 *
 * glsl shaders init
 *
 **************************************************************************************************/
static uint32
shader_build()
{
    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    check_shader_errors(vertexShader, "VERTEX");
    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        print("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    check_shader_errors(fragmentShader, "FRAGMENT");
    if(!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        print("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    check_shader_errors(shaderProgram, "PROGRAM");
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        print("ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}




/**************************************************************************************************
 *
 * opengl init
 *
 **************************************************************************************************/
static uint32
init()
{
    // generate a list of 100 quad locations/translation-vectors
    // ---------------------------------------------------------
    struct pos { float x, y; };
    struct pos translations[100];
    int index = 0;
    float offsetx = -1.0f;
    float offsety = 1.0f;
    float move = 0.2f;
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            translations[index].x = offsetx;
            translations[index++].y = offsety;
            offsetx += move;
        }
        offsety -= move;
        offsetx = -1.0f;
    }


    unsigned int instanceVBO;
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(struct pos) * 100, &translations[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float quadVertices[] = {
        // positions     // colors
        -0.05f, 0.05f, 1.0f, 0.0f, 0.0f,
        0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
        -0.05f, -0.05f, 0.0f, 0.0f, 1.0f,

        -0.05f, 0.05f, 1.0f, 0.0f, 0.0f,
        0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
        0.05f, 0.05f, 0.0f, 1.0f, 1.0f
    };
    unsigned int VAO, quadVBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (2 * sizeof(float)));

    // also set instance data
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    // this attribute comes from a different vertex buffer
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // tell OpenGL this is an instanced vertex attribute.

    return VAO;
}
