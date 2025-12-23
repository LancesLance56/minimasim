#ifndef MASS_SIM_H
#define MASS_SIM_H

#include <functional> // Required for std::reference_wrapper
#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include <optional>
#include <ostream>
#include <span>
#include <vector>

#include "engine/entity/components/mass_component.h"

namespace NBodySim {

    // holds the data: no references
    struct MassData {
        glm::vec3 center_of_mass;
        glm::vec3 velocity;
        float mass{};
    };



    // State vector for RK4: holds the derivatives (d/dt) of position (velocity) and velocity (acceleration)
    struct StateDerivative {
        glm::vec3 dp;
        glm::vec3 dv;
    };

    /**
     * @brief Calculates the derivatives (velocity and acceleration) for a single Mass due to gravity.
     * @param current_mass The mass whose derivatives are being calculated.
     * @param minus_self All other masses in the system.
     * @param G The gravitational constant.
     * @return StateDerivative containing the instantaneous velocity and acceleration.
     */
    StateDerivative calculate_derivatives(
        const MassData& current_mass,
        const std::vector<MassData>& minus_self,
        float G);

    /**
     * @brief Integrates the N-body system forward by one time step using RK4.
     * @param G Gravitational constant.
     * @param time_step Time step (dt).
     * @param entities references to MassAttribute objects (bodies) in the system that will be edited.
     */
    template <typename... Args>
    void integrate_system_rk4(float G, float time_step, Args&... entities)
    {
        // Static assertion ensures all input arguments are references to MassAttribute
        static_assert((std::is_same_v<std::remove_cv_t<Args>, MassComponent> && ...),
                      "All entities arguments must be references to NBodySim::MassAttribute.");

        const size_t N = sizeof...(entities);
        if (N == 0) return;

        // Create a container of reference wrappers to hold original attributes.
        // This is necessary to modify the original objects passed by the variadic template.
        std::vector<std::reference_wrapper<MassComponent>> masses_to_update = {
            std::ref(entities)...
        };

        std::vector<StateDerivative> k1(N);
        std::vector<StateDerivative> k2(N);
        std::vector<StateDerivative> k3(N);
        std::vector<StateDerivative> k4(N);

        std::vector<MassData> temp_state_k2(N);
        std::vector<MassData> temp_state_k3(N);
        std::vector<MassData> temp_state_k4(N);

        // 1. Calculate k1 (Derivative at t)
        // Store the initial state (MassData) by value for calculating intermediate steps
        std::vector<MassData> state_t(N);
        for (size_t i = 0; i < N; ++i) {
            MassComponent& attr = masses_to_update[i].get();
            state_t[i].mass = attr.mass;
            state_t[i].center_of_mass = attr.center_of_mass; // Position from attribute becomes center_of_mass for calculation
            state_t[i].velocity = attr.velocity;
        }

        for (size_t i = 0; i < N; ++i) {
            k1[i] = calculate_derivatives(state_t[i], state_t, G);
        }

        // 2. Calculate k2 (Derivative at t + dt/2, using k1)
        for (size_t i = 0; i < N; ++i) {
            temp_state_k2[i].mass = state_t[i].mass;
            temp_state_k2[i].center_of_mass = state_t[i].center_of_mass + k1[i].dp * (0.5f * time_step);
            temp_state_k2[i].velocity = state_t[i].velocity + k1[i].dv * (0.5f * time_step);
        }

        for (size_t i = 0; i < N; ++i) {
            k2[i] = calculate_derivatives(temp_state_k2[i], temp_state_k2, G);
        }

        // 3. Calculate k3 (Derivative at t + dt/2, using k2)
        for (size_t i = 0; i < N; ++i) {
            temp_state_k3[i].mass = state_t[i].mass;
            temp_state_k3[i].center_of_mass = state_t[i].center_of_mass + k2[i].dp * (0.5f * time_step);
            temp_state_k3[i].velocity = state_t[i].velocity + k2[i].dv * (0.5f * time_step);
        }
        // Calculate derivatives (k3) based on the intermediate state
        for (size_t i = 0; i < N; ++i) {
            k3[i] = calculate_derivatives(temp_state_k3[i], temp_state_k3, G);
        }

        // 4. Calculate k4 (Derivative at t + dt, using k3)
        for (size_t i = 0; i < N; ++i) {
            temp_state_k4[i].mass = state_t[i].mass;
            temp_state_k4[i].center_of_mass = state_t[i].center_of_mass + k3[i].dp * time_step;
            temp_state_k4[i].velocity = state_t[i].velocity + k3[i].dv * time_step;
        }

        for (size_t i = 0; i < N; ++i) {
            k4[i] = calculate_derivatives(temp_state_k4[i], temp_state_k4, G);
        }

        // 5. Final Update (Modify the input MassAttributes in place)
        for (size_t i = 0; i < N; ++i) {
            glm::vec3 dp_avg = (k1[i].dp + 2.0f * k2[i].dp + 2.0f * k3[i].dp + k4[i].dp) / 6.0f;
            glm::vec3 dv_avg = (k1[i].dv + 2.0f * k2[i].dv + 2.0f * k3[i].dv + k4[i].dv) / 6.0f;

            MassComponent& attr = masses_to_update[i].get();

            attr.center_of_mass += dp_avg * time_step;
            attr.velocity += dv_avg * time_step;
        }
    }

    template <typename T, size_t N>
    void integrate_system_rk4_multi(float G, float time_step, const std::array<T, N>& components) {
        std::apply(
            [&](auto&&... elems) {
                integrate_system_rk4(G, time_step, elems...);
            },
            components
        );
    }
    
} // namespace NBodySim

#endif // MASS_SIM_H
