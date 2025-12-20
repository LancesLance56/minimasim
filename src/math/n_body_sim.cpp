#include "n_body_sim.h"

#include <optional>
#include <functional> // Required for std::reference_wrapper

namespace NBodySim {

    /**
     * @brief Calculates the derivatives (velocity and acceleration) for a single Mass due to gravity.
     */
    StateDerivative calculate_derivatives(
        const MassData& current_mass,
        const std::vector<MassData>& minus_self,
        float G)
    {
        // The derivative of position (dp/dt) is simply the current velocity
        StateDerivative derivative;
        derivative.dp = current_mass.velocity;

        // The derivative of velocity (dv/dt) is the total acceleration
        auto total_acceleration = glm::vec3(0.0f);

        for (const auto& other_mass : minus_self) {
            glm::vec3 r_vector = other_mass.center_of_mass - current_mass.center_of_mass;
            float distance_sq = glm::dot(r_vector, r_vector);

            // Check for zero distance to prevent division by zero
            if (distance_sq > 1e-6f) {
                float distance = std::sqrt(distance_sq);

                // Acceleration vector: a = G * m_other * r_vector / r^3
                float acc_magnitude = G * other_mass.mass / distance_sq;
                glm::vec3 acceleration_vector = acc_magnitude * (r_vector / distance);

                total_acceleration += acceleration_vector;
            }
        }
        derivative.dv = total_acceleration;
        return derivative;
    }
} // namespace NBodySim