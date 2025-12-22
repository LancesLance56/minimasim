#ifndef ENTITY_REGISTRY_H
#define ENTITY_REGISTRY_H

#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include "entity/entity_base.h"

class EntityRegistry {
public:
    template <typename T, typename... Args>
    T* create_entity(Args&&... args) {
        auto entity_uptr = std::make_unique<T>(std::forward<Args>(args)...);
        T* raw_ptr = entity_uptr.get();

        active_entities.push_back(raw_ptr);

        entity_indices[raw_ptr] = active_entities.size() - 1;
        entity_storage[raw_ptr] = std::move(entity_uptr);

        return raw_ptr;
    }

    void destroy_entity(EntityBase* entity) {
        auto it = entity_indices.find(entity);
        if (it == entity_indices.end()) return;

        size_t index_to_remove = it->second;
        EntityBase* last_entity = active_entities.back();

        if (index_to_remove != active_entities.size() - 1) {
            active_entities[index_to_remove] = last_entity;
            entity_indices[last_entity] = index_to_remove;
        }

        active_entities.pop_back();
        entity_indices.erase(it);
        entity_storage.erase(entity);
    }

    void update_systems(double dt) {
        for (auto* entity : active_entities) {
            entity->update(dt);
        }
    }

    void draw_systems(const Camera& cam, const RenderSettings& rs, const std::vector<Light>& lights) {
        if (rs.wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        for (auto* entity : active_entities) {
            entity->draw(cam, rs, lights);
        }

        if (rs.wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    void draw_gui() {
        for (size_t index = 0; auto* entity : active_entities) {
            entity->on_gui(index);
            index++;
        }
    }

    const std::vector<EntityBase*>& get_active_entities() const {
        return active_entities;
    }

private:
    std::unordered_map<EntityBase*, std::unique_ptr<EntityBase>> entity_storage;
    std::unordered_map<EntityBase*, size_t> entity_indices;
    std::vector<EntityBase*> active_entities;
};

#endif