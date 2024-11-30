#include "Particle.cpp"
#include <glm/glm.hpp>
#include <vector>
#include <memory>

class PhysicsWorld {
private:
    struct MouseConstraint {
        bool active;
        glm::vec2 position;
        float radius;
        float strength;
    };

    std::vector<std::unique_ptr<PhysicsObject>> objects;
    glm::vec2 gravity;
    glm::vec2 bounds;
    float iterations;
    MouseConstraint mouse;

    void solve_collisions() {
        for (size_t i = 0; i < objects.size(); ++i) {
            for (size_t j = i + 1; j < objects.size(); ++j) {
                objects[i]->resolve_collision(objects[j].get());
            }
        }
    }

    void apply_bounds() {
        for (auto& obj : objects) {
            obj->constrain_to_bounds(bounds);
        }
    }

    void apply_mouse_force() {
        for (auto& obj : objects) {
            obj->handle_mouse_force(mouse.position, mouse.radius, mouse.strength);
        }
    }

public:
    PhysicsWorld(const glm::vec2& world_bounds)
        : bounds(world_bounds)
        , gravity(0.0f, -9.81f)
        , iterations(8)
    {
        mouse.active = false;
        mouse.radius = 50.0f;
        mouse.strength = 1000.0f;
    }

    void add_object(std::unique_ptr<PhysicsObject> object) {
        objects.push_back(std::move(object));
    }

    void update(float dt) {
        // 3lach makhdemtich for 3adiya?
        for (auto& obj : objects) {
            obj->apply_force(gravity);
        }

        if (mouse.active) {
            apply_mouse_force();
        }

        for (auto& obj : objects) {
            obj->update(dt);
        }

        for (int i = 0; i < iterations; ++i) {
            solve_collisions();
            apply_bounds();
        }
    }

    void render() const {
        for (const auto& obj : objects) {
            obj->render();
        }
    }

    void update_mouse(const glm::vec2& pos, bool active) {
        mouse.position = pos;
        mouse.active = active;
    }
};
