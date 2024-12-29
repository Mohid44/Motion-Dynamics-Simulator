#include <SDL2/SDL.h>
#include <vector>
#include <cmath>

// Vector2D class for positions and velocities
struct Vec2 {
    float x, y;
    Vec2(float x = 0, float y = 0) : x(x), y(y) {}
    Vec2 operator+(const Vec2& v) const { return {x + v.x, y + v.y}; }
    Vec2 operator*(float scalar) const { return {x * scalar, y * scalar}; }
};

// Object structure
struct Object {
    Vec2 position;
    Vec2 velocity;
    float radius;

    Object(Vec2 pos, Vec2 vel, float r) : position(pos), velocity(vel), radius(r) {}
};

// Update object positions and velocities
void update(std::vector<Object>& objects, float deltaTime) {
    for (auto& obj : objects) {
        obj.position = obj.position + obj.velocity * deltaTime;
        obj.velocity.y += 9.8f * deltaTime; // Apply gravity

        // Collision detection with the ground
        if (obj.position.y > 580) { // Ground level
            obj.position.y = 580;
            obj.velocity.y = -obj.velocity.y * 0.8; // Bounce with damping
        }
    }
}

// Render objects on the screen
void render(SDL_Renderer* renderer, const std::vector<Object>& objects) {
    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw each object
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (const auto& obj : objects) {
        SDL_Rect rect = {
            static_cast<int>(obj.position.x),
            static_cast<int>(obj.position.y),
            static_cast<int>(obj.radius * 2),
            static_cast<int>(obj.radius * 2)
        };
        SDL_RenderFillRect(renderer, &rect);
    }

    // Present the rendered frame
    SDL_RenderPresent(renderer);
}

int main() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    // Create SDL window and renderer
    SDL_Window* window = SDL_CreateWindow("Physics Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Create objects
    std::vector<Object> objects = {
        Object({100, 100}, {50, 0}, 20),
        Object({200, 200}, {0, -50}, 20)
    };

    // Simulation loop
    bool running = true;
    SDL_Event event;
    float deltaTime = 0.016f; // 60 FPS

    while (running) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Update and render objects
        update(objects, deltaTime);
        render(renderer, objects);

        // Delay to simulate frame rate
        SDL_Delay(16);
    }

    // Cleanup SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
