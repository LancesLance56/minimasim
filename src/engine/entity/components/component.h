#ifndef COMPONENT_H
#define COMPONENT_H

class Camera;
struct RenderSettings;
struct Light;
#include <span>

class IComponent {
public:
    virtual ~IComponent() = default;

    virtual void draw(const Camera &, const RenderSettings &, const std::span<const Light> &) {}
    virtual void gui(int entity_id) {}
};


template<typename Derived>
class Component : public IComponent {
public:
    Derived &as_derived() { return static_cast<Derived &>(*this); }
    const Derived &as_derived() const { return static_cast<const Derived &>(*this); }
};

#endif
