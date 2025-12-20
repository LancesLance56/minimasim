#ifndef ABO_H
#define ABO_H

#include <any>    // Keep if you still intend to use std::any for other data types
#include <vector>

#include "glad/glad.h"

// buffer object
struct ABO {
    GLuint ID;
    GLenum type; // e.g. GL_ELEMENT_ARRAY_BUFFER
    GLenum setting; // e.g. GL_DYNAMIC_DRAW
};

ABO abo_create(GLenum type, GLenum setting);
void abo_destroy(ABO self);
void abo_bind(ABO self);

void abo_data(ABO self, GLsizeiptr size, const std::vector<GLfloat>& data);
void abo_data(ABO self, GLsizeiptr size, const std::vector<GLuint>& data);

void abo_data(ABO self, GLsizeiptr size, const std::any &data);

#endif