#include "definicje.h"
int x,y,z;
unsigned int iBOindex;
float ox, oy,oz;
const int maxLoD(10);
bool autolod=false;
const double optfps(20);
char ball=0;
int startx, starty;

inline int min(int a, int b)
{
  if(a<b)
    return a;
  else
    return b;
}
inline int max(int a, int b)
{
  if(a>b)
    return a;
  else
    return b;
}
void draw(GLuint& vaoObject, GLuint& vertexBufferObject, GLuint& indexBufferObject, unsigned int numberOfVertices)
{
    glBindVertexArray(vaoObject);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    glDrawElements(GL_TRIANGLES, numberOfVertices, GL_UNSIGNED_INT,0);
}

void CleanVBOs(GLuint* vaoObjects, GLuint* vBO, const unsigned int& vBOsize,  GLuint* iBO)
{
  glDeleteBuffers(vBOsize, vBO);
  glDeleteBuffers(maxLoD, iBO);
  glDeleteVertexArrays(vBOsize, vaoObjects);
}

void GLFWCALL Key_Callback(int key, int action)
{
  int mod=500;
  if(action == GLFW_PRESS)
  {
//    std::cout << (char)key << " " << iBOindex <<  "\n";
    switch(key)
    {
      case 'Q':
        x-=mod;
        break;
      case 'A':
        x+=mod;
        break;
      case 'W':
        y-=mod;
        break;
      case 'S':
        y+=mod;
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
      case 'Y':
        oz+=5;
        break;
      case 'H':
        oz-=5;
        break;
      case GLFW_KEY_UP:
        iBOindex+=1;
        iBOindex%=maxLoD;
        break;
      case GLFW_KEY_DOWN:
        iBOindex-=1;
        iBOindex%=maxLoD;
        break;
      case GLFW_KEY_SPACE:
        autolod=!autolod;
        break;
      case GLFW_KEY_ENTER:
        ball=1-ball;
        oz=0;
        oy=0;
        ox=0;
        z=0;
        if(ball==0)
        {
          x=startx;
          y=starty;
        }
        else
        {
          y=0;
          x=0;
        }
        break;


    }
  }
}

int main( int argc, char** argv )
{

  std::cout << "Starting..\n";
  std::vector<std::string> file_names;
  parse_args(argc, argv, file_names);
  file_names.push_back(std::string("b0e0.hgt"));
  InitGraphics();

  glfwSetWindowTitle( "p6" );

  // Ensure we can capture the escape key being pressed below
  glfwEnable( GLFW_STICKY_KEYS );

  // Dark blue background
  glClearColor(0.7f, 0.7f, 0.7f, 0.0f);

  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS); 
  glFrontFace(GL_CCW);

  // Create and compile our GLSL program from the shaders
  GLuint programIDs[2] = { LoadShaders( "tvs.vertexshader", "cfs.fragmentshader" ), LoadShaders( "tvs2.vertexshader", "cfs.fragmentshader" )};
  std::cout << "Linked shaders..\n";
  // Get a handle for our "MVP" uniform
  GLuint MatrixIDs[2] = {glGetUniformLocation(programIDs[0], "MVP"), glGetUniformLocation(programIDs[1], "MVP")};
  GLuint EdgexIDs[2] = {glGetUniformLocation(programIDs[0], "Edgex"), glGetUniformLocation(programIDs[1], "Edgex")};
  GLuint EdgeyIDs[2] = {glGetUniformLocation(programIDs[0], "Edgey"), glGetUniformLocation(programIDs[1], "Edgey")};
  GLuint SideIDs[2] = {glGetUniformLocation(programIDs[0], "side"), glGetUniformLocation(programIDs[1], "side")};
  std::cout << "Got uniforms..\n";
  
  const int side(1201);
  const int vBOsize(file_names.size());



  //Vertices:
  short piece_map[360][180];
  for(int i=0; i<360; i++)
    for(int y=0; y<180; y++)
      piece_map[i][y] = -1;
  unsigned int numberOfVertices=side*side;
  GLuint vaoObjects[vBOsize+1], vertexBufferObject, vBOs[vBOsize+1];
  std::vector<std::pair<int, int> > edges(vBOsize+1);
  glGenVertexArrays(vBOsize+1, vaoObjects);
  glGenBuffers(vBOsize+1, vBOs);
  int height;
  for(short i=0; i< vBOsize;i++)
  {
    std::vector< int > vertexPositionsVec(3*numberOfVertices);
    int* vertexPositions = &vertexPositionsVec[0];
    loadVertices(file_names[i], vertexPositionsVec, true, side, edges[i], height);
    glBindVertexArray(vaoObjects[i]);
    glBindBuffer(GL_ARRAY_BUFFER, vBOs[i]);
    glVertexAttribPointer(
          0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
          3,                  // size
          GL_INT,           // type
          GL_FALSE,           // normalized?
          0,                  // stride
          (void*)0            // array buffer offset
    );
    glBufferData(GL_ARRAY_BUFFER, sizeof(int)*3*numberOfVertices, vertexPositions, GL_STATIC_DRAW);
    if(i<vBOsize-1)
    {
      piece_map[edges[i].second+180][edges[i].first+90]=i;
      std::cout << edges[i].second+180 << " " << edges[i].first+90 << std::endl;
    }
  }

  //Indices::
  GLuint indexBufferObject, iBOs[maxLoD], numberOfIndices;
  std::vector<GLuint> nOIs(5);
  glGenBuffers(maxLoD, iBOs);
  for(unsigned int density=1, i=0;i<maxLoD; i++, density*=2)
  {  
    nOIs[i]=6*((side-1)/(density)+1)*((side-1)/(density)+1);
    std::vector< GLuint> indicesVec(nOIs[i]);
    GLuint* indices = &indicesVec[0];
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBOs[i]);
    std::cout << "Density: " << density << " Number of indices: " << nOIs[i] << std::endl;
    genIndices(indicesVec, side, density);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*nOIs[i], indices, GL_STATIC_DRAW);
  }
  
  // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
  glm::mat4 Projection = 
   // glm::mat4(1.0f);
