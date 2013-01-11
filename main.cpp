#include "definicje.h"

void init(GLuint& vaoObject1,GLuint& vertexBufferObject,GLuint& indexBufferObject)
{
  glGenVertexArrays(1, &vaoObject1);
  glBindVertexArray(vaoObject1);
  glGenBuffers(1, &vertexBufferObject);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
  glVertexAttribPointer(
          0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
          3,                  // size
          GL_INT,           // type
          GL_FALSE,           // normalized?
          0,                  // stride
          (void*)0            // array buffer offset
    );
}

void draw(GLuint& vertexBufferObject, GLuint& indexBufferObject, unsigned int numberOfVertices)
{
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    glDrawElements(GL_TRIANGLES,numberOfVertices, GL_UNSIGNED_INT,0);
}


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
  glClearColor(0.0f, 0.0f, 0.5f, 0.0f);

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
  glm::mat4 Projection = 
   // glm::mat4(1.0f);
glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
  // Camera matrix
  glm::mat4 View       = glm::lookAt(
                                                          glm::vec3(1,1,3), // Camera is at (4,3,-3), in World Space
                                                          glm::vec3(1,1,0), // and looks at the origin
                                                          glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                                             );
  // Model matrix : an identity matrix (model will be at the origin)
  glm::mat4 Model      = glm::mat4(1.0f);
  // Our ModelViewProjection : multiplication of our 3 matrices
	
  glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around
  
  const int side(10);
  unsigned int numberOfVertices=side*side, density=1, numberOfIndices=6*((side-1)/(density))*((side-1)/(density));
  std::vector< int > vertexPositionsVec(3*numberOfVertices);
  int* vertexPositions = &vertexPositionsVec[0];
  std::vector< GLuint> indicesVec(numberOfIndices);
  GLuint* indices = &indicesVec[0];
  std::cout << numberOfVertices << " " << numberOfIndices << std::endl;
  loadVertices(file_names[0], vertexPositionsVec, true, side);
//  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
//  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  GLuint vaoObject1, vertexBufferObject, indexBufferObject;
  init(vaoObject1,vertexBufferObject,indexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(int)*3*numberOfVertices, vertexPositions, GL_STATIC_DRAW);
  glGenBuffers(1, &indexBufferObject);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
  genIndices(indices, side, 1);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*numberOfIndices, indices, GL_STATIC_DRAW);
  
  float start = mcc::get_time();
  do{
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use our shader
    glUseProgram(programID);

    // Send our transformation to the currently bound shader, 
    // in the "MVP" uniform
    glm::mat4 temp=glm::rotate(MVP, ((float)mcc::get_time()-start)/100000, glm::vec3(0, 1, 0));
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &temp[0][0]);

   // DrawVBOs(vaoObject1, numberOfIndices);
    draw(vertexBufferObject, indexBufferObject, 6);
    // Swap buffers
    glfwSwapBuffers();

  } // Check if the ESC key was pressed or the window was closed
  while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
             glfwGetWindowParam( GLFW_OPENED ) );

  // Cleanup VBO and shader
  glDeleteProgram(programID);
  CleanVBOs(vaoObject1, vertexBufferObject, indexBufferObject);

  // Close OpenGL window and terminate GLFW
  glfwTerminate();

  return 0;
}

