#ifndef BO_H
#define BO_H

#include <glad/glad.h>
#include <span>
#include <vector>

enum class BufferType {
    VBO, // GL_ARRAY_BUFFER
    EBO, // GL_ELEMENT_ARRAY_BUFFER
    UBO  // GL_UNIFORM_BUFFER
};

// Traits to map BufferType → OpenGL target
template<BufferType T> struct BufferTraits;

template<> struct BufferTraits<BufferType::VBO> {
    static constexpr GLenum Target = GL_ARRAY_BUFFER;
};

template<> struct BufferTraits<BufferType::EBO> {
    static constexpr GLenum Target = GL_ELEMENT_ARRAY_BUFFER;
};

template<> struct BufferTraits<BufferType::UBO> {
    static constexpr GLenum Target = GL_UNIFORM_BUFFER;
};

template<BufferType T>
struct BO {
    GLuint ID = 0;
    GLenum usage_hint = GL_STATIC_DRAW;

    BO() {
        glGenBuffers(1, &ID);
    }

    explicit BO(const GLenum usage_) : usage_hint(usage_) {
        glGenBuffers(1, &ID);
    }

    ~BO() {
        if (ID) {
            glDeleteBuffers(1, &ID);
        }
    }

    BO(const BO&) = delete;
    BO& operator=(const BO&) = delete;

    BO(BO&& other) noexcept : ID(other.ID), usage_hint(other.usage_hint) {
        other.ID = 0;
    }
    BO& operator=(BO&& other) noexcept {
        if (this != &other) {
            if (ID) glDeleteBuffers(1, &ID);
            ID = other.ID;
            usage_hint = other.usage_hint;
            other.ID = 0;
        }
        return *this;
    }

    void bind() const {
        glBindBuffer(BufferTraits<T>::Target, ID);
    }

    template<typename U>
    void set_data(const std::vector<U> & data) const {
        bind();
        glBufferData(BufferTraits<T>::Target,
                     data.size() * sizeof(U),
                     data.data(),
                     usage_hint);
    }

    void set_data(GLsizeiptr size, const void* data) const {
        bind();
        glBufferData(BufferTraits<T>::Target, size, data, usage_hint);
    }
};

#endif // BO_H
