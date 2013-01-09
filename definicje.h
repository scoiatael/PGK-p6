#ifndef definicje
#define definicje

// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GL/glfw.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>

#include<sys/time.h>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>

struct mcc
{
  static int get_time();
  static float get_timef();

  std::vector< std::pair<int,int> > locks;
  int last_update;
  mcc()
  :  locks(), last_update(get_time())
    {}

  int make_lock(const float& timeout);
  int get_lock(const int& ID);
  void reset_lock(const int& ID);
  void update();
};

void InitGraphics();
void InitVBOs(GLuint& vaoObject1, const unsigned int& numberOfVertices, GLuint& vertexBufferObject, GLuint& indexBufferObject);
void DrawVBOs(const GLuint& vaoObject1, const unsigned int& indexDataSize);
void CleanVBOs(const GLuint& vaoObject1);
void loadVertices(const std::string& filename, unsigned short int* arg);
void genIndices(unsigned int* indices, const unsigned int& density);
void parse_args(const int& argc, char** argv, std::vector<std::string>& arg);
#endif
