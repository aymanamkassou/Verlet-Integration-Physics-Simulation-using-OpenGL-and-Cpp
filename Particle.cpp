#include "PhysicsObject.cpp"
#include <GLFW/glfw3.h>
#include <cmath>

class Particle : public PhysicsObject {
private:
    float radius;
    glm::vec4 color;
public:
    Particle(const glm::vec2& pos, float particle_radius, const glm::vec4& particle_color)
        : PhysicsObject(pos)
        , radius(particle_radius)
        , color(particle_color) {}
    void update(float dt) override {
        if (static_state) return;

        glm::vec2 velocity = position - prev_position;
        prev_position = position;
        position = position + velocity + acceleration * dt * dt;
        acceleration = glm::vec2(0.0f);
    }

    void render() const override {
        const int segments = 32;
        glBegin(GL_TRIANGLE_FAN);
        glColor4f(color.r, color.g, color.b, color.a);
        glVertex2f(position.x, position.y);
        for (int i = 0; i <= segments; i++) {
            float angle = 2.0f * M_PI * i / segments;
            glVertex2f(
                position.x + radius * cos(angle),
                position.y + radius * sin(angle)
            );
        }
        glEnd();
    }

    void resolve_collision(PhysicsObject* other) override {
        // Khalid chhad lkhedma asahbi
        auto* p = dynamic_cast<Particle*>(other);
        if (!p) return;
        glm::vec2 delta = position - p->position;
        float dist = glm::length(delta);
        float min_dist = radius + p->radius;
        if (dist < min_dist) {
            glm::vec2 n = delta / dist;
            float delta_dist = min_dist - dist;
            position += 0.5f * delta_dist * n;
            p->position -= 0.5f * delta_dist * n;
        }
    }

    void constrain_to_bounds(const glm::vec2& bounds) override {
        if (position.x - radius < 0) position.x = radius;

        if (position.x + radius > bounds.x) position.x = bounds.x - radius;

        if (position.y - radius < 0) position.y = radius;
        if (position.y + radius > bounds.y) position.y = bounds.y - radius;
    }

    void handle_mouse_force(const glm::vec2& mouse_pos, float mouse_radius, float strength) override {
        glm::vec2 to_mouse = mouse_pos - position;
        float distance = glm::length(to_mouse);
        
        if (distance < mouse_radius) {
            float force_strength = (1.0f - distance / mouse_radius) * strength;
            apply_force(glm::normalize(to_mouse) * force_strength);
        }
    }
};
