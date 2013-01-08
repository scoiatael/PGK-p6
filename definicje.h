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
void InitVBOs(GLuint vaoObject1, unsigned int numberOfVertices, GLuint vertexBufferObject, GLuint indexBufferObject);
void DrawVBOs(GLuint vaoObject1, unsigned int indexDataSize);
void CleanVBOs(GLuint vaoObject1);
void loadVertices(string filename, unsigned short* arg);
void genIndices(unsigned int* indices, unsigned int& density);
#endif
