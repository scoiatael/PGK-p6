#include "definicje.h"

int mcc::get_time()
{
  timeval timer;
  gettimeofday(&timer, NULL);
  return timer.tv_sec*1000000 + timer.tv_usec;
}
float mcc::get_timef()
{
  return (float)mcc::get_time()/10000.0f;
}
int mcc::make_lock(const float& timeout)
{
  locks.push_back(std::pair<float,float>(0.,timeout));
  return locks.size()-1;
}
int mcc::get_lock(const int& ID)
{
  if(ID<locks.size())
    return locks[ID].first;
  return 0;
}
void mcc::reset_lock(const int& ID)
{
  if(ID < locks.size())
    locks[ID].first = locks[ID].second;
}
void mcc::update()
{
  int cur_time = get_time();
  int interval = cur_time-last_update;
  last_update = cur_time;
  for(unsigned int i=0; i<locks.size();i++)
    locks[i].first-=interval;
}

void parse_args(const int& argc, char** argv, std::vector<std::string>& arg)
{
  arg.clear();
  if(argc==1 || (argv[1][0]=='-' && argc==2))
  {
    std::cerr << "Usage: " << argv[0] << " (-[options]) [filenames]\n";
    exit(-1);
  }
  int i=1;
  if(argv[1][0]=='-')
  {
    std::cout << "Options unimplemented yet.\n";
    i++;
  }
  for(;i<argc;i++)
  {
    arg.push_back(std::string(argv[i]));
  }

}
void InitGraphics()
{
  // Initialise GLFW
  if( !glfwInit() )
  {
          fprintf( stderr, "Failed to initialize GLFW\n" );
          exit(-1);
  }

  glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
  glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Open a window and create its OpenGL context
  if( !glfwOpenWindow( 1024, 768, 0,0,0,0, 32,0, GLFW_WINDOW ) )
  {
          fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
          glfwTerminate();
          exit(-1);
  }

  // Initialize GLEW
  if (glewInit() != GLEW_OK) {
          fprintf(stderr, "Failed to initialize GLEW\n");
          exit(-1);
  }
  std::cout << "Initialized GL Components.\n";
}

void InitVBOs(GLuint& vaoObject1, const unsigned int& numberOfVertices, GLuint& vertexBufferObject, GLuint& indexBufferObject)
{
  std::cout << "Init start.\n";
  glGenVertexArrays(1, &vaoObject1);
  glBindVertexArray(vaoObject1);
  
  glGenBuffers(1, &vertexBufferObject);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_SHORT, GL_FALSE, 0, (void*)0);
  glGenBuffers(1, &indexBufferObject);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
  
  glBindVertexArray(0);
  std::cout << "Init done.\n";
}

void DrawVBOs(const GLuint& vaoObject1, const unsigned int& indexDataSize)
{
  glBindVertexArray(vaoObject1);
  //glDrawElements(GL_TRIANGLES, indexDataSize, GL_UNSIGNED_INT, 0); 
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glBindVertexArray(0);
}

void CleanVBOs(const GLuint& vaoObject1)
{
  glBindVertexArray(vaoObject1);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDeleteVertexArrays(1, &vaoObject1);
}
void loadVertices(const std::string& filename, unsigned short int* arg)
{
  std::cout << "Loading vertices from " << filename << "...\n";
  std::fstream file;
  file.open(filename.c_str(), std::fstream::in);
  unsigned short max_h=0;
  for(unsigned int i=0; i<1201*1201; i++)
  {
    char p1, p2;
    file >> p1 >> p2;
    arg[3*i]    = i%1201;
    arg[3*i+1]  = i/1201;
    arg[3*i+2]  = p1*256+p2;
    if(arg[3*i+2]>max_h)
      max_h=arg[3*i+2];
  }
  file.close();
  std::cout << "Done. Maximal height is " << max_h << "\n";
}

void genIndices(unsigned int* indices, const unsigned int& density)
{
  //ASSERT(density!=0)
  for(int y=1,i=0; y<1201; y+=density)
    for(int x=1; x<1201; x+=density)
    {
      indices[i++] = y*1201+x-1;
      indices[i++] = y*1201+x;
      indices[i++] = (y-1)*1201+x-1;
      indices[i++] = y*1201+x;
      indices[i++] = (y-1)*1201+x;
      indices[i++] = (y-1)*1201-1;
    }
}
