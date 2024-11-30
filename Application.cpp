#include "PhysicsWorld.cpp"
#include <random>
#include <GLFW/glfw3.h>
#include <stdexcept>

class Application {
private:
    GLFWwindow* window;
    std::unique_ptr<PhysicsWorld> world;
    float last_time;

    void handle_mouse_move(float x, float y) {
        world->update_mouse(glm::vec2(x, 600 - y), true);
    }

    void handle_mouse_button(int button, int action, float x, float y) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            world->update_mouse(glm::vec2(x, 600 - y), action == GLFW_PRESS);
        }
    }

    void init_glfw() {
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        window = glfwCreateWindow(800, 600, "Physics Simulation", nullptr, nullptr);
        if (!window) {
            glfwTerminate();
            throw std::runtime_error("Failed to create window");
        }

        glfwMakeContextCurrent(window);
        glfwSetWindowUserPointer(window, this);

        glfwSetCursorPosCallback(window, [](GLFWwindow* w, double x, double y) {
            auto* app = static_cast<Application*>(glfwGetWindowUserPointer(w));
            app->handle_mouse_move(static_cast<float>(x), static_cast<float>(y));
        });

        
        glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int button, int action, int mods) {
        auto* app = static_cast<Application*>(glfwGetWindowUserPointer(w));
        double x, y;
        glfwGetCursorPos(w, &x, &y);
        app->handle_mouse_button(button, action, static_cast<float>(x), static_cast<float>(y));
        });
    }

    void update(float dt) {
        world->update(dt);
    }

    void render() {
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();
        glOrtho(0, 800, 0, 600, -1, 1);
        world->render();
    }

    

public:
    Application() {
        init_glfw();
        world = std::make_unique<PhysicsWorld>(glm::vec2(800, 600));
        last_time = static_cast<float>(glfwGetTime());
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dist_x(0.0f, 800.0f);
        std::uniform_real_distribution<float> dist_y(0.0f, 600.0f);

        for (int i = 0; i < 100; ++i) {
            auto particle = std::make_unique<Particle>(
                glm::vec2(dist_x(gen), dist_y(gen)),
                5.0f,
                glm::vec4(0.2f, 0.5f, 0.9f, 1.0f)
            );
            world->add_object(std::move(particle));
        }
    }

    void run() {
        while (!glfwWindowShouldClose(window)) {
            float current_time = static_cast<float>(glfwGetTime());
            float dt = current_time - last_time;
            last_time = current_time;

            update(dt);
            render();

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    ~Application() {
        glfwTerminate();
    }
};
