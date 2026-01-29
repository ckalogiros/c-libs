#include "glad.h"
#include "glfw3.h"
#include "logger.h"

int main(void)
{
  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);


  /* ↓↓↓↓ CRITICAL: INITIALIZE GLAD ↓↓↓↓ */
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    print("Failed to initialize GLAD!\n");
    return -1;
  }

  // Optional: Verify OpenGL version
  print("OpenGL %s\n", glGetString(GL_VERSION));
  /* ↑↑↑↑ MUST DO THIS BEFORE ANY OpenGL CALLS ↑↑↑↑ */

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}



