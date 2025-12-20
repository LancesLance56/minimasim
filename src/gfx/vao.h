#ifndef VAO_H
#define VAO_H

#include "glad_include.h"
#include <cstddef>

class VAO {
public:
    VAO();
    ~VAO();

    VAO(const VAO&) = delete;
    VAO& operator=(const VAO&) = delete;

    VAO(VAO&& other) noexcept;
    VAO& operator=(VAO&& other) noexcept;

    void bind() const;
    static void unbind();

    void vattribptr(GLuint index,
                       GLint size,
                       GLenum type,
                       GLsizei stride,
                       std::size_t offset) const;

    [[nodiscard]] GLuint id() const { return ID_; }

private:
    GLuint ID_ = 0;
};

#endif // VAO_H
