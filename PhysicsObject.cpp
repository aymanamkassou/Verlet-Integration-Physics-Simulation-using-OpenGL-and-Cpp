#include <glm/glm.hpp>

class PhysicsObject {
protected:
    glm::vec2 position;
    glm::vec2 prev_position;
    glm::vec2 acceleration;
    float mass;
    bool static_state;

public:
    PhysicsObject(const glm::vec2& initial_pos, float object_mass = 1.0f)
        : position(initial_pos)
        , prev_position(initial_pos)
        , acceleration(0.0f, 0.0f)
        , mass(object_mass)
        , static_state(false) {}

    virtual ~PhysicsObject() = default;
    virtual void update(float dt) = 0;
    virtual void render() const = 0;
    virtual void resolve_collision(PhysicsObject* other) = 0;
    virtual void constrain_to_bounds(const glm::vec2& bounds) = 0;
    virtual void handle_mouse_force(const glm::vec2& mouse_pos, float radius, float strength) = 0;

    void apply_force(const glm::vec2& force) {
        if (!static_state) {
            acceleration += force / mass;
        }
    }

    void make_static(bool state) { static_state = state; }
};
