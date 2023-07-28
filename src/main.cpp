// Jimena Hernández 21199
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

const int FRAMEBUFFER_WIDTH = 80;
const int FRAMEBUFFER_HEIGHT = 80;
const int FRAMEBUFFER_SIZE = FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT;

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

Color currentFramebuffer[FRAMEBUFFER_SIZE];
Color nextFramebuffer[FRAMEBUFFER_SIZE];
Color currentColor; // Moved this line after the Color struct

bool shouldLive(int x, int y) {
    int liveNeighbors = 0;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx != 0 || dy != 0) {
                int nx = (x + dx + FRAMEBUFFER_WIDTH) % FRAMEBUFFER_WIDTH;
                int ny = (y + dy + FRAMEBUFFER_HEIGHT) % FRAMEBUFFER_HEIGHT;
                if (currentFramebuffer[ny * FRAMEBUFFER_WIDTH + nx].r > 0) {
                    liveNeighbors++;
                }
            }
        }
    }

    if (currentFramebuffer[y * FRAMEBUFFER_WIDTH + x].r > 0) {
        return liveNeighbors >= 2 && liveNeighbors <= 3;
    } else {
        return liveNeighbors == 3;
    }
}

// Agregar esta función al código
void point(int x, int y) {
    if (x >= 0 && x < FRAMEBUFFER_WIDTH && y >= 0 && y < FRAMEBUFFER_HEIGHT) {
        currentFramebuffer[y * FRAMEBUFFER_WIDTH + x] = currentColor;
    }
}

void pulsar(int top_left_x, int top_left_y) {
    currentColor = {0, 255, 255, 255}; // Celeste

    int offsets[48][2] = {
            {2, 0}, {3, 0}, {4, 0}, {8, 0}, {9, 0}, {10, 0},
            {0, 2}, {5, 2}, {7, 2}, {12, 2},
            {0, 3}, {5, 3}, {7, 3}, {12, 3},
            {0, 4}, {5, 4}, {7, 4}, {12, 4},
            {2, 5}, {3, 5}, {4, 5}, {8, 5}, {9, 5}, {10, 5},
            {2, 7}, {3, 7}, {4, 7}, {8, 7}, {9, 7}, {10, 7},
            {0, 8}, {5, 8}, {7, 8}, {12, 8},
            {0, 9}, {5, 9}, {7, 9}, {12, 9},
            {0, 10}, {5, 10}, {7, 10}, {12, 10},
            {2, 12}, {3, 12}, {4, 12}, {8, 12}, {9, 12}, {10, 12}
    };

    for (auto& offset : offsets) {
        point(top_left_x + offset[0], top_left_y + offset[1]);
    }
}

void renderBuffer(SDL_Renderer* renderer) {
    SDL_Texture* texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_ABGR8888,
            SDL_TEXTUREACCESS_STREAMING,
            FRAMEBUFFER_WIDTH,
            FRAMEBUFFER_HEIGHT
    );

    SDL_UpdateTexture(
            texture,
            NULL,
            currentFramebuffer,
            FRAMEBUFFER_WIDTH * sizeof(Color)
    );

    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_DestroyTexture(texture);
}

void render(SDL_Renderer* renderer) {
    for (int x = 0; x < FRAMEBUFFER_WIDTH; x++) {
        for (int y = 0; y < FRAMEBUFFER_HEIGHT; y++) {
            if (shouldLive(x, y)) {
                nextFramebuffer[y * FRAMEBUFFER_WIDTH + x] = {204, 255, 229, 255};
            } else {
                nextFramebuffer[y * FRAMEBUFFER_WIDTH + x] = {0, 0, 0, 255};
            }
        }
    }

    for (int i = 0; i < FRAMEBUFFER_SIZE; i++) {
        currentFramebuffer[i] = nextFramebuffer[i];
    }

    renderBuffer(renderer);
}


int main() {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow("life", 0, 0, FRAMEBUFFER_WIDTH * 10, FRAMEBUFFER_HEIGHT * 10, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Población inicial aleatoria
    for (int i = 0; i < FRAMEBUFFER_SIZE; i++) {
        if (rand() % 2 == 0) {
            currentFramebuffer[i] = {255, 255, 255, 255};
        } else {
            currentFramebuffer[i] = {0, 0, 0, 255};
        }
    }

    bool running = true;
    SDL_Event event;

    currentColor = {255, 255, 255, 255};
    pulsar(20, 20);
    pulsar(40, 40);
    pulsar(60, 60);
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        render(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / 60);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    SDL_PumpEvents();

    return 0;
}
