#include "definicje.h"
int x,y,z;
unsigned int iBOindex;
float ox, oy;
const int maxLoD(10);

void init(GLuint& vaoObject1,GLuint& vertexBufferObject)
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
    glDrawElements(GL_TRIANGLES, numberOfVertices, GL_UNSIGNED_INT,0);
}

void CleanVBOs(GLuint& vaoObject1, GLuint* vBO, GLuint* iBO)
{
  glBindVertexArray(vaoObject1);
  glDisableVertexAttribArray(0);
  glDeleteBuffers(1, vBO);
  glDeleteBuffers(maxLoD, iBO);
  glDeleteVertexArrays(1, &vaoObject1);
}

void GLFWCALL Key_Callback(int key, int action)
{
  int mod=100;
  if(action == GLFW_PRESS)
  {
//    std::cout << (char)key << " " << x <<" " << y << " " << z<< "\n";
    switch(key)
    {
      case 'Q':
        x+=mod;
        break;
      case 'A':
        x-=mod;
        break;
      case 'W':
        y+=mod;
        break;
      case 'S':
        y-=mod;
        break;
      case 'E':
        z+=mod;
        break;
      case 'D':
        z-=mod;
        break;
      case 'R':
        ox+=5;
        break;
      case 'F':
        ox-=5;
        break;
      case 'T':
        oy+=5;
        break;
      case 'G':
        oy-=5;
        break;
      case GLFW_KEY_UP:
        iBOindex+=1;
        iBOindex%=maxLoD;
        break;
      case GLFW_KEY_DOWN:
        iBOindex-=1;
        iBOindex%=maxLoD;
        break;


    }
  }
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
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS); 

  // Create and compile our GLSL program from the shaders
  GLuint programID = LoadShaders( "tvs.vertexshader", "cfs.fragmentshader" );
  std::cout << "Linked shaders..\n";
  // Get a handle for our "MVP" uniform
  GLuint MatrixID = glGetUniformLocation(programID, "MVP");
  std::cout << "Got uniforms..\n";
  // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
  glm::mat4 Projection = 
   // glm::mat4(1.0f);
glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 20000.0f);
  // Camera matrix
  glm::mat4 View       = glm::lookAt(
                                                          glm::vec3(1,1,3000), // Camera is at (4,3,-3), in World Space
                                                          glm::vec3(1,1,0), // and looks at the origin
                                                          glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                                             );
  // Model matrix : an identity matrix (model will be at the origin)
  glm::mat4 Model      = glm::mat4(1.0f);
  // Our ModelViewProjection : multiplication of our 3 matrices
	
  glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around
  
  const int side(1201);
  
  //Vertices:
  unsigned int numberOfVertices=side*side;
  std::vector< int > vertexPositionsVec(3*numberOfVertices);
  int* vertexPositions = &vertexPositionsVec[0];
  loadVertices(file_names[0], vertexPositionsVec, true, side);
  GLuint vaoObject1, vertexBufferObject;
  init(vaoObject1,vertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(int)*3*numberOfVertices, vertexPositions, GL_STATIC_DRAW);

  //Indices::
  GLuint indexBufferObject, iBOs[maxLoD], numberOfIndices;
  std::vector<GLuint> nOIs(5);
  glGenBuffers(maxLoD, iBOs);
  for(unsigned int density=1, i=0;i<maxLoD; i++, density*=2)
  {  
    nOIs[i]=6*((side-1)/(density))*((side-1)/(density));
    std::vector< GLuint> indicesVec(nOIs[i]);
    GLuint* indices = &indicesVec[0];
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBOs[i]);
    genIndices(indicesVec, side, density);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*nOIs[i], indices, GL_STATIC_DRAW);
  }
  
  std::cout << "Init done.\n";
  
  float start = mcc::get_time();
  glfwSetKeyCallback(Key_Callback);
  do{
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use our shader
    glUseProgram(programID);

    // Send our transformation to the currently bound shader, 
    // in the "MVP" uniform
    glm::mat4 temp=glm::rotate( glm::rotate(glm::translate(MVP, vec3(x,y,z)), oy, glm::vec3(0, 1, 0)), ox, glm::vec3(1,0,0));
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &temp[0][0]);

   // DrawVBOs(vaoObject1, numberOfIndices);
    indexBufferObject=iBOs[iBOindex];
    numberOfIndices=nOIs[iBOindex];
    draw(vertexBufferObject, indexBufferObject, numberOfIndices);
    // Swap buffers
    glfwSwapBuffers();

  } // Check if the ESC key was pressed or the window was closed
  while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
             glfwGetWindowParam( GLFW_OPENED ) );

  // Cleanup VBO and shader
  glDeleteProgram(programID);
  CleanVBOs(vaoObject1, &vertexBufferObject, iBOs);

  // Close OpenGL window and terminate GLFW
  glfwTerminate();

  return 0;
}

