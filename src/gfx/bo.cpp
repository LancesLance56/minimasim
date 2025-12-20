#include "ABO.h" // Include the ABO header

ABO abo_create(GLenum type, GLenum setting) {
    ABO new_abo;
    glGenBuffers(1, &new_abo.ID);
    new_abo.type = type;
    new_abo.setting = setting;
    return new_abo;
}

void abo_destroy(ABO self) {
    glDeleteBuffers(1, &self.ID);
}

void abo_bind(ABO self) {
    glBindBuffer(self.type, self.ID);
}

// *** Implementation for GLfloat vector ***
void abo_data(ABO self, GLsizeiptr size, const std::vector<GLfloat>& data) {
    abo_bind(self); // Ensure the buffer is bound
    // Use data.data() to get the raw pointer to the vector's underlying array
    glBufferData(self.type, size, data.data(), self.setting);
}

// *** Implementation for GLuint vector ***
void abo_data(ABO self, GLsizeiptr size, const std::vector<GLuint>& data) {
    abo_bind(self); // Ensure the buffer is bound
    // Use data.data() to get the raw pointer to the vector's underlying array
    glBufferData(self.type, size, data.data(), self.setting);
}

// *** Implementation for std::any (if you choose to keep it) ***
// This is less type-safe and more complex, generally prefer specific overloads
void abo_data(const ABO self, const GLsizeiptr size, const std::any &data) {
    abo_bind(self);
    if (data.type() == typeid(const std::vector<GLfloat>&)) { // Note: std::any stores by value or reference wrappers
        const auto& vec = std::any_cast<const std::vector<GLfloat>&>(data);
        glBufferData(self.type, size, vec.data(), self.setting);
    } else if (data.type() == typeid(const std::vector<GLuint>&)) {
        const auto& vec = std::any_cast<const std::vector<GLuint>&>(data);
        glBufferData(self.type, size, vec.data(), self.setting);
    } else {
        // Handle unsupported type or throw an exception
        // std::cerr << "Error: Unsupported data type passed to abo_data (std::any overload)." << std::endl;
        // Optionally: throw std::runtime_error("Unsupported data type for abo_data.");
    }
}
