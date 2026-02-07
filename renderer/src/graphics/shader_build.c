/*
 * Created by ckalo on 2/5/26.
 */

#include "glad.h"
#include "shader_build.h"
#include "bits/types/FILE.h"
#include "logger.h"


static void
check_shader_errors(unsigned int shader, const char* type)
{
    int success;
    char infoLog[1024] = {'\0'}; // Increased buffer size
    FILE* file;

    // compare first 8 chars if they match
    if((uint64)type == (uint64)"PROGRAM") {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        glGetProgramInfoLog(shader, sizeof(infoLog), NULL, infoLog);
        if(!success) {
            dprint(file, 3, "ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
        } else {
            dprint(file, 3, "SHADER::%s::LINKING SUCCEEDED\n", type);
        }
    } else {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
        if(!success) {
            dprint(file, 3, "ERROR::SHADER::%s::COMPILATION_FAILED\n%s\n", type, infoLog);
        } else {
            dprint(file, 3, "SHADER::%s::COMPILATION SUCCEEDED\n", type);
        }

    }
}


uint32 // build and compile shader program
shader_build(const char* vs_src, const char* fs_src)
{
    FILE* file;
    dprint(file, 1, "\n*** shader_build.c ***\n");

    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vs_src, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    check_shader_errors(vertexShader, "VERTEX");


    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fs_src, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    check_shader_errors(fragmentShader, "FRAGMENT");


    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    check_shader_errors(shaderProgram, "PROGRAM");


    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}