glm::perspective(45.0f, 4.0f / 3.0f, 100.f, 30000.0f);
  // Camera matrix
//  int xVw = edges[0].first*side, yVw = edges[0].second*side;
  int xVw = 6000, yVw = -6000;
  height = 3000;
  glm::mat4 View       = glm::lookAt(
                                                          glm::vec3(xVw,yVw,2*height), // Camera is at (4,3,-3), in World Space
                                                          glm::vec3(xVw,yVw,0), // and looks at the origin
                                                          glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                                             );
  // Model matrix : an identity matrix (model will be at the origin)
  glm::mat4 Model      = glm::mat4(1.0f);
  // Our ModelViewProjection : multiplication of our 3 matrices
	
  glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around

  std::cout << "Init done.\n";
  
  glfwSetKeyCallback(Key_Callback);
  
  double last_time = glfwGetTime(), last_reset=last_time;
  int FPScounter=0;

  x = edges[0].second*12010;
  startx=x;
  y = edges[0].first*12010;
  starty=y;
  std::cout << edges[0].first << " " << edges[0].second << std::endl;
  do{
    //time statistics:
    FPScounter++;
    double cur_time = glfwGetTime();
    if(cur_time-last_reset>=2)
    {
      double FPS = (float)FPScounter/(cur_time-last_reset);
      std::cout << "FPS: " << FPS << " lod: " << iBOindex << std::endl;
      std::cout << x/1201+180 << " " << y/1201+90 << " " << z << "\n";
      if(autolod && abs(FPS-optfps)>4)
      {
        if(FPS<optfps && iBOindex<maxLoD)
          iBOindex++;
        if(FPS>4*optfps && iBOindex > 0)
          iBOindex--;
      }
      FPScounter=0;
      last_reset=cur_time;
    }
    last_time=cur_time;

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use our shader
    glUseProgram(programIDs[ball]);

    // Send our transformation to the currently bound shader, 
    // in the "MVP" uniform
    glm::mat4 Vw=MVP * glm::translate( glm::rotate( 
                                    glm::rotate(
                                                glm::rotate(
                                                              mat4(1.0),
                                                             oy, glm::vec3(0, 0, 1)),
                                                ox, glm::vec3(1,0,0)),
                                    oz, glm::vec3(0,1,0)),vec3(-x,-y,z));
    glUniformMatrix4fv(MatrixIDs[ball], 1, GL_FALSE, &Vw[0][0]);
    glUniform1i(SideIDs[ball], side);

    indexBufferObject=iBOs[iBOindex];
    numberOfIndices=nOIs[iBOindex];

    int ex = x/12010+180;
    int ey = y/12010+90;
 //   std::cout << ex << " " << ey << std::endl;
    if(ball==0)
    {
      for(int i = max(ex-3,0); i<= min(ex+3,360) ;i++)
        for(int j=max(ey-3,0); j<= min(ey+3,180); j++)
        {
          glUniform1i(EdgexIDs[ball], (i-180));
          glUniform1i(EdgeyIDs[ball], (j-90));
          int point;
          if(piece_map[i][j]==-1)
          {
            point=vBOsize-1;
            draw(vaoObjects[point], vBOs[point], iBOs[maxLoD-1], nOIs[maxLoD-1]);
          }
          else
          {
            point = piece_map[i][j];
            draw(vaoObjects[point], vBOs[point], indexBufferObject, numberOfIndices);
          }
   //         std::cout << "Drawing " << file_names[point] << "with mods " << i-180 << " " << j-90 << std::endl
   //           << i << " "  << ex << " " << j << " " << ey << std::endl;

        }
        glCullFace(GL_BACK);
    }
    else
    {
      for(int i=edges[0].second-4+180; i<edges[0].second+5+180;i++)
        for(int j=edges[0].first-4+90; j<edges[0].first+5+90;j++)
        {

          glUniform1i(EdgexIDs[ball], (i-180));
          glUniform1i(EdgeyIDs[ball], (j-90));

          int point;
          if(piece_map[i][j]==-1)
          {
            point=vBOsize-1;
            draw(vaoObjects[point], vBOs[point], iBOs[6], nOIs[6]);
          }
          else
          {
            point = piece_map[i][j];
            draw(vaoObjects[point], vBOs[point], indexBufferObject, numberOfIndices);
          }
        }
        glCullFace(GL_FRONT);
    }

    // Swap buffers
    glfwSwapBuffers();

  } // Check if the ESC key was pressed or the window was closed
  while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
             glfwGetWindowParam( GLFW_OPENED ) );

  // Cleanup VBO and shader
  glDeleteProgram(programIDs[0]);
  glDeleteProgram(programIDs[1]);
  CleanVBOs(vaoObjects, vBOs, vBOsize, iBOs);

  std::cout << "Cleaning done, terminating..\n";
  // Close OpenGL window and terminate GLFW
  glfwTerminate();

  return 0;
}

