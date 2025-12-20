#ifndef ENTITY_H
#define ENTITY_H

#include <optional>
#include <tuple>
#include <type_traits>
#include <utility>
#include <map>
#include <typeindex>
#include <stdexcept>
#include <memory>
#include <iostream>
#include <ranges>

#include "entity_base.h"
#include "entity_concepts.h"
#include "components/component.h"

namespace NBodySim { struct MassComponent; }
class MeshFilter;
class TrailComponent3D;
class MeshRenderer;
class Shader;
class Light;


struct Transform {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

template<typename T>
concept IsComponent = std::is_base_of_v<Component<std::remove_reference_t<T>>, std::remove_reference_t<T>>;


template <typename Derived>
class Entity : public EntityBase {
public:
    RenderSettings render_settings;
    Transform transform;

    Entity() {
        static_assert(HasAnyDraw<Derived>, "Entity must implement at least one draw() overload!");
    }

    void update(double dt) override {
        static_cast<Derived*>(this)->update(dt);
    }

    void draw(const Camera &cam, const RenderSettings & rs) override {
        if constexpr (HasUnlitDraw<Derived>) {
            static_cast<Derived*>(this)->draw(cam, rs);
        }
        draw_components(cam, rs, {});
    }

    void draw(const Camera& cam, const RenderSettings& rs,
              const std::span<const Light>& lights) override {
        if constexpr (HasLitDraw<Derived>) {
            static_cast<Derived*>(this)->draw(cam, rs, lights);
        }
        draw_components(cam, rs, lights);
    }

    [[nodiscard]] RenderSettings& get_render_settings() override {
        return static_cast<Derived *>(this)->render_settings;
    }

    void set_render_settings(RenderSettings rs) override {
        static_cast<Derived *>(this)->render_settings = rs;
    }

    template <typename T>
    [[nodiscard]] bool has_component() const {
        static_assert(std::is_base_of_v<IComponent, T>, "T must derive from IComponent.");
        return components.contains(std::type_index(typeid(T)));
    }

    template <typename T>
    [[nodiscard]] T& get_component() {
        static_assert(std::is_base_of_v<IComponent, T>, "T must derive from IComponent.");

        auto it = components.find(std::type_index(typeid(T)));
        if (it == components.end()) {
            throw std::runtime_error("Error: Attempted to retrieve component that was not added.");
        }
        return static_cast<T&>(*it->second);
    }

    template<typename T>
    [[nodiscard]] const T& get_component() const {
        static_assert(std::is_base_of_v<IComponent, T>, "T must derive from IComponent.");

        auto it = components.find(std::type_index(typeid(T)));
        if (it == components.end()) {
            throw std::runtime_error("Error: Attempted to retrieve const component that was not added.");
        }
        return static_cast<const T&>(*it->second);
    }

    template <typename T = MeshRenderer,
        std::enable_if_t<std::is_same_v<T, MeshRenderer>, int> = 0>
    void add_component(const std::shared_ptr<Shader>& shader) {
        if (!has_component<MeshFilter>()) {
            std::cerr << "Failed to add mesh renderer component:" << std::endl
                      << "  Component MeshRenderer requires MeshFilter" << std::endl;
            return;
        }

        auto& mesh_filter_comp = static_cast<Derived*>(this)->template get_component<MeshFilter>();

        auto component_ptr = std::make_shared<MeshRenderer>(shader, mesh_filter_comp);

        components[std::type_index(typeid(MeshRenderer))] = component_ptr;
    }

    template <typename T = NBodySim::MassComponent, typename... Args,
        std::enable_if_t<std::is_same_v<T, NBodySim::MassComponent>, int> = 0>
    void add_component(Args&&... args) {
        static_assert(std::is_base_of_v<IComponent, T>, "T must derive from IComponent.");
        auto component_ptr = std::make_shared<NBodySim::MassComponent>(
            static_cast<Derived*>(this)->transform.position, std::forward<Args>(args)...);

        components[std::type_index(typeid(NBodySim::MassComponent))] = component_ptr;
    }

    template <typename T, typename... Args,
        std::enable_if_t<!std::is_same_v<T, NBodySim::MassComponent> && !std::is_same_v<T, MeshRenderer>, int> = 0>
    void add_component(Args&&... args) {
        static_assert(std::is_base_of_v<IComponent, T>, "T must derive from IComponent.");
        auto component_ptr = std::make_shared<T>(std::forward<Args>(args)...);
        components[std::type_index(typeid(T))] = component_ptr;
    }

private:
    std::map<std::type_index, std::shared_ptr<IComponent>> components;

    void draw_components(const Camera& cam, const RenderSettings& rs, const std::span<const Light>& lights) {
        for (const auto &val: components | std::views::values) {
            val->draw(cam, rs, lights);
        }
    }
};

#endif