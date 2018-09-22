#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>
#include <vector>

GLuint load_shaders(const char *vertexFilePath, const char *fragmentFilePath);