/*
 * Created by ckalo on 1/25/26.
 */

#include "qtriangle.h"
#include "glad.h"





uint32 qtriangle_init() {

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  float vertices[] = {
    // positions         // colors
    0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
   -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
    0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
  };

  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
  glEnableVertexAttribArray(1);

  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
  // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
  glBindVertexArray(0);

  return VAO;
}
// void qtriangle_init() {
//
//   GLuint VertexArrayID;
//   glGenVertexArrays(1, &VertexArrayID);
//   glBindVertexArray(VertexArrayID);
//
//   // An array of 3 vectors which represents 3 vertices
//   static const GLfloat g_vertex_buffer_data[] = {
//     -1.0f, -1.0f, 0.0f,
//     1.0f, -1.0f, 0.0f,
//     0.0f,  1.0f, 0.0f,
//  };
//
//
//   // This will identify our vertex buffer
//   GLuint VBO;
//   // Generate 1 buffer, put the resulting identifier in vertexbuffer
//   glGenBuffers(1, &VBO);
//   // The following commands will talk about our 'vertexbuffer' buffer
//   glBindBuffer(GL_ARRAY_BUFFER, VBO);
//   // Give our vertices to OpenGL.
//   glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
//
//   // 1st attribute buffer : vertices
//   glEnableVertexAttribArray(0);
//   glBindBuffer(GL_ARRAY_BUFFER, VBO);
//   glVertexAttribPointer(
//      0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//      3,                  // size
//      GL_FLOAT,           // type
//      GL_FALSE,           // normalized?
//      0,                  // stride
//      (void*)0            // array buffer offset
//   );
// }

void qtriangle_draw() {
  // Draw the triangle !
  glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
  // glDisableVertexAttribArray(0);
}