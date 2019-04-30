#ifndef SHADER_HPP
#define SHADER_HPP

#include <algorithm>
#include <fstream>
#include <GL/glew.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
using namespace std;

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

#endif
