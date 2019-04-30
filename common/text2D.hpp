#ifndef TEXT2D_HPP
#define TEXT2D_HPP

#include <cstring>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "shader.hpp"
#include "text2D.hpp"
#include "texture.hpp"

using namespace glm;

void initText2D(const char * texturePath);
void printText2D(const char * text, int x, int y, int size);
void cleanupText2D();

#endif