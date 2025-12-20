#ifndef TEXTURE_H
#define TEXTURE_H

#include "gfx.h"
#include <string>
#include <iostream>
#include <stb_image.h>
#define STB_IMAGE_IMPLEMENTATION
#include "shader.h"

GLuint create_opengl_texture(const std::string& path);

#endif
