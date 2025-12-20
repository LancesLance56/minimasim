#include "vao.h"

VAO::VAO() {
    glGenVertexArrays(1, &ID_);
}

VAO::~VAO() {
    if (ID_ != 0) {
        glDeleteVertexArrays(1, &ID_);
    }
}

VAO::VAO(VAO&& other) noexcept : ID_(other.ID_) {
    other.ID_ = 0;
}

VAO& VAO::operator=(VAO&& other) noexcept {
    if (this != &other) {
        if (ID_ != 0) {
            glDeleteVertexArrays(1, &ID_);
        }
        ID_ = other.ID_;
        other.ID_ = 0;
    }
    return *this;
}

void VAO::bind() const {
    glBindVertexArray(ID_);
}

void VAO::unbind() {
    glBindVertexArray(0);
}

void VAO::vattribptr(GLuint index,
                        GLint size,
                        GLenum type,
                        GLsizei stride,
                        std::size_t offset) const {
    glVertexAttribPointer(index,
                          size,
                          type,
                          GL_FALSE,
                          stride,
                          reinterpret_cast<const void*>(offset));
    glEnableVertexAttribArray(index);
}
