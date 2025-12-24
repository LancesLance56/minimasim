#ifndef MASS_SIM_H
#define MASS_SIM_H

#include <glm/glm.hpp>
#include <vector>
#include <span>
#include <type_traits>
#include "engine/entity/components/mass_component.h"

namespace NBodySim {

struct MassData {
  glm::dvec3 center_of_mass;
  glm::dvec3 velocity;
  double mass{};
};

struct StateDerivative {
  glm::dvec3 dp;
  glm::dvec3 dv;
};

StateDerivative calculate_derivatives(const MassData &current_mass, const std::vector<MassData> &all_masses, double G);

void integrate_system_rk4(double G, double time_step, std::span<MassComponent*> components);

inline void integrate_system_rk4(double G, double time_step, std::vector<MassComponent*>& components) {
  integrate_system_rk4(G, time_step, std::span<MassComponent*>(components));
}

template<typename... Args>
void integrate_system_rk4(double G, double time_step, Args &...entities) {
  if constexpr ((std::is_same_v<std::remove_cv_t<Args>, MassComponent> && ...)) {
    std::vector<MassComponent*> components = {&entities...};
    integrate_system_rk4(G, time_step, std::span(components));
  }
}

}
#endif