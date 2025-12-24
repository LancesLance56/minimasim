#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <typeindex>
#include <unordered_map> // faster lookups than std::map
#include <utility>

#include "../../../include/glm/detail/type_vec3.hpp"
#include "components/component.h"
#include "components/mesh_renderer_component.h"
#include "entity_base.h"
#include "entity_concepts.h"

// Forward Declarations
namespace NBodySim {
    struct MassComponent;
}
class MeshFilter;
class MeshRenderer;
class Shader;
class Light;

struct Transform {
    glm::vec3 position{0.0f};
    glm::vec3 rotation{0.0f};
    glm::vec3 scale{1.0f};
};

template<typename T>
concept IsComponent = std::is_base_of_v<IComponent, std::remove_reference_t<T>>;

template<typename Derived>
class Entity : public EntityBase {
public:
    RenderSettings render_settings;
    Transform transform;

    Entity() { static_assert(HasAnyDraw<Derived>, "Entity must implement at least one draw() overload!"); }

    // --- Standard Virtual Overrides (Bridging to CRTP) ---

    void update(double dt) override { static_cast<Derived *>(this)->update(dt); }

    void draw(const Camera &cam, const RenderSettings &rs) override {
        if constexpr (HasUnlitDraw<Derived>) {
            static_cast<Derived *>(this)->draw(cam, rs);
        }
        draw_components(cam, rs, {});
    }

    void draw(const Camera &cam, const RenderSettings &rs, const std::span<const Light> &lights) override {
        if constexpr (HasLitDraw<Derived>) {
            static_cast<Derived *>(this)->draw(cam, rs, lights);
        }
        draw_components(cam, rs, lights);
    }

    [[nodiscard]] RenderSettings &get_render_settings() override { return render_settings; }

    void set_render_settings(const RenderSettings &rs) override { render_settings = rs; }

    // --- Component Management (CRTP Logic) ---

    template<typename T>
    [[nodiscard]] bool has_component() const {
        static_assert(std::is_base_of_v<IComponent, T>, "T must derive from IComponent.");
        return components.contains(std::type_index(typeid(T)));
    }

    template<typename T>
    [[nodiscard]] T &get_component() {
        static_assert(std::is_base_of_v<IComponent, T>, "T must derive from IComponent.");
        auto it = components.find(std::type_index(typeid(T)));
        if (it == components.end()) {
            throw std::runtime_error("Error: Attempted to retrieve component that was not added.");
        }
        // unique_ptr::get() returns a raw pointer, we dereference it to return a reference
        return static_cast<T &>(*it->second);
    }

    template<typename T>
    [[nodiscard]] const T &get_component() const {
        static_assert(std::is_base_of_v<IComponent, T>, "T must derive from IComponent.");
        auto it = components.find(std::type_index(typeid(T)));
        if (it == components.end()) {
            throw std::runtime_error("Error: Attempted to retrieve const component that was not added.");
        }
        return static_cast<const T &>(*it->second);
    }

    template<typename T = MeshRenderer, std::enable_if_t<std::is_same_v<T, MeshRenderer>, int> = 0>
    void add_component(const std::shared_ptr<Shader> &shader) {
        if (!has_component<MeshFilter>()) {
            std::cerr << "Failed to add MeshRenderer: Requires MeshFilter." << std::endl;
            return;
        }

        auto &mesh_filter_comp = get_component<MeshFilter>();
        auto component_ptr = std::make_unique<MeshRenderer>(shader, mesh_filter_comp);
        components[std::type_index(typeid(MeshRenderer))] = std::move(component_ptr);
    }

    template<
            typename T = NBodySim::MassComponent, typename... Args,
            std::enable_if_t<std::is_same_v<T, NBodySim::MassComponent>, int> = 0>
    void add_component(Args &&...args) {
        auto component_ptr = std::make_unique<NBodySim::MassComponent>(transform.position, std::forward<Args>(args)...);
        components[std::type_index(typeid(NBodySim::MassComponent))] = std::move(component_ptr);
    }

    template<
            typename T, typename... Args,
            std::enable_if_t<!std::is_same_v<T, NBodySim::MassComponent> && !std::is_same_v<T, MeshRenderer>, int> = 0>
    void add_component(Args &&...args) {
        static_assert(std::is_base_of_v<IComponent, T>, "T must derive from IComponent.");
        auto component_ptr = std::make_unique<T>(std::forward<Args>(args)...);
        components[std::type_index(typeid(T))] = std::move(component_ptr);
    }

private:
    std::unordered_map<std::type_index, std::unique_ptr<IComponent>> components;

    void draw_components(const Camera &cam, const RenderSettings &rs, const std::span<const Light> &lights) {
        for (const auto &kv: components) {
            kv.second->draw(cam, rs, lights);
        }
    }
};

#endif
