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

void InitGraphics()
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
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
		return -1;
	}

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
}

void InitVBOs(GLuint vaoObject1, unsigned int numberOfVertices, GLuint vertexBufferObject, GLuint indexBufferObject)
{
  glGenVertexArrays(1, &vaoObject1);
  glBindVertexArray(vaoObject1);
  
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_UNSIGNED_SHORT, GL_FALSE, 0, (void*)0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
  
  glBindVertexArray(0);
}

void DrawVBOs(GLuint vaoObject1, unsigned int indexDataSize)
{
  glBindVertexArray(vaoObject1);
  glDrawElements(GL_TRIANGLES, indexDataSize, GL_UNSIGNED_SHORT, 0); 
  glBindVertexArray(0);
}

void CleanVBOs(GLuint vaoObject1)
{
  glBindVertexArray(vaoObject11);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDeleteVertexArrays(1, &vaoObject1);
}
void loadVertices(string filename, unsigned short* arg)
{
  fstream file;
  file.open(filename, ios_base::in);
  for(unsigned int i=0; i<1201*1201; i++)
  {
    unsigned short p;
    file >> p;
    arg[3*i]    = i%1201;
    arg[3*i+1]  = i/1201;
    arg[3*i+2]  = p;
  }
}

void genIndices(unsigned int* indices, unsigned int& density)
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
