#ifndef MASS_COMPONENT_H
#define MASS_COMPONENT_H

#include "glm/detail/type_vec.hpp"
#include "glm/detail/type_vec3.hpp"
#include "component.h" // Include the new CRTP base

namespace NBodySim {
    /**
     * @brief Mass struct as a datatype for nbodysim, inheriting from the CRTP base.
     * @param center_of_mass pointer to the mass of the object the attribute is attached to
     * @param velocity pointer to the velocity of the object the attribute is attached to
     * @param mass is the mass of the object used to calculate its impact on other bodies
     */
    struct MassComponent : public Component<MassComponent> { // CRTP Inheritance
        glm::vec3 &center_of_mass;
        glm::vec3 velocity;
        float mass;

        MassComponent(glm::vec3& com, glm::vec3 v, float m)
            : center_of_mass(com), velocity(v), mass(m) {}
    };
}

#endif //MASS_COMPONENT_H
