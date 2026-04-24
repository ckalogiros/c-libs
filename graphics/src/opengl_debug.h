/*
 * Created by ckalo on 4/16/26.
 */
#ifndef ENGINE_TEST_OPENGL_DEBUG_H
#define ENGINE_TEST_OPENGL_DEBUG_H

// #include "C_logger/Logger.h"
#include <glad/gl.h>

#include "glfw3.h"
#include "logger.h"


static void opengl_debug_shader(const GLuint shader)
{
    int  success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        print("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s%s%s\n", YELLOW, infoLog, CLEAR);
    }
}

static void opengl_debug_program(const GLuint program)
{
    int  success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        print("ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n%s%s%s\n", YELLOW, infoLog, CLEAR);
    }
}


static void APIENTRY glDebugOutput( GLenum source,
                                    GLenum type,
                                    unsigned int id,
                                    GLenum severity,
                                    GLsizei length,
                                    const char* message,
                                    const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;


	//Error code 	Description
	GL_INVALID_ENUM 	;	//GLenum argument out of range.
	GL_INVALID_VALUE 	;	//Numeric argument out of range.
	GL_INVALID_OPERATION ;	//Operation illegal in current state.
	GL_STACK_OVERFLOW 	;	//Function would cause a stack overflow.
	GL_STACK_UNDERFLOW 	;	//Function would cause a stack underflow.
	GL_OUT_OF_MEMORY;		//Not enough memory left to execute function.


	print("Error id:%d | msg:%s User param:%s\n ", id, message, (char*)userParam);

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             print("%s", "Source: API"); break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   print("%s", "Source: Window System"); break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: print("%s", "Source: Shader Compiler"); break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     print("%s", "Source: Third Party"); break;
	case GL_DEBUG_SOURCE_APPLICATION:     print("%s", "Source: Application"); break;
	case GL_DEBUG_SOURCE_OTHER:           print("%s", "Source: Other"); break;
	}

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:                print( "%s", "Type: Error"); break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:  print( "%s", "Type: Deprecated Behaviour"); break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:   print( "%s", "Type: Undefined Behaviour"); break;
	case GL_DEBUG_TYPE_PORTABILITY:          print( "%s", "Type: Portability"); break;
	case GL_DEBUG_TYPE_PERFORMANCE:          print( "%s", "Type: Performance"); break;
	case GL_DEBUG_TYPE_MARKER:               print( "%s", "Type: Marker"); break;
	case GL_DEBUG_TYPE_PUSH_GROUP:           print( "%s", "Type: Push Group"); break;
	case GL_DEBUG_TYPE_POP_GROUP:            print( "%s", "Type: Pop Group"); break;
	case GL_DEBUG_TYPE_OTHER:                print( "%s", "Type: Other"); break;
	}

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:          print( "%s", "Severity: high"); break;
	case GL_DEBUG_SEVERITY_MEDIUM:        print( "%s", "Severity: medium"); break;
	case GL_DEBUG_SEVERITY_LOW:           print( "%s", "Severity: low"); break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:  print( "%s", "Severity: notification"); break;
	}
	print("\n");

}


static void OpenGlDebugContext()
{
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(glDebugOutput, NULL);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
}


#endif //ENGINE_TEST_OPENGL_DEBUG_H