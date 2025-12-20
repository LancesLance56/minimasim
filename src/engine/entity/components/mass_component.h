#ifndef MASS_COMPONENT_H
#define MASS_COMPONENT_H

#include "glm/detail/type_vec.hpp"
#include "glm/detail/type_vec3.hpp"

namespace NBodySim {
    // Define an attribute Mass
    /**
     * @brief Mass struct as a datatype for nbodysim
     * @param center_of_mass pointer to the mass of the object the attribute is attached to
     * @param velocity pointer to the velocity of the object the attribute is attached to
     * @param mass is the mass of the object used to calculate its impact on other bodies
     */
    struct MassComponent {
        glm::vec3 &center_of_mass;
        glm::vec3 velocity;
        float mass;
    };
}

#endif //MASS_COMPONENT_H
