#include "mass_component.h"

#include "imgui.h"

void NBodySim::MassComponent::gui(int entity_id) {
    ImGui::Begin(std::format("Mass Component: ID#", entity_id).c_str());
    ImGui::InputFloat("Mass", &mass);
    ImGui::InputFloat3("Velocity", &velocity[0]);
    ImGui::InputFloat3("center-of-mass", &center_of_mass[0]);
    ImGui::End();
}
