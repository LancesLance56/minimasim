#ifndef VECTOR_UTILS_H
#define VECTOR_UTILS_H

#include <glm/glm.hpp>
#include <vector>
#include <algorithm>
#include <type_traits>
#include <span>

/**
 * @brief Utility namespace for vector/span conversion helpers.
 */
namespace VectorUtils {
    template <typename T>
    struct ComponentCount {
        static constexpr size_t value = 1; // Default for scalar types (like GLfloat)
    };

    template <>
    struct ComponentCount<glm::vec2> {
        static constexpr size_t value = 2;
    };

    template <>
    struct ComponentCount<glm::vec3> {
        static constexpr size_t value = 3;
    };

    template <>
    struct ComponentCount<glm::vec4> {
        static constexpr size_t value = 4;
    };

    template <typename T_Container>
    std::vector<GLfloat> flatten_container(const T_Container& container) {
        using T_Element = typename T_Container::value_type;

        if (container.empty()) {
            return {};
        }

        constexpr size_t component_count = ComponentCount<T_Element>::value;
        const size_t total_float_size = container.size() * component_count;

        std::vector<GLfloat> float_list;
        float_list.resize(total_float_size);

        std::copy(
            reinterpret_cast<const GLfloat*>(container.data()),
            reinterpret_cast<const GLfloat*>(container.data()) + total_float_size,
            float_list.begin()
        );

        return float_list;
    }

    template <typename T_Element>
    std::vector<GLfloat> flatten_container(const std::span<const T_Element>& container) {
        if (container.empty()) {
            return {};
        }

        constexpr size_t component_count = ComponentCount<T_Element>::value;
        const size_t total_float_size = container.size() * component_count;

        std::vector<GLfloat> float_list;
        float_list.resize(total_float_size);

        std::copy(
            reinterpret_cast<const GLfloat*>(container.data()),
            reinterpret_cast<const GLfloat*>(container.data()) + total_float_size,
            float_list.begin()
        );

        return float_list;
    }
} // namespace VectorUtils

#endif // VECTOR_UTILS_H