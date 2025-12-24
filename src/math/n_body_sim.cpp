#include "n_body_sim.h"
#include <cmath>

namespace NBodySim {

    StateDerivative calculate_derivatives(const MassData &current_mass, const std::vector<MassData> &all_masses, double G) {
        StateDerivative derivative;
        derivative.dp = current_mass.velocity;
        glm::dvec3 total_acceleration(0.0);

        for (const auto &other : all_masses) {
            glm::dvec3 r_vector = other.center_of_mass - current_mass.center_of_mass;
            double dist_sq = glm::dot(r_vector, r_vector);

            if (dist_sq > 1e-9f) {
                double dist = std::sqrt(dist_sq);
                total_acceleration += (G * other.mass / (dist_sq * dist)) * r_vector;
            }
        }
        derivative.dv = total_acceleration;
        return derivative;
    }

    void integrate_system_rk4(double G, double time_step, std::span<MassComponent*> components) {
        const size_t n = components.size();
        if (n == 0) return;

        std::vector<MassData> state_t(n);
        std::vector<MassData> temp_state(n);
        std::vector<StateDerivative> k1(n);
        std::vector<StateDerivative> k2(n);
        std::vector<StateDerivative> k3(n);
        std::vector<StateDerivative> k4(n);

        for (size_t i = 0; i < n; ++i) {
            state_t[i] = {components[i]->center_of_mass, components[i]->velocity, components[i]->mass};
        }

        for (size_t i = 0; i < n; ++i) k1[i] = calculate_derivatives(state_t[i], state_t, G);

        for (size_t i = 0; i < n; ++i) {
            temp_state[i] = state_t[i];
            temp_state[i].center_of_mass += k1[i].dp * (time_step * 0.5f);
            temp_state[i].velocity += k1[i].dv * (time_step * 0.5f);
        }
        for (size_t i = 0; i < n; ++i) k2[i] = calculate_derivatives(temp_state[i], temp_state, G);

        for (size_t i = 0; i < n; ++i) {
            temp_state[i].center_of_mass = state_t[i].center_of_mass + k2[i].dp * (time_step * 0.5f);
            temp_state[i].velocity = state_t[i].velocity + k2[i].dv * (time_step * 0.5f);
        }
        for (size_t i = 0; i < n; ++i) k3[i] = calculate_derivatives(temp_state[i], temp_state, G);

        for (size_t i = 0; i < n; ++i) {
            temp_state[i].center_of_mass = state_t[i].center_of_mass + k3[i].dp * time_step;
            temp_state[i].velocity = state_t[i].velocity + k3[i].dv * time_step;
        }
        for (size_t i = 0; i < n; ++i) k4[i] = calculate_derivatives(temp_state[i], temp_state, G);

        for (size_t i = 0; i < n; ++i) {
            components[i]->center_of_mass += (time_step / 6.0) * (k1[i].dp + 2.0 * k2[i].dp + 2.0 * k3[i].dp + k4[i].dp);
            components[i]->velocity += (time_step / 6.0) * (k1[i].dv + 2.0 * k2[i].dv + 2.0 * k3[i].dv + k4[i].dv);
        }
    }
}