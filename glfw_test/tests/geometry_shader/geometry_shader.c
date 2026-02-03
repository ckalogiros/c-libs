/*
 * Created by ckalo on 1/31/26.
 */

#include "geometry_shader.h"
#include <string.h>
#include "glad.h"
#include "glfw3.h"
#include "logger.h"
#include "gl_perf_timer.h"


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
        "out VS_OUT {\n"
            "vec3 color;\n"
        "} vs_out;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos, 0.0, 1.0);\n"
        "   vs_out.color = aColor;\n"
        "}\0";

static const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec3 fColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(fColor, 1.0);\n"
        "}\n\0";

static const char* geometryShaderSource = "#version 330 core\n"
"layout (points) in;\n"
"layout (triangle_strip, max_vertices = 5) out;\n"
"in VS_OUT {\n"
    "vec3 color;\n"
"} gs_in[];\n"
"out vec3 fColor;\n"
"void build_house(vec4 position)\n"
"{\n"
    "fColor = gs_in[0].color; // gs_in[0] since there's only one input vertex\n"
    "gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0); // 1:bottom-left\n"
    "EmitVertex();\n"
    "gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0); // 2:bottom-right\n"
    "EmitVertex();\n"
    "gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0); // 3:top-left\n"
    "EmitVertex();\n"
    "gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0); // 4:top-right\n"
    "EmitVertex();\n"
    "gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0); // 5:top\n"
    "fColor = vec3(1.0, 1.0, 1.0);\n"
    "EmitVertex();\n"
    "EndPrimitive();\n"
"}\n"
"void main() {\n"
    "build_house(gl_in[0].gl_Position);\n"
"}";


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
geometry_shader_render()
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



    gl_perf_init();


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
        glDrawArrays(GL_POINTS, 0, 4);
        glBindVertexArray(0);

        gl_perf_stop();


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);


    gl_perf_destroy();
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

    // geometry shader
    unsigned int geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometryShader, 1, &geometryShaderSource, NULL);
    glCompileShader(geometryShader);
    // check for shader compile errors
    glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
    check_shader_errors(geometryShader, "GEOMETRY");
    if(!success) {
        glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
        print("ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n");
    }


    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glAttachShader(shaderProgram, geometryShader);
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

static uint32
init()
{
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float points[] = {
        -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
        -0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
    };
    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindVertexArray(0);


    return VAO;
}

