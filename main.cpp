#include "definicje.h"

int main( int argc, char** argv )
{

  std::cout << "Starting..\n";
  std::vector<std::string> file_names;
  parse_args(argc, argv, file_names);
  InitGraphics();

  glfwSetWindowTitle( "p6" );

  // Ensure we can capture the escape key being pressed below
  glfwEnable( GLFW_STICKY_KEYS );

  // Dark blue background
  glClearColor(0.0f, 0.0f, 0.1f, 0.0f);

  // Enable depth test
//  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
//  glDepthFunc(GL_LESS); 

  // Create and compile our GLSL program from the shaders
  GLuint programID = LoadShaders( "tvs.vertexshader", "cfs.fragmentshader" );
  std::cout << "Linked shaders..\n";
  // Get a handle for our "MVP" uniform
  GLuint MatrixID = glGetUniformLocation(programID, "MVP");
  std::cout << "Got uniforms..\n";
  // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
  glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
  // Camera matrix
  glm::mat4 View       = glm::lookAt(
                                                          glm::vec3(0,0,1), // Camera is at (4,3,-3), in World Space
                                                          glm::vec3(0,0,0), // and looks at the origin
                                                          glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                                             );
  // Model matrix : an identity matrix (model will be at the origin)
  glm::mat4 Model      = glm::mat4(1.0f);
  // Our ModelViewProjection : multiplication of our 3 matrices
	
  glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around

  GLuint vaoObject1, vertexBufferObject, indexBufferObject;
//  unsigned int numberOfVertices=1201*1201, density=1, numberOfIndices=6*(1201/density)*(1201/density);
//  std::cout << "Allocating memory.. Need " << 3*numberOfVertices*sizeof(short int) + numberOfIndices*sizeof(int) << "\n";
//  unsigned short int* vertexPositions = new(unsigned short int[3*numberOfVertices]);
//  unsigned int* indices = new(unsigned int[numberOfIndices]);
//  std::cout << "Done.\n";
//  loadVertices(file_names[0], vertexPositions);
//  genIndices(indices, 1);
//  InitVBOs(vaoObject1, numberOfVertices, vertexBufferObject, indexBufferObject);
//  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
//  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  
    GLfloat vertexPositions[] = { -1., -1., -1., 0., 1., 0., 1., -1. , -1.};
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
//  unsigned int indices[] = { 0, 3, 4, 3, 1, 4};
//  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6, indices, GL_STATIC_DRAW);

  do{

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use our shader
    glUseProgram(programID);

    // Send our transformation to the currently bound shader, 
    // in the "MVP" uniform
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

   // DrawVBOs(vaoObject1, numberOfIndices);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
            0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
      );
    glDrawArrays(GL_TRIANGLES,0,3);
    // Swap buffers
    glfwSwapBuffers();

  } // Check if the ESC key was pressed or the window was closed
  while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
             glfwGetWindowParam( GLFW_OPENED ) );

  // Cleanup VBO and shader
  glDeleteProgram(programID);
  CleanVBOs(vaoObject1);

  // Close OpenGL window and terminate GLFW
  glfwTerminate();

  return 0;
}

